///////////////////////////////////////////////////////////////////////////////
#include "Stntuple/base/TCalibRunRange.hh"

ClassImp(TCalibRunRange)

//_____________________________________________________________________________
TCalibRunRange::TCalibRunRange() {
  fMinRun          = 0;
  fMaxRun          = 0;
  fFilename        = "";
}

//_____________________________________________________________________________
TCalibRunRange::TCalibRunRange(int MinRun, int MaxRun, const char* Filename) {
  fMinRun          = MinRun;
  fMaxRun          = MaxRun;
  fFilename        = Filename;
}

//_____________________________________________________________________________
TCalibRunRange::~TCalibRunRange() {
}
