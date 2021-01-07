#ifndef STNTUPLE_TTrackStrawHitBlock
#define STNTUPLE_TTrackStrawHitBlock

#include "TClonesArray.h"

#include "Stntuple/base/TStnArrayI.hh"
#include "Stntuple/obj/TStnDataBlock.hh"
#include "Stntuple/obj/TTrackStrawHitData.hh"

#include "Stntuple/mod/InitStntupleDataBlocks.hh"
#include "TBuffer.h"

class TTrackStrawHitBlock: public TStnDataBlock {

  friend Int_t StntupleInitMu2eTrackStrawHitBlock(TStnDataBlock* block, 
						  AbsEvent*      event, 
						  int            mode);
public:
  Int_t          fNTracks;	        // total number of tracks
  Int_t          fNStrawHits;
  TStnArrayI*    fNTrackHits;           // number of hits per track 
  TStnArrayI*    fFirst;                // index of the first hit of the i-th track in the common list
  TClonesArray*  fListOfHits;		// list of hits
//-----------------------------------------------------------------------------
//  functions
//-----------------------------------------------------------------------------
public:
					// ****** constructors and destructor
  TTrackStrawHitBlock();
  virtual ~TTrackStrawHitBlock();
					// ****** accessors
  Int_t          NTracks  ()      { return fNTracks;  }
  Int_t          NTrackHits(int i) { return fNTrackHits->At(i); }
  Int_t          First    (int i) { return fFirst->At(i); } 

  TTrackStrawHitData* Hit (int i) { return (TTrackStrawHitData*) fListOfHits->UncheckedAt(i); }

  TTrackStrawHitData* Hit (int ITrack, int I) { 
    return (TTrackStrawHitData*) fListOfHits->UncheckedAt(fFirst->At(ITrack)+I); 
  }
  
  TClonesArray* GetListOfHits () { return fListOfHits; }
//-----------------------------------------------------------------------------
// modifiers
//-----------------------------------------------------------------------------
                                        // create hit, increse number of hits

  TTrackStrawHitData* NewHit() { return new ((*fListOfHits)[fNStrawHits++]) TTrackStrawHitData(); } 
//-----------------------------------------------------------------------------
// overloaded methods of TObject
//-----------------------------------------------------------------------------
  void Clear(Option_t* opt="");
  void Print(Option_t* opt="") const;

  ClassDef(TTrackStrawHitBlock,1)	// track straw hit data block
};


#endif


