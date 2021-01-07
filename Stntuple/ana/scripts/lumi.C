///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#include "Stntuple/scripts/global_vars.h"
#include "Stntuple/ana/scripts/modules.hh"

def_name lumi_001("lumi");
//-----------------------------------------------------------------------------
// standard luminosity studies
//-----------------------------------------------------------------------------
void lumi() {
  stntuple::m_lum = (TStnLumiMonModule*) g.x->AddModule("TStnLumiMonModule",0);
}
