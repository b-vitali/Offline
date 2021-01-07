///////////////////////////////////////////////////////////////////////////////
// May 04 2013 P.Murat
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
#include "TObjArray.h"
#include "TPolyLine.h"

// #include "art/Framework/Principal/Handle.h"

// #include "GeometryService/inc/GeometryService.hh"
// #include "GeometryService/inc/GeomHandle.hh"

#include "Stntuple/geom/THexCrystalMap.hh"
#include "Stntuple/geom/TSsqCrystalMap.hh"
#include "Stntuple/geom/TSqrCrystalMap.hh"

#include "Stntuple/geom/TDisk.hh"
#include "Stntuple/geom/TStnCrystal.hh"

ClassImp(TDisk)

//_____________________________________________________________________________
TDisk::TDisk(): TObject() {
  fEnergy = 0;
}

//_____________________________________________________________________________
TDisk::TDisk(int SectionID, double RMin, double RMax, double Z0, int NEdges,
             double Size, double DeadSpace, double MinFraction) : TObject() {

  //  TStnCrystal     *crystal;
  //  int             n1;
  TVector2        pos;
//------------------------------------------------------------------------------
// define number of rings and the total number of crystals with a safety margin, 
// count only crystals fully inside
//-----------------------------------------------------------------------------
  fSectionID    = SectionID;
  fRMin         = RMin;
  fRMax         = RMax;
  fZ0           = Z0;
  fNEdges       = NEdges;
  fSize         = Size;         // full size
  fDeadSpace    = DeadSpace;
  fMinFraction  = MinFraction;

  if (NEdges == 4) {
//-----------------------------------------------------------------------------
// staggered square crystals (current option)
//-----------------------------------------------------------------------------
    InitSsqCrystals();
  }
  else if (NEdges == 6) {
//-----------------------------------------------------------------------------
// hex crystals (obsolete option)
//-----------------------------------------------------------------------------
    InitHexCrystals();
  }

  fEnergy = 0;
}

//-----------------------------------------------------------------------------
TDisk::~TDisk() {
}

//-----------------------------------------------------------------------------
// 'Clear' knows about hits and energies
//-----------------------------------------------------------------------------
void TDisk::Clear(Option_t* Opt) {

  TStnCrystal  *cr;
  
  int nc = NCrystals();
  for (int i=0; i<nc; i++) {
    cr     = (TStnCrystal*) fListOfCrystals->UncheckedAt(i);
    cr->Clear();
  }

  fEnergy = 0;
}

//-----------------------------------------------------------------------------
void TDisk::Print(Option_t* Opt) const {
  printf("------------------------------------------------------------------------\n");
  printf(" ID   RMin    RMax      Size   MaxFraction NCrystals   NRings  Offset  \n");
  printf("------------------------------------------------------------------------\n");
  printf("%3i %8.3f %8.3f %8.3f  %5.1f        %5i  %5i  %5i\n",
         fSectionID,fRMin,fRMax,fSize,fMinFraction,NCrystals(),
         GetNRings(),fFirstChanOffset);

  fCrystalMap->Print(Opt);
}

//-----------------------------------------------------------------------------
int TDisk::InitHexCrystals() {

  TVector2     pos;
  TStnCrystal* crystal;
                                        // account for spacing between the crystals
  double size   = fSize+2*fDeadSpace;

  fCrystalMap = new THexCrystalMap(size,fRMin,fRMax);

  int nr = GetNRings();

  int         inside; 
  double      fraction;
  TDiskIndex* index;

  fListOfCrystals = new TObjArray(fCrystalMap->GetNTotal());
  //  fNCrystals   = 0;
  
  for (int ir=0; ir<nr; ir++) {
    
    for (int ic=0; ic<fCrystalMap->GetNCrystalsPerRing(ir); ic++) {
      int loc = fCrystalMap->GetFirst(ir)+ic;
      index   = fCrystalMap->GetDiskIndex(loc);
      inside  = fCrystalMap->InsideCode(index,&fraction);
        
      if (inside == 0) {
        fCrystalMap->GetPosition(index,&pos); // *FIXME* - this returns index, not multiplied by fSize
        crystal = new TStnCrystal(index,pos.X(),pos.Y(),fZ0,fNEdges,fSize);
        crystal->SetDisk(this);
        fListOfCrystals->Add(crystal);
      }
    }
  }

  return 0;
}


