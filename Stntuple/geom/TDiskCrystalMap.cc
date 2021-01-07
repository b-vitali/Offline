//

#include "Stntuple/geom/TDiskCrystalMap.hh"

//-----------------------------------------------------------------------------
// make sure that a) all data members are initialized b) the values don't make sense
//-----------------------------------------------------------------------------
TDiskCrystalMap::TDiskCrystalMap() {
  fSize         = -1.;
  fRMin         = -1.;
  fRMax         = -1;
  fNRings       = -1;
  fNCrystals    = -1;
  fMinFraction  =  1.e6;
  fIndex        = NULL;
  fIndexMap     = NULL;
  fNInside      = NULL;
};


//-----------------------------------------------------------------------------
TDiskCrystalMap::TDiskCrystalMap(double Size, double RMin, double RMax) {
  fSize         = Size;
  fRMin         = RMin;
  fRMax         = RMax;
  fNRings       = -1;
  fNCrystals    = -1;
  fMinFraction  =  1;
  fIndex        = NULL;
  fIndexMap     = NULL;
  fNInside      = NULL;
}

//-----------------------------------------------------------------------------
TDiskCrystalMap::~TDiskCrystalMap() {
  if (fIndex)    delete fIndex;
  if (fIndexMap) delete fIndexMap;
  if (fNInside ) delete fNInside;
};


//-----------------------------------------------------------------------------
void TDiskCrystalMap::Print(Option_t* Opt) const {
  int ip;

  int nintot = GetNInsideTotal();
  printf(" Size, RMin, RMax : %10.3f %10.3f %10.3f\n",fSize,fRMin,fRMax);
  printf(" NRings, NCrystalsTot, NInsideTotal: %5i %5i %5i\n",
	 GetNRings(),GetNTotal(),nintot);

  for (int ir=0; ir<fNRings; ir++) {
    int ncr   = GetNCrystalsPerRing(ir);
    int first = GetFirst(ir);

    printf("ring , N(crystals), N(inside), first: %5i %5i %5i %5i\n",
	   ir,GetNCrystalsPerRing(ir),GetNInside(ir),first);

    ip = 0;
    for (int ic=0; ic<ncr; ic++) {
      TDiskIndex* di = GetDiskIndex(first+ic);
      if (ip == 0) printf("ic,fL,fK :");
      if (di == NULL) printf(" (%4i %4i %4i)",ic,-999,-999);
      else            printf(" (%4i %4i %4i)",ic,di->fL,di->fK);
      ip += 1;
      if (ip >= 10) {
	printf("\n");
	ip = 0;
      }
    }
    if (ip != 0) printf("\n");
  }

  printf(" index map\n");

  ip = 0;
  for (int i=0; i<nintot; i++) {
    int loc = GetIndexMap(i);
    printf("(%4i %5i)",i,loc);
    ip += 1;
    if (ip >= 10) {
      printf("\n");
      ip = 0;
    }
  }

  if (ip != 0) printf("\n");
};

