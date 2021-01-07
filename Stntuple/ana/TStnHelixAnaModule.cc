//////////////////////////////////////////////////////////////////////////////
// use of tmp:
//
// use of debug bits: bits 0-2 are reserved
//  0  : all events
//  1  : passed events
//  2  : rejected events
//  3  : N(CalHelixFinder helices hel>0) > 0
//  4  : N(CalHelixFinder helices hel<0) > 0
// 
///////////////////////////////////////////////////////////////////////////////
#include "TF1.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TEnv.h"
#include "TSystem.h"

#include "Stntuple/loop/TStnAna.hh"
#include "Stntuple/obj/TStnHeaderBlock.hh"
#include "Stntuple/alg/TStntuple.hh"
#include "Stntuple/geom/TDisk.hh"
#include "Stntuple/val/stntuple_val_functions.hh"
//------------------------------------------------------------------------------
// Mu2e offline includes
//-----------------------------------------------------------------------------
#include "ana/TStnHelixAnaModule.hh"

ClassImp(TStnHelixAnaModule)

//-----------------------------------------------------------------------------
TStnHelixAnaModule::TStnHelixAnaModule(const char* name, const char* title):
  TStnModule(name,title)
{
  fPtMin  = 1.;

  fMinT0 = 0; // do not cut on time by default
//-----------------------------------------------------------------------------
// MC truth: define which MC particle to consider as signal
//-----------------------------------------------------------------------------
  fPdgCode     = 11;
  fProcessCode = 2;			// conversionGun, 28:StoppedParticleReactionGun
//-----------------------------------------------------------------------------
// default names of the helix block branches correspond to settings of StntupleMakerDe 
// configuration in Stntuple/fcl/prolog.fcl
//-----------------------------------------------------------------------------

  fHelixBlockName[0] = "HelixBlockTpr";
  fHelixBlockName[1] = "HelixBlockCpr";
  fHelixBlockName[2] = "HelixBlock";
}

//-----------------------------------------------------------------------------
TStnHelixAnaModule::~TStnHelixAnaModule() {
}


//-----------------------------------------------------------------------------
void TStnHelixAnaModule::BookTimeClusterHistograms   (TimeClusterHist_t*   Hist, const char* Folder){
 
  HBook1F(Hist->fNHits         ,"nhits"      ,Form("%s: # of straw hits"              ,Folder), 150,   0,   150,Folder);
  HBook1F(Hist->fNComboHits    ,"ncombohits" ,Form("%s: # of combo hits"              ,Folder), 150,   0,   150,Folder);
  HBook1F(Hist->fT0            ,"t0"         ,Form("%s: TimeCluster; t_{0}[ns]"       ,Folder), 800, 400,  1700,Folder);
  HBook1F(Hist->fClusterEnergy ,"clusterE"   ,Form("%s: cluster energy; E [MeV]      ",Folder), 400,   0,  200,Folder);  
}

