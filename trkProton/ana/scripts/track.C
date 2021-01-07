///////////////////////////////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////////////////////////////
#include "trkProton/ana/scripts/modules.hh"
#include "Stntuple/scripts/global_vars.h"

def_name proton_001("proton_ana001");
//def_name track_ce ("track_ana001");
//def_name track_dio("track_ana001");
///////////////////////////////////////////////////////////////////////////////



//-----------------------------------------------------------------------------
void  proton_ana001(int GenCode=28, double NEvents=-1) {
//-----------------------------------------------------------------------------
// configure analysis module
//-----------------------------------------------------------------------------
  m_ptrk = (TProtonAna001Module*) g.x->AddModule("TProtonAna001Module",0);  
  //  m_ptrk->SetDebugBit(43, 1);
  m_ptrk->SetNormalization(NEvents);
  m_ptrk->SetGeneratorCode(GenCode);

}
