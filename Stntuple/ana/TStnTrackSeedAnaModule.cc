//////////////////////////////////////////////////////////////////////////////
// use of tmp:
//
// Tmp(0) : nax seg
// Tmp(1) : nst seg
// 
// use of debug bits: bits 0-2 are reserved
//  0  : all events
//  1  : passed events
//  2  : rejected events
// 
//  3  : events with N(track seeds) = 0
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
#include "ana/TStnTrackSeedAnaModule.hh"

ClassImp(TStnTrackSeedAnaModule)

//-----------------------------------------------------------------------------
TStnTrackSeedAnaModule::TStnTrackSeedAnaModule(const char* name, const char* title):
  TStnModule(name,title)
{
  fPtMin  = 1.;
  //  fTrackNumber.Set(100);
//-----------------------------------------------------------------------------
// MC truth: define which MC particle to consider as signal
//-----------------------------------------------------------------------------
  fPdgCode       = 11;
  fGeneratorCode = 2;			// conversionGun, 28:StoppedParticleReactionGun
}

//-----------------------------------------------------------------------------
TStnTrackSeedAnaModule::~TStnTrackSeedAnaModule() {
}


//-----------------------------------------------------------------------------
void TStnTrackSeedAnaModule::BookTrackSeedHistograms   (TrackSeedHist_t*   Hist, const char* Folder){
  
  HBook1F(Hist->fNHits         ,"nhits"    ,Form("%s: # of straw hits"              ,Folder), 150,   0,   150,Folder);
  HBook1F(Hist->fClusterTime ,"clusterTime",Form("%s: cluster time; t_{cluster}[ns]",Folder), 800, 400,  1700,Folder);
  HBook1F(Hist->fClusterEnergy ,"clusterE" ,Form("%s: cluster energy; E [MeV]      ",Folder), 400,   0,  200,Folder);
  HBook1F(Hist->fRadius        ,"radius"   ,Form("%s: curvature radius; r [mm]"     ,Folder), 500,   0,   500,Folder);
  HBook1F(Hist->fMom           ,"p"        ,Form("%s: momentum; p [MeV/c]"          ,Folder), 300,   50,   200,Folder);
  HBook1F(Hist->fPt            ,"pT"       ,Form("%s: pT; pT [MeV/c]"               ,Folder), 600,   0,   150,Folder);
  HBook1F(Hist->fTanDip        ,"tanDip"   ,Form("%s: tanDip; tanDip"               ,Folder), 300,   0,     3,Folder);
  HBook1F(Hist->fChi2          ,"chi2"   ,Form("%s: #chi^{2}-XY; #chi^{2}/ndof"     ,Folder), 100,   0,    10,Folder);
  HBook1F(Hist->fFitCons      ,"FitCons" ,Form("%s: Fit consistency; Fit-cons"      ,Folder), 100,   0,    1, Folder);
  HBook1F(Hist->fD0            ,"d0"       ,Form("%s: D0; d0 [mm]"                  ,Folder), 1600,   -400,    400,Folder);
}

