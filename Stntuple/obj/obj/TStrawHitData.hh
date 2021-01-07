//-----------------------------------------------------------------------------
//  2014-01-26 P.Murat - TStrawHitData
//-----------------------------------------------------------------------------
#ifndef TStrawHitData_hh
#define TStrawHitData_hh

#include <math.h>
#include "TMath.h"
#include "TObject.h"
#include "TBuffer.h"

class TStrawHitData : public TObject {
//-----------------------------------------------------------------------------
// constants imported from DataProducts/inc/StrawId.hh
//-----------------------------------------------------------------------------
  constexpr static uint16_t _layermsk = 0x1; // mask for layer field
  constexpr static uint16_t _strawmsk = 0x7F; // mask for straw field
  constexpr static uint16_t _preampmsk = 0x7E; // mask for preamp
  constexpr static uint16_t _panelmsk = 0x380; // mask for panel field
  constexpr static uint16_t _preampsft = 1; // shift for preamp field
  constexpr static uint16_t _panelsft = 7; // shift for panel field
  constexpr static uint16_t _facemsk = 0x80; // mask for face field
  constexpr static uint16_t _facesft = 7; // shift for face field
  constexpr static uint16_t _planemsk = 0xFC00; // mask for plane field
  constexpr static uint16_t _planesft = 10; // shift for plane field
  constexpr static uint16_t _stationmsk = 0xF800; // mask for station field
  constexpr static uint16_t _stationsft = 11; // shift for station field

protected:
  int     fIndex;
  int     fPdgCode;
  int     fMotherPdgCode;
  int     fGeneratorCode;
  int     fSimID;
  float   fTime;
  float   fDt;
  float   fEnergy;
  float   fMcMomentum;			// MC particle momentum
  
public:
                                        // ****** constructors and destructors
  TStrawHitData();
  virtual ~TStrawHitData();
//-----------------------------------------------------------------------------
// accessors
//-----------------------------------------------------------------------------
  int     Index  () { return fIndex; }

  int     Station() { return  (fIndex & _stationmsk) >> _stationsft; }
  int     Panel  () { return  (fIndex & _panelmsk  ) >> _panelsft  ; }
  int     Face   () { return  (fIndex & _facemsk   ) >> _facesft   ; }
  int     Layer  () { return  (fIndex & _layermsk  )               ; }
  int     Straw  () { return  (fIndex & _strawmsk  )               ; }
  int     Preamp () { return  (fIndex & _preampmsk ) >> _preampsft ; }

  float   Time   () { return fTime; }
  float   Dt     () { return fDt; }
  float   Energy () { return fEnergy; }

  int     PdgCode      () { return fPdgCode;       }
  int     MotherPdgCode() { return fMotherPdgCode; }
  int     GeneratorCode() { return fGeneratorCode; }
  int     SimID        () { return fSimID;         }

  float   McMomentum   () { return fMcMomentum;    }
//-----------------------------------------------------------------------------
// modifiers
//-----------------------------------------------------------------------------
  void    Set(int Index, float Time, float Dt, float EnergyDep,
	      int PdgID, int MotherPdgID, int GenCode, int SimID, 
	      float McMomentum);
//-----------------------------------------------------------------------------
// overloaded methods of TObject
//-----------------------------------------------------------------------------
  void Clear(Option_t* opt = "");
  void Print(Option_t* opt = "") const;
//-----------------------------------------------------------------------------
// schema evolution
//-----------------------------------------------------------------------------
  void ReadV1(TBuffer &R__b);

  ClassDef (TStrawHitData,2)
};

#endif
