#ifndef STNTUPLE_TCrvCoincidence
#define STNTUPLE_TCrvCoincidence

#include "TClonesArray.h"
#include "TArrayI.h"


class TCrvCoincidence: public TObject {
public:
  int          fNumber;                 // index in the list
  int          fSectorType;
  int          fNPulses;
//-----------------------------------------------------------------------------
//  functions
//-----------------------------------------------------------------------------
public:
					// ****** constructors and destructor

  TCrvCoincidence();
  virtual ~TCrvCoincidence();
					// ****** accessors

  int     Number         () const { return fNumber;      }
  int     SectorType     () const { return fSectorType;  }
  int     NPulses        () const { return fNPulses;     }
//-----------------------------------------------------------------------------
// modifiers
//-----------------------------------------------------------------------------
  void Set(int Number, int SectorType, int NPulses);
//-----------------------------------------------------------------------------
// overloaded methods of TObject
//-----------------------------------------------------------------------------
  void Clear(Option_t* opt="");
  void Print(Option_t* opt="") const;

  ClassDef(TCrvCoincidence,1)	         // CRV reco pulse
};


#endif