//-----------------------------------------------------------------------------
int TDisk::InitSsqCrystals() {

  TVector2     pos;
  TStnCrystal* crystal;

  double size = fSize+2*fDeadSpace;

  fCrystalMap     = new TSsqCrystalMap(size,fRMin,fRMax);
  int nin         = fCrystalMap->GetNInsideTotal();
  fListOfCrystals = new TObjArray(nin);
//-----------------------------------------------------------------------------
// for the first several rings, in general, there could be no crystals within 
// the acceptance - skip them
// only crystals which belong to the disk are stored
//-----------------------------------------------------------------------------
  TDiskIndex *index;

  for (int ic = 0; ic<nin; ic++) {
    int loc = fCrystalMap->GetIndexMap(ic);
    index   = fCrystalMap->GetDiskIndex(loc); // not zero by construction
    fCrystalMap->GetPosition(index,&pos);
    crystal = new TStnCrystal(index,pos.X(),pos.Y(),fZ0,fNEdges,fSize);
    crystal->SetDisk(this);
    fListOfCrystals->AddAt(crystal,ic);
  }

  return 0;
}



//-----------------------------------------------------------------------------
int TDisk::InitSqrCrystals() {

  TVector2     pos;
  //  TStnCrystal* crystal;

  double size = fSize+2*fDeadSpace;
  fCrystalMap = new TSqrCrystalMap(size,fRMin,fRMax);
//-----------------------------------------------------------------------------
// for the first several rings, in general, there could be no crystals within 
// the acceptance - skip them
//-----------------------------------------------------------------------------
  // TDiskIndex *inside, *index;
  // for (int ir = 0; ir<fCrystalMap->GetNRings(); ir++) {
  //   int nin = fCrystalMap->GetNInside(ir);
  //   if (nin > 0) {
  //     for (int ic=0; ic<fCrystalMap->GetNCrystalsPerRing(ir); ic++) {
  // 	int loc = fCrystalMap->GetFirst(ir)+ic;
  // 	fCrystalMap->GetDiskIndex(loc, inside); // not zero if crystal is present
  // 	if (inside) {
  // 	  fCrystalMap->GetDiskIndex(loc,index); // loc is sequential number in the array sparse
  // 	  fCrystalMap->GetPosition(index,&pos);
  // 	  crystal = new TStnCrystal(index,pos.X(),pos.Y(),fZ0,fNEdges,fSize);
  // 	  crystal->SetDisk(this);
  // 	  fListOfCrystals->AddAt(crystal,loc);
  // 	}
  //     }
  //   } 
  // }

  return 0;
}



//-----------------------------------------------------------------------------
void TDisk::Paint(Option_t* Opt) {

  TVector2 pos;

  //  TDiskIndex*  index;

  TPolyLine p;
  TEllipse  e;

  //  double x[7], y[7];

  //  TCanvas* c = new TCanvas("c","c",800,800);

  // fListOfPolyLines->Delete();

  TStnCrystal* cr;

  int nc = fCrystalMap->GetNInsideTotal();
  for (int i=0; i<nc; i++) {
    int loc = fCrystalMap->GetIndexMap(i);
    cr  = (TStnCrystal*) fListOfCrystals->At(loc);
    //    index = cr->DiskIndex();

    // pos.Set(cr->Center()->X(),cr->Center()->Y());
    // double   x0, y0;

    // x0 = cr->Center()->X();
    // y0 = cr->Center()->Y();

    //    GetPosition(i,&pos);

    //    int ring = GetRing(i);

    //    printf(" i, r, l,k, x,y : %5i %5i %5i %5i %10.4lf %10.4lf\n",i,ring, hex_index.fL,hex_index.fK,pos.X(),pos.Y());

    

    if (strstr(Opt,"trkxy") != 0) {
      cr->PaintXY(Opt);
    }
    else if (strstr(Opt,"cal") != 0) {
      cr->PaintCal(Opt);
    }

  //   double rho = fSize/sqrt(3.);

  //   for (int iv=0; iv<7; iv++) {
  //     int l = iv % 6;
  //     x[iv] = x0 + rho*cos(TMath::Pi()*l/3);
  //     y[iv] = y0 + rho*sin(TMath::Pi()*l/3);
  //   }

  //   p.SetLineColor(1);
  //   p.SetLineWidth(1);

  //   double fr;

  //   int inside_code = fCrystalMap->InsideCode(index,&fr);

  //   if (inside_code == 0) {

  //     if (fr == 1) {
  // 	p.SetFillColor(2);                     // fully inside
  // 	p.SetFillStyle(3001);
  //     }
  //     else {
  // 	p.SetFillColor(kBlue);             // partially inside
  // 	p.SetFillStyle(1001);
  //     }

  //     //      p.SetFillStyle(3001);
  //     p.DrawPolyLine(7,x,y,"F");
  //   }
  //   else {
  // 				// fully outside
  //     p.SetFillStyle(0);
  //     p.SetFillColor(0);
  //   }

  //   p.DrawPolyLine(7,x,y,"");

  } 

  e.SetFillStyle(0);
  e.SetFillColor(0);
  e.DrawEllipse(0,0,fRMin,fRMin,0,360,0,"");
  e.DrawEllipse(0,0,fRMax,fRMax,0,360,0,"");
  

//   c->Update();
//   c->Draw();
}
  
