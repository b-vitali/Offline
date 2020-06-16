//
// Give generated tracks to G4. This implements two algorithms:
//
// 1) testTrack - a trivial 1 track generator for debugging geometries.
// 2) fromEvent - copies generated tracks from the event.
//
// Original author Rob Kutschke
//

// C++ includes
#include <iostream>
#include <stdexcept>

// Framework includes
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "art/Framework/Principal/Handle.h"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "cetlib_except/exception.h"
#include "canvas/Persistency/Provenance/ProductID.h"
#include "art/Utilities/Globals.h"

// G4 Includes
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#if G4VERSION<4099
#include "G4ParticleTable.hh"
#endif
#include "G4IonTable.hh"
#include "Randomize.hh"
#include "globals.hh"
#include "G4Threading.hh"
#include "G4IsotopeProperty.hh"
#include "G4NuclideTable.hh"

// Mu2e includes
#include "ConfigTools/inc/SimpleConfig.hh"
#include "Mu2eG4/inc/PrimaryGeneratorAction.hh"
#include "Mu2eG4/inc/SimParticlePrimaryHelper.hh"
#include "Mu2eUtilities/inc/RandomUnitSphere.hh"
#include "Mu2eUtilities/inc/ThreeVectorUtil.hh"
#include "MCDataProducts/inc/GenParticleCollection.hh"
#include "GeometryService/inc/GeomHandle.hh"
#include "GeometryService/inc/WorldG4.hh"
#include "DataProducts/inc/PDGCode.hh"
#include "Mu2eG4/inc/Mu2eG4PerThreadStorage.hh"
#include "Mu2eG4/inc/SimParticleHelper.hh"


using namespace std;

namespace mu2e {

  PrimaryGeneratorAction::PrimaryGeneratorAction(const Mu2eG4Config::Debug& debug,
                                                 Mu2eG4PerThreadStorage* tls)
    : verbosityLevel_(debug.diagLevel())
    , pdgIdToGenerate_(static_cast<PDGCode::type>(debug.pdgIdToGenerate()))
    , perThreadObjects_(tls)
  {
    if ( verbosityLevel_ > 0 ) {
      cout << __func__ << " verbosityLevel_  : " <<  verbosityLevel_ << endl;
    }
  }

  //load in per-art-event data from GenEventBroker and per-G4-event data from EventObjectManager
  void PrimaryGeneratorAction::setEventData()
  {
    genParticles_ = perThreadObjects_->gensHandle.isValid() ?
      perThreadObjects_->gensHandle.product() :
      nullptr;

    hitInputs_ = perThreadObjects_->genInputHits;

    parentMapping_ = perThreadObjects_->simParticlePrimaryHelper;

  }


  void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
  {
    //load the GenParticleCollection etc for the event
    setEventData();
    // For debugging.
    //testTrack(anEvent);

    fromEvent(anEvent);
  }

  // Copy generated particles from the event into G4.
  // At the moment none of the supported generators make multi-particle vertices.
  // That may change in the future.
  void PrimaryGeneratorAction::fromEvent(G4Event* event)
  {

    art::ServiceHandle<GeometryService> geom;
    SimpleConfig const & _config = geom->config();

    // Get the offsets to map from generator world to G4 world.
    // check if this is standard mu2e configuration or not

    G4ThreeVector const mu2eOrigin =
      (!_config.getBool("mu2e.standardDetector",true) || !(geom->isStandardMu2eDetector()))
      ?  G4ThreeVector(0.0,0.0,0.0) : (GeomHandle<WorldG4>())->mu2eOriginInWorld();

    // For each generated particle, add it to the event.
    if(genParticles_) {
      for (unsigned i=0; i < genParticles_->size(); ++i) {
        const GenParticle& genpart = (*genParticles_)[i];
        addG4Particle(event,
                      genpart.pdgId(),
                      // Transform into G4 world coordinate system
                      genpart.position() + mu2eOrigin,
                      genpart.time(),
                      genpart.properTime(),
                      genpart.momentum());

        parentMapping_->addEntryFromGenParticle(i);
      }
    }

    if ( pdgIdToGenerate_ != -1111111111 ) {
      cout << __func__ << " *TESTING*: Adding *ONLY*  : " << pdgIdToGenerate_ << endl;

      // a test
      int ovl = verbosityLevel_;
      int govl = G4ParticleTable::GetParticleTable()->GetVerboseLevel();
      verbosityLevel_ = 2;
      G4ParticleTable::GetParticleTable()->SetVerboseLevel(verbosityLevel_);
      addG4Particle(event,
                    pdgIdToGenerate_,
                    // Transform into G4 world coordinate system
                    G4ThreeVector()+mu2eOrigin,
                    0.0,
                    0.0,
                    G4ThreeVector());
      verbosityLevel_ = ovl;
      G4ParticleTable::GetParticleTable()->SetVerboseLevel(govl);

    } else {
      // Also create particles from the input hits
      for(const auto& hitcoll : *hitInputs_) {

        for(const auto& hit : *hitcoll) {
          addG4Particle(event,
                        hit.simParticle()->pdgId(),
                        // Transform into G4 world coordinate system
                        hit.position() + mu2eOrigin,
                        hit.time(),
                        hit.properTime(),
                        hit.momentum());

          parentMapping_->addEntryFromStepPointMC(hit.simParticle()->id());

        }
      }

    }
  }

