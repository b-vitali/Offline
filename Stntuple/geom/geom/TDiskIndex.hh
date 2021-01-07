#ifndef Stntuple_geom_TDiskIndex_hh
#define Stntuple_geom_TDiskIndex_hh

#include "TObject.h"

class TDiskIndex: public TObject {
public:
  int  fL;
  int  fK;
  
  TDiskIndex() {}
  TDiskIndex(int L, int K) {
    fL = L;
    fK = K;
  }

  void Set(int L, int K) { fL = L; fK = K; }

  ~TDiskIndex() {}
};

#endif
