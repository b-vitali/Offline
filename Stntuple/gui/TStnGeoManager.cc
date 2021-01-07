///////////////////////////////////////////////////////////////////////////////
// draw different parts of Mu2e
//
// dmg = new DrawMu2eGeometry("mu2e.gdml")
// dmg->HideBuilding()
// dmg->fGm->GetVolume("HallAir")->Draw("ogl")
//
///////////////////////////////////////////////////////////////////////////////
#include "Stntuple/gui/TStnGeoManager.hh"

#include "TGeoManager.h"

ClassImp(TStnGeoManager)

//-----------------------------------------------------------------------------
TStnGeoManager::TStnGeoManager() {
  fTop       = NULL;
  fDs2Vacuum = NULL;
  fDs3Vacuum = NULL;
  fSttMother = NULL;
  fTrkMother = NULL;
  fCalMother = NULL;
  fMbsMother = NULL;
  fTransp    = -1;
}

//-----------------------------------------------------------------------------
TStnGeoManager::TStnGeoManager(const char* Fn, int OriginalColors) {
  TGeoManager::Import(Fn);

  fTop       = gGeoManager->GetTopNode();

  fDs2Vacuum = FindNodeByVolumeName(fTop,"DS2Vacuum");
  fDs3Vacuum = FindNodeByVolumeName(fTop,"DS3Vacuum");

  fSttMother = FindNodeByVolumeName(fDs2Vacuum,"StoppingTargetMother");

  fTrkMother = FindNodeByVolumeName(fDs3Vacuum,"TrackerMother");
  fCalMother = FindNodeByVolumeName(fDs3Vacuum,"CalorimeterMother");
  fMbsMother = FindNodeByVolumeName(fDs3Vacuum,"MBSMother");

  fTransp = 40;
  
  HideBuilding(OriginalColors);
}

//-----------------------------------------------------------------------------
TStnGeoManager::~TStnGeoManager() {
}

//-----------------------------------------------------------------------------
TGeoNode* TStnGeoManager::FindNodeByName(TGeoNode* Top, const char* Name) {
  TGeoNode  *top, *found(0);

  if (Top) top = fTop;
  else     top = gGeoManager->GetTopNode();
  
  TObjArray* o =  top->GetNodes();

  int n = o->GetEntriesFast();
  
  for (int i=0; i<n; i++) {
    TGeoNode* node = (TGeoNode*) o->UncheckedAt(i);
    const char* name = node->GetName();
    if (strcmp(name,Name) == 0) {
      found = node;
      break;
    }
    else if (node->GetNodes() != NULL) {
      found = FindNodeByName(node,Name);
      if (found) break;
    }
  }
  return found;
}

//-----------------------------------------------------------------------------
TGeoNode* TStnGeoManager::FindNodeByVolumeName(TGeoNode* Top, const char* VolumeName) {
  TGeoNode  *top, *found(0);

  if (Top) top = Top;
  else     top = fTop;
  
  TObjArray* o =  top->GetNodes();

  int n = o->GetEntriesFast();
  
  for (int i=0; i<n; i++) {
    TGeoNode* node = (TGeoNode*) o->UncheckedAt(i);
    const char* name = node->GetVolume()->GetName();
    if (strcmp(name,VolumeName) == 0) {
      found = node;
      break;
    }
    else if (node->GetNodes() != NULL) {
      found = FindNodeByVolumeName(node,VolumeName);
      if (found) break;
    }
  }
  return found;
}

//-----------------------------------------------------------------------------
// assume that we're looking for one of the daughters
//-----------------------------------------------------------------------------
TGeoNode* TStnGeoManager::FindNodeByVolumeName(TGeoVolume* Top, const char* VolumeName) {
  TGeoVolume  *top;
  TGeoNode*    found(0);

  if (Top) top = Top;
  else     top = gGeoManager->GetTopVolume();

  TObjArray* o =  top->GetNodes();

  int n = o->GetEntriesFast();
  
  for (int i=0; i<n; i++) {
    TGeoNode* node = (TGeoNode*) o->UncheckedAt(i);
    const char* name = node->GetVolume()->GetName();
    if (strcmp(name,VolumeName) == 0) {
      found = node;
      break;
    }
    else if (node->GetNodes() != NULL) {
      found = FindNodeByVolumeName(node,VolumeName);
      if (found) break;
    }
  }
  return found;
}

