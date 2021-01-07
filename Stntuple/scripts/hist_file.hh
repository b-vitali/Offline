//-----------------------------------------------------------------------------
// a 'dataset' here is a histogram file plus data fields for plotting attributes
//-----------------------------------------------------------------------------
#ifndef __Stntuple_scripts_hist_file_hh__
#define __Stntuple_scripts_hist_file_hh__

#include "stn_dataset.hh"

struct hist_file_t {

  TString         fName   ;			// histogram file name
  TString         fJobName;			// job name
  stn_dataset_t*  fDataset;			// pointer to the STN dataset

  hist_file_t() {
    fName        = "";
    fJobName     = "";
    fDataset     = nullptr;
  }
  
};

#endif
