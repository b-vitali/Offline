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
#include "TEllipse.h"
#include "TObjArray.h"
#include "CLHEP/Vector/ThreeVector.h"

#include "art/Framework/Principal/Handle.h"

#include "GeometryService/inc/GeometryService.hh"
#include "GeometryService/inc/GeomHandle.hh"

#include "TrackerGeom/inc/Straw.hh"
//#include "TrackerConditions/inc/Types.hh"

#include "Stntuple/gui/TEvdStraw.hh"
#include "Stntuple/gui/TEvdStrawHit.hh"
#include "Stntuple/gui/TStnVisManager.hh"

ClassImp(TEvdStrawHit)

//-----------------------------------------------------------------------------
TEvdStrawHit::TEvdStrawHit() {
}

//-----------------------------------------------------------------------------
TEvdStrawHit::TEvdStrawHit(const mu2e::ComboHit*    Hit,
			   TEvdStraw*               Straw,
			   const mu2e::StrawDigiMC* StrawDigiMC,
			   double X, double Y, double Z, 
			   double                Wx,
			   double                Wy,
			   double                SigW,
			   double                SigR,
			   int                   Mask, 
			   int                   Color): 
  TObject(),
  fHit(Hit),
  fStrawDigiMC(StrawDigiMC),
  fStraw(Straw),
  fPos(X,Y,Z),
  fDir(Wx,Wy),
  fEllipse()
 {
  fSigW  = SigW;
  fSigR  = SigR;
  fMask  = Mask;
  fColor = Color;
//-----------------------------------------------------------------------------
// define lines
//-----------------------------------------------------------------------------
  fLineW.SetX1(fPos.X()-fDir.X()*fSigW);
  fLineW.SetY1(fPos.Y()-fDir.Y()*fSigW);
  fLineW.SetX2(fPos.X()+fDir.X()*fSigW);
  fLineW.SetY2(fPos.Y()+fDir.Y()*fSigW);
  fLineW.SetLineColor(Color);

  fLineR.SetX1(fPos.X()+fDir.Y()*fSigR);
  fLineR.SetY1(fPos.Y()-fDir.X()*fSigR);
  fLineR.SetX2(fPos.X()-fDir.Y()*fSigR);
  fLineR.SetY2(fPos.Y()+fDir.X()*fSigR);
  fLineR.SetLineColor(Color);

  const CLHEP::Hep3Vector* mid_point = &fStraw->GetStraw()->getMidPoint();
  
  double rdrift(0.);
  if (fStrawDigiMC) {
    if (fStrawDigiMC->wireEndTime(mu2e::StrawEnd::cal) < fStrawDigiMC->wireEndTime(mu2e::StrawEnd::hv)) {
      rdrift = fStrawDigiMC->driftDistance(mu2e::StrawEnd::cal);
    }
    else {
      rdrift = fStrawDigiMC->driftDistance(mu2e::StrawEnd::hv);
    }
  }
      
  fEllipse.SetX1(mid_point->z());
  fEllipse.SetY1(mid_point->perp());
  fEllipse.SetR1(rdrift);
  fEllipse.SetR2(rdrift);
  fEllipse.SetFillStyle(3003);
  fEllipse.SetFillColor(kBlue+2);
  fEllipse.SetLineColor(kBlue+2);
}

//-----------------------------------------------------------------------------
TEvdStrawHit::~TEvdStrawHit() {
}

//-----------------------------------------------------------------------------
void TEvdStrawHit::Paint(Option_t* Option) {
  const char oname[] = "TEvdStrawHit::Paint";

  //  int   iv;

  const char* view = TVisManager::Instance()->GetCurrentView();


  if      (strstr(view,"trkxy" ) != 0) PaintXY(Option);
  else if (strstr(view,"trkrz" ) != 0) PaintRZ(Option);
  else if (strstr(view,"cal"   ) != 0) PaintCal(Option);
  else {
    printf("[%s] >>> ERROR: unknown view: %s, DO NOTHING\n",oname,view);
  }

  gPad->Modified();
}

//_____________________________________________________________________________
void TEvdStrawHit::PaintXY(Option_t* Option) {
  fLineW.Paint(Option);
  fLineR.Paint(Option);
}

//_____________________________________________________________________________
void TEvdStrawHit::PaintRZ(Option_t* Option) {
  fEllipse.SetFillColor(kBlue+2);
  fEllipse.SetFillStyle(3001);
  fEllipse.Paint(Option);
}

//_____________________________________________________________________________
void TEvdStrawHit::PaintCal(Option_t* option) {
  // nothing to draw...
}


//_____________________________________________________________________________
Int_t TEvdStrawHit::DistancetoPrimitive(Int_t px, Int_t py) {
  return 9999;
}

//_____________________________________________________________________________
Int_t TEvdStrawHit::DistancetoPrimitiveXY(Int_t px, Int_t py) {

  Int_t dist = 9999;

  static TVector3 global;
//   static TVector3 local;

//   Double_t    dx1, dx2, dy1, dy2, dx_min, dy_min, dr;

  global.SetXYZ(gPad->AbsPixeltoX(px),gPad->AbsPixeltoY(py),0);

  return dist;
}

//_____________________________________________________________________________
Int_t TEvdStrawHit::DistancetoPrimitiveRZ(Int_t px, Int_t py) {
  return 9999;
}