//-----------------------------------------------------------------------------
void TStnGeoManager::SetRecursiveVisibilityByName(TGeoNode* Node, const char* Pattern, int OnOff) {

  TString name(Node->GetName());
  
  if (name.Index(Pattern) >= 0) {
    Node->SetVisibility(OnOff);
    //std::cout <<"hiding "<< name << std::endl;
  }
				        // Descend recursively into each daughter TGeoNode.
  int nd = Node->GetNdaughters();
  for (int i=0; i<nd; ++i) {
    TGeoNode * d = Node->GetDaughter(i);
    SetRecursiveVisibilityByName(d,Pattern,OnOff);
  }
}

//-----------------------------------------------------------------------------
void TStnGeoManager::SetRecursiveVisibility(TGeoNode* Node, int OnOff) {

  TString name(Node->GetName());
  
  Node->SetVisibility(OnOff);
    //std::cout <<"hiding "<< name << std::endl;
				        // Descend recursively into each daughter TGeoNode.
  int nd = Node->GetNdaughters();
  for (int i=0; i<nd; ++i) {
    TGeoNode * d = Node->GetDaughter(i);
    SetRecursiveVisibility(d,OnOff);
  }
}

//-----------------------------------------------------------------------------
void TStnGeoManager::SetRecursiveVisibilityByMaterial(TGeoNode* Node, const char* Material, int OnOff) {

  TString mat(Node->GetVolume()->GetMaterial()->GetName());
  
  if (mat.Index(Material) >= 0) Node->SetVisibility(OnOff);

				        // Descend recursively into each daughter TGeoNode.
  int ndau = Node->GetNdaughters();
  for ( int i=0; i<ndau; ++i ){
    TGeoNode * d = Node->GetDaughter(i);
    SetRecursiveVisibilityByMaterial(d,Material,OnOff);
  }
}

//-----------------------------------------------------------------------------
void TStnGeoManager::SetRecursiveColorTransp(TGeoVolume *Vol, Int_t Color, Int_t Transp) {

  TString name = Vol->GetName();
  
  int col    = Color;
  int transp = Transp;

  if      (name.Index("TargetFoil") >= 0) { col = kBlue+4;  transp = 10; }
  else if (name.Index("CaloPipe"  ) >= 0) { col = kOrange+7; }
    
  if (col    >=0 ) Vol->SetLineColor   (col   );
  if (Transp >=0 ) Vol->SetTransparency(transp);
     
  int nd = Vol->GetNdaughters();
  for (int i=0; i<nd; i++) {
    TGeoVolume* vd = Vol->GetNode(i)->GetVolume();
    SetRecursiveColorTransp(vd, Color, transp);
  }
}

//-----------------------------------------------------------------------------
void TStnGeoManager::SetRecursiveColorTranspByName(TGeoNode* Node, const char* Name, Int_t Color, Int_t Transp) {

  
  TString node_name = Node->GetName();
  TGeoVolume*  vol = Node->GetVolume();

  if (node_name.Index(Name) >= 0) {
    vol->SetLineColor   (Color );
    vol->SetTransparency(Transp);
  }
     
  int nd = vol->GetNdaughters();
  for (int i=0; i<nd; i++) {
    TGeoNode* dn = vol->GetNode(i);
    SetRecursiveColorTranspByName(dn, Name, Color, Transp);
  }
}

//-----------------------------------------------------------------------------
void TStnGeoManager::SetRecursiveColorTranspByMaterial(TGeoNode* Node, const char* MatName, Int_t Color, Int_t Transp) {

  
  TGeoVolume*  vol = Node->GetVolume();
  TString mat_name = vol->GetMaterial()->GetName();

  if (mat_name.Index(MatName) >= 0) {
    vol->SetLineColor   (Color );
    vol->SetTransparency(Transp);
  }
     
  int nd = vol->GetNdaughters();
  for (int i=0; i<nd; i++) {
    TGeoNode* dn = vol->GetNode(i);
    SetRecursiveColorTranspByMaterial(dn, MatName, Color, Transp);
  }
}

