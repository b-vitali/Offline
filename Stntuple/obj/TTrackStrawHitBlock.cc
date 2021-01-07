//
// 2014-01-27 P.Murat
//

#include "Stntuple/obj/TTrackStrawHitBlock.hh"

ClassImp(TTrackStrawHitBlock)

//-----------------------------------------------------------------------------
void TTrackStrawHitBlock::Streamer(TBuffer &R__b) {
  if(R__b.IsReading()) {
    R__b.ReadVersion();
    R__b >> fNTracks;
    R__b >> fNStrawHits;
    fNTrackHits->Streamer(R__b);
    fFirst->Streamer(R__b);
    fListOfHits->Streamer(R__b);
  }
  else {
    R__b.WriteVersion(TTrackStrawHitBlock::IsA());
    R__b << fNTracks;
    R__b << fNStrawHits;
    fNTrackHits->Streamer(R__b);
    fFirst->Streamer(R__b);
    fListOfHits->Streamer(R__b);
  }
}

//______________________________________________________________________________
TTrackStrawHitBlock::TTrackStrawHitBlock() {

  fNTracks    = 0;
  fNStrawHits = 0;

  fNTrackHits = new TStnArrayI(0);
  fFirst      = new TStnArrayI(0);
  
  fListOfHits = new TClonesArray("TTrackStrawHitData",200);
  fListOfHits->BypassStreamer(kFALSE);

  Clear();
}

//______________________________________________________________________________
TTrackStrawHitBlock::~TTrackStrawHitBlock() {
  fListOfHits->Delete();
  delete fListOfHits;

  delete fNTrackHits;
  delete fFirst;
}

//______________________________________________________________________________
void TTrackStrawHitBlock::Clear(Option_t* opt) {
  fNTracks    = 0;
  fNStrawHits = 0;
  fNTrackHits->Clear();
  fFirst->Clear();
  fListOfHits->Clear();

  f_EventNumber       = -1;
  f_RunNumber         = -1;
  f_SubrunNumber      = -1;
  fLinksInitialized   =  0;
}

//______________________________________________________________________________
void TTrackStrawHitBlock::Print(Option_t* opt) const {
  // print all hits in the straw tracker
  printf(" *** print Track Straw Hits*** \nNumber of hits: %d\n",fNStrawHits);
  for(int i=0; i<fNStrawHits; i++) {
    fListOfHits->At(i)->Print();
  }
}

