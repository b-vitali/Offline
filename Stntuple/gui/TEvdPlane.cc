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

#include "Stntuple/gui/TEvdStation.hh"
#include "Stntuple/gui/TEvdPlane.hh"
#include "Stntuple/gui/TEvdPanel.hh"
#include "Stntuple/gui/TStnVisManager.hh"

#include "TrackerGeom/inc/Plane.hh"


ClassImp(TEvdPlane)

//_____________________________________________________________________________
TEvdPlane::TEvdPlane(): TObject() {
  fID           = -1;
  fListOfPanels = NULL;
  fNPanels      = 0;
}

//_____________________________________________________________________________
TEvdPlane::TEvdPlane(int ID, const mu2e::Plane* Plane, TEvdStation* Station): TObject() {

  int        id;
  TEvdPanel*  evd_panel;

  fID      = ID;
  fStation = Station;
  fPlane   = Plane;
  fNPanels  = Plane->nPanels();

  fListOfPanels = new TObjArray(fNPanels);

  for (int i=0; i<fNPanels; i++) {
    const mu2e::Panel* panel = &fPlane->getPanel(i);

    id       = -1; // fNPanels*Plane->id()+i;
    evd_panel = new TEvdPanel(id,panel,this);

    fListOfPanels->Add(evd_panel);
  }
}

//_____________________________________________________________________________
TEvdPlane::~TEvdPlane() {
  fListOfPanels->Delete();
  delete fListOfPanels;
}

//-----------------------------------------------------------------------------
void TEvdPlane::Paint(Option_t* option) {
  // paints one disk (.. or vane, in the past), i.e. section

				// parse option list

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
void TEvdPlane::PaintXY(Option_t* Option) {
}



//_____________________________________________________________________________
void TEvdPlane::PaintRZ(Option_t* option) {
}

//_____________________________________________________________________________
Int_t TEvdPlane::DistancetoPrimitive(Int_t px, Int_t py) {
  return 9999;
}

//_____________________________________________________________________________
Int_t TEvdPlane::DistancetoPrimitiveXY(Int_t px, Int_t py) {

  Int_t dist = 9999;

  static TVector3 global;
//   static TVector3 local;

  //  Double_t    dx1, dx2, dy1, dy2, dx_min, dy_min, dr;

  global.SetXYZ(gPad->AbsPixeltoX(px),gPad->AbsPixeltoY(py),0);

  return dist;
}

//_____________________________________________________________________________
Int_t TEvdPlane::DistancetoPrimitiveRZ(Int_t px, Int_t py) {
  return 9999;
}