  void PrimaryGeneratorAction::addG4Particle(G4Event *event,
                                             PDGCode::type pdgId,
                                             const G4ThreeVector& pos,
                                             double time,
                                             double properTime,
                                             const G4ThreeVector& mom)
  {
    int error(0);

    if ( verbosityLevel_ > 1) {
      cout << __func__ << " pdgId   : " <<pdgId << endl;
    }

    if (pdgId>PDGCode::G4Threshold) {

      G4int ZZ,AA,JJ;
      G4double EE;

      int exc = pdgId%10;

      // subtract exc to get Z,A,...

      bool retCode = G4IonTable::GetNucleusByEncoding(pdgId-exc,ZZ,AA,EE,JJ);

      // if g4 complains about no GenericIon we need to abort and add it in the physics list...

      if ( verbosityLevel_ > 1) {
        cout << __func__ << " will set up nucleus pdgId,Z,A,E,J, exc: "
             << pdgId
             << ", " << ZZ << ", " << AA << ", " << EE << ", " << JJ
             << ", " << exc << ", " << retCode
             << endl;
      }

      G4ParticleDefinition const * pDef = nullptr;

      if (exc==0) {

        // a ground state

#if G4VERSION<4099
        pDef = G4ParticleTable::GetParticleTable()->GetIon(ZZ,AA,0.0);
#else
        pDef = G4IonTable::GetIonTable()->GetIon(ZZ,AA,0.0);
#endif
        // looks like spin is ignored and should never be non zero...

      } else {

        G4NuclideTable* pNuclideTable = G4NuclideTable::GetNuclideTable();
        // for some reason G4NuclideTable::GetIsotopeByIsoLvl does not return non 0 levels
        // hopefully will not need to do it often or till the far end of the loop
        for ( size_t i = 0 ; i != pNuclideTable->entries() ; ++i ) {
          const G4IsotopeProperty*  pProperty = pNuclideTable->GetIsotopeByIndex( i );
          if ( ZZ != pProperty->GetAtomicNumber() ) continue;
          if ( AA != pProperty->GetAtomicMass()   ) continue;
          G4double eExc  = pProperty->GetEnergy();
          if (eExc <= 0.0) continue;
          pDef = G4IonTable::GetIonTable()->GetIon(ZZ,AA,eExc);
          // hopefully we'll find the right one
          if ( pdgId != pDef->GetPDGEncoding() ) continue;
          break;
        }

      }

      // the ids should be the same
      if ( !pDef || pdgId != pDef->GetPDGEncoding() ) {

	printf("ERROR in PrimaryGeneratorAction::addG4Particle : ignore particle with unknown PDG code: %i\n",pdgId);
	error = 1;
	
        // if (pDef) {cout << __func__ << " got " << pDef->GetPDGEncoding() << " expected " << pdgId << endl;}
        // throw cet::exception("GENE")
        //   << "Problem creating " << pdgId << "\n";

      }

      if ( verbosityLevel_ > 1) {
        cout << __func__ << " will set up : "
             << pDef->GetParticleName()
             << " with id: " << pDef->GetPDGEncoding()
             << endl;
      }

    }

    if (error == 0) {
      // Create a new vertex
      G4PrimaryVertex* vertex = new G4PrimaryVertex(pos, time);

      G4PrimaryParticle* particle =
	new G4PrimaryParticle(pdgId,
			      mom.x(),
			      mom.y(),
			      mom.z());

      // Add the particle to the event.
      vertex->SetPrimary( particle );

      // Add the vertex to the event.
      event->AddPrimaryVertex( vertex );
    }
  }


} // end namespace mu2e
