///////////////////////////////////////////////////////////////////////////////
#include "Stntuple/base/TCalibTable.hh"
#include "Stntuple/base/TCalibRunRange.hh"

ClassImp(TCalibTable)

//_____________________________________________________________________________
TCalibTable::TCalibTable() {
  fListOfRunRanges = 0;
}

//_____________________________________________________________________________
TCalibTable::TCalibTable(const char* Detector, const char* Name) {
  TString name(Detector);

  name = name + "/" + Name;
  name.ToLower();
  SetName(name.Data());
  SetTitle(name.Data());
  fListOfRunRanges = new TObjArray();
}

//_____________________________________________________________________________
TCalibTable::~TCalibTable() {
  if (fListOfRunRanges) {
    fListOfRunRanges->Delete();
    delete fListOfRunRanges;
  }
}


//_____________________________________________________________________________
TCalibRunRange* TCalibTable::GetRunRange(int RunNumber) {

  TCalibRunRange *rr, *rr_found(0);
  int nr=fListOfRunRanges->GetEntriesFast();

  for (int i=0; i<nr; i++) {
    rr = (TCalibRunRange*) fListOfRunRanges->UncheckedAt(i);
    if ((RunNumber >= rr->GetMinRun()) && (RunNumber <= rr->GetMaxRun())) {
      rr_found = rr;
    }
  }

  return rr_found;
}

//_____________________________________________________________________________
const char* TCalibTable::GetFilename(int RunNumber) {
  const char* fn = 0;

  TCalibRunRange *rr, *rr_found(0);
  int nr=fListOfRunRanges->GetEntriesFast();

  for (int i=0; i<nr; i++) {
    rr = (TCalibRunRange*) fListOfRunRanges->UncheckedAt(i);
    if ((RunNumber >= rr->GetMinRun()) && (RunNumber <= rr->GetMaxRun())) {
      rr_found = rr;
    }
  }

  if (rr_found) fn = rr_found->GetFilename();

  return fn;
}

  
