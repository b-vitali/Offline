//-----------------------------------------------------------------------------
//  2014-01-26 P.Murat - TTrackStrawHitData
//-----------------------------------------------------------------------------
#ifndef TTrackStrawHitData_hh
#define TTrackStrawHitData_hh

#include <math.h>
#include "TMath.h"
#include "TObject.h"
#include "TBuffer.h"

class TTrackStrawHitData : public TObject {
public:
  int     fIndex;
  int     fActive;
  int     fAmbig;
  int     fPdgCode;
  int     fMotherPdgCode;
  int     fGeneratorCode;
  int     fSimID;
  float   fTime;
  float   fDt;
  float   fEnergy;
  float   fDriftRadius;
  float   fMcDoca;
  float   fMcMomentum;			// MC particle momentum, assumed to be the last word
public:
                                        // ****** constructors and destructors
  TTrackStrawHitData();
  virtual ~TTrackStrawHitData();
//-----------------------------------------------------------------------------
// accessors
//-----------------------------------------------------------------------------
  int     Index      () { return fIndex;  }
  float   Time       () { return fTime;   }
  float   Dt         () { return fDt;     }
  float   Energy     () { return fEnergy; }
  float   DriftRadius() { return fDriftRadius; }
  int     Active     () { return fActive; }

  int     PdgCode      () { return fPdgCode;       }
  int     MotherPdgCode() { return fMotherPdgCode; }
  int     GeneratorCode() { return fGeneratorCode; }
  int     SimID        () { return fSimID;         }

  float   McDoca       () { return fMcDoca;        }
  float   McMomentum   () { return fMcMomentum;    }
//-----------------------------------------------------------------------------
// modifiers
//-----------------------------------------------------------------------------
  void    Set(int Index, float Time, float Dt, float EnergyDep,
	      int Active, int Ambig, float DriftRadius,
	      int PdgID, int MotherPdgID, int GenCode, int SimID, 
	      float McDoca, float McMomentum);
//-----------------------------------------------------------------------------
// overloaded methods of TObject
//-----------------------------------------------------------------------------
  void Clear(Option_t* opt = "");
  void Print(Option_t* opt = "") const;
//-----------------------------------------------------------------------------
// schema evolution
//-----------------------------------------------------------------------------
  void ReadV1(TBuffer &R__b);

  ClassDef (TTrackStrawHitData,1)
};

#endif
