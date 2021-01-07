///////////////////////////////////////////////////////////////////////////////
//  2019-06-10 P.Murat TCrvCoincidence
///////////////////////////////////////////////////////////////////////////////
#include "TString.h"
#include "TBuffer.h"

#include "Stntuple/obj/TCrvCoincidence.hh"

ClassImp(TCrvCoincidence)

//_____________________________________________________________________________
void TCrvCoincidence::Streamer(TBuffer &R__b) {

  int nwi = ((int*) &fNPulses) - &fNumber + 1;
  // int nwf = -1; // &fMcMomentum - &fTime +1;
  
  if (R__b.IsReading()) {
    //    Version_t R__v = R__b.ReadVersion();
    R__b.ReadVersion();
//-----------------------------------------------------------------------------
// curent version: V1
//-----------------------------------------------------------------------------
    R__b.ReadFastArray(&fNumber,nwi);
  }
  else {
    R__b.WriteVersion(TCrvCoincidence::IsA());
    R__b.WriteFastArray(&fNumber,nwi);
  } 
}

//_____________________________________________________________________________
TCrvCoincidence::TCrvCoincidence(): TObject() {
  Clear();
}

//_____________________________________________________________________________
TCrvCoincidence::~TCrvCoincidence() {
}

//_____________________________________________________________________________
void TCrvCoincidence::Set(int Number, int SectorType, int NPulses) {
  fNumber     = Number;
  fSectorType = SectorType;
  fNPulses    = NPulses;
}

//_____________________________________________________________________________
void TCrvCoincidence::Clear(Option_t* opt) {
  fNumber     = -1;
  fSectorType = -1;
  fNPulses    = -1;
}

//_____________________________________________________________________________
void TCrvCoincidence::Print(Option_t* Option) const {
  // print Straw hit properties
  //  printf("Superlayer: %d, Wire: %d, Cell: %d,\n",fSuperLayer,fWire,fCell);
  
  TString opt = Option;
  opt.ToLower();

  if ((opt == "") || (opt.Index("banner") >= 0)) {
    printf("-------------------------------\n");
    printf("  Index: %5i  SectorType: %5i NPulses: %5i\n",fNumber,fSectorType,fNPulses);
  }

  if (opt == "banner") return;
  
}