//-----------------------------------------------------------------------------
void TStnGeoManager::SetRecursiveVisibilityColorTranspByNameAndMaterial(TGeoNode*   Top    ,
									const char* Name   ,
									const char* MatName,
									int         Visibility,
									int         Color  ,
									int         Transp) {
  TGeoVolume*  vol = Top->GetVolume();
  TString name     = vol->GetName();
  TString mat_name = vol->GetMaterial()->GetName();

  if ((name.Index(Name) >= 0) && (mat_name.Index(MatName) >= 0)) {
    Top->SetVisibility  (Visibility);
    vol->SetLineColor   (Color );
    vol->SetTransparency(Transp);
  }
     
  int nd = vol->GetNdaughters();
  for (int i=0; i<nd; i++) {
    TGeoNode* node = vol->GetNode(i);
    SetRecursiveVisibilityColorTranspByNameAndMaterial(node,Name,MatName,Visibility,Color,Transp);
  }
}

//-----------------------------------------------------------------------------
// set default colors
//-----------------------------------------------------------------------------
void TStnGeoManager::SetDefaultColorTransp(int Transp) {
  SetRecursiveColorTransp(fTop->GetVolume(),kCyan-10,Transp);
  fTransp = Transp;

//-----------------------------------------------------------------------------
// color target foils
//-----------------------------------------------------------------------------
  SetRecursiveColorTranspByName(fSttMother,"TargetFoil",kGray+3,60);


}


//-----------------------------------------------------------------------------
void TStnGeoManager::HideTsCoils(int KeepOriginalColors) {

  // Volumes will be made invisible if their name contains one
  // of these strings.
  
  static TString name[] = {
    "TS1_Coil", "TS2_Coil", "TS3_Coil", "TS4_Coil", "TS5_Coil",
    "pBendLogic", "pipeLogic",
    "centerRing", "leftSideRing", "rightSideRing",
    ""
  };

  for (int i=0; name[i] != ""; i++) {
    SetRecursiveVisibilityByName(fTop,name[i].Data(),0);
  }
}

//-----------------------------------------------------------------------------
void TStnGeoManager::HideDsCoils(int KeepOriginalColors) {

  // Volumes will be made invisible if their name contains one
  // of these strings.
  
  static TString name[] = {
    "Ceiling", "backfill", "dirt", "concrete",
    ""
  };

  for (int i=0; name[i] != ""; i++) {
    SetRecursiveVisibilityByName(fTop,name[i].Data(),0);
  }
}

