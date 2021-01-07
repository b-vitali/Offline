#ifndef Stntuple_base_TSqrCrystalMap_hh
#define Stntuple_base_TSqrCrystalMap_hh

#include "Stntuple/geom/TDiskIndex.hh"
#include "Stntuple/geom/TDiskCrystalMap.hh"

class TSqrCrystalMap: public TDiskCrystalMap {
protected:

public:

  TSqrCrystalMap();
  TSqrCrystalMap(double Size, double RMin, double RMax); 

  ~TSqrCrystalMap();

//-----------------------------------------------------------------------------
// virtual functions of TDiskCrystalMap
//-----------------------------------------------------------------------------
  virtual int  GetFirst(int Ir) const { 
    if (Ir == 0) return 0;
    else         {
      printf(" TSqrCrystalMap::GetFirst ERROR: not implemented yet\n");
      return -1;
    }
  }
					// total number of crystals per ring (including ones outside the disk)

  virtual int GetNCrystalsPerRing(int I) const { 
    if (I == 0) return 1;
    else        return 4*(I+1); 
  }

  virtual int    GetNTotal() const { return 1+4*(fNRings*(fNRings+1)/2-1) ; }

  virtual int GetRing(int I);
  virtual int GetRing(TDiskIndex* Index);

  virtual void GetPosition(int I, TVector2* Pos);
  virtual void GetPosition(TDiskIndex* Index, TVector2* Pos);

  virtual double GetRadius(TDiskIndex* Index);
  virtual double GetRadius(int I);

  virtual int    InsideCode(TDiskIndex* Index, double* Fraction);
};

#endif
