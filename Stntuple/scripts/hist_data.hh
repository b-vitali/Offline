//-----------------------------------------------------------------------------
// a 'dataset' here is a histogram file plus data fields for plotting attributes
//-----------------------------------------------------------------------------
#ifndef __Stntuple_scripts_hist_data_hh__
#define __Stntuple_scripts_hist_data_hh__

#include "hist_file.hh"

struct hist_data_t {
  TH1*          fHist;                  // if non-null, fFile is not needed
  hist_file_t*  fFile;                  // it contains a pointer to the dataset
  TString       fLabel  ;		// label to appear on a plot
  TString       fModule;	        // module name
  TString       fName;	                // histogram name, as saved in a file
  TString       fNewName;	        // redefines histogram name at plotting time
  int           fLineColor;             // these are utility fields to be used as needed
  int           fLineWidth;
  int           fFillColor;             // these are utility fields to be used as needed
  int           fFillStyle;             // these are utility fields to be used as needed
  int           fMarkerStyle;
  float         fMarkerSize;
  int           fMarkerColor;
  float         fXMin;
  float         fXMax;
  float         fYMin;
  float         fYMax;
  float         fStatBoxXMin;
  float         fStatBoxYMin;
  float         fStatBoxXMax;
  float         fStatBoxYMax;
  int           fRebin;
  int           fYLogScale;
  long int      fScale;			// 1:normalize to NGenEvents, 2:normalize to integral
  TString       fPlotName;
  TString       fPlotLabel;
  TString       fXAxisTitle;
  float         fLegendXMin;
  float         fLegendYMin;
  float         fLegendXMax;
  float         fLegendYMax;
  TCanvas*      fCanvas;
  TString       fOutputFn;

  hist_data_t(const char* Dataset = "", const char* JobName = "", const char* Module = "", const char* HistName = "") {
    fHist        = nullptr;
    fFile        = nullptr; // get_hist_file(Dataset,JobName);
    fName        = HistName;
    fModule      = Module;
    fNewName     = "";
    fLabel       = "";
    fLineColor   = -1;
    fLineWidth   =  1;
    fMarkerSize  = -1;
    fMarkerStyle = -1;
    fMarkerColor = -1;
    fFillColor   = -1;
    fFillStyle   = -1;
    fXMin        =  0;
    fXMax        = -1;
    fYMin        =  0;
    fYMax        = -1;
    fStatBoxXMin =  0.65;
    fStatBoxYMin =  0.65;
    fStatBoxXMax =  0.90;
    fStatBoxYMax =  0.90;
    fRebin       = -1;
    fYLogScale   =  0;
    fScale       = -1;
    fPlotName    = "";
    fPlotLabel   = "";
    fXAxisTitle  = "";
    fLegendXMin  = -1;
    fLegendYMin  = -1;
    fLegendXMax  = -1;
    fLegendYMax  = -1;
  }
  
  hist_data_t(TH1* Hist, const char* JobName = "", const char* Module = "") {
    fHist        = Hist;
    fFile        = nullptr; // get_hist_file(Dataset,JobName);
    fName        = Hist->GetName();
    fModule      = Module;
    fNewName     = "";
    fLabel       = "";
    fLineColor   = -1;
    fLineWidth   =  1;
    fMarkerSize  = -1;
    fMarkerStyle = -1;
    fMarkerColor = -1;
    fFillColor   = -1;
    fFillStyle   = -1;
    fXMin        =  0;
    fXMax        = -1;
    fYMin        =  0;
    fYMax        = -1;
    fStatBoxXMin =  0.65;
    fStatBoxYMin =  0.65;
    fStatBoxXMax =  0.90;
    fStatBoxYMax =  0.90;
    fRebin       = -1;
    fYLogScale   =  0;
    fScale       = -1;
    fPlotName    = "";
    fPlotLabel   = "";
    fXAxisTitle  = "";
    fLegendXMin  = -1;
    fLegendYMin  = -1;
    fLegendXMax  = -1;
    fLegendYMax  = -1;
  }
  
};

#endif
