//
// - ROOT-based 3D event display for ART toy experiment.  Requires
//   EvtDisplayUtils, NavState, and EvtDisplayService.
//
#include "TEnv.h"
#include "Stntuple/ana/TStnEventDisplayModule.hh"

#include <string.h>

ClassImp(TStnEventDisplayModule)

using namespace std;

//-----------------------------------------------------------------------------
TStnEventDisplayModule::TStnEventDisplayModule(const char* Name, const char* Title):
  TStnModule(Name,Title) {

  fTrkMaxStepSize = 5.;
  fTrkMaxZ        = 1300.;
  fTrkMaxR        = 1000.;

  fEvdUtils      = new TStnEventDisplayUtils();
  fHitsList      = NULL;
  fTrackList     = NULL;
  fDisplayTracks = 1;

  fDisplayBeamMuMinus = 0;
  fDisplayBeamMuPlus  = 0;

  const char* geom_gdml_file = gEnv->GetValue("mu2e.GdmlGeometry",(const char*) NULL);
  fGeoManager  = new TStnGeoManager(geom_gdml_file,0);

  // the next two lines look as if they were not doing anything... 
  TGeoVolume*  topvol = gGeoManager->GetTopVolume();
  gGeoManager->SetTopVolume(topvol);

  fBField = new TStnMu2eEveMagField();

  fTrackPropagator = new TEveTrackPropagator("my","",fBField,kFALSE);
  fTrackPropagator->SetMagFieldObj(fBField,kTRUE);
  fTrackPropagator->SetMaxR(fTrkMaxR);
  fTrackPropagator->SetMaxZ(fTrkMaxZ);
  fTrackPropagator->SetMaxStep(fTrkMaxStepSize);
  fTrackPropagator->SetMaxOrbs(100);
  fTrackPropagator->SetStepper(TEveTrackPropagator::kRungeKutta);
  
  fBeamPropagator = new TEveTrackPropagator("my_beam","",fBField,kFALSE);
  fBeamPropagator->SetMagFieldObj(fBField,kTRUE);
  fBeamPropagator->SetMaxR(fTrkMaxR);
  fBeamPropagator->SetMaxZ(540);
  fBeamPropagator->SetMaxStep(fTrkMaxStepSize);
  fBeamPropagator->SetMaxOrbs(100);
  fBeamPropagator->SetStepper(TEveTrackPropagator::kRungeKutta);
}

//-----------------------------------------------------------------------------
TStnEventDisplayModule::~TStnEventDisplayModule() {
  delete fEvdUtils;
}


