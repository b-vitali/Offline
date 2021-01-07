//

#include "Stntuple/geom/TSqrCrystalMap.hh"




//-----------------------------------------------------------------------------
TSqrCrystalMap::TSqrCrystalMap() {
  fNRings = -1;
}



//-----------------------------------------------------------------------------
TSqrCrystalMap::TSqrCrystalMap(double Size, double RMin, double RMax): 
  TDiskCrystalMap(Size,RMin,RMax) {
  // int nrmax = 2*fRMax/fSize;
  // int ncr   = 0;
}


//-----------------------------------------------------------------------------
TSqrCrystalMap::~TSqrCrystalMap() {
}


//-----------------------------------------------------------------------------
int TSqrCrystalMap::GetRing(int I) {
  return -1;
}

//-----------------------------------------------------------------------------
int TSqrCrystalMap::GetRing(TDiskIndex* Index) {
  return -1;
}



//-----------------------------------------------------------------------------
void TSqrCrystalMap::GetPosition(int I, TVector2* Pos) {
}

//-----------------------------------------------------------------------------
void TSqrCrystalMap::GetPosition(TDiskIndex* Index, TVector2* Pos) {
}

//-----------------------------------------------------------------------------
double TSqrCrystalMap::GetRadius(TDiskIndex* Index) {
  return -1.;
}

//-----------------------------------------------------------------------------
double TSqrCrystalMap::GetRadius(int I) {
  return -1.;
}

//-----------------------------------------------------------------------------
int    TSqrCrystalMap::InsideCode(TDiskIndex* Index, double* Fraction) {
  return -1;
}
