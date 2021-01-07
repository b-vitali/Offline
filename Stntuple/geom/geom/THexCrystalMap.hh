#ifndef Stntuple_base_THexCrystalMap_hh
#define Stntuple_base_THexCrystalMap_hh

#include "Stntuple/geom/TDiskIndex.hh"
#include "Stntuple/geom/TDiskCrystalMap.hh"

class THexCrystalMap : public TDiskCrystalMap {
protected:
					// hexagon vertices
  static TDiskIndex fgPos[6];

public:

  THexCrystalMap(double Size, double RMin, double RMax);
  THexCrystalMap(); 

  ~THexCrystalMap();

  virtual int  GetFirst(int Ir) const { 
    if (Ir == 0) return 0;
    else         {
      printf(" THexCrystalMap::GetFirst ERROR: not implemented yet\n");
      return -1;
    }
  }
					// total number of crystals per ring (including ones outside the disk)

  virtual int GetNCrystalsPerRing(int I) const { 
    if (I == 0) return 1;
    else        return 6*I; 
  }

  virtual int    GetNTotal() const { return 3*fNRings*(fNRings-1)+1 ; }

  virtual int GetRing(int I);
  virtual int GetRing(TDiskIndex* Index);

  virtual void GetPosition(int I, TVector2* Pos);
  virtual void GetPosition(TDiskIndex* Index, TVector2* Pos);

  virtual double GetRadius(TDiskIndex* Index);
  virtual double GetRadius(int I);

  virtual TDiskIndex&    Pos(int I) { return fgPos[I]; }

  virtual int InsideCode(TDiskIndex* Index, double* Fraction);

};

#endif
