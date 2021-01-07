///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#include "Stntuple/scripts/global_vars.h"
#include "Stntuple/ana/scripts/modules.hh"

def_name stn_trk_001("stn_trk_ana");

void stn_trk_ana(int PdgCode = 11, int GeneratorCode = 2) {
//-----------------------------------------------------------------------------
// configure validation module
//-----------------------------------------------------------------------------
  stntuple::m_trk = (TStnTrackAnaModule*) g.x->AddModule("TStnTrackAnaModule",0);  
  stntuple::m_trk->SetPdgCode      (PdgCode);
  stntuple::m_trk->SetGeneratorCode(GeneratorCode);
}
