


///////////////////////////////////////////////////////////////////////////////
// May 04 2013 P.Murat
// 
// in 'XY' mode draw calorimeter clusters as circles with different colors 
// in 'Cal' mode draw every detail...
//
// BaBar interface:
// ----------------
//      r     = fabs(1./om);
//      phi0  = Trk->helix(0.).phi0();
//      x0    =  -(1/om+d0)*sin(phi0);
//      y0    =   (1/om+d0)*cos(phi0);
//
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
#include "TPolyLine.h"
#include "TObjArray.h"


#include "BTrk/KalmanTrack/KalRep.hh"

#include "art/Framework/Principal/Handle.h"

#include "BTrk/KalmanTrack/KalRep.hh"

#include "BTrk/TrkBase/HelixParams.hh"
#include "BTrk/TrkBase/HelixTraj.hh"

#include "GeometryService/inc/GeometryService.hh"
#include "GeometryService/inc/GeomHandle.hh"

#include "BTrkData/inc/TrkStrawHit.hh"

#include "TrackerGeom/inc/Tracker.hh"

#include "Stntuple/gui/TEvdTrack.hh"
#include "Stntuple/gui/TStnVisManager.hh"
#include "Stntuple/gui/TEvdTrkStrawHit.hh"
#include "Stntuple/base/TObjHandle.hh"

#include "CLHEP/Vector/ThreeVector.h"

ClassImp(TEvdTrack)

TEllipse* TEvdTrack::fgEllipse(0);

//-----------------------------------------------------------------------------
TEvdTrack::TEvdTrack(): TObject() {
  fListOfHits = NULL;
}

//-----------------------------------------------------------------------------
// pointer to track is just cached
//-----------------------------------------------------------------------------
TEvdTrack::TEvdTrack(int Number, const KalRep* Krep): TObject() {
  fNumber = Number;
  fKrep   = Krep;

  fListOfHits = new TObjArray();

  if (fgEllipse == 0) fgEllipse = new TEllipse();

  fgEllipse->SetFillStyle(3001);		// make it transparent
    
  fgEllipse->SetLineColor(kBlue-7);
}

//-----------------------------------------------------------------------------
TEvdTrack::~TEvdTrack() {
  if (fgEllipse) {
    delete fgEllipse;
    fgEllipse = NULL;
  }

  if (fListOfHits) {
    fListOfHits->Delete();
    delete fListOfHits;
  }
}

//-----------------------------------------------------------------------------
void TEvdTrack::Paint(Option_t* Option) {
  const char oname[] = "TEvdTrack::Paint";

  TVisManager* vm = TVisManager::Instance();

  const char* view = vm->GetCurrentView();

  if      (strstr(view,"trkxy" ) != 0) PaintXY(Option);
  else if (strstr(view,"trkrz" ) != 0) PaintRZ(Option);
  else if (strstr(view,"cal"   ) != 0) {
//-----------------------------------------------------------------------------
// calorimeter-specific view: do not draw tracks
//-----------------------------------------------------------------------------
  }
  else {
    printf("[%s] >>> ERROR: unknown view: %s, DO NOTHING\n",oname,view);
  }

  gPad->Modified();
}

//-----------------------------------------------------------------------------
// to display the reconstructed track in XY, use its parameters in the middle 
// of the tracker, at s=0
//-----------------------------------------------------------------------------
void TEvdTrack::PaintXY(Option_t* Option) {

  double d0, om, r, phi0, x0, y0;

  HelixParams hel = fKrep->helix(0);

  d0   = hel.d0();
  om   = hel.omega();
  r    = fabs(1./om);
  phi0 = hel.phi0();
    
  x0   =  -(1/om+d0)*sin(phi0);
  y0   =   (1/om+d0)*cos(phi0);
// 	printf("[MuHitDispla::printHelixParams] d0 = %5.3f r = %5.3f phi0 = %5.3f x0 = %5.3f y0 = %5.3f\n",
// 	       d0, r, phi0, x0, y0);
     
  fgEllipse->SetFillStyle(0);
  fgEllipse->SetFillColor(1);
  fgEllipse->PaintEllipse(x0,y0,r,0,0,2*M_PI,0);
}