//-----------------------------------------------------------------------------
void TStnHelixAnaModule::BookHelixHistograms   (HelixHist_t*   Hist, const char* Folder){
  
  HBook1F(Hist->fNHits         ,"nhits"      ,Form("%s: # of straw hits"              ,Folder),  150,     0,  150,Folder);
  HBook1F(Hist->fHelicity      ,"hel"        ,Form("%s: Helicity"                     ,Folder),   10,    -5,    5,Folder);
  HBook1F(Hist->fClusterTime   ,"clusterTime",Form("%s: cluster time; t_{cluster}[ns]",Folder),  400,     0, 2000,Folder);
  HBook1F(Hist->fClusterEnergy ,"clusterE"   ,Form("%s: cluster energy; E [MeV]      ",Folder),  400,     0,  200,Folder);
  HBook1F(Hist->fRadius        ,"radius"     ,Form("%s: helix radius; r [mm]"         ,Folder),  500,     0,  500,Folder);
  HBook1F(Hist->fMom           ,"p"          ,Form("%s: momentum; p [MeV/c]"          ,Folder),  300,    50,  200,Folder);
  HBook1F(Hist->fPt            ,"pT"         ,Form("%s: pT; pT [MeV/c]"               ,Folder),  600,     0,  150,Folder);
  HBook1F(Hist->fLambda        ,"lambda"     ,Form("%s: lambda; #lambda"              ,Folder),  200, -1000, 1000,Folder);
  HBook1F(Hist->fT0            ,"t0"         ,Form("%s: t0; t0[ns]"                   ,Folder),  400,     0, 2000,Folder);
  HBook1F(Hist->fT0Err         ,"t0err"      ,Form("%s: t0err; t0err [ns]"            ,Folder),  100,     0,   10,Folder);
  HBook1F(Hist->fD0            ,"d0"         ,Form("%s: D0; d0 [mm]"                  ,Folder), 1600,  -400,  400,Folder);
  HBook1F(Hist->fAlgMask       ,"algmask"    ,Form("%s: Algorithm Mask"               ,Folder),   10,     0,   10,Folder);
  HBook1F(Hist->fBestAlg       ,"bestalg"    ,Form("%s: Best Algorithm"               ,Folder),   10,     0,   10,Folder);
  HBook1F(Hist->fChi2XY        ,"chi2xy"     ,Form("%s: Chi2(XY)/DOF"                 ,Folder),  100,     0,   10,Folder);
  HBook1F(Hist->fChi2ZPhi      ,"chi2zphi"   ,Form("%s: Chi2(ZPhi)/DOF"               ,Folder),  100,     0,   10,Folder);
}

//-----------------------------------------------------------------------------
void TStnHelixAnaModule::BookGenpHistograms(GenpHist_t* Hist, const char* Folder) {
//   char name [200];
//   char title[200];

  HBook1F(Hist->fP      ,"p"       ,Form("%s: Momentum"     ,Folder),1000,     0, 200,Folder);
  HBook1F(Hist->fPdgCode[0],"pdg_code_0",Form("%s: PDG Code[0]"     ,Folder),200, -100, 100,Folder);
  HBook1F(Hist->fPdgCode[1],"pdg_code_1",Form("%s: PDG Code[1]"     ,Folder),500, -2500, 2500,Folder);
  HBook1F(Hist->fGenID  ,"gen_id"  ,Form("%s: Generator ID" ,Folder), 100,     0, 100,Folder);
  HBook1F(Hist->fZ0     ,"z0"      ,Form("%s: Z0"           ,Folder), 500,  5400, 6400,Folder);
  HBook1F(Hist->fT0     ,"t0"      ,Form("%s: T0"           ,Folder), 200,     0, 2000,Folder);
  HBook1F(Hist->fR0     ,"r"       ,Form("%s: R0"           ,Folder), 100,     0,  100,Folder);
  HBook1F(Hist->fCosTh  ,"cos_th"  ,Form("%s: Cos(Theta)"   ,Folder), 200,   -1.,   1.,Folder);
}


