///////////////////////////////////////////////////////////////////////////////
// 2014-01-26 P.Murat
///////////////////////////////////////////////////////////////////////////////

// conditions
#include "ConditionsService/inc/ConditionsHandle.hh"
#include "ConditionsService/inc/AcceleratorParams.hh"

#include "Stntuple/mod/InitStntupleDataBlocks.hh"
#include "Stntuple/obj/TVDetDataBlock.hh"

#include "MCDataProducts/inc/SimParticle.hh"
#include "MCDataProducts/inc/StepPointMC.hh"
#include "MCDataProducts/inc/StepPointMCCollection.hh"
#include "MCDataProducts/inc/PtrStepPointMCVectorCollection.hh"

#include "GlobalConstantsService/inc/GlobalConstantsHandle.hh"
#include "GlobalConstantsService/inc/ParticleDataTable.hh"

#include "Mu2eUtilities/inc/SimParticleTimeOffset.hh"

#include "Stntuple/mod/THistModule.hh"
#include "Stntuple/base/TNamedHandle.hh"

//-----------------------------------------------------------------------------
Int_t StntupleInitMu2eVDetDataBlock(TStnDataBlock* Block, AbsEvent* AnEvent, int Mode) 
{
  static int    initialized(0);
  static char   oname[] = "StntupleInitMu2eVirtualDataBlock";

  static mu2e::SimParticleTimeOffset* _timeOffsets(NULL);

  static char   step_module_label[100], step_description[100];
  int           ev_number, rn_number, nhits;
  char          module_name      [100], time_offsets_name[100];

  ev_number = AnEvent->event();
  rn_number = AnEvent->run();

  if (Block->Initialized(ev_number,rn_number)) return 0;

  TVDetDataBlock* data = (TVDetDataBlock*) Block;
  data->Clear();

  if (initialized == 0) {
    initialized = 1;

    data->GetModuleLabel("TimeOffsetMapsHandle",module_name);
    data->GetDescription("TimeOffsetMapsHandle",time_offsets_name);

   THistModule*  m  = static_cast<THistModule*>  (THistModule::GetListOfModules()->FindObject(module_name));
   TNamedHandle* nh = static_cast<TNamedHandle*> (m->GetFolder()->FindObject(time_offsets_name));
   _timeOffsets     = static_cast<mu2e::SimParticleTimeOffset*> (nh->Object());
  }

//-----------------------------------------------------------------------------
//  virtual hit information
//-----------------------------------------------------------------------------
  data->GetModuleLabel("mu2e::StepPointMCCollection",step_module_label);
  data->GetDescription("mu2e::StepPointMCCollection",step_description);

  art::Handle<mu2e::StepPointMCCollection>       step_handle;
  const mu2e::StepPointMCCollection*             list_of_hits(0);

  static mu2e::GlobalConstantsHandle<mu2e::ParticleDataTable> pdt;
  mu2e::ParticleDataTable::maybe_ref info;

  mu2e::ConditionsHandle<mu2e::AcceleratorParams> accPar("ignored");
  double _mbtime = accPar->deBuncherPeriod;
  
  if (step_module_label[0] != 0) {
    if (step_description[0] != 0) 
      AnEvent->getByLabel(step_module_label, step_description, step_handle);
    if (step_handle.isValid()) list_of_hits = step_handle.product();
  }

  if (list_of_hits == NULL) {
    char warning[100];
    sprintf(warning," WARNING: StepPointMCCollection by %s:%s is missing. BAIL OUT\n",
	   step_module_label,step_description);
    mf::LogWarning(oname) << warning;
    return -1;
  }

  // load simulation time offsets for this event: timeOffsets may not be defined yet (stages 1, 2, 3)
  if (_timeOffsets) _timeOffsets->updateMap(*AnEvent);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
  nhits = list_of_hits->size();

  const mu2e::StepPointMC* step;
  art::Ptr<mu2e::SimParticle> sim;

  TVDetHitData*           hit; 

  int                     vdIndex, pdg_id, gen_index;
  float time;
  float energyKin, energy;
  float mass;
  float mc_mom;
  float mc_momX, mc_momY, mc_momZ;
  float mc_posX, mc_posY, mc_posZ;

  float min_energy(40.);

  for (int i=0; i<nhits; i++) {
    step  = &list_of_hits->at(i);

    sim = step->simParticle();
    if (!(sim->fromGenerator())) goto NEXT_VHIT;

    vdIndex   = step->volumeId();
    if (_timeOffsets) {
//-----------------------------------------------------------------------------
// time - within the microbunch
//-----------------------------------------------------------------------------
      double tx = _timeOffsets->timeWithOffsetsApplied(*step);
      time = fmod(tx,_mbtime);
    }
    else              time =  step->time();

    pdg_id    = sim->pdgId();
    info      = pdt->particle(pdg_id);    
    
    mass      = info.ref().mass();
    energy    = sqrt(step->momentum().mag2() + mass*mass);
    energyKin = energy - mass;
    if (energy < min_energy) goto NEXT_VHIT;

    gen_index = sim->genParticle()->generatorId().id();
          
    mc_mom    = step->momentum().mag();
    mc_momX   = step->momentum().x();
    mc_momY   = step->momentum().y();
    mc_momZ   = step->momentum().z();
//-----------------------------------------------------------------------------
// poor-man global-to-detector (tracker) coordinate system transformation
//-----------------------------------------------------------------------------
    mc_posX   = step->position().x()+3904.;
    mc_posY   = step->position().y();
    mc_posZ   = step->position().z()-10200;

    hit = data->NewHit();
    hit->Set(vdIndex, time, mass, energyKin, energy, 
	     pdg_id, gen_index, 
	     mc_mom, mc_momX, mc_momY, mc_momZ,
	     mc_posX, mc_posY, mc_posZ);
  NEXT_VHIT:;
  }

  data->f_RunNumber   = rn_number;
  data->f_EventNumber = ev_number;
  
  return 0;
}

