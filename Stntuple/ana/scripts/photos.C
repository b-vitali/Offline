///////////////////////////////////////////////////////////////////////////////
// mode = 1 : decay of a resting 210 MeV "muon" resulting in a 105 MeV/c electron
//        2 : mu->e conversion
//        3 : DIO : (103.0+2.0*fRn3.Rndm())*1.e-3; parameterization in (emax-e)/mmu rather than in (emax-e)
//        4 : full range LO DIO spectrum
//        5 : mu --> e nu nu decay modeled as X --> e Al
//        6 : mu --> e nu nu decay modeled as X --> e Y
///////////////////////////////////////////////////////////////////////////////
#include "Stntuple/ana/scripts/modules.hh"

def_name photos_001("photos");
def_name photos_002("photos_no_isec");
def_name photos_003("photos_iexp");

//-----------------------------------------------------------------------------
// use PHOTOS, Mode=2: CE, 3:high end of the DIO, 4: DIO
//-----------------------------------------------------------------------------
void  photos(int Mode = 1, int DebugLevel = 0) {
  TPhotos* ph = TPhotos::Instance();

  ph->SetMode(Mode);
  ph->SetDebugLevel(DebugLevel);

  printf("photos: Setting XPHCUT to 1.e-4\n");
  ph->SetXPHCUT(1.e-6);
  printf("XPHCUT = %12.5f\n",ph->GetPhocop()->XPHCUT);
  
  stntuple::m_pho = (TStnPhotosAnaModule*) g.x->AddModule("TStnPhotosAnaModule",0);
  g.x->SetNEventsToReport(10000);
}

//-----------------------------------------------------------------------------
// use PHOTOS, Mode=2: CE, 3:high end of the DIO, 4: DIO
//-----------------------------------------------------------------------------
void  photos_no_isec(int Mode = 2, int DebugLevel = 0) {
  TPhotos* ph = TPhotos::Instance();

  ph->SetMode(Mode);
  ph->SetDebugLevel(DebugLevel);

  printf("photos: Setting XPHCUT to 1.e-4\n");
  ph->SetXPHCUT(1.e-6);
  printf("XPHCUT = %12.5f\n",ph->GetPhocop()->XPHCUT);

  ph->SetISEC(0);
  
  stntuple::m_pho = (TStnPhotosAnaModule*) g.x->AddModule("TStnPhotosAnaModule",0);
  g.x->SetNEventsToReport(10000);
}

//-----------------------------------------------------------------------------
// use PHOTOS, Mode=2: CE, 3:high end of the DIO, 4: DIO
//-----------------------------------------------------------------------------
void  photos_iexp(int Mode = 2, int DebugLevel = 0) {
  TPhotos* ph = TPhotos::Instance();

  ph->SetMode(Mode);
  ph->SetDebugLevel(DebugLevel);

  printf("photos: Setting XPHCUT to 1.e-4\n");
  ph->SetXPHCUT(1.e-6);
  printf("XPHCUT = %12.5f\n",ph->GetPhocop()->XPHCUT);

  ph->SetIEXP(1);
  
  stntuple::m_pho = (TStnPhotosAnaModule*) g.x->AddModule("TStnPhotosAnaModule",0);
  g.x->SetNEventsToReport(10000);
}

