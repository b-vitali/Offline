///////////////////////////////////////////////////////////////////////////////
// 2014-01-26 P.Murat
///////////////////////////////////////////////////////////////////////////////

#include "Stntuple/mod/InitStntupleDataBlocks.hh"
#include "Stntuple/obj/TStrawDataBlock.hh"

#include "RecoDataProducts/inc/StrawHitCollection.hh"

#include "MCDataProducts/inc/SimParticle.hh"
#include "MCDataProducts/inc/StepPointMC.hh"
#include "MCDataProducts/inc/StepPointMCCollection.hh"
#include "MCDataProducts/inc/StrawDigiMC.hh"
#include "MCDataProducts/inc/StrawDigiMCCollection.hh"

//-----------------------------------------------------------------------------
Int_t StntupleInitMu2eStrawDataBlock(TStnDataBlock* Block, AbsEvent* AnEvent, int Mode) 
{
  // initialize COT data block with the `event' data
  // return -1, if bank doesn't exist, 0, if everything is OK

  int ev_number, rn_number, nhits;

  static char   strh_module_label[100], strh_description[100];
  static char   sdmc_module_label[100], sdmc_description[100];

  ev_number = AnEvent->event();
  rn_number = AnEvent->run();

  if (Block->Initialized(ev_number,rn_number)) return 0;

  TStrawDataBlock* data = (TStrawDataBlock*) Block;
  data->Clear();
//-----------------------------------------------------------------------------
//  straw hit information
//-----------------------------------------------------------------------------
  data->GetModuleLabel("mu2e::StrawHitCollection",strh_module_label);
  data->GetDescription("mu2e::StrawHitCollection",strh_description );

  const mu2e::StrawHitCollection*       shc(nullptr);
  const mu2e::StrawDigiMCCollection* mcdigis;

  if (strh_module_label[0] != 0) {
    art::Handle<mu2e::StrawHitCollection> shcH;

    if (strh_description[0] == 0) AnEvent->getByLabel(strh_module_label,shcH);
    else                          AnEvent->getByLabel(strh_module_label,strh_description,shcH);

    if (shcH.isValid()) shc = shcH.product();

    data->GetModuleLabel("mu2e::StrawDigiMCCollection",sdmc_module_label);
    data->GetDescription("mu2e::StrawDigiMCCollection",sdmc_description );

    char sdmc_tag[100];
    strcpy(sdmc_tag,sdmc_module_label);
    if (sdmc_description[0] != 0) {
      strcat(sdmc_tag,":");
      strcat(sdmc_tag,sdmc_description);
    }
    
    auto mcdH = AnEvent->getValidHandle<mu2e::StrawDigiMCCollection>(sdmc_tag);
    mcdigis = mcdH.product();
  }

  if (shc == nullptr) {
    printf(" >>> ERROR in StntupleInitMu2eStrawDataBlock: no list_of_hits. BAIL OUT\n");
    return -1;
  }
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
  nhits = shc->size();

   
  const mu2e::StepPointMC* step;
  const mu2e::SimParticle* sim;

  TStrawHitData*           hit; 

  int   pdg_id, mother_pdg_id, sim_id, gen_index;
  float mc_mom;

  for (int i=0; i<nhits; i++) {
    const mu2e::StrawHit*    sh     = &shc->at(i);
    const mu2e::StrawDigiMC* mcdigi = &mcdigis->at(i);

    if (mcdigi->wireEndTime(mu2e::StrawEnd::cal) < mcdigi->wireEndTime(mu2e::StrawEnd::hv)) {
      step = mcdigi->stepPointMC(mu2e::StrawEnd::cal).get();
    }
    else {
      step = mcdigi->stepPointMC(mu2e::StrawEnd::hv ).get();
    }

    hit = data->NewHit();

    if (step) {
      art::Ptr<mu2e::SimParticle> const& simptr = step->simParticle(); 
      art::Ptr<mu2e::SimParticle> mother = simptr;

      while (mother->hasParent()) mother = mother->parent();

      sim = mother.operator ->();

      pdg_id        = simptr->pdgId();
      mother_pdg_id = sim->pdgId();

      if (simptr->fromGenerator()) gen_index = simptr->genParticle()->generatorId().id();
      else                         gen_index = -1;
      
      sim_id        = simptr->id().asInt();
      mc_mom        = step->momentum().mag();
    }
    else {
      pdg_id        = -1;
      mother_pdg_id = -1;
      gen_index     = -1;
      sim_id        = -1;
      mc_mom        = -1.;
    }

    hit->Set(sh->strawId().asUint16(), sh->time(), sh->dt(), sh->energyDep(),
	     pdg_id, mother_pdg_id, gen_index, sim_id, mc_mom);
    
  }

  data->f_RunNumber   = rn_number;
  data->f_EventNumber = ev_number;
  
  return 0;
}

