//-----------------------------------------------------------------------------
//  Apr 2013 P.Murat: initialization of the MU2E STNTUPLE cluster block
//
//-----------------------------------------------------------------------------
#include <cstdio>
#include "TROOT.h"
#include "TFolder.h"
#include "TLorentzVector.h"

#include "Stntuple/obj/TStnDataBlock.hh"

#include "Stntuple/obj/TStnPid.hh"
#include "Stntuple/obj/TStnPidBlock.hh"

#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Event.h"

#include "GeometryService/inc/GeometryService.hh"
#include "GeometryService/inc/GeomHandle.hh"

#include "RecoDataProducts/inc/AvikPIDNewProductCollection.hh"
//-----------------------------------------------------------------------------
// assume that the collection name is set, so we could grab it from the event
//-----------------------------------------------------------------------------
int  StntupleInitMu2ePidBlock(TStnDataBlock* Block, AbsEvent* Evt, int Mode) {

  mu2e::AvikPIDNewProductCollection*  list_of_pid;
  int           npid;
  static char   pid_module_label[100], pid_description[100]; 

  TStnPidBlock* block = (TStnPidBlock*) Block;
  TStnPid*      pid;

  block->Clear();

  //
  // "makeCaloCluster" would be the process name, "AlgoCLOSESTSeededByENERGY" - the description,
  // 
  
  block->GetModuleLabel("mu2e::AvikPIDNewProductCollection",pid_module_label);
  block->GetDescription("mu2e::AvikPIDNewProductCollection",pid_description);

  art::Handle<mu2e::AvikPIDNewProductCollection> pid_handle;
  if (pid_description[0] == 0) Evt->getByLabel(pid_module_label,pid_handle);
  else                         Evt->getByLabel(pid_module_label,pid_description,pid_handle);
  list_of_pid = (mu2e::AvikPIDNewProductCollection*) pid_handle.product();
//-----------------------------------------------------------------------------
// tracks are supposed to be already initialized
//-----------------------------------------------------------------------------
  const mu2e::AvikPIDNewProduct   *avik;
  
  npid = list_of_pid->size();
  for (int i=0; i<npid; i++) {
    pid                    = block->NewPid();
    avik                   = &list_of_pid->at(i);
    pid->fAvikPid          = avik;
    pid->fTrkNumber        = avik->trkID();
//-----------------------------------------------------------------------------
// dedx and Vadim's drds and dx/ds
//-----------------------------------------------------------------------------
    pid->fLogDedxProbEle   = avik->logDedxProbEle();
    pid->fLogDedxProbMuo   = avik->logDedxProbMuo();

    pid->fDrdsVadim        = avik->drdsVadim();
    pid->fDrdsVadimErr     = avik->drdsVadimErr();
//-----------------------------------------------------------------------------
// Avik: 'sum' is the Sum(dr^2), sq2 is the Sum(wr), where wr is the 'weighed residual'
// see ParticleID/src/AvikPID_module.cc for the definition
//-----------------------------------------------------------------------------
    pid->fNMatched         = avik->nMatched();
    pid->fNMatchedAll      = avik->nMatchedAll();
    pid->fSumAvik          = avik->sumAvik();
    pid->fSq2Avik          = avik->sq2Avik();
//-----------------------------------------------------------------------------
// these are not defined, should not be better than all hits
//-----------------------------------------------------------------------------
    pid->fDrdsOs        = avik->drdsOs();
    pid->fDrdsOsErr     = avik->drdsOsErr();
    pid->fNUsedOsH      = avik->nUsedOsH();
//-----------------------------------------------------------------------------
// Avik: weighted sum of doublet residuals (drds(d)-drds(trk))
//-----------------------------------------------------------------------------
    pid->fSumAvikOs     = avik->sumAvikOs();
    pid->fNUsedOsD      = avik->nUsedOsD();
//-----------------------------------------------------------------------------
// dr/ds fit using SS doublets - useless
//-----------------------------------------------------------------------------
    pid->fNUsedSsH      = avik->nUsedSsH();

    pid->fDrdsSs        = avik->drdsSs();
    pid->fDrdsSsErr     = avik->drdsSsErr();
  }
  return 0;
}

//_____________________________________________________________________________
Int_t StntupleInitMu2ePidBlockLinks(TStnDataBlock* Block, AbsEvent* AnEvent, int Mode) 
{
  // Mu2e version, do nothing

  Int_t  ev_number, rn_number;

  ev_number = AnEvent->event();
  rn_number = AnEvent->run();

  if (! Block->Initialized(ev_number,rn_number)) return -1;

					// do not do initialize links 2nd time

  if (Block->LinksInitialized()) return 0;

  TStnPidBlock* header = (TStnPidBlock*) Block;
  //  TStnEvent* ev   = header->GetEvent();
//-----------------------------------------------------------------------------
// mark links as initialized
//-----------------------------------------------------------------------------
  header->fLinksInitialized = 1;

  return 0;
}

