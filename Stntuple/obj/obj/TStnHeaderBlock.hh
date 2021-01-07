#ifndef STNTUPLE_TStnHeaderBlock
#define STNTUPLE_TStnHeaderBlock
//-----------------------------------------------------------------------------
//  definition of the STNTUPLE event header
//  Author:    Pasha Murat (CDF/FNAL)
//  Date:      Oct 31 2000
// 
//-----------------------------------------------------------------------------
#include "TStnDataBlock.hh"
#include "TBuffer.h"

class TStnEvent;

class TStnHeaderBlock : public TStnDataBlock {

  friend Int_t StntupleInitMu2eHeaderBlock     (TStnDataBlock*, AbsEvent* , int);
  friend Int_t StntupleInitMu2eHeaderBlockLinks(TStnDataBlock*, AbsEvent* , int);

  enum {
    kNFreeInts   =  5,                  // starting from version 2
    kNFreeFloats =  5
  };

public:
  Int_t             fVersion;
  Int_t             fEventNumber;
  Int_t             fRunNumber;
  Int_t             fSectionNumber;	// section number within the run
  Int_t             fMcFlag;		// MC flag, 0 for real data
  Int_t             fGoodRun;		// run flag
  Int_t             fBrCode;		// 
  Int_t             fGoodTrig;
  Int_t             fTrigWord;		// 
  Int_t             fNTracks;           //
  int               fNStrawHits;        // *** added in V2 
  int               fNCaloHits;         // *** added in V2 - total N reco pulses
  int               fNCRVHits;          // *** added in V2 - total N CRV hits
  Int_t             fCpu;               // packed word with processing time
  int               fInt[kNFreeInts];   // provision for future I/O expansion

  float             fInstLum;		 // instantaneous luminosity
  float             fMeanLum;		 // *** added in V2 : mean luminosity (MC)
  float             fFloat[kNFreeFloats];// provision for future I/O expansion
  TString           fStnVersion;         // STNTUPLE version, like "dev_243_16"
//------------------------------------------------------------------------------
//  transient data, all persistent should go above
//------------------------------------------------------------------------------
  Int_t             fLastNumber;	//! event/run numbers for the last printed event
  Int_t             fLastRunNumber;	//! 
//------------------------------------------------------------------------------
//  function members
//------------------------------------------------------------------------------
public:
					// ****** constructors and destructor
  TStnHeaderBlock();
  virtual ~TStnHeaderBlock();
					// ****** accessors 

  Int_t  EventNumber  () const { return fEventNumber;   }
  Int_t  RunNumber    () const { return fRunNumber;     }
  Int_t  SectionNumber() const { return fSectionNumber; }
  Int_t  SubrunNumber () const { return fSectionNumber; }    // Mu2e synonim of the CDF "section number"
  Int_t  McFlag       () const { return fMcFlag;        }
  Int_t  NTracks      () const { return fNTracks;       }

  Float_t InstLum     () const { return fInstLum;       }
  Float_t MeanLum     () const { return fMeanLum;       }
  Float_t LumWeight   () const { return fInstLum/fMeanLum; }

  Float_t CpuTime     () const { return ((fCpu>>8)/10.0);   } // in s
  Float_t CpuSpeed    () const { return ((fCpu&0xFF)/5.0); } // in GHz
  const TString& StnVersion () const { return fStnVersion;    }

					// ****** setters/modifiers

//-----------------------------------------------------------------------------
// overloaded functions of TObject
//-----------------------------------------------------------------------------
  virtual void   Clear(Option_t* opt = "");
  virtual void   Print(Option_t* opt = "") const;

					// ****** schema evolution
  void   ReadV1(TBuffer& R__b);


  ClassDef(TStnHeaderBlock,2)	       // Mu2e STNTUPLE event header
};

#endif