//-----------------------------------------------------------------------------
void TStnHelixAnaModule::BookEventHistograms(EventHist_t* Hist, const char* Folder) {
  //  char name [200];
  //  char title[200];

  HBook1F(Hist->fEleCosTh  ,"ce_costh" ,Form("%s: Conversion Electron Cos(Theta)"  ,Folder),100,-1,1,Folder);
  HBook1F(Hist->fEleMom    ,"ce_mom"   ,Form("%s: Conversion Electron Momentum"    ,Folder),1000,  0,200,Folder);
  HBook1F(Hist->fRv         ,"rv"      ,Form("%s: R(Vertex)"                       ,Folder), 100, 0, 1000,Folder);
  HBook1F(Hist->fZv         ,"zv"      ,Form("%s: Z(Vertex)"                       ,Folder), 300, 0,15000,Folder);
  HBook1F(Hist->fNGenp      ,"ngenp"   ,Form("%s: N(Gen Particles)"                ,Folder),500,0,500,Folder);

  HBook1F(Hist->fNHelices[0],"nhel_0"  ,Form("%s: N(helices)[0]"                   ,Folder), 10,0, 10,Folder);
  HBook1F(Hist->fNHelices[1],"nhel_1"  ,Form("%s: N(helices)[2]"                   ,Folder), 10,0, 10,Folder);
  HBook1F(Hist->fNHelices[2],"nhel_2"  ,Form("%s: N(helices)[3]"                   ,Folder), 10,0, 10,Folder);

  HBook1F(Hist->fNHelPos [0],"nhp_0"   ,Form("%s: N(helices)[0] hel > 0"           ,Folder), 10,0, 10,Folder);
  HBook1F(Hist->fNHelPos [1],"nhp_1"   ,Form("%s: N(helices)[2] hel > 0"           ,Folder), 10,0, 10,Folder);
  HBook1F(Hist->fNHelPos [2],"nhp_2"   ,Form("%s: N(helices)[3] hel > 0"           ,Folder), 10,0, 10,Folder);

  HBook1F(Hist->fNHelNeg [0],"nhn_0"   ,Form("%s: N(helices)[0] hel < 0"           ,Folder), 10,0, 10,Folder);
  HBook1F(Hist->fNHelNeg [1],"nhn_1"   ,Form("%s: N(helices)[2] hel < 0"           ,Folder), 10,0, 10,Folder);
  HBook1F(Hist->fNHelNeg [2],"nhn_2"   ,Form("%s: N(helices)[3] hel < 0"           ,Folder), 10,0, 10,Folder);

  HBook1F(Hist->fNTimeClusters[0],"ntcl_0"  ,Form("%s: N(time clusters)[0]"        ,Folder),100,0,100,Folder);
  HBook1F(Hist->fNTimeClusters[1],"ntcl_1"  ,Form("%s: N(time clusters)[1]"        ,Folder),100,0,100,Folder);
}

//-----------------------------------------------------------------------------
void TStnHelixAnaModule::BookSimpHistograms(SimpHist_t* Hist, const char* Folder) {
  //  char name [200];
  //  char title[200];

  HBook1F(Hist->fPdgCode   ,"pdg"         ,Form("%s: PDG code"                     ,Folder),200,-100,100,Folder);
  HBook1F(Hist->fNStrawHits,"nsth"        ,Form("%s: n straw hits"                 ,Folder),200,   0,200,Folder);
  HBook1F(Hist->fMomTargetEnd    ,"ptarg" ,Form("%s: CE mom after Stopping Target" ,Folder),400,  90,110,Folder);
  HBook1F(Hist->fMomTrackerFront ,"pfront",Form("%s: CE mom at the Tracker Front"  ,Folder),400,  90,110,Folder);
}