//-----------------------------------------------------------------------------
void TStnGeoManager::HideBuilding(int KeepOriginalColors) {

  // Volumes will be made invisible if their name contains one
  // of these strings.
  //"CRS", "ExtShield""CRV"
  
  static TString name[] = {
    "Ceiling", "backfill", "dirt", "concrete",
    "VirtualDetector",
    "pipeType",
    "CRSAluminium",        // CRV
    "CRV","CRS","crv",     // CRV
    "ElectronicRackBox",   // electronics aside
    "ExtShield",
    "ExtMon",              // ExtMon
    "collimator1Channel",  // ExtMon
    "collimator2Channel",  // ExtMon
    "EMFPlane"          ,  // ExtMon
    "coll2Shielding",
    "pBendType22",         // who knows what it is ?
    "ProtonBeam",
    "collimatorFOV",
    "FOVliner",
				// pieces inside DS
    "DSCoil",
    "DSSpacer",
    "DScenterRing",
    "DSleftSideRing",
    "DSrightSideRing",
    "BearingBlock",
				// calorimeter
    "DiskFEB",

    "VPSP",			// pieces behind the calorimeter
    "IFB",
    
    "stmMagnet",           // STM magnet and its support
    "stmDet",              // STM far behind
    "collimatorSS",	     // STM
    
    "PSEnclosureShell",    // part of PS
    "PSEnclosureWindow",   // part of PS
    
    "BearingBlock_DS2",
    ""
  };

  for (int i=0; name[i] != ""; i++) {
    SetRecursiveVisibilityByName(fTop,name[i].Data(),0);
  }
//-----------------------------------------------------------------------------
// Volumes with these material names will be made invisible : 
//-----------------------------------------------------------------------------
  static TString material[] = {
    "MBOverburden",
    "CONCRETE",
    "BARITE",
    ""
  };

  for (int i=0; material[i] != ""; i++) {
    SetRecursiveVisibilityByMaterial(fTop,material[i].Data(),0);
  }

//-----------------------------------------------------------------------------
// hide last saddle boxes
//-----------------------------------------------------------------------------
  SetRecursiveVisibilityByName(fTop,"SaddleBox_107",0);
  SetRecursiveVisibilityByName(fTop,"SaddleBox_108",0);
  SetRecursiveVisibilityByName(fTop,"SaddleBox_109",0);
  SetRecursiveVisibilityByName(fTop,"SaddleBox_110",0);
  SetRecursiveVisibilityByName(fTop,"SaddleBox_111",0);
  SetRecursiveVisibilityByName(fTop,"SaddleBox_112",0);
  SetRecursiveVisibilityByName(fTop,"SaddleBox_113",0);
  SetRecursiveVisibilityByName(fTop,"SaddleBox_114",0);
  SetRecursiveVisibilityByName(fTop,"SaddleBox_115",0);
  SetRecursiveVisibilityByName(fTop,"SaddleBox_116",0);
  SetRecursiveVisibilityByName(fTop,"SaddleBox_117",0);
  SetRecursiveVisibilityByName(fTop,"SaddleBox_118",0);
  SetRecursiveVisibilityByName(fTop,"SaddleBox_119",0);
//-----------------------------------------------------------------------------
// inside DS3Vacuum: hide calorimeter electronics, MBS
//-----------------------------------------------------------------------------
  SetRecursiveVisibilityByName(fDs3Vacuum,"VPSP_"     ,0);
  SetRecursiveVisibilityByName(fDs3Vacuum,"IFB_"      ,0);
  SetRecursiveVisibilityByName(fDs3Vacuum,"protonabs4",0);

  SetRecursiveVisibilityByName(fCalMother,"DiskFEB"    ,0);

  SetRecursiveVisibility(fMbsMother,0);
//-----------------------------------------------------------------------------
// colors
//-----------------------------------------------------------------------------
  if (KeepOriginalColors == 0) SetDefaultColorTransp(70);
}


//-----------------------------------------------------------------------------
void TStnGeoManager::DrawCalorimeter() {
  HideBuilding(0);
  gGeoManager->GetVolume("CalorimeterMother")->Draw("ogl");
}

//-----------------------------------------------------------------------------
void TStnGeoManager::DrawExtShielding() {

  HideBuilding(0);
  
  static TString name[] = {
    "ExtShield",
    ""
  };

  for (int i=0; name[i] != ""; i++) {
    SetRecursiveVisibilityColorTranspByNameAndMaterial(fTop,name[i].Data(),"BARITE"       ,1,kBlue+2   ,0);
    SetRecursiveVisibilityColorTranspByNameAndMaterial(fTop,name[i].Data(),"CONCRETE_CB4" ,1,kGray+2   ,0);
    SetRecursiveVisibilityColorTranspByNameAndMaterial(fTop,name[i].Data(),"CONCRETE_MARS",1,kMagenta+2,0);
  }

  // SetRecursiveColorTranspByMaterial(fTop,"BARITE"       ,kBlue+2   ,0);
  // SetRecursiveColorTranspByMaterial(fTop,"CONCRETE_CB4" ,kGray+2   ,0);
  // SetRecursiveColorTranspByMaterial(fTop,"CONCRETE_MARS",kMagenta-2,0);

  static TString crv_name[] = {
    "CRSAluminium","CRV","CRS","crv",
    ""
  };

  for (int i=0; crv_name[i] != ""; i++) {
    SetRecursiveVisibilityByName(fTop,crv_name[i].Data(),1);
  }
  SetRecursiveColorTranspByMaterial(fTop,"G4_POLYSTYRENE",kYellow-9  ,30);

  gGeoManager->GetVolume("HallAir")->Draw("ogl");
}

