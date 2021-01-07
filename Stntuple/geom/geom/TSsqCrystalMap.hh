#ifndef Stntuple_base_TSsqCrystalMap_hh
#define Stntuple_base_TSsqCrystalMap_hh

#include "Stntuple/geom/TDiskIndex.hh"
#include "Stntuple/geom/TDiskCrystalMap.hh"

class TSsqCrystalMap: public TDiskCrystalMap {
protected:
  static int fgStep[12];

public:

  TSsqCrystalMap();
  TSsqCrystalMap(double Size, double RMin, double RMax); 

  ~TSsqCrystalMap();

//-----------------------------------------------------------------------------
// virtual functions of TDiskCrystalMap
//-----------------------------------------------------------------------------
  virtual int  GetFirst(int Ir) const { 
    if (Ir == 0) return 0;
    else         return 3*Ir*(Ir-1)+1;
  }
					// total number of crystals per ring (including ones outside the disk)

  virtual int GetNCrystalsPerRing(int I) const { 
    if (I == 0) return 1;
    else        return 6*I; 
  }

  virtual int    GetNTotal() const { return 3*fNRings*(fNRings-1)+1 ; }

  virtual int    GetRing(int I);

  virtual int    GetRing(TDiskIndex* Index);

  virtual void   GetPosition(int I, TVector2* Pos);
  virtual void   GetPosition(TDiskIndex* Index, TVector2* Pos);

  virtual double GetRadius(TDiskIndex* Index);
  virtual double GetRadius(int I);

  virtual int    InsideCode(TDiskIndex* Index, double* Fraction);
};

#endif