//_____________________________________________________________________________
void TStnHelixAnaModule::BookHistograms() {

  //  char name [200];
  //  char title[200];
  
  TFolder* fol;
  TFolder* hist_folder;
  char     folder_name[200];
  
  DeleteHistograms();
  hist_folder = (TFolder*) GetFolder()->FindObject("Hist");
//--------------------------------------------------------------------------------
// book timecluster histograms
//--------------------------------------------------------------------------------
  int book_timecluster_histset[kNTimeClusterHistSets];
  for (int i=0; i<kNTimeClusterHistSets; ++i)  book_timecluster_histset[i] = 0;

  book_timecluster_histset[0] = 1;   // all time clusters
  book_timecluster_histset[1] = 1;   // time clusters with NHits>10
  book_timecluster_histset[2] = 1;   // timeclusters with NHits>15

  for (int i=0; i<kNTimeClusterHistSets; i++) {
    if (book_timecluster_histset[i] != 0) {
      sprintf(folder_name,"timecluster_%i",i);
      fol = (TFolder*) hist_folder->FindObject(folder_name);
      if (! fol) fol = hist_folder->AddFolder(folder_name,folder_name);
      fHist.fTimeCluster[i] = new TimeClusterHist_t;
      BookTimeClusterHistograms(fHist.fTimeCluster[i],Form("Hist/%s",folder_name));
    }
  }
//--------------------------------------------------------------------------------
// book helix histograms
//--------------------------------------------------------------------------------
  int book_helix_histset[kNHelixHistSets];
  for (int i=0; i<kNHelixHistSets; ++i)  book_helix_histset[i] = 0;

  book_helix_histset[  0] = 1;   // all TPR    helices
  book_helix_histset[  1] = 1;   //     TPR    helices  with P > 80

  book_helix_histset[100] = 1;   // all CPR    helices
  book_helix_histset[101] = 1;   //     CPR    helices  with P > 80

  book_helix_histset[200] = 1;   // all merged helices
  book_helix_histset[201] = 1;   //     merged helices  with P > 80

  for (int i=0; i<kNHelixHistSets; i++) {
    if (book_helix_histset[i] != 0) {
      sprintf(folder_name,"helix_%i",i);
      fol = (TFolder*) hist_folder->FindObject(folder_name);
      if (! fol) fol = hist_folder->AddFolder(folder_name,folder_name);
      fHist.fHelix[i] = new HelixHist_t;
      BookHelixHistograms(fHist.fHelix[i],Form("Hist/%s",folder_name));
    }
  }
//-----------------------------------------------------------------------------
// book event histograms
//-----------------------------------------------------------------------------
  int book_event_histset[kNEventHistSets];
  for (int i=0; i<kNEventHistSets; i++) book_event_histset[i] = 0;

  book_event_histset[ 0] = 1;		// all events
  book_event_histset[ 1] = 1;	        // events with N>0 reconstructed helices
  book_event_histset[ 2] = 1;	        // events with N=0 reconstructed helices

  for (int i=0; i<kNEventHistSets; i++) {
    if (book_event_histset[i] != 0) {
      sprintf(folder_name,"evt_%i",i);
      fol = (TFolder*) hist_folder->FindObject(folder_name);
      if (! fol) fol = hist_folder->AddFolder(folder_name,folder_name);
      fHist.fEvent[i] = new EventHist_t;
      BookEventHistograms(fHist.fEvent[i],Form("Hist/%s",folder_name));
    }
  }
//-----------------------------------------------------------------------------
// book simp histograms
//-----------------------------------------------------------------------------
  int book_simp_histset[kNSimpHistSets];
  for (int i=0; i<kNSimpHistSets; i++) book_simp_histset[i] = 0;

  book_simp_histset[ 0] = 1;		// all events

  for (int i=0; i<kNSimpHistSets; i++) {
    if (book_simp_histset[i] != 0) {
      sprintf(folder_name,"sim_%i",i);
      fol = (TFolder*) hist_folder->FindObject(folder_name);
      if (! fol) fol = hist_folder->AddFolder(folder_name,folder_name);
      fHist.fSimp[i] = new SimpHist_t;
      BookSimpHistograms(fHist.fSimp[i],Form("Hist/%s",folder_name));
    }
  }
//-----------------------------------------------------------------------------
// book Genp histograms
//-----------------------------------------------------------------------------
  int book_genp_histset[kNGenpHistSets];
  for (int i=0; i<kNGenpHistSets; i++) book_genp_histset[i] = 0;

  book_genp_histset[0] = 1;		// all particles

  for (int i=0; i<kNGenpHistSets; i++) {
    if (book_genp_histset[i] != 0) {
      sprintf(folder_name,"gen_%i",i);
      fol = (TFolder*) hist_folder->FindObject(folder_name);
      if (! fol) fol = hist_folder->AddFolder(folder_name,folder_name);
      fHist.fGenp[i] = new GenpHist_t;
      BookGenpHistograms(fHist.fGenp[i],Form("Hist/%s",folder_name));
    }
  }
}


//-----------------------------------------------------------------------------
// need MC truth branch
//-----------------------------------------------------------------------------
void TStnHelixAnaModule::FillEventHistograms(EventHist_t* Hist) {
  double            cos_th(-2.), xv(-1.e6), yv(-1.e6), rv(-1.e6), zv(-1.e6), p(-1.);
  TLorentzVector    mom;

  if (fParticle) {
    fParticle->Momentum(mom);
    p      = mom.P();
    cos_th = mom.Pz()/p;
    xv = fParticle->Vx()+3904.;
    yv = fParticle->Vy();
    rv = sqrt(xv*xv+yv*yv);
    zv = fParticle->Vz();
  }

  Hist->fEleMom->Fill(p);
  Hist->fEleCosTh->Fill(cos_th);
  Hist->fRv->Fill(rv);
  Hist->fZv->Fill(zv);

  for (int i=0; i<3; i++) {
    Hist->fNHelices[i]->Fill(fNHelices[i]);
    Hist->fNHelPos [i]->Fill(fNHelPos [i]);
    Hist->fNHelNeg [i]->Fill(fNHelNeg [i]);
  }

  Hist->fNTimeClusters[0]->Fill(fNTimeClusters[0]);
  Hist->fNTimeClusters[1]->Fill(fNTimeClusters[1]);
}