//-----------------------------------------------------------------------------
// Create control panel for event navigation
//-----------------------------------------------------------------------------
void TStnEventDisplayModule::MakeNavPanel() {

  TEveBrowser* browser = gEve->GetBrowser();
  browser->StartEmbedding(TRootBrowser::kLeft); // insert nav frame as new tab in left pane

  TGMainFrame* frmMain = new TGMainFrame(gClient->GetRoot(), 1000, 600);
  frmMain->SetWindowName("EVT NAV");
  frmMain->SetCleanup(kDeepCleanup);

  TGHorizontalFrame* navFrame = new TGHorizontalFrame(frmMain);
  TGVerticalFrame* evtidFrame = new TGVerticalFrame(frmMain);
  {
    TString icondir(TString::Format("%s/icons/", gSystem->Getenv("ROOTSYS")) );
    TGPictureButton* b = 0;

    // ... Create back button and connect to "PrevEvent" rcvr in visutils
    b = new TGPictureButton(navFrame, gClient->GetPicture(icondir + "GoBack.gif"));
    navFrame->AddFrame(b);
    b->Connect("Clicked()", "TStnEventDisplayUtils", fEvdUtils, "PrevEvent()");

    // ... Create forward button and connect to "NextEvent" rcvr in visutils
    b = new TGPictureButton(navFrame, gClient->GetPicture(icondir + "GoForward.gif"));
    navFrame->AddFrame(b);
    b->Connect("Clicked()", "TStnEventDisplayUtils", fEvdUtils, "NextEvent()");

    // ... Create run num text entry widget and connect to "GotoEvent" rcvr in visutils
    TGHorizontalFrame* runoFrame = new TGHorizontalFrame(evtidFrame);
    fTlRun = new TGLabel(runoFrame,"Run Number");
    fTlRun->SetTextJustify(kTextLeft);
    fTlRun->SetMargins(5,5,5,0);
    runoFrame->AddFrame(fTlRun);
    
    fTeRun = new TGTextEntry(runoFrame, fEvdUtils->fTbRun = new TGTextBuffer(5), 1);
    fEvdUtils->fTbRun->AddText(0, "1");
    fTeRun->Connect("ReturnPressed()","TStnEventDisplayUtils", fEvdUtils,"GotoEvent()");
    runoFrame->AddFrame(fTeRun,new TGLayoutHints(kLHintsExpandX));

    // ... Create evt num text entry widget and connect to "GotoEvent" rcvr in visutils
    TGHorizontalFrame* evnoFrame = new TGHorizontalFrame(evtidFrame);
    fTlEvt = new TGLabel(evnoFrame,"Evt Number");
    fTlEvt->SetTextJustify(kTextLeft);
    fTlEvt->SetMargins(5,5,5,0);
    evnoFrame->AddFrame(fTlEvt);

    fTeEvt = new TGTextEntry(evnoFrame, fEvdUtils->fTbEvt = new TGTextBuffer(5), 1);
    fEvdUtils->fTbEvt->AddText(0, "1");
    fTeEvt->Connect("ReturnPressed()","TStnEventDisplayUtils", fEvdUtils,"GotoEvent()");
    evnoFrame->AddFrame(fTeEvt,new TGLayoutHints(kLHintsExpandX));

    // ... Add horizontal run & event number subframes to vertical evtidFrame
    evtidFrame->AddFrame(runoFrame,new TGLayoutHints(kLHintsExpandX));
    evtidFrame->AddFrame(evnoFrame,new TGLayoutHints(kLHintsExpandX));

    // ... Add navFrame and evtidFrame to MainFrame
    frmMain->AddFrame(navFrame);
    TGHorizontal3DLine *separator = new TGHorizontal3DLine(frmMain);
    frmMain->AddFrame(separator, new TGLayoutHints(kLHintsExpandX));
    frmMain->AddFrame(evtidFrame);

    frmMain->MapSubwindows();
    frmMain->Resize();
    frmMain->MapWindow();

    browser->StopEmbedding();
    browser->SetTabTitle("Event Nav", 0);
  }
}

