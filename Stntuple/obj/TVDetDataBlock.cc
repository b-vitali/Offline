//
// 2014-07-15 G. Pezzullo
//

#include "Stntuple/obj/TVDetDataBlock.hh"

ClassImp(TVDetDataBlock)

//-----------------------------------------------------------------------------
// TBuffer::ReadVersion returns an integer, however so far it is not used 
//-----------------------------------------------------------------------------
  void TVDetDataBlock::Streamer(TBuffer &R__b) {
  if(R__b.IsReading()) {
    //    Version_t R__v = R__b.ReadVersion();
    R__b.ReadVersion();
    R__b >> fNHits;
    fListOfHits->Streamer(R__b);
  }
  else {
    R__b.WriteVersion(TVDetDataBlock::IsA());
    R__b << fNHits;
    fListOfHits->Streamer(R__b);
  }
}

//______________________________________________________________________________
TVDetDataBlock::TVDetDataBlock() {
  fListOfHits = new TClonesArray("TVDetHitData",30240);
  fListOfHits->BypassStreamer(kFALSE);
  Clear();
}

//______________________________________________________________________________
TVDetDataBlock::~TVDetDataBlock() {
  fListOfHits->Delete();
  delete fListOfHits;
}

//______________________________________________________________________________
void TVDetDataBlock::Clear(Option_t* opt) {
  fListOfHits->Clear();
  fNHits=0;

  f_EventNumber       = -1;
  f_RunNumber         = -1;
  f_SubrunNumber      = -1;
  fLinksInitialized   =  0;
}

//______________________________________________________________________________
void TVDetDataBlock::Print(Option_t* Option) const {
  // print all hits in the virtual detectors

  int banner_printed(0);
  const TVDetHitData* hit;

  TString opt = Option;
  opt.ToLower();

  for(int i=0; i<fNHits; i++) {
    hit = ((TVDetDataBlock*) this)->Hit(i);
    if ((opt == "") || (opt.Index("banner") >= 0)) {
      if (banner_printed == 0) {
	hit->Print("banner");
	banner_printed = 1;
      }
    }

    if ((opt == "") || (opt.Index("data") >= 0)) {
      hit->Print("data");
    }
  }
}