//--------------------------------------------------------------------------------
// function to fill Helix block
//--------------------------------------------------------------------------------
void TStnHelixAnaModule::FillTimeClusterHistograms(TimeClusterHist_t*   Hist, TStnTimeCluster*    TimeCluster){
  
  int         nhits      = TimeCluster->NHits      ();
  int         ncombohits = TimeCluster->NComboHits ();

  double      time       = TimeCluster->T0();
  double      clusterE   = TimeCluster->ClusterEnergy();

  Hist->fNHits         ->Fill(nhits);	 
  Hist->fNComboHits    ->Fill(ncombohits);	 
  Hist->fT0            ->Fill(time);
  Hist->fClusterEnergy ->Fill(clusterE);

}

//--------------------------------------------------------------------------------
// function to fill Helix block
//--------------------------------------------------------------------------------
void TStnHelixAnaModule::FillHelixHistograms(HelixHist_t*   Hist, TStnHelix*    Helix){
  
  int         nhits    = Helix->NHits      ();
  double      clusterT = Helix->ClusterTime();
  double      clusterE = Helix->ClusterEnergy();
  
  double      radius   = Helix->Radius();

  double      lambda   = Helix->Lambda();  
  double      tanDip   = lambda/radius;
					// assuming B=1T
  double      mm2MeV   = 3/10.;
  double      pT       = radius*mm2MeV;
  double      p        = pT/std::cos( std::atan(tanDip));
  

  Hist->fHelicity      ->Fill(Helix->Helicity());	 
  Hist->fNHits         ->Fill(nhits);	 
  Hist->fClusterTime   ->Fill(clusterT);
  Hist->fClusterEnergy ->Fill(clusterE);
  
  Hist->fRadius        ->Fill(radius);
  Hist->fMom           ->Fill(p);
  Hist->fPt            ->Fill(pT);
  Hist->fLambda        ->Fill(lambda);
  		       
  Hist->fBestAlg       ->Fill(Helix->BestAlg());
  Hist->fAlgMask       ->Fill(Helix->AlgMask());
  Hist->fD0            ->Fill(Helix->D0());
  Hist->fT0            ->Fill(Helix->T0());
  Hist->fT0Err         ->Fill(Helix->T0Err());
  Hist->fChi2XY        ->Fill(Helix->Chi2XY());
  Hist->fChi2ZPhi      ->Fill(Helix->Chi2ZPhi());
}

//-----------------------------------------------------------------------------
void TStnHelixAnaModule::FillGenpHistograms(GenpHist_t* Hist, TGenParticle* Genp) {
  int    gen_id;
  float  p, cos_th, z0, t0, r0, x0, y0;

  TLorentzVector mom, v;

  Genp->Momentum(mom);
  //  Genp->ProductionVertex(v);

  p      = mom.P();
  cos_th = mom.CosTheta();

  x0     = Genp->Vx()+3904.;
  y0     = Genp->Vy();

  z0     = Genp->Vz();
  t0     = Genp->T();
  r0     = sqrt(x0*x0+y0*y0);
  gen_id = Genp->GetStatusCode();

  Hist->fPdgCode[0]->Fill(Genp->GetPdgCode());
  Hist->fPdgCode[1]->Fill(Genp->GetPdgCode());
  Hist->fGenID->Fill(gen_id);
  Hist->fZ0->Fill(z0);
  Hist->fT0->Fill(t0);
  Hist->fR0->Fill(r0);
  Hist->fP->Fill(p);
  Hist->fCosTh->Fill(cos_th);
}

//-----------------------------------------------------------------------------
void TStnHelixAnaModule::FillSimpHistograms(SimpHist_t* Hist, TSimParticle* Simp) {

  Hist->fPdgCode->Fill(Simp->fPdgCode);
  Hist->fMomTargetEnd->Fill(Simp->fMomTargetEnd);
  Hist->fMomTrackerFront->Fill(Simp->fMomTrackerFront);
  Hist->fNStrawHits->Fill(Simp->fNStrawHits);
}

