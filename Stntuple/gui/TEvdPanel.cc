///////////////////////////////////////////////////////////////////////////////
// May 04 2013 P.Murat
// 
// in 'XY' mode draw calorimeter clusters as circles with different colors 
// in 'Cal' mode draw every detail...
///////////////////////////////////////////////////////////////////////////////
#include "TVirtualX.h"
#include "TPad.h"
#include "TStyle.h"
#include "TVector3.h"
#include "TLine.h"
#include "TArc.h"
#include "TArrow.h"
#include "TMath.h"
#include "TBox.h"
#include "TObjArray.h"

#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"

#include "GeometryService/inc/GeometryService.hh"
#include "GeometryService/inc/GeomHandle.hh"

// #include "TrackerGeom/inc/Layer.hh"
#include "TrackerGeom/inc/Straw.hh"
// #include "TrackerGeom/inc/Sector.hh"

#include "Stntuple/gui/TEvdPanel.hh"
#include "Stntuple/gui/TEvdStraw.hh"
#include "Stntuple/gui/TStnVisManager.hh"

#include "TrackerGeom/inc/Panel.hh"
//#include "TTrackerGeom/inc/ZLayer.hh"

ClassImp(TEvdPanel)

namespace {
  static int fgLocalDebug = 0;
};

//_____________________________________________________________________________
TEvdPanel::TEvdPanel(): TObject() {
}

//_____________________________________________________________________________
TEvdPanel::TEvdPanel(int ID, const mu2e::Panel* Panel, TEvdPlane* Plane): TObject() {

  TEvdStraw* evd_straw;

  fID      = ID;
  fNLayers = Panel->nLayers();
  fPanel   = Panel;
					// assume that the number of straws is the same
  int id;

  for (int il=0; il<fNLayers; il++) {
    //    const mu2e::Layer* layer = &fPanel->getLayer(il);

    fNStraws     [il] = Panel->nStraws()/2;
    fListOfStraws[il] = new TObjArray(fNStraws[il]);
  }

  int nst = Panel->nStraws();

  for (uint16_t ist=0; ist<nst; ist++) {
    const mu2e::Straw* straw = &Panel->getStraw(ist);

    //    const mu2e::Layer* layer = &fPanel->getLayer(il);

    id        = straw->id().asUint16();
    
    int ill      = straw->id().getLayer();
    int iss      = straw->id().getStraw();
    int ipp      = straw->id().getPanel();
    int istation = straw->id().getStation();

    if (fgLocalDebug != 0) {
      printf(" station, panel, layer, straw, z : %3i %3i %3i %3i %10.3f\n",
	     istation, ipp, ill, iss, straw->getMidPoint().z());
    }

    evd_straw = new TEvdStraw(id,straw,this);
    fListOfStraws[ill]->Add(evd_straw);
  }
}

//_____________________________________________________________________________
TEvdPanel::~TEvdPanel() {
}

//-----------------------------------------------------------------------------
void TEvdPanel::Paint(Option_t* option) {


  const char* view = TVisManager::Instance()->GetCurrentView();

  if      (strstr(view,"trkxy" ) != 0) PaintXY (option);
  else if (strstr(view,"trkrz" ) != 0) PaintRZ (option);
  else {
    // what is the default?
    //    Warning("Paint",Form("Unknown option %s",option));
  }

  gPad->Modified();
}


//_____________________________________________________________________________
void TEvdPanel::PaintXY(Option_t* Option) {
}



//_____________________________________________________________________________
void TEvdPanel::PaintRZ(Option_t* option) {
  // draw straws
}

//_____________________________________________________________________________
Int_t TEvdPanel::DistancetoPrimitive(Int_t px, Int_t py) {
  return 9999;
}

//_____________________________________________________________________________
Int_t TEvdPanel::DistancetoPrimitiveXY(Int_t px, Int_t py) {

  Int_t dist = 9999;

  static TVector3 global;
//   static TVector3 local;

  //  Double_t    dx1, dx2, dy1, dy2, dx_min, dy_min, dr;

  global.SetXYZ(gPad->AbsPixeltoX(px),gPad->AbsPixeltoY(py),0);

  return dist;
}

//_____________________________________________________________________________
Int_t TEvdPanel::DistancetoPrimitiveRZ(Int_t px, Int_t py) {
  return 9999;
}