//-----------------------------------------------------------------------------
void TEvdTrack::PaintRZ(Option_t* Option) {

  double            flen, zwire[2], ds, rdrift, zt[4], rt[4], zw, rw;
  CLHEP::Hep3Vector tdir;
  HepPoint          tpos;
  TPolyLine         pline;
  int               nplanes, /*npanels,*/ nl;

  mu2e::GeomHandle<mu2e::Tracker> handle;

  const mu2e::Tracker* tracker = handle.get();

  const mu2e::Straw  *hstraw, *s, *straw[2];		// first straw
  
  nplanes = tracker->nPlanes();
//-----------------------------------------------------------------------------
// first display track hits - active and not 
//-----------------------------------------------------------------------------
  const mu2e::TrkStrawHit  *hit;
  //   const TrkHitVector*       hits = &fKrep->hitVector();

  int nhits = NHits();
  for (int i=0; i<nhits; i++) {
    hit = Hit(i)->TrkStrawHit();
    rdrift = hit->driftRadius();

    hstraw = &hit->straw();
    zw     = hstraw->getMidPoint().z();
    rw     = hstraw->getMidPoint().perp();

    fgEllipse->SetX1(zw);
    fgEllipse->SetY1(rw);
    fgEllipse->SetR1(0);
    fgEllipse->SetR2(rdrift);
    fgEllipse->SetFillStyle(3003);

    if (hit->isActive()) fgEllipse->SetFillColor(kRed);
    else                 fgEllipse->SetFillColor(kBlue+3);

    fgEllipse->PaintEllipse(zw,rw,rdrift,0,0,2*M_PI,0);
  }
//-----------------------------------------------------------------------------
// now draw the trajectory in Z-R(local) space - device is a station
//-----------------------------------------------------------------------------
  HelixTraj trkHel(fKrep->helix(0).params(),fKrep->helix(0).covariance());

  for (int iplane=0; iplane<nplanes; iplane++) {
    const mu2e::Plane* plane = &tracker->getPlane(iplane);
//-----------------------------------------------------------------------------
// a plane is made of 2 'faces' or 6 panels, panels 0 and 1 on different faces
//-----------------------------------------------------------------------------
//    npanels = plane->nPanels();
//-----------------------------------------------------------------------------
// 3 panels in the same plane have the same Z and do not overlap - 
// no need to duplicate; panels 0 and 1 are at different Z
//-----------------------------------------------------------------------------
    for (int iface=0; iface<2; iface++) {
      const mu2e::Panel* panel = &plane->getPanel(iface);
      nl       = panel->nLayers();
					// deal with the compiler warnings
      zwire[0] = -1.e6;
      zwire[1] = -1.e6;

      for (int il=0; il<nl; il++) {
//-----------------------------------------------------------------------------
// assume all wires in a layer have the same Z, extrapolate track to the layer
//-----------------------------------------------------------------------------
	straw[il] = &panel->getStraw(il);
	zwire[il] = straw[il]->getMidPoint().z();
      }
					// order locally in Z
      if (zwire[0] > zwire[1]) {
	zt[1] = zwire[1];
	zt[2] = zwire[0];
	//	flip  = 1.;
      }
      else {
	zt[1] = zwire[0];
	zt[2] = zwire[1];
	//	flip  = -1.;
      }
					// z-step between the layers, want two more points
      zt[0] = zt[1]-3.;
      zt[3] = zt[2]+3.;

      const CLHEP::Hep3Vector* wd;
      double r;
      
      for (int ipoint=0; ipoint<4; ipoint++) {
//-----------------------------------------------------------------------------
// estimate flen using helix
//-----------------------------------------------------------------------------
	flen   = trkHel.zFlight(zt[ipoint]);
	fKrep->traj().getInfo(flen,tpos,tdir);
//-----------------------------------------------------------------------------
// try to extrapolate helix to a given Z a bit more accurately 
//-----------------------------------------------------------------------------
	ds     = (zt[ipoint]-tpos.z())/tdir.z();
	fKrep->traj().getInfo(flen+ds,tpos,tdir);
//-----------------------------------------------------------------------------
// for each face, loop over 3 panels in it
//-----------------------------------------------------------------------------
	rt[ipoint] = -1.e6;
	for (int ipp=0; ipp<3; ipp++) {
	  const mu2e::Panel* pp = &plane->getPanel(2*ipp+iface);
	  s  = &pp->getStraw(0);
	  wd = &s->getDirection();
	  r  = (tpos.x()*wd->y()-tpos.y()*wd->x()); // *flip;
	  if (r > rt[ipoint]) rt[ipoint] = r;
	}
      }
      pline.SetLineWidth(2);
      pline.PaintPolyLine(4,zt,rt);
    }
  }

}

//-----------------------------------------------------------------------------
void TEvdTrack::PaintCal(Option_t* Option) {
}

//_____________________________________________________________________________
Int_t TEvdTrack::DistancetoPrimitive(Int_t px, Int_t py) {
  return 9999;
}

//_____________________________________________________________________________
Int_t TEvdTrack::DistancetoPrimitiveXY(Int_t px, Int_t py) {

  Int_t dist = 9999;

  static TVector3 global;
//   static TVector3 local;

//   Double_t    dx1, dx2, dy1, dy2, dx_min, dy_min, dr;

  global.SetXYZ(gPad->AbsPixeltoX(px),gPad->AbsPixeltoY(py),0);

  return dist;
}

//_____________________________________________________________________________
Int_t TEvdTrack::DistancetoPrimitiveRZ(Int_t px, Int_t py) {
  return 9999;
}

//_____________________________________________________________________________
Int_t TEvdTrack::DistancetoPrimitiveCal(Int_t px, Int_t py) {
  return 9999;
}


//-----------------------------------------------------------------------------
void TEvdTrack::Clear(Option_t* Opt) {
  //  SetLineColor(1);
}


//-----------------------------------------------------------------------------
void TEvdTrack::Print(Option_t* Opt) const {

//   TObjHandle*                  h;
//   const mu2e::CaloCrystalHit*  hit;

//   printf (" X0 = %10.3f Y0 = %10.3f  E = %10.3f  njits = %5i\n",
// 	  X0(),Y0(),fEnergy,fNHits);

//   printf("----------------------------------------------------------------\n");
//   printf("CrystalID      Time   Energy    EnergyTot  NRoids               \n");
//   printf("----------------------------------------------------------------\n");
//   for (int i=0; i<fNHits; i++) {

//     h   = (TObjHandle*) fListOfHits->At(i);
//     hit = (const mu2e::CaloCrystalHit*) h->Object();

//     printf("%7i  %10.3f %10.3f %10.3f %5i\n",
// 	   hit->id(),
// 	   hit->time(),
// 	   hit->energyDep(),
// 	   hit->energyDepTotal(),
// 	   hit->numberOfROIdsUsed());
//   }
}