//-----------------------------------------------------------------------------
// register data blocks and book histograms
//-----------------------------------------------------------------------------
int TStnHelixAnaModule::BeginJob() {
//-----------------------------------------------------------------------------
// register data blocks, 'HelixBlock' - OR of TPR and CPR
//-----------------------------------------------------------------------------
  RegisterDataBlock(fHelixBlockName[0].Data(), "TStnHelixBlock" ,&fHelixBlock[0]);
  RegisterDataBlock(fHelixBlockName[1].Data(), "TStnHelixBlock" ,&fHelixBlock[1]);
  RegisterDataBlock(fHelixBlockName[2].Data(), "TStnHelixBlock" ,&fHelixBlock[2]);

  RegisterDataBlock("TimeClusterBlockTpr","TStnTimeClusterBlock",&fTimeClusterBlock[0]);
  RegisterDataBlock("TimeClusterBlockCpr","TStnTimeClusterBlock",&fTimeClusterBlock[1]);

  RegisterDataBlock("GenpBlock"       ,"TGenpBlock"          ,&fGenpBlock       );
  RegisterDataBlock("SimpBlock"       ,"TSimpBlock"          ,&fSimpBlock       );
//-----------------------------------------------------------------------------
// book histograms
//-----------------------------------------------------------------------------
  BookHistograms();

  return 0;
}


//_____________________________________________________________________________
int TStnHelixAnaModule::BeginRun() {
  int rn = GetHeaderBlock()->RunNumber();
  TStntuple::Init(rn);
  return 0;
}


//_____________________________________________________________________________
void TStnHelixAnaModule::FillHistograms() {

  //  double       cos_th (-2.);
  //  float        pfront;

//-----------------------------------------------------------------------------
// event histograms
//-----------------------------------------------------------------------------
  FillEventHistograms(fHist.fEvent[0]);

  if (fNHelices[2] > 0) FillEventHistograms(fHist.fEvent[1]);
  else                  FillEventHistograms(fHist.fEvent[2]);
//-----------------------------------------------------------------------------
// fill GENP histograms
// GEN_0: all particles
//-----------------------------------------------------------------------------
  TGenParticle* genp;
  for (int i=0; i<fNGenp; i++) {
    genp = fGenpBlock->Particle(i);
    FillGenpHistograms(fHist.fGenp[0],genp);
  }
//-----------------------------------------------------------------------------
// Simp histograms
//-----------------------------------------------------------------------------
  if (fSimp) {
    FillSimpHistograms(fHist.fSimp[0],fSimp);
  }

//--------------------------------------------------------------------------------
// TimeCluster histograms
//--------------------------------------------------------------------------------
  TStnTimeCluster* tCluster;
  for (int i=0; i<fNTimeClusters[0]; ++i){
    tCluster = fTimeClusterBlock[0]->TimeCluster(i);
    
    FillTimeClusterHistograms(fHist.fTimeCluster[0], tCluster);
    
    int nhits = tCluster->NHits();
    if (nhits >= 10 ) FillTimeClusterHistograms(fHist.fTimeCluster[1], tCluster);
    if (nhits >= 15 ) FillTimeClusterHistograms(fHist.fTimeCluster[2], tCluster);
  }  
//--------------------------------------------------------------------------------
// fill helix histograms
//--------------------------------------------------------------------------------
  TStnHelix* helix;
  double     mm2MeV{3/10.};

  for (int ib=0; ib<3; ib++) {
    int hist_offset = 100*ib;

    for (int i=0; i<fNHelices[ib]; ++i){
    
      helix = fHelixBlock[ib]->Helix(i);
    
      FillHelixHistograms(fHist.fHelix[hist_offset+0], helix);
    
      // int         nhits    = helix->NHits();
      double      radius   = helix->Radius();
      double      lambda   = helix->Lambda();
      double      tanDip   = lambda/radius;
      double      p        = radius*mm2MeV/cos(atan(tanDip));
    
      // double      chi2xy   = helix->Chi2XY();
      // double      chi2zphi = helix->Chi2ZPhi();
   
      if (p >  80.) FillHelixHistograms(fHist.fHelix[hist_offset+1], helix);
    }
  }

  //  first_entry = 0;
}



