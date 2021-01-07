///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#include "Stntuple/scripts/global_vars.h"
#include "Stntuple/ana/scripts/modules.hh"

def_name validation_005("val_stn");
def_name validation_006("evd");

void val_stn(int PdgCode = 11, int GeneratorCode = 2) {
//-----------------------------------------------------------------------------
// configure validation module
//-----------------------------------------------------------------------------
  stntuple::m_val = (TStnValidationModule*) g.x->AddModule("TStnValidationModule",0);  
  stntuple::m_val->SetPdgCode      (PdgCode);
  stntuple::m_val->SetGeneratorCode(GeneratorCode);
}


//-----------------------------------------------------------------------------
void  evd(int DebugBit = -1) {
//-----------------------------------------------------------------------------
// configure analysis module to write TMVA training trees
//-----------------------------------------------------------------------------
  stntuple::m_evd = (TStnEventDisplayModule*) g.x->AddModule("TStnEventDisplayModule",0);
  stntuple::m_evd->SetDisplayBeamMuMinus(1);
  stntuple::m_evd->SetDisplayBeamMuPlus (1);

  if (DebugBit >= 0) stntuple::m_evd->SetDebugBit(DebugBit,1);
}

