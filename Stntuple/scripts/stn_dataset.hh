//-----------------------------------------------------------------------------
// a 'dataset' here is a histogram file plus data fields for plotting attributes
//-----------------------------------------------------------------------------
#ifndef __Stntuple_scripts_stn_dataset_hh__
#define __Stntuple_scripts_stn_dataset_hh__

struct stn_dataset_t {

  TString  fID        ;			// dataset ID (short name)
  TString  fName      ;			// full name of the ntupled dataset
  int      fNEvents   ;                 // number of events in all files
  int      fNGenEvents; 		// N generated events (if MC, corresponding to the files, for normalization)
  float    fEMin      ;			// sometimes, the signal particle energy range is constrained
  float    fEMax      ;
  int      fMinRun    ;                 // minimal run number
  int      fMaxRun    ;			// maximal run number

  stn_dataset_t() {
    fID          = "";
    fName        = "";
    fNEvents     = -1;
    fNGenEvents  = -1;
    fEMin        =  1.;                 // make sure that non-initialized case is easy to see
    fEMax        = -1.;
    fMinRun      = -1;
    fMaxRun      = -1;
  }
  
};

#endif
