//_____________________________________________________________________________
// Mar 10 2001 P.Murat: initialization of GENP block starting from the
//                      TMcGenEvent
//                      GENP block is missing event weight
// Mar 13 1017: Mu2e has only one vertex
//_____________________________________________________________________________
#include <cmath>
#include "TROOT.h"
#include "TFolder.h"
#include "TParticle.h"
#include "mc/base/TMcGenEvent.hh"

#include "Stntuple/obj/TStnNode.hh"
#include "Stntuple/obj/TStnEvent.hh"
#include "Stntuple/obj/TGenpBlock.hh"

//_____________________________________________________________________________
int StnGenInitGenpBlock(TStnDataBlock* Block, 
			TStnEvent*     Event, 
			Int_t          Mode ) 
{
  // init GENP block starting from the output of MC generators

  static TMcGenEvent* mcev = 0;

  int        passed, np, nint;

  if (! mcev) {
    mcev = (TMcGenEvent*) 
      gROOT->GetRootFolder()->FindObject("Ana/GeneratorModule/McGenEvent");
  }

  TGenpBlock* genp = (TGenpBlock*) Block;
  genp->Clear();
//-----------------------------------------------------------------------------
// this is just a convention - MC event should be named "McGenEvent"
//-----------------------------------------------------------------------------
  if (! mcev ) return 0;
  nint = mcev->NPrimaryInteractions();

  for (int i=0; i<nint; i++) {
    TPrimaryInteraction* pint = mcev->PrimaryInteraction(i);
    //    genp->AddPrimaryVertex();
    np = pint->NParticles();
    for (int ip=0; ip<np; ip++) {

      TParticle* part = pint->Particle(ip);

      passed = 1;
      // if (genp->UseCuts()) {
      // 	passed    = 0;
      // 	eta = part->Eta();
      // 	pt  = part->Pt();
	
      // 	if ( (pt > genp->PtMin()) && 
      // 	     (eta > genp->EtaMin()) && (eta <= genp->EtaMax()) ) {
      // 	  passed = 1;
      // 	}
      // }
      // if (! passed) goto NEXT_PARTICLE;

      // pdg_code = part->GetPdgCode();
      // ncodes = genp->NPdgCodes();
      // if (ncodes == 0) {
      // 					// if there is no codes defined, pass
      // 					// on all the particles
      // 	passed = 1;
      // }
      // else {
      // 	passed = 0;
      // 					// store only the particles with 
      // 					// PDG codes defined by the user

      // 	for (int i=0; i<genp->NPdgCodes(); i++) {
      // 	  if (TMath::Abs(pdg_code) == genp->PdgCode(i)) {
      // 	    passed = 1;
      // 	    break;
      // 	  }
      // 	}
      // }
      
      if (passed) {
	genp->NewParticle(part->GetPdgCode(),
			  1,
			  part->GetFirstMother(),
			  part->GetSecondMother(),
			  part->GetFirstDaughter(),
			  part->GetLastDaughter(),
			  part->Px(),part->Py(),part->Pz(),part->Energy(),
			  part->Vx(),part->Vy(),part->Vz(),part->T(),
			  -1.);
      }
      //    NEXT_PARTICLE:;
    }
  }

  return 0;
}
