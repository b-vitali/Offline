///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#include "Stntuple/scripts/global_vars.h"
#include "Stntuple/ana/scripts/modules.hh"

def_name stn_helix_001("stn_helix_ana");
def_name stn_helix_002("stn_helix_ana_old");


//-----------------------------------------------------------------------------
void  stn_helix_ana(int DebugBit = -1) {
  stntuple::m_hel = (TStnHelixAnaModule*) g.x->AddModule("TStnHelixAnaModule",0);  

  if (DebugBit >= 0)stntuple::m_hel->SetDebugBit(DebugBit,1);
}
//-----------------------------------------------------------------------------
void  stn_helix_ana_old(int DebugBit = -1) {
  stntuple::m_hel = (TStnHelixAnaModule*) g.x->AddModule("TStnHelixAnaModule",0);

  stntuple::m_hel->SetHelixBlockName(0,"HelixBlockTpr");
  stntuple::m_hel->SetHelixBlockName(1,"HelixBlockCpr");
  stntuple::m_hel->SetHelixBlockName(2,"HelixBlock"   );

  if (DebugBit >= 0) stntuple::m_hel->SetDebugBit(DebugBit,1);
}