//-----------------------------------------------------------------------------
void TStnGeoManager::DrawCRV() {

  HideBuilding(0);
  
  static TString name[] = {
    //    "ExtShield",
    "CRSAluminium",        // CRV
    "CRV","CRS","crv",     // CRV
    ""
  };

  for (int i=0; name[i] != ""; i++) {
    SetRecursiveVisibilityByName(fTop,name[i].Data(),1);
  }

  SetRecursiveColorTranspByMaterial(fTop,"G4_POLYSTYRENE",kYellow-9  ,0);
  SetRecursiveColorTranspByMaterial(fTop,"G4_Al"         ,kGray      ,0);
  SetRecursiveColorTranspByMaterial(fTop,"ElectronicsFEB",kGray+2    ,0);

  SetRecursiveColorTranspByMaterial(fTop,"BARITE"       ,kBlue+2   ,0);
  SetRecursiveColorTranspByMaterial(fTop,"CONCRETE_CB4" ,kGray+2   ,0);
  SetRecursiveColorTranspByMaterial(fTop,"CONCRETE_MARS",kMagenta-2,0);

  gGeoManager->GetVolume("HallAir")->Draw("ogl");
}

//-----------------------------------------------------------------------------
void TStnGeoManager::DrawStrawTracker() {

  HideBuilding(0);
  
  static TString name[] = {
    "TTrackerSupport",
    "TTrackerEndRingUpstream",
    ""
  };
  
  SetRecursiveColorTranspByName(fTrkMother,"TTracker",kYellow   ,90);
  SetRecursiveColorTranspByName(fTrkMother,"Plane"   ,kYellow   ,99);
  SetRecursiveColorTranspByName(fTrkMother,"Panel"   ,kYellow   ,99);
  
  SetRecursiveColorTranspByName(fTrkMother,"TTrackerEndRingUpstream"    ,kGray,0);
  SetRecursiveColorTranspByName(fTrkMother,"TTrackerSupport"    ,kGray,     0);
  SetRecursiveColorTranspByName(fTrkMother,"TTrackerSupportBeam",kGray+2,0);
  SetRecursiveColorTranspByName(fTrkMother,"TTrackerStrawGas"   ,kYellow  ,0);

  gGeoManager->GetVolume("TrackerMother")->Draw("ogl");
}

//-----------------------------------------------------------------------------
void TStnGeoManager::DrawCalorimeterDisk() {
  gGeoManager->GetVolume("DiskCalorimeter_0")->Draw("ogl");
}

