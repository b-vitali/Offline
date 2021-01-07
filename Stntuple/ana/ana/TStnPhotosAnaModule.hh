///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#ifndef TStnPhotosAnaModule_hh
#define TStnPhotosAnaModule_hh

#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"

#include "Stntuple/loop/TStnModule.hh"
#include "Stntuple/obj/TStnHeaderBlock.hh"
#include "Stntuple/obj/TGenpBlock.hh"

#include "mc/photos/TPhotos.hh"

class TStnPhotosAnaModule: public TStnModule {
public:

  struct EventHist_t {
    TH1F*    fRunNumber;
    TH1F*    fNGenp;
    TH1F*    fNPhotons;			// N(photons)
    TH1F*    fMomLO[2];			// momentum
    TH1F*    fELO  [2];			// energy
  };

  struct GenpHist_t {
    TH1F*   fMom   [2];			// particle momentum
    TH1F*   fEnergy[2];			// particle energy
  };


  enum { kNEventHistSets = 100 }; 
  enum { kNGenpHistSets  = 100 }; 

public:
//-----------------------------------------------------------------------------
// pointers to the data blocks used
//-----------------------------------------------------------------------------
  TGenpBlock*       fGenpBlock;
//-----------------------------------------------------------------------------
// histograms filled
//-----------------------------------------------------------------------------
  struct Hist_t {
    GenpHist_t*        fGenp [kNGenpHistSets ];
    EventHist_t*       fEvent[kNEventHistSets];
  } fHist;
//-----------------------------------------------------------------------------
//  filtering
//-----------------------------------------------------------------------------
  TPhotos*   fPhotos;
  
  int        fNPhotons;
  double     fP;
  double     fE;
  double     fWeight;
//-----------------------------------------------------------------------------
//  functions
//-----------------------------------------------------------------------------
public:
  TStnPhotosAnaModule(const char* name="StnPhotosAna", const char* title="StnPhotosAna");
  ~TStnPhotosAnaModule();
//-----------------------------------------------------------------------------
// accessors
//-----------------------------------------------------------------------------
  Hist_t*       Hist       () { return &fHist; }

  TGenpBlock*       GetGenpBlock  () { return fGenpBlock; }
//-----------------------------------------------------------------------------
// setters
// other methods
//-----------------------------------------------------------------------------
  void              BookHistograms();
  void              BookEventHistograms(EventHist_t* Hist, const char* Folder);
  void              BookGenpHistograms (GenpHist_t*  Hist, const char* Folder);

  void              FillEventHistograms(EventHist_t* Hist);
  void              FillGenpHistograms (GenpHist_t*  Hist, TGenParticle* Genp);
  void              FillHistograms     ();
//-----------------------------------------------------------------------------
// overloaded methods of TStnModule
//-----------------------------------------------------------------------------
  virtual int       BeginJob();
  virtual int       BeginRun();
  virtual int       Event   (int ientry);
  virtual int       EndRun  ();
  virtual int       EndJob  ();

  int Debug();
//-----------------------------------------------------------------------------
// overloaded methods of TObject
//-----------------------------------------------------------------------------
  void         Print(Option_t* opt) const ;

  ClassDef(TStnPhotosAnaModule,0)
};

#endif
