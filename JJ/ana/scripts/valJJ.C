///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#include "Stntuple/scripts/global_vars.h"
#include "Stntuple/ana/scripts/modules.hh"

def_name val_052("valJJ_stn");

void valJJ_stn(const char* TrackBlockName, int PdgCode = 11, int GeneratorCode = 2) {
//-----------------------------------------------------------------------------
// configure validation module
//-----------------------------------------------------------------------------
  m_valJJ = (TValidationModuleJJ*) g.x->AddModule("TValidationModuleJJ",0);  
  m_valJJ->SetTrackBlockName (TrackBlockName);
  m_valJJ->SetPdgCode      (PdgCode);
  m_valJJ->SetGeneratorCode(GeneratorCode);
}