//-----------------------------------------------------------------------------
void TStnTrackSeedAnaModule::BookGenpHistograms(GenpHist_t* Hist, const char* Folder) {
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
void TStnTrackSeedAnaModule::BookEventHistograms(EventHist_t* Hist, const char* Folder) {
  //  char name [200];
  //  char title[200];

  HBook1F(Hist->fEleCosTh   ,"ce_costh" ,Form("%s: Conversion Electron Cos(Theta)" ,Folder),100,-1,1,Folder);
  HBook1F(Hist->fEleMom     ,"ce_mom"   ,Form("%s: Conversion Electron Momentum"   ,Folder),1000,  0,200,Folder);
  HBook1F(Hist->fRv         ,"rv"      ,Form("%s: R(Vertex)"                       ,Folder), 100, 0, 1000,Folder);
  HBook1F(Hist->fZv         ,"zv"      ,Form("%s: Z(Vertex)"                       ,Folder), 300, 0,15000,Folder);
  HBook1F(Hist->fNTrackSeeds,"nts"     ,Form("%s: Number of Reco Track Seeds"      ,Folder),100,0,100,Folder);
  HBook1F(Hist->fNGenp      ,"ngenp"    ,Form("%s: N(Gen Particles)"               ,Folder),500,0,500,Folder);
}

//-----------------------------------------------------------------------------
void TStnTrackSeedAnaModule::BookSimpHistograms(SimpHist_t* Hist, const char* Folder) {
  //  char name [200];
  //  char title[200];

  HBook1F(Hist->fPdgCode   ,"pdg"         ,Form("%s: PDG code"                     ,Folder),200,-100,100,Folder);
  HBook1F(Hist->fNStrawHits,"nsth"        ,Form("%s: n straw hits"                 ,Folder),200,   0,200,Folder);
  HBook1F(Hist->fMomTargetEnd    ,"ptarg" ,Form("%s: CE mom after Stopping Target" ,Folder),400,  90,110,Folder);
  HBook1F(Hist->fMomTrackerFront ,"pfront",Form("%s: CE mom at the Tracker Front"  ,Folder),400,  90,110,Folder);
}


//_____________________________________________________________________________
void TStnTrackSeedAnaModule::BookHistograms() {

  //  char name [200];
  //  char title[200];
  
  TFolder* fol;
  TFolder* hist_folder;
  char     folder_name[200];
  
  DeleteHistograms();
  hist_folder = (TFolder*) GetFolder()->FindObject("Hist");
//--------------------------------------------------------------------------------
// book trackSeed histograms
//--------------------------------------------------------------------------------
  int book_trackSeed_histset[kNTrackSeedHistSets];
  for (int i=0; i<kNTrackSeedHistSets; ++i)  book_trackSeed_histset[i] = 0;

  book_trackSeed_histset[0] = 1;   // events with at least one trackSeed
  book_trackSeed_histset[1] = 1;   // events with at least one trackSeed with p > 80 MeV/c
  book_trackSeed_histset[2] = 1;   // events with at least one trackSeed with p > 90 MeV/c
  book_trackSeed_histset[3] = 1;   // events with at least one trackSeed with p > 100 MeV/c
  book_trackSeed_histset[4] = 1;   // events with at least one trackSeed with 10 < nhits < 15
  book_trackSeed_histset[5] = 1;   // events with at least one trackSeed with nhits >= 15
  book_trackSeed_histset[6] = 1;   // events with at least one trackSeed with nhits >= 15 and chi2(ZPhi)<4

  for (int i=0; i<kNTrackSeedHistSets; i++) {
    if (book_trackSeed_histset[i] != 0) {
      sprintf(folder_name,"trkseed_%i",i);
      fol = (TFolder*) hist_folder->FindObject(folder_name);
      if (! fol) fol = hist_folder->AddFolder(folder_name,folder_name);
      fHist.fTrackSeed[i] = new TrackSeedHist_t;
      BookTrackSeedHistograms(fHist.fTrackSeed[i],Form("Hist/%s",folder_name));
    }
  }
//-----------------------------------------------------------------------------
// book event histograms
//-----------------------------------------------------------------------------
  int book_event_histset[kNEventHistSets];
  for (int i=0; i<kNEventHistSets; i++) book_event_histset[i] = 0;

  book_event_histset[ 0] = 1;		// all events

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
//   book_genp_histset[1] = 1;		// all crystals, e > 0
//   book_genp_histset[2] = 1;		// all crystals, e > 0.1
//   book_genp_histset[3] = 1;		// all crystals, e > 1.0

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
void TStnTrackSeedAnaModule::FillEventHistograms(EventHist_t* Hist) {
  double            cos_th(-2), xv(-1.e6), yv(-1.e6), rv(-1.e6), zv(-1.e6), p(-1.);
  //  double            e, m, r;
  TLorentzVector    mom;

  if (fParticle) {
    fParticle->Momentum(mom);
    p      = mom.P();
    cos_th = mom.Pz()/p;
    xv     = fParticle->Vx()+3904.;
    yv     = fParticle->Vy();
    rv     = sqrt(xv*xv+yv*yv);
    zv     = fParticle->Vz();
  }

  Hist->fEleMom->Fill(p);
  Hist->fEleCosTh->Fill(cos_th);
  Hist->fRv->Fill(rv);
  Hist->fZv->Fill(zv);

  Hist->fNGenp->Fill(fNGenp);
  Hist->fNTrackSeeds->Fill(fNTrackSeeds);
}

//--------------------------------------------------------------------------------
// function to fill TrasckSeedHit block
//--------------------------------------------------------------------------------
void TStnTrackSeedAnaModule::FillTrackSeedHistograms(TrackSeedHist_t*   Hist, TStnTrackSeed* TrkSeed) {
  
  int         nhits    = TrkSeed->NHits      ();
  double      clusterT = TrkSeed->ClusterTime();
  double      clusterE = TrkSeed->ClusterEnergy();
  
  double      mm2MeV   = 3/10.;
  double      pT       = TrkSeed->Pt();
  double      radius   = pT/mm2MeV;

  double      tanDip   = TrkSeed->TanDip();  
  double      p        = pT/std::cos( std::atan(tanDip));
  

  Hist->fNHits      ->Fill(nhits);	 
  Hist->fClusterTime->Fill(clusterT);
  Hist->fClusterEnergy->Fill(clusterE);
  
  Hist->fRadius     ->Fill(radius);    
  Hist->fMom        ->Fill(p);	 
  Hist->fPt         ->Fill(pT);	 
  Hist->fTanDip     ->Fill(tanDip);    
  
  Hist->fChi2       ->Fill(TrkSeed->Chi2());
  Hist->fFitCons    ->Fill(TrkSeed->FitCons());
  Hist->fD0         ->Fill(TrkSeed->D0());
}

//-----------------------------------------------------------------------------
void TStnTrackSeedAnaModule::FillGenpHistograms(GenpHist_t* Hist, TGenParticle* Genp) {
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
void TStnTrackSeedAnaModule::FillSimpHistograms(SimpHist_t* Hist, TSimParticle* Simp) {

  Hist->fPdgCode->Fill(Simp->fPdgCode);
  Hist->fMomTargetEnd->Fill(Simp->fMomTargetEnd);
  Hist->fMomTrackerFront->Fill(Simp->fMomTrackerFront);
  Hist->fNStrawHits->Fill(Simp->fNStrawHits);
}

//-----------------------------------------------------------------------------
// register data blocks and book histograms
//-----------------------------------------------------------------------------
int TStnTrackSeedAnaModule::BeginJob() {
//-----------------------------------------------------------------------------
// register data blocks
//-----------------------------------------------------------------------------
  RegisterDataBlock("TrackSeedBlock","TStnTrackSeedBlock",&fTrackSeedBlock  );
  RegisterDataBlock("GenpBlock"     ,"TGenpBlock"        ,&fGenpBlock       );
  RegisterDataBlock("SimpBlock"     ,"TSimpBlock"        ,&fSimpBlock       );
//-----------------------------------------------------------------------------
// book histograms
//-----------------------------------------------------------------------------
  BookHistograms();

  return 0;
}


//_____________________________________________________________________________
int TStnTrackSeedAnaModule::BeginRun() {
  int rn = GetHeaderBlock()->RunNumber();
  TStntuple::Init(rn);
  return 0;
}


//_____________________________________________________________________________
void TStnTrackSeedAnaModule::FillHistograms() {

  //  double       cos_th (-2.),  cl_e(-1.);
  //  int          disk_id(-1), alg_mask, nsh, nactive;
  //  float        pfront, ce_pitch, reco_pitch, fcons, t0, sigt, sigp, p; 
//-----------------------------------------------------------------------------
// event histograms
//-----------------------------------------------------------------------------
  FillEventHistograms(fHist.fEvent[0]);

//-----------------------------------------------------------------------------
// Simp histograms
//-----------------------------------------------------------------------------
  if (fSimp) {
    FillSimpHistograms(fHist.fSimp[0],fSimp);
  }

//--------------------------------------------------------------------------------
// track seed histograms
//--------------------------------------------------------------------------------
  TStnTrackSeed* trkSeed;
  for (int i=0; i<fNTrackSeeds; ++i) {
    trkSeed = fTrackSeedBlock->TrackSeed(i);
    
    FillTrackSeedHistograms(fHist.fTrackSeed[0], trkSeed);
    
    int     nhits    = trkSeed->NHits();
    double  p        = trkSeed->P();
    double  chi2     = trkSeed->Chi2();
   
    if (p >  80.) FillTrackSeedHistograms(fHist.fTrackSeed[1], trkSeed);
    if (p >  90.) FillTrackSeedHistograms(fHist.fTrackSeed[2], trkSeed);
    if (p > 100.) FillTrackSeedHistograms(fHist.fTrackSeed[3], trkSeed);

    if ( (nhits>10) && (nhits <  15)) FillTrackSeedHistograms(fHist.fTrackSeed[4], trkSeed);
    if ( nhits>=15                  ) FillTrackSeedHistograms(fHist.fTrackSeed[5], trkSeed);
    if ( (chi2 < 4) && (nhits >= 15)) FillTrackSeedHistograms(fHist.fTrackSeed[6], trkSeed);
  }
//-----------------------------------------------------------------------------
// fill GENP histograms
// GEN_0: all particles
//-----------------------------------------------------------------------------
  TGenParticle* genp;
  for (int i=0; i<fNGenp; i++) {
    genp = fGenpBlock->Particle(i);
    FillGenpHistograms(fHist.fGenp[0],genp);
  }
}

//-----------------------------------------------------------------------------
// 2014-04-30: it looks that reading the straw hits takes a lot of time - 
//              turn off by default by commenting it out
//-----------------------------------------------------------------------------
int TStnTrackSeedAnaModule::Event(int ientry) {

  double                p;
  //  TEmuLogLH::PidData_t  dat;
  //  TStnTrack*            track;
  //  int                   id_word;
  TLorentzVector        mom;

  //  TDiskCalorimeter::GeomData_t disk_geom;

  fTrackSeedBlock->GetEntry(ientry);
  fGenpBlock->GetEntry(ientry);
  fSimpBlock->GetEntry(ientry);
//-----------------------------------------------------------------------------
// assume electron in the first particle, otherwise the logic will need to 
// be changed
//-----------------------------------------------------------------------------
  fNGenp    = fGenpBlock->NParticles();
  fNTrackSeeds = fTrackSeedBlock->NTrackSeeds();

  TGenParticle* genp;
  int           pdg_code, generator_code;

  fParticle = NULL;
  for (int i=fNGenp-1; i>=0; i--) {
    genp           = fGenpBlock->Particle(i);
    pdg_code       = genp->GetPdgCode();
    generator_code = genp->GetStatusCode();
    if ((abs(pdg_code) == fPdgCode) && (generator_code == fGeneratorCode)) {
      fParticle = genp;
      break;
    }
  }
					// may want to revisit the definition of fSimp
  fSimp     = fSimpBlock->Particle(0);

  if (fParticle) {
    fParticle->Momentum(mom);
    p         = mom.P();
  }
  else p = 0.;

  fEleE     = sqrt(p*p+0.511*0.511);

  FillHistograms();

  Debug();

  return 0;		       
}

//-----------------------------------------------------------------------------
void TStnTrackSeedAnaModule::Debug() {
  if (GetDebugBit(3)) {
    if (fNTrackSeeds == 0) GetHeaderBlock()->Print(Form("%s:bit:003 N(track seeds) = 0",GetName()));
  }
}

//_____________________________________________________________________________
int TStnTrackSeedAnaModule::EndJob() {
  printf("----- end job: ---- %s\n",GetName());
  return 0;
}

//_____________________________________________________________________________
void TStnTrackSeedAnaModule::Test001() {
}
 