//-----------------------------------------------------------------------------
// the names are this is for v4_0_6
// clipping: xc = -387.93
//           zlen = 1429
//-----------------------------------------------------------------------------
void TStnGeoManager::DrawDetectorSolenoid() {

  TGeoVolume* hall = fTop->GetVolume();

  //  TObjArray* list_of_nodes = hall->GetNodes();

  //  int n_nodes = list_of_nodes->GetEntries();

  //  TGeoVolume* node;

  // for (int i=0; i<n_nodes; i++) {
  //   node  = (TGeoVolume*) list_of_nodes->At(i);

  //   const char* name = node->GetName();

  //   //    printf(" -- node name: %s\n",name);

  //   if ((strstr(name,"DS2Vacuum") != 0) ||
  // 	(strstr(name,"DS3Vacuum") != 0)    ) {
  //     node->SetVisibility(1);
  //     node->SetVisDaughters(1);
  //     node->SetVisLeaves(1);
  //   }
  //   else {
  //     node->SetVisibility(0);
  //     node->SetVisDaughters(0);
  //     node->SetVisLeaves(0);
  //   }
  // }

  gGeoManager->GetVolume("DS1Vacuum")->SetVisibility(0);
  gGeoManager->GetVolume("DS2Vacuum")->SetVisibility(0);
  gGeoManager->GetVolume("DS3Vacuum")->SetVisibility(0);

  gGeoManager->GetVolume("protonabs1")->SetLineColor(807);


  SetRecursiveVisibilityByName(fTop,"MBSMother",0);
  
  // TGeoVolume* mbs = fGm->GetVolume("MBSMother");
  // mbs->SetVisibility(0);
  // mbs->SetVisDaughters(0);
  // mbs->SetVisLeaves(0);

  gGeoManager->GetVolume("protonabs1")->SetLineColor(804);
  gGeoManager->GetVolume("protonabs3")->SetLineColor(808);

  // fGm->GetVolume("InternalNeutronAbsorber1" )->SetLineColor(900); // default = 920
  // fGm->GetVolume("InternalNeutronAbsorber2" )->SetLineColor(850); // default = 920
  // fGm->GetVolume("InternalNeutronAbsorber3a")->SetLineColor(860); // default = 920

  //  fGm->GetVolume("Foil")->SetLineColor(20);

  hall->Draw("ogl");
}


//-----------------------------------------------------------------------------
// the names are this is for v4_0_6
// clipping: xc = -387.93
//           zlen = 1429
//-----------------------------------------------------------------------------
void TStnGeoManager::DrawDetectorSolenoidDev2() {

  TGeoVolume* hall = fTop->GetVolume();

  TObjArray* list_of_nodes = hall->GetNodes();

  int n_nodes = list_of_nodes->GetEntries();

  TGeoNode    *node, *node2;
  const char  *name, *name2;

  for (int i=0; i<n_nodes; i++) {
    node  = hall->GetNode(i);

    name = node->GetName();

    //    printf(" -- node name: %s\n",name);

    if ((strstr(name,"DS2Vacuum") != 0) ||
	(strstr(name,"DS3Vacuum") != 0)    ) {

      node->SetVisibility(0);
      node->SetVisDaughters(1);
      node->SetVisLeaves(1);


      TObjArray* list_of_nodes_2 = node->GetNodes();
      int n_nodes_2 = list_of_nodes_2->GetEntries();

      for (int i2=0; i2<n_nodes_2; i2++) {
	node2  = (TGeoNode*) node->GetVolume()->GetNode(i2);
	name2 = node2->GetName();
	//	printf("            -- node2 name: %s\n",name2);
	if (strstr(name2,"VirtualDetector") != 0) {
	  node2->SetVisibility(0);
	  node2->SetVisDaughters(0);
	  node2->SetVisLeaves(0);
	}
	else if (strstr(name2,"MBSMother") != 0) {
	  node2->SetVisibility(0);
	  node2->SetVisDaughters(0);
	  node2->SetVisLeaves(0);
	}
	else if (strstr(name2,"VPSP") != 0) {
	  node2->SetVisibility(0);
	  node2->SetVisDaughters(0);
	  node2->SetVisLeaves(0);
	}
	else if (strstr(name2,"IFB") != 0) {
	  node2->SetVisibility(0);
	  node2->SetVisDaughters(0);
	  node2->SetVisLeaves(0);
	}
      }
    }
    else {
      node->SetVisibility(0);
      node->SetVisDaughters(0);
      node->SetVisLeaves(0);
    }
  }

  gGeoManager->GetVolume("protonabs1")->SetLineColor(804);
  gGeoManager->GetVolume("protonabs3")->SetLineColor(808);

  gGeoManager->GetVolume("InternalNeutronAbsorber1")->SetLineColor(900); // default = 920
  gGeoManager->GetVolume("InternalNeutronAbsorber2")->SetLineColor(850); // default = 920


  //  fGm->GetVolume("InternalNeutronAbsorber3a")->SetLineColor(860); // default = 920

  //  fGm->GetVolume("Foil")->SetLineColor(20);

  hall->Draw("ogl");
}


//-----------------------------------------------------------------------------