//-----------------------------------------------------------------------------
int TStnEventDisplayModule::BeginJob() {

  RegisterDataBlock("TrackBlock"     ,"TStnTrackBlock"   ,&fTrackBlock);
  RegisterDataBlock("GenpBlock"      ,"TGenpBlock"       ,&fGenpBlock );
  RegisterDataBlock("SimpBlock"      ,"TSimpBlock"       ,&fSimpBlock );

  // Initialize global Eve application manager (return gEve)
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  TEveManager::Create();

  // Create detector and event scenes for ortho views
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  fDetXYScene = gEve->SpawnNewScene("Det XY Scene", "");
  fDetRZScene = gEve->SpawnNewScene("Det RZ Scene", "");
  fEvtXYScene = gEve->SpawnNewScene("Evt XY Scene", "");
  fEvtRZScene = gEve->SpawnNewScene("Evt RZ Scene", "");

  // Create XY/RZ projection mgrs, draw projected axes, & add them to scenes
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  fXYMgr = new TEveProjectionManager(TEveProjection::kPT_RPhi);
  TEveProjectionAxes* axes_xy = new TEveProjectionAxes(fXYMgr);
  fDetXYScene->AddElement(axes_xy);

  fRZMgr = new TEveProjectionManager(TEveProjection::kPT_RhoZ);
  TEveProjectionAxes* axes_rz = new TEveProjectionAxes(fRZMgr);
  fDetRZScene->AddElement(axes_rz);

  // Create side-by-side ortho XY & RZ views in new tab & add det/evt scenes
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  TEveWindowSlot *slot = 0;
  TEveWindowPack *pack = 0;

  slot = TEveWindow::CreateWindowInTab(gEve->GetBrowser()->GetTabRight());
  pack = slot->MakePack();
  pack->SetElementName("Ortho Views");
  pack->SetHorizontal();
  pack->SetShowTitleBar(kFALSE);

  pack->NewSlot()->MakeCurrent();
  fXYView = gEve->SpawnNewViewer("XY View", "");
  fXYView->GetGLViewer()->SetCurrentCamera(TGLViewer::kCameraOrthoXOY);
  fXYView->AddScene(fDetXYScene);
  fXYView->AddScene(fEvtXYScene);

  pack->NewSlot()->MakeCurrent();
  fRZView = gEve->SpawnNewViewer("RZ View", "");
  fRZView->GetGLViewer()->SetCurrentCamera(TGLViewer::kCameraOrthoXOY);
  fRZView->AddScene(fDetRZScene);
  fRZView->AddScene(fEvtRZScene);

  gEve->GetBrowser()->GetTabRight()->SetTab(0);

  // Create navigation panel
  // ~~~~~~~~~~~~~~~~~~~~~~~~
  MakeNavPanel();

  // Add new Eve event into the "Event" scene and make it the current event
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // (Subsequent elements added using "AddElements" will be added to this event)
  gEve->AddEvent(new TEveEventManager("Event", "Mu2e Event"));

  // ... Set up initial camera orientation in main 3d view
  //   - rotate camera by "camRotateCenterH_" radians about horizontal going through
  //     center, followed by "camRotateCenterV_" radians about vertical going through
  //     center
  //   - move camera by "camDollyDelta_" units towards(+'ve) or away from(-'ve) center,
  //     combination of two boolean args controls sensitivity.
  TGLViewer *glv = gEve->GetDefaultGLViewer();
  glv->SetGuideState(TGLUtil::kAxesEdge, kTRUE, kFALSE, 0);
  glv->CurrentCamera().RotateRad(camRotateCenterH_,camRotateCenterV_);
  glv->CurrentCamera().Dolly(camDollyDelta_,kFALSE,kFALSE);

  return 0;
}

//-----------------------------------------------------------------------------
int TStnEventDisplayModule::BeginRun() {

  // if(gGeoManager){
  //   gGeoManager->GetListOfNodes()->Delete();
  //   gGeoManager->GetListOfVolumes()->Delete();
  //   gGeoManager->GetListOfShapes()->Delete();
  // }

  gEve->GetGlobalScene()->DestroyElements();
  fDetXYScene->DestroyElements();
  fDetRZScene->DestroyElements();
  // // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // // Using the detector specifications provided by the geometry service, create
  // // a TGeoCompositeShape for drawing the detector in the main 3D view and an 
  // // TEveElementList for drawing the detector in the 2D orthographic views
  // // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // TGeoShape *composite=0;
  TEveElementList *orthodet = new TEveElementList("OrthoDet");
  // std::string layerid="Layer %d";
  // Int_t i=0;
  // for ( auto const& shell : geom_->tracker().shells() ){
  //   Double_t dz = 0.2*shell.halfLength();
  //   Double_t rmin = 0.1*shell.radius() ;
  //   Double_t rmax = rmin+0.1*shell.thickness();
  //   if(i==0){
  //     composite = new TGeoTube(Form(layerid.c_str(),i),rmin,rmax,dz); 
  //   }else{
  //     TGeoShape *gs = new TGeoTube(Form(layerid.c_str(),i),rmin,rmax,dz); 
  //     TGeoBoolNode *bn = new TGeoUnion(composite,gs);
  //     composite = new TGeoCompositeShape("TGeoCompositeShape", bn);
  //   }
  //   TEveGeoShape *egs = new TEveGeoShape(Form(layerid.c_str(),i));
  //   egs->SetShape(new TGeoTube(rmin, rmax, dz));
  //   egs->SetMainColor(kPink+7);
  //   orthodet->AddElement(egs);
  //   i++;
  // }

  // // ... Create tracker out of Silicon using the composite shape defined above
  // TGeoVolume *tracker = new TGeoVolume("Tracker",composite, Si);
  // tracker->SetVisLeaves(kTRUE);
  // topvol->AddNode(tracker, 1, new TGeoTranslation(0,0,0));
  // gGeoManager->CloseGeometry();

  TGeoNode* topnode        = gGeoManager->GetTopNode();
  TEveGeoTopNode* etopnode = new TEveGeoTopNode(gGeoManager, topnode);
  etopnode->SetVisLevel(5);
  etopnode->GetNode()->GetVolume()->SetVisibility(kFALSE);

  // ... Use helper to recursively make inner/outer tracker descendants 
  //     transparent & set custom colors

  fGeoManager->HideBuilding(0);
  fGeoManager->HideTsCoils (1);
    
  // ... Add static detector geometry to global scene
  gEve->AddGlobalElement(etopnode);

  // Draw the 2D projections using the EVE element list created above 
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // .... Add the EVE element list to the global scene first
  gEve->AddGlobalElement(orthodet);

  // ... Import elements of the list into the projected views
  fXYMgr->ImportElements(orthodet, fDetXYScene);
  fRZMgr->ImportElements(orthodet, fDetRZScene);

  // ... Turn OFF rendering of duplicate detector in main 3D view
  gEve->GetGlobalScene()->FindChild("OrthoDet")->SetRnrState(kFALSE);

  // ... Turn ON rendering of detector in RPhi and RZ views
  //  fDetXYScene->FindChild("OrthoDet")->SetRnrState(kTRUE);
  //  fDetRZScene->FindChild("OrthoDet")->SetRnrState(kTRUE);
  return 0;
}


