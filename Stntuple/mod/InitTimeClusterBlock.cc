//-----------------------------------------------------------------------------
//  Apr 2016 G. Pezzullo: initialization of the MU2E STNTUPLE TimePeak block
//
//-----------------------------------------------------------------------------
#include <cstdio>
#include "TROOT.h"
#include "TFolder.h"
#include "TLorentzVector.h"
#include "TVector2.h"

#include "Stntuple/obj/TStnDataBlock.hh"
#include "Stntuple/obj/TStnEvent.hh"

#include "Stntuple/obj/TStnTimeCluster.hh"
#include "Stntuple/obj/TStnTimeClusterBlock.hh"

#include "Stntuple/obj/TStnHelix.hh"
#include "Stntuple/obj/TStnHelixBlock.hh"

#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Event.h"

#include "GeometryService/inc/GeometryService.hh"
#include "GeometryService/inc/GeomHandle.hh"

#include "CalorimeterGeom/inc/Calorimeter.hh"

#include "RecoDataProducts/inc/TimeCluster.hh"
#include "RecoDataProducts/inc/HelixSeed.hh"

#include "RecoDataProducts/inc/CaloCluster.hh"
//-----------------------------------------------------------------------------
// assume that the collection name is set, so we could grab it from the event
//-----------------------------------------------------------------------------
int  StntupleInitMu2eTimeClusterBlock(TStnDataBlock* Block, AbsEvent* Evt, int Mode) {

  const mu2e::TimeClusterCollection*               list_of_tclusters(0);

  char                       tcluster_module_label[100], tcluster_description[100]; 

  TStnTimeClusterBlock*         cb = (TStnTimeClusterBlock*) Block;
  TStnTimeCluster*              tcluster;

  cb->Clear();

  cb->GetModuleLabel("mu2e::TimeClusterCollection", tcluster_module_label);
  cb->GetDescription("mu2e::TimeClusterCollection", tcluster_description );

  art::Handle<mu2e::TimeClusterCollection>               tcluster_handle;
  if (tcluster_module_label[0] != 0){
    Evt->getByLabel(tcluster_module_label, tcluster_handle);
    if (tcluster_handle.isValid()) list_of_tclusters = tcluster_handle.product();
  }

  const mu2e::TimeCluster     *tmpTCl(0);
  int                          ntclusters(0);
  const mu2e::CaloCluster     *cluster(0);
 
  if (list_of_tclusters) ntclusters = list_of_tclusters->size();
  
  for (int i=0; i<ntclusters; i++) {
    tcluster               = cb->NewTimeCluster();
    tmpTCl                 = &list_of_tclusters->at(i);
    cluster                = tmpTCl->caloCluster().get();
    if (cluster != 0){
      mu2e::GeomHandle<mu2e::Calorimeter> ch;
      const mu2e::Calorimeter* _calorimeter = ch.get();      
      
      tcluster->fClusterTime    = cluster->time();
      tcluster->fClusterEnergy  = cluster->energyDep();
      CLHEP::Hep3Vector         gpos = _calorimeter->geomUtil().diskToMu2e(cluster->diskId(),cluster->cog3Vector());
      CLHEP::Hep3Vector         tpos = _calorimeter->geomUtil().mu2eToTracker(gpos);
      tcluster->fClusterX       = tpos.x();
      tcluster->fClusterY       = tpos.y();
      tcluster->fClusterZ       = tpos.z();
    }else {
      tcluster->fClusterTime    = 0; 
      tcluster->fClusterEnergy  = 0; 
      tcluster->fClusterX       = 0; 
      tcluster->fClusterY       = 0; 
      tcluster->fClusterZ       = 0; 
    }

    tcluster->fTimeCluster  = tmpTCl;
    tcluster->fNComboHits   = tmpTCl->hits().size();
    tcluster->fNHits        = tmpTCl->nStrawHits();
    tcluster->fT0           = tmpTCl->t0()._t0;
    tcluster->fT0Err        = tmpTCl->t0()._t0err;     
    tcluster->fPosX         = tmpTCl->position().x();     
    tcluster->fPosY         = tmpTCl->position().y();     
    tcluster->fPosZ         = tmpTCl->position().z();     
  }

  return 0;
}

//_____________________________________________________________________________
Int_t StntupleInitMu2eTimeClusterBlockLinks(TStnDataBlock* Block, AbsEvent* AnEvent, int Mode) 
{
  // Mu2e version, do nothing

  Int_t  ev_number, rn_number;

  ev_number = AnEvent->event();
  rn_number = AnEvent->run();

  if (! Block->Initialized(ev_number,rn_number)) return -1;

					// do not do initialize links 2nd time

  if (Block->LinksInitialized()) return 0;

  TStnEvent*                 ev;
  TStnTimeClusterBlock*      hb;
  TStnTimeCluster*           tcluster;
  TStnHelixBlock*            tsb;
  TStnHelix*                 helixseed;

  const mu2e::TimeCluster*   ktcluster, *fktcluster;
  const mu2e::HelixSeed*     kseed;

  char                       short_tcluster_block_name[100];

  ev     = Block->GetEvent();
  hb     = (TStnTimeClusterBlock*) Block;
  
  hb->GetModuleLabel("mu2e::TimeClusterCollection"  , short_tcluster_block_name);

  tsb    = (TStnHelixBlock*) ev->GetDataBlock(short_tcluster_block_name);
  
  int    ntcluster   = hb ->NTimeClusters();
  int    nhelixseed    = tsb->NHelices();

  for (int i=0; i<ntcluster; ++i){
    tcluster  = hb   ->TimeCluster(i);
    ktcluster = tcluster->fTimeCluster;
    int      helixseedIndex(-1);
    for (int j=0; j<nhelixseed; ++j){
      helixseed   = tsb->Helix(j);
      kseed       = helixseed->fHelix;
      fktcluster  = kseed->timeCluster().get();
      if (fktcluster == ktcluster) {
	helixseedIndex = j;
	break;
      }
    }
    
    if (helixseedIndex < 0) {
      printf(">>> ERROR: TimeClusterFinder timeCluster %i -> no HelixSeed associated\n", i);//FIXME!
	  continue;
    }
    
    tcluster->SetHelixSeedIndex(helixseedIndex);
  }

//-----------------------------------------------------------------------------
// mark links as initialized
//-----------------------------------------------------------------------------
  hb->fLinksInitialized = 1;

  return 0;
}

