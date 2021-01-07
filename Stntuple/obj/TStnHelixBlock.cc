#include <iostream>
#include <iomanip>

#include "obj/TStnHelixBlock.hh"
#include "obj/TStnHelix.hh"

ClassImp(TStnHelixBlock)

//_____________________________________________________________________________
void TStnHelixBlock::Streamer(TBuffer &R__b) {
  // Stream in/out an object of class TStnHelixBlock.

  if (R__b.IsReading()) {
    Version_t R__v = R__b.ReadVersion(); if (R__v) { }
    R__b >> fNHelices;
    fListOfHelices->Streamer(R__b);
    for (int i=0; i<fNHelices; i++) {
       Helix(i)->SetNumber(i);
    }
  }
  else {
    R__b.WriteVersion(TStnHelixBlock::IsA());
    R__b << fNHelices;
    fListOfHelices->Streamer(R__b);
  }
}
TStnHelixBlock::TStnHelixBlock() {
  fNHelices      = 0;
  fListOfHelices = new TClonesArray("TStnHelix",100);
  fListOfHelices->BypassStreamer(kFALSE);
  fCollName  = "default";
}


//_____________________________________________________________________________
TStnHelixBlock::~TStnHelixBlock() {
  fListOfHelices->Delete();
  delete fListOfHelices;
}


//_____________________________________________________________________________
void TStnHelixBlock::Clear(Option_t* opt) {
  fNHelices = 0;
  fListOfHelices->Clear(opt);

  f_EventNumber       = -1;
  f_RunNumber         = -1;
  f_SubrunNumber      = -1;
  fLinksInitialized   =  0;
}

//------------------------------------------------------------------------------
void TStnHelixBlock::Print(Option_t* opt) const {

  int banner_printed = 0;
  for (int i=0; i<fNHelices; i++) {
    TStnHelix* t = ((TStnHelixBlock*) this)->Helix(i);
    if (! banner_printed) {
      t->Print("banner");
      banner_printed = 1;
    }
    t->Print("data");
  }
}
