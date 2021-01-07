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

#include "Stntuple/gui/TEvdPlane.hh"
#include "Stntuple/gui/TEvdStation.hh"
#include "Stntuple/gui/TStnVisManager.hh"


ClassImp(TEvdStation)

//_____________________________________________________________________________
TEvdStation::TEvdStation(): TObject() {
  fListOfPlanes = NULL;
}

//_____________________________________________________________________________
TEvdStation::TEvdStation(int ID, const mu2e::Tracker* Tracker): TObject() {

  int         id;
  TEvdPlane*  evd_plane;

  fID      = ID;
  //  fStation = Station;
  fNPlanes = 2; // was Station->nPlanes();

  fListOfPlanes = new TObjArray(fNPlanes);

  for (int i=0; i<fNPlanes; i++) {
    id = 2*ID+i;
    const mu2e::Plane* plane = &Tracker->getPlane(id);

    evd_plane = new TEvdPlane(id,plane,this);

    fListOfPlanes->Add(evd_plane);
  }
}

//_____________________________________________________________________________
TEvdStation::~TEvdStation() {
  fListOfPlanes->Delete();
  delete fListOfPlanes;
}

//-----------------------------------------------------------------------------
void TEvdStation::Paint(Option_t* option) {
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
void TEvdStation::PaintXY(Option_t* Option) {
}



//_____________________________________________________________________________
void TEvdStation::PaintRZ(Option_t* option) {
}

//_____________________________________________________________________________
Int_t TEvdStation::DistancetoPrimitive(Int_t px, Int_t py) {
  return 9999;
}

//_____________________________________________________________________________
Int_t TEvdStation::DistancetoPrimitiveXY(Int_t px, Int_t py) {

  Int_t dist = 9999;

  static TVector3 global;
//   static TVector3 local;

  //  Double_t    dx1, dx2, dy1, dy2, dx_min, dy_min, dr;

  global.SetXYZ(gPad->AbsPixeltoX(px),gPad->AbsPixeltoY(py),0);

  return dist;
}

//_____________________________________________________________________________
Int_t TEvdStation::DistancetoPrimitiveRZ(Int_t px, Int_t py) {
  return 9999;
}