//-----------------------------------------------------------------------------
int TStnEventDisplayModule::Event(int IEntry) {

  // ... Update the run and event numbers in the TGTextEntry widgets in the Navigation panel
  //  std::ostringstream sstr;
  //  sstr << event.id().run();
  // fEvdUtils->fTbRun->Clear();
  // fEvdUtils->fTbRun->AddText(0,sstr.str().c_str());

  // get gata
  fTrackBlock->GetEntry(IEntry);
  fGenpBlock ->GetEntry(IEntry);
  fSimpBlock ->GetEntry(IEntry);

  gClient->NeedRedraw(fTeRun);

  // sstr.str("");
  // sstr << event.id().event();
  // fEvdUtils->fTbEvt->Clear();
  // fEvdUtils->fTbEvt->AddText(0,sstr.str().c_str());
  gClient->NeedRedraw(fTeEvt);

  // ... Delete visualization structures associated with previous event
  gEve->GetViewers()->DeleteAnnotations();
  gEve->GetCurrentEvent()->DestroyElements();

  // Draw the detector hits
  // ~~~~~~~~~~~~~~~~~~~~~~~
  if (drawHits_) {
    // std::vector<art::Handle<IntersectionCollection>> hitsHandles;
    // event.getManyByType(hitsHandles);

    if (fHitsList == 0) {
      fHitsList = new TEveElementList("Hits"); 
      fHitsList->IncDenyDestroy();              // protect element against destruction
    }
    else {
      fHitsList->DestroyElements();             // destroy children of the element
    }

    TEveElementList* KpHitsList  = new TEveElementList("K+ Hits"); 
    TEveElementList* KmHitsList  = new TEveElementList("K- Hits"); 
    TEveElementList* BkgHitsList = new TEveElementList("Bkg Hits"); 

    // int ikp=0,ikm=0,ibkg=0;
    // for ( auto const& handle: hitsHandles ){
    //   for ( auto const& hit: *handle ){
    //     if ( hit.genTrack()->pdgId() == PDGCode::K_plus ){
    //       drawHit("K+",kGreen,hitMarkerSize_,ikp++,hit,KpHitsList);
    //     } else if ( hit.genTrack()->pdgId() == PDGCode::K_minus ){
    //       drawHit("K-",kYellow,hitMarkerSize_,ikm++,hit,KmHitsList);
    //     } else{
    //       drawHit("Bkg",kViolet+1,hitMarkerSize_,ibkg++,hit,BkgHitsList);
    //     }
    //   }
    // }

    fHitsList->AddElement(KpHitsList);  
    fHitsList->AddElement(KmHitsList);  
    fHitsList->AddElement(BkgHitsList);  
    gEve->AddElement(fHitsList);
  }

  // Draw the generated tracks as helices in a uniform axial field
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (fDisplayTracks) {

    if (fTrackList == 0) {
      fTrackList = new TEveTrackList("Tracks"); 
      fTrackList->SetLineWidth(2);
      fTrackList->SetLineColor(kRed);
      fTrackList->IncDenyDestroy();                 // protect element against destruction

      fBeamList = new TEveTrackList("BeamTracks"); 
      fBeamList->SetLineWidth(2);
      fBeamList->SetLineColor(kGreen+3);
      fBeamList->IncDenyDestroy();                 // protect element against destruction
    }
    else {
      fTrackList->DestroyElements();                // destroy children of the element
      fBeamList->DestroyElements();                // destroy children of the element
    }

    fTrackList->SetPropagator(fTrackPropagator);
    fBeamList->SetPropagator(fBeamPropagator);
//-----------------------------------------------------------------------------
// initialize beam
//-----------------------------------------------------------------------------
    double px(20.), py(20.), pz(15.), mmu(105.658), e;

    e = sqrt(px*px+py*py+pz*pz+mmu*mmu);

    if (fDisplayBeamMuMinus != 0) {
      TParticle mcbeam(13,0,-1,-1,-1,-1,px*1e-3,py*1e-3,pz*1e-3,e*1e-3,390.5,-1.5,-400,0);
      TEveTrack* beam = new TEveTrack(&mcbeam,0,fBeamPropagator);
      beam->SetIndex(0);
      beam->SetStdTitle();
      beam->SetAttLineAttMarker(fBeamList);

      beam->SetMainColor(kGreen+3);
      fBeamList->AddElement(beam);
    }
    
    if (fDisplayBeamMuPlus != 0) {
      TParticle mcbeam1(-13,0,-1,-1,-1,-1,px*1e-3,py*1e-3,pz*1e-3,e*1e-3,385.5, 3.5,-400,0);
      TEveTrack* beam1 = new TEveTrack(&mcbeam1,0,fBeamPropagator);
      beam1->SetIndex(0);
      beam1->SetStdTitle();
      beam1->SetAttLineAttMarker(fBeamList);
      
      beam1->SetMainColor(kRed+3);
      fBeamList->AddElement(beam1);
    }

    //    if (fBeamList->GetNItems() > 0) {
      fBeamList->MakeTracks();
      gEve->AddElement(fBeamList);
      //    }
//-----------------------------------------------------------------------------
// initialize tracks
//-----------------------------------------------------------------------------
    int np = fGenpBlock->NParticles();
    for (int i=0; i<np; i++) {
      TGenParticle* genp = fGenpBlock->Particle(i);

      TParticle mcpart(*genp);
//-----------------------------------------------------------------------------
// convert particle momentum from MeV to GeV and coordinates - from mm to cm
//-----------------------------------------------------------------------------
      mcpart.SetMomentum        (genp->Px()*1e-3,genp->Py()*1e-3,genp->Pz()*1e-3,genp->Energy()*1.e-3);
      mcpart.SetProductionVertex(genp->Vx()*0.1,genp->Vy()*0.1,genp->Vz()*0.1,genp->T());

      TEveTrack* track = new TEveTrack(&mcpart,i,fTrackPropagator);

      if (i == 0) {
	fBeamPropagator->SetMaxZ(genp->Vz()*0.1);
      }

      track->SetIndex(0);
      track->SetStdTitle();
      track->SetAttLineAttMarker(fTrackList);

      track->SetMainColor(kRed+1);
      fTrackList->AddElement(track);
    }

    fTrackList->MakeTracks();
    gEve->AddElement(fTrackList);
  }
  
  // Import event into ortho views and apply projections
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  TEveElement* currevt = gEve->GetCurrentEvent();

  fEvtXYScene->DestroyElements();
  fXYMgr->ImportElements(currevt, fEvtXYScene);

  fEvtRZScene->DestroyElements();
  fRZMgr->ImportElements(currevt, fEvtRZScene);

  gEve->Redraw3D(kFALSE,kTRUE);
    
  // gApplication->Run(kTRUE);

  // if(!gEve){
  //   gApplication->SetReturnFromRun(kFALSE);
  //   gApplication->Terminate(0);
  // }

  return 0;
}


//-----------------------------------------------------------------------------
int TStnEventDisplayModule::EndJob() {
  return 0;
}
