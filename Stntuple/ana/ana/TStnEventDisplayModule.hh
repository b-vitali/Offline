//-----------------------------------------------------------------------------

#ifndef __murat_ana_TStnEventDisplayModule_hh__
#define __murat_ana_TStnEventDisplayModule_hh__

// ROOT includes
// ... libCore
#include <TApplication.h>
#include <TString.h>
#include <TSystem.h>
// ... libRIO
#include <TFile.h>
// ... libGui
#include <TGString.h>
#include <TGLabel.h>
#include <TGButton.h>
#include <TGButtonGroup.h>
#include <TGTextEntry.h>
#include <TGTextView.h>
#include <TGLayout.h>
#include <TGTab.h>
#include <TG3DLine.h>
// ... libGeom
#include <TGeoManager.h>
#include <TGeoTube.h>
#include <TGeoCompositeShape.h>
#include <TGeoBoolNode.h>
// ... libEG
#include <TParticle.h>
// ... libRGL
#include <TGLViewer.h>
// ... libEve
#include <TEveManager.h>
#include <TEveEventManager.h>
#include <TEveBrowser.h>
#include <TEveGeoNode.h>
#include <TEveViewer.h>
#include <TEveScene.h>
#include <TEveProjectionManager.h>
#include <TEveProjectionAxes.h>
#include <TEvePointSet.h>
#include <TEveTrack.h>
#include <TEveTrackPropagator.h>

#include "Stntuple/ana/TStnEventDisplayUtils.hh"
#include "Stntuple/ana/TStnMu2eEveMagField.hh"

#include "Stntuple/loop/TStnModule.hh"
#include "Stntuple/obj/TStnTrackBlock.hh"
#include "Stntuple/obj/TGenpBlock.hh"
#include "Stntuple/obj/TSimpBlock.hh"
#include "Stntuple/gui/TStnGeoManager.hh"

class TStnEventDisplayModule : public TStnModule {
protected:

  // Set by parameter set variables.
  
  int             fDisplayTracks;
  bool            drawHits_;
  double          hitMarkerSize_;
  double          fTrkMaxR;
  double          fTrkMaxZ;
  double          fTrkMaxStepSize;
  double          camRotateCenterH_;
  double          camRotateCenterV_;
  double          camDollyDelta_;
  
  TStnEventDisplayUtils*     fEvdUtils;
  TEveGeoShape*           fSimpleGeom;

  TStnGeoManager*         fGeoManager;
  TStnMu2eEveMagField*    fBField;
  TEveTrackPropagator*    fTrackPropagator;
  TEveTrackPropagator*    fBeamPropagator;

  TEveViewer              *fXYView;
  TEveViewer              *fRZView;
  TEveProjectionManager   *fXYMgr;
  TEveProjectionManager   *fRZMgr;
  TEveScene               *fDetXYScene;
  TEveScene               *fDetRZScene;
  TEveScene               *fEvtXYScene;
  TEveScene               *fEvtRZScene;
  
  TGTextEntry             *fTeRun,*fTeEvt;
  TGLabel                 *fTlRun,*fTlEvt;
  
  TEveTrackList           *fBeamList;
  TEveTrackList           *fTrackList;
  TEveElementList         *fHitsList;

  TStnTrackBlock*         fTrackBlock;
  TGenpBlock*             fGenpBlock;
  TSimpBlock*             fSimpBlock;

  int                     fDisplayBeamMuMinus;
  int                     fDisplayBeamMuPlus;
//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
  public:
  TStnEventDisplayModule(const char* Name = "EventDisplay", const char* Title = "EventDisplay");
  ~TStnEventDisplayModule();

  virtual int  BeginJob()           ;
  virtual int  BeginRun()           ;
  virtual int  EndJob  ()           ;
  virtual int  Event   (int IEntry) ;
  
  void MakeNavPanel();
  
  void SetDisplayBeamMuMinus(int OnOff) { fDisplayBeamMuMinus = OnOff; }
  void SetDisplayBeamMuPlus (int OnOff) { fDisplayBeamMuPlus  = OnOff; }

  ClassDef(TStnEventDisplayModule,0)
};

#endif
