///////////////////////////////////////////////////////////////////////////////
// stores one StepPointMCCollection in STNTUPLE data block
///////////////////////////////////////////////////////////////////////////////
#include "TLorentzVector.h"
#include "TDatabasePDG.h"
#include "TParticlePDG.h"

#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Selector.h"

// conditions
#include "ConditionsService/inc/ConditionsHandle.hh"
#include "ConditionsService/inc/AcceleratorParams.hh"

#include "Mu2eUtilities/inc/SimParticleTimeOffset.hh"

#include "MCDataProducts/inc/StepPointMCCollection.hh"
#include "MCDataProducts/inc/StrawHitMCTruthCollection.hh"
#include "MCDataProducts/inc/PtrStepPointMCVectorCollection.hh"
#include "DataProducts/inc/VirtualDetectorId.hh"

#include "Stntuple/obj/TStepPointMCBlock.hh"

#include "Stntuple/mod/InitStntupleDataBlocks.hh"
#include "Stntuple/mod/THistModule.hh"
#include "Stntuple/base/TNamedHandle.hh"

//-----------------------------------------------------------------------------
int StntupleInitMu2eStepPointMCBlock(TStnDataBlock* Block, AbsEvent* AnEvent, int Mode) 
{
  // 
  // const char* oname = {"StntupleInitMu2eStepPointMCBlock"};

  static int    initialized(0);
  static char   module_label  [100], description  [100];
  char          module_name   [100], time_offsets_name[100];
  static float _mbtime;

  static mu2e::SimParticleTimeOffset* _timeOffsets(NULL);

  const mu2e::StepPointMCCollection*       spmc_coll(0);
  art::Handle<mu2e::StepPointMCCollection> spmc_handle;

  if (initialized == 0) {
    initialized = 1;

    Block->GetModuleLabel("mu2e::StepPointMCCollection",module_label);
    Block->GetDescription("mu2e::StepPointMCCollection",description );

    Block->GetModuleLabel("TimeOffsetMapsHandle",module_name);
    Block->GetDescription("TimeOffsetMapsHandle",time_offsets_name);

    THistModule*  m  = static_cast<THistModule*>  (THistModule::GetListOfModules()->FindObject(module_name));
    TNamedHandle* nh = static_cast<TNamedHandle*> (m->GetFolder()->FindObject(time_offsets_name));
    _timeOffsets     = static_cast<mu2e::SimParticleTimeOffset*> (nh->Object());

    mu2e::ConditionsHandle<mu2e::AcceleratorParams> accPar("ignored");
    _mbtime = accPar->deBuncherPeriod;
  }

  TStepPointMCBlock* spmc_block = (TStepPointMCBlock*) Block;
  spmc_block->Clear();
//-----------------------------------------------------------------------------
// load simulation time offsets for this event: timeOffsets may not be defined yet (stages 1, 2, 3)
//-----------------------------------------------------------------------------
  if (_timeOffsets) _timeOffsets->updateMap(*AnEvent);

  const mu2e::StepPointMC*                    spmc;
//-----------------------------------------------------------------------------
//  const art::Provenance*                 prov;
//  art::Selector  selector(art::ProductInstanceNameSelector(""));
//  AnEvent->getMany(selector, list_of_sp);
//
//   for (std::vector<art::Handle<mu2e::SimParticleCollection>> ::const_iterator it = list_of_sp.begin();
//        it != list_of_sp.end(); it++) {
//     handle = it.operator -> ();

  art::Selector  selector(art::ProcessNameSelector("")               && 
			  art::ModuleLabelSelector(module_label)     && 
			  art::ProductInstanceNameSelector(description) );

  AnEvent->get(selector,spmc_handle);

  if (spmc_handle.isValid()) {
    spmc_coll = spmc_handle.product();
    //      prov = handle->provenance();
      
    //       printf("moduleLabel = %-20s, producedClassname = %-30s, productInstanceName = %-20s\n",
    // 	     prov->moduleLabel().data(),
    // 	     prov->producedClassName().data(),
    // 	     prov->productInstanceName().data());

    for (mu2e::StepPointMCCollection::const_iterator ip = spmc_coll->begin(); ip != spmc_coll->end(); ip++) {
      spmc = &(*ip);
      
      int   volume_id = spmc->volumeId();

      int   id (-1), gen_index(-1), pdg_code(-1), parent_pdg_code (-1), parent_id(-1);
      int   creation_code(-1), end_process_code(-1);
      float edep_tot (-1.), edep_nio(-1.), time(-1.), step_length(-1.);

      art::Ptr<mu2e::SimParticle> const& simptr = spmc->simParticle();
      const mu2e::SimParticle* sim              = simptr.get();
//-----------------------------------------------------------------------------
// a semi-kludge: store VD hits of e+ and e- from external photon conversions
//-----------------------------------------------------------------------------
      const mu2e::GenParticle* genp;

      if (sim->parent()) { 
	parent_id       = sim->parent()->id().asInt();
	parent_pdg_code = sim->parent()->pdgId();
	genp            = sim->parent()->genParticle().get();
      }
      else {
	genp      = sim->genParticle().get();
      }

      int generator_id(-1);
      if (genp) generator_id = genp->generatorId().id();   // ID of the MC generator
//-----------------------------------------------------------------------------
// if spmc_block->fGenProcessID is set, store only hits of the particles from 
// the signal process
//-----------------------------------------------------------------------------
      if ((spmc_block->GenProcessID() > 0) && (generator_id != spmc_block->GenProcessID())) continue;

      id            = sim->id().asInt();
      gen_index     = sim->generatorIndex();
      pdg_code      = sim->pdgId();
      creation_code = sim->creationCode();

      end_process_code = spmc->endProcessCode();

      edep_tot    = spmc->totalEDep();
      edep_nio    = spmc->nonIonizingEDep();

      if (_timeOffsets) {
//-----------------------------------------------------------------------------
// time - within the microbunch
//-----------------------------------------------------------------------------
	double tx = _timeOffsets->timeWithOffsetsApplied(*spmc);
	time = fmod(tx,_mbtime);
      }
      else time =  spmc->time();

      step_length = spmc->stepLength();

      spmc_block->NewStepPointMC(volume_id, gen_index, 
				 id       , pdg_code       ,
				 parent_id, parent_pdg_code, 
				 creation_code, end_process_code,
				 edep_tot,edep_nio,
				 time, step_length,
				 spmc->position().x(),
				 spmc->position().y(),
				 spmc->position().z(),
				 spmc->momentum().x(),
				 spmc->momentum().y(),
				 spmc->momentum().z());
    }
  }

  return 0;
}