//-----------------------------------------------------------------------------
// 2014-04-30: it looks that reading the straw hits takes a lot of time - 
//              turn off by default by commenting it out
//-----------------------------------------------------------------------------
int TStnHelixAnaModule::Event(int ientry) {

  //  TEmuLogLH::PidData_t  dat;
  //  TStnTrack*            track;
  //  int                   id_word;
  TLorentzVector        mom;

  //  TDiskCalorimeter::GeomData_t disk_geom;

  fHelixBlock[0]  ->GetEntry(ientry);
  fHelixBlock[1]  ->GetEntry(ientry);
  fHelixBlock[2]  ->GetEntry(ientry);

  fGenpBlock->GetEntry(ientry);
  fSimpBlock->GetEntry(ientry);

  fTimeClusterBlock[0]->GetEntry(ientry);
  fTimeClusterBlock[1]->GetEntry(ientry);
//-----------------------------------------------------------------------------
// assume electron in the first particle, otherwise the logic will need to 
// be changed
//-----------------------------------------------------------------------------
  fNGenp    = fGenpBlock->NParticles();

  TGenParticle* genp;
  int           pdg_code, process_code;

  fParticle = NULL;
  for (int i=fNGenp-1; i>=0; i--) {
    genp         = fGenpBlock->Particle(i);
    pdg_code     = genp->GetPdgCode();
    process_code = genp->GetStatusCode();
    if ((abs(pdg_code) == fPdgCode) && (process_code == fProcessCode)) {
      fParticle = genp;
      break;
    }
  }
					// may want to revisit the definition of fSimp
  fSimp     = fSimpBlock->Particle(0);

  double p = 0;

  if (fParticle) {
    fParticle->Momentum(mom);
					// this is a kludge, to be removed at the next 
					// ntupling 
    p         = mom.P();
    fEleE     = sqrt(p*p+0.511*0.511);
  }

  fNTimeClusters[0] = fTimeClusterBlock[0]->NTimeClusters();
  fNTimeClusters[1] = fTimeClusterBlock[1]->NTimeClusters();
  
  fNHelices[0] = fHelixBlock[0]->NHelices();
  fNHelices[1] = fHelixBlock[1]->NHelices();
  fNHelices[2] = fHelixBlock[2]->NHelices();

  for (int ib=0; ib<3; ib++) {
    fNHelPos[ib] = 0;
    fNHelNeg[ib] = 0;
    for (int i=0; i<fNHelices[ib]; i++) {
      TStnHelix* h = fHelixBlock[ib]->Helix(i);
      if      (h->Helicity() > 0) fNHelPos[ib]++;
      else if (h->Helicity() < 0) fNHelNeg[ib]++;
      else {
	printf("ERROR 001 in TStnHelixAnaModule: helix with helicity=0\n");
      }
    }
  }

  FillHistograms();

  Debug();

  return 0;		       
}

//-----------------------------------------------------------------------------
void TStnHelixAnaModule::Debug() {

  if (GetDebugBit(3) == 1) {
    if (fNHelPos[1] > 0) {
      GetHeaderBlock()->Print(Form("N(CalHelixFinder helices hel > 0) = %2i",fNHelPos[1]));
    }
  }

  if (GetDebugBit(4) == 1) {
    if (fNHelNeg[1] > 0) {
      GetHeaderBlock()->Print(Form("N(CalHelixFinder helices hel < 0) = %2i",fNHelNeg[1]));
    }
  }
}

//_____________________________________________________________________________
int TStnHelixAnaModule::EndJob() {
  return 0;
}

//_____________________________________________________________________________
void TStnHelixAnaModule::Test001() {

  // mu2e::HexMap* hmap      = new mu2e::HexMap();

  // mu2e::HexLK hex_index(0,0);

  // for (int i=0; i<40; i++) {
  //   hex_index = hmap->lk(i);
  //   printf(" i,l,k = %5i %5i %5i\n",i,hex_index._l,hex_index._k);
  // }
}
