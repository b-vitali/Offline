#ifndef STNTUPLE_TCrvCoincidenceCluster
#define STNTUPLE_TCrvCoincidenceCluster

#include "TClonesArray.h"
#include "TArrayI.h"
#include "TVector3.h"

class TCrvCoincidenceCluster: public TObject {
public:
  int          fIndex;                   // index in the list
  int          fSectorType;
  int          fNPulses;
  int          fNPe;
  float        fStartTime;
  float        fEndTime;
  TVector3     fPosition;
//-----------------------------------------------------------------------------
//  functions
//-----------------------------------------------------------------------------
public:
					// ****** constructors and destructor
  TCrvCoincidenceCluster();
  virtual ~TCrvCoincidenceCluster();
					// ****** accessors

  int     Index           () const { return fIndex;          }
  int     SectorType      () const { return fSectorType;      }
  int     NPe             () const { return fNPe;             }
  int     NPulses         () const { return fNPulses;         }

  float   StartTime       () const { return fStartTime;       }
  float   EndTime         () const { return fEndTime;         }

  const TVector3* Position() const { return &fPosition;       }
//-----------------------------------------------------------------------------
// modifiers
//-----------------------------------------------------------------------------
  void Set(int Index, int SectorType, int Np, int NPe, 
	   float X, float Y, float Z, float T1, float T2);
//-----------------------------------------------------------------------------
// overloaded methods of TObject
//-----------------------------------------------------------------------------
  void Clear(Option_t* Opt = "");
  void Print(Option_t* Opt = "") const;

  ClassDef(TCrvCoincidenceCluster,1)	         // STNTUPLE representation of CrvCoincidenceCluster
};


#endif
