#include <iostream>
#include <iomanip>

#include "obj/TStnTrackSeedBlock.hh"
#include "obj/TStnTrackSeed.hh"

ClassImp(TStnTrackSeedBlock)

//_____________________________________________________________________________
void TStnTrackSeedBlock::Streamer(TBuffer &R__b) {
  // Stream an object of class TStnTrackBlock.

  if (R__b.IsReading()) {
    Version_t R__v = R__b.ReadVersion(); if (R__v) { }
    R__b >> fNTrackSeeds;
    fListOfTrackSeeds->Streamer(R__b);
    for (int i=0; i<fNTrackSeeds; i++) {
       TrackSeed(i)->SetNumber(i);
     }
  } 
  else {
    R__b.WriteVersion(TStnTrackSeedBlock::IsA());
    R__b << fNTrackSeeds;
    fListOfTrackSeeds->Streamer(R__b);
  }
}

//-----------------------------------------------------------------------------
TStnTrackSeedBlock::TStnTrackSeedBlock() {
  fNTrackSeeds      = 0;
  fListOfTrackSeeds = new TClonesArray("TStnTrackSeed",100);
  fListOfTrackSeeds->BypassStreamer(kFALSE);
  fCollName  = "default";
}


//_____________________________________________________________________________
TStnTrackSeedBlock::~TStnTrackSeedBlock() {
  fListOfTrackSeeds->Delete();
  delete fListOfTrackSeeds;
}


//_____________________________________________________________________________
void TStnTrackSeedBlock::Clear(Option_t* opt) {
  fNTrackSeeds = 0;
  fListOfTrackSeeds->Clear(opt);

  f_EventNumber       = -1;
  f_RunNumber         = -1;
  f_SubrunNumber      = -1;
  fLinksInitialized   =  0;
}

//------------------------------------------------------------------------------
void TStnTrackSeedBlock::Print(Option_t* opt) const {

  int banner_printed = 0;
  for (int i=0; i<fNTrackSeeds; i++) {
    TStnTrackSeed* t = ((TStnTrackSeedBlock*) this)->TrackSeed(i);
    if (! banner_printed) {
      t->Print("banner");
      banner_printed = 1;
    }
    t->Print("data");
  }
}
