///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

#include "Stntuple/mod/InitCrvClusterBlock.hh"
#include "RecoDataProducts/inc/CrvRecoPulse.hh"
#include "RecoDataProducts/inc/CrvCoincidenceCollection.hh"
#include "RecoDataProducts/inc/CrvCoincidenceCluster.hh"

//-----------------------------------------------------------------------------
// in this case AbsEvent is just not used
//-----------------------------------------------------------------------------
int StntupleInitCrvClusterBlock::InitDataBlock(TStnDataBlock* Block, AbsEvent* Event, int Mode) {

  int ev, rn, sr;

  ev = Event->event();
  rn = Event->run();
  sr = Event->subRun();

  if (Block->Initialized(ev,rn,sr)) return 0;
  
  TCrvClusterBlock* block = (TCrvClusterBlock*) Block;

  block->f_EventNumber  = ev;
  block->f_RunNumber    = rn;
  block->f_SubrunNumber = sr;
//-----------------------------------------------------------------------------
// initialize pointer to the pulse collection
//-----------------------------------------------------------------------------
  art::Handle<mu2e::CrvRecoPulseCollection> crpch;
  const mu2e::CrvRecoPulseCollection*       crpc(nullptr);
  int   ncrp(0);

  if (! fCrvRecoPulseCollTag.empty() != 0) {
    bool ok = Event->getByLabel(fCrvRecoPulseCollTag,crpch);
    if (ok) { 
      crpc = crpch.product();
      ncrp = crpc->size();
    }
  }

  const mu2e::CrvRecoPulse* p0(nullptr);
  if (ncrp > 0) p0 = &crpc->at(0);
//-----------------------------------------------------------------------------
// store CrvCoincidenceCluster's
//-----------------------------------------------------------------------------
  art::Handle<mu2e::CrvCoincidenceClusterCollection> cccch;
  const mu2e::CrvCoincidenceClusterCollection*       cccc(nullptr);
  int                                                nccc(0);

  if (! fCrvCoincidenceClusterCollTag.empty()) {
    bool ok = Event->getByLabel(fCrvCoincidenceClusterCollTag,cccch);
    if (ok) { 
      cccc = cccch.product();
      nccc = cccc->size();
    }
  }

  //  block->fNClusters = nccc;

  for (int iccc=0; iccc<nccc; iccc++) {
    const mu2e::CrvCoincidenceCluster* ralph_ccc = &cccc->at(iccc);

    TCrvCoincidenceCluster* ccc = block->NewCluster();

    const std::vector<art::Ptr<mu2e::CrvRecoPulse>>* list_of_pulses = &ralph_ccc->GetCrvRecoPulses();

    int    sector = ralph_ccc->GetCrvSectorType();
    int    np     = list_of_pulses->size();
    int    npe    = ralph_ccc->GetPEs();

    double x      = ralph_ccc->GetAvgCounterPos().x();
    double y      = ralph_ccc->GetAvgCounterPos().y();
    double z      = ralph_ccc->GetAvgCounterPos().z();
    float  t1     = ralph_ccc->GetStartTime();
    float  t2     = ralph_ccc->GetEndTime();

    ccc->Set(iccc,sector,np,npe,x,y,z,t1,t2);
//-----------------------------------------------------------------------------
// now store pulses associated with the cluster
//-----------------------------------------------------------------------------
    for (int ip=0; ip<np; ip++) {
      const mu2e::CrvRecoPulse* ralph = list_of_pulses->at(ip).get();
      int index = ralph-p0;
//-----------------------------------------------------------------------------
// check if the pulse is already stored
//-----------------------------------------------------------------------------
      int loc(-1);
      int npulses = block->NPulses();

      for (int i=0; i<npulses; i++) {
	TCrvRecoPulse* p = block->Pulse(i);
	if (p->Index() == index) {
	  loc   = i;
	  break;
	}
      }
      if (loc == -1) {
//-----------------------------------------------------------------------------
// add pulse to the list
//-----------------------------------------------------------------------------
	loc                  = npulses;

	TCrvRecoPulse* pulse = block->NewPulse();

	int   npes           = ralph->GetPEs();
	int   npes_height    = ralph->GetPEsPulseHeight();
	int   nind           = ralph->GetWaveformIndices().size();
	int   bar            = ralph->GetScintillatorBarIndex().asInt();
	int   sipm           = ralph->GetSiPMNumber();

	float time           = ralph->GetPulseTime();
	float height         = ralph->GetPulseHeight();
	float width          = ralph->GetPulseBeta(); // was GetPulseWidth();
	float chi2           = ralph->GetPulseFitChi2();
	float le_time        = ralph->GetLEtime();

	pulse->Set(index,npes,npes_height,nind,bar,sipm,time,height,width,chi2,le_time);
      }
      block->fClusterPulseLinks->Add(iccc,loc);
    }
  }
  return 0;
}


//-----------------------------------------------------------------------------
// keep this function as an example, don't really need it
//-----------------------------------------------------------------------------
int StntupleInitCrvClusterBlock::ResolveLinks(TStnDataBlock* Block, AbsEvent* Event, int Mode) {

  int ev, rn, sr;
  
  ev = Event->event();
  rn = Event->run();
  sr = Event->subRun();
  
  if (! Block->Initialized(ev,rn,sr)) return -1;

  return 0;
}
