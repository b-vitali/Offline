///////////////////////////////////////////////////////////////////////////////
// analyse output of PHOTOS mu -> e nu
///////////////////////////////////////////////////////////////////////////////
#include "TF1.h"
#include "Stntuple/loop/TStnAna.hh"
#include "Stntuple/obj/TStnNode.hh"
#include "Stntuple/obj/TStnEvent.hh"

#include "ana/TStnPhotosAnaModule.hh"

ClassImp(TStnPhotosAnaModule)

//_____________________________________________________________________________
TStnPhotosAnaModule::TStnPhotosAnaModule(const char* name, const char* title):
  TStnModule(name,title)
{
  fPhotos = TPhotos::Instance();
}

//_____________________________________________________________________________
TStnPhotosAnaModule::~TStnPhotosAnaModule() {
}

//_____________________________________________________________________________
void TStnPhotosAnaModule::BookEventHistograms(EventHist_t* Hist, const char* Folder) {
  //  char name [200];
  //  char title[200];
					// book histograms

  HBook1F(Hist->fNGenp       ,"ngenp"        ,"N(GenParticles)" ,  20 ,0, 20,Folder);
  HBook1F(Hist->fNPhotons    ,"nphot"        ,"N(photons)"      ,  20 ,0, 20,Folder);
  HBook1F(Hist->fMomLO[0]    ,"mom_lo_0"     ,"MomLO[0]"        ,11000, 0   ,110   ,Folder);
  HBook1F(Hist->fMomLO[1]    ,"mom_lo_1"     ,"MomLO[1]"        , 1100,-0.05,109.95,Folder);
  HBook1F(Hist->fELO[0]      ,"e_lo_0"       ,"EnergyLO[0]"     ,11000, 0   ,110   ,Folder);
  HBook1F(Hist->fELO[1]      ,"e_lo_1"       ,"EnergyLO[1]"     , 1100,-0.05,109.95,Folder);

}

//_____________________________________________________________________________
void TStnPhotosAnaModule::BookGenpHistograms(GenpHist_t* Hist, const char* Folder) {
  //  char name [200];
  //  char title[200];
					// book histograms
  
  HBook1F(Hist->fMom[0],"mom_0","Momentum[0]",11000, 0.  ,110   ,Folder);
  HBook1F(Hist->fMom[1],"mom_1","Momentum[1]", 1100,-0.05,109.95,Folder);
  HBook1F(Hist->fEnergy[0],"e_0","Energy[0]", 11000,-0.  ,110.  ,Folder);
  HBook1F(Hist->fEnergy[1],"e_1","Energy[1]",  1100,-0.05,109.95,Folder);
}

//_____________________________________________________________________________
void TStnPhotosAnaModule::FillEventHistograms(EventHist_t* Hist) {
  
  //  Hist->fRunNumber->Fill(GetHeaderBlock()->RunNumber());

  int ngenp = fGenpBlock->NParticles();
  Hist->fNGenp->Fill(ngenp);
  Hist->fNPhotons->Fill(fNPhotons);

  Hist->fMomLO[0]->Fill(fP*1000,fWeight);
  Hist->fMomLO[1]->Fill(fP*1000,fWeight);
  Hist->fELO  [0]->Fill(fE*1000,fWeight);
  Hist->fELO  [1]->Fill(fE*1000,fWeight);
}

//_____________________________________________________________________________
void TStnPhotosAnaModule::FillGenpHistograms(GenpHist_t* Hist, TGenParticle* Genp) {
  
  TLorentzVector mom;
  float          p, e;
  
  Genp->Momentum(mom);

  p = mom.P();
  e = mom.P();
					// convert GeV --> MeV
  Hist->fMom[0]->Fill(p*1000.,fWeight);
  Hist->fMom[1]->Fill(p*1000.,fWeight);
  Hist->fEnergy[0]->Fill(e*1000.,fWeight);
  Hist->fEnergy[1]->Fill(e*1000.,fWeight);
}


//_____________________________________________________________________________
void TStnPhotosAnaModule::BookHistograms() {
  //  char name [200];
  //  char title[200];
					// book histograms
  TFolder* fol;
  TFolder* hist_folder;
  char     folder_name[200];

  DeleteHistograms();
  hist_folder = (TFolder*) GetFolder()->FindObject("Hist");
//-----------------------------------------------------------------------------
//  book event-level histograms
//-----------------------------------------------------------------------------
  int book_event_histset[kNEventHistSets];
  for (int i=0; i<kNEventHistSets; i++) book_event_histset[i] = 0;

  book_event_histset[ 0] = 1;		// all events
  book_event_histset[ 1] = 0;		// *available*

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
//  book particle-level histograms
//-----------------------------------------------------------------------------
  int book_genp_histset[kNGenpHistSets];
  for (int i=0; i<kNGenpHistSets; i++) book_genp_histset[i] = 0;

  book_genp_histset[ 0] = 1;		// electrons
  book_genp_histset[ 1] = 1;		// photons

  for (int i=0; i<kNGenpHistSets; i++) { 
    if (book_genp_histset[i] != 0) {
      sprintf(folder_name,"genp_%i",i);
      fol = (TFolder*) hist_folder->FindObject(folder_name);
      if (! fol) fol = hist_folder->AddFolder(folder_name,folder_name);
      fHist.fGenp[i] = new GenpHist_t;
      BookGenpHistograms(fHist.fGenp[i],Form("Hist/%s",folder_name));
    }
  }
}

//_____________________________________________________________________________
int TStnPhotosAnaModule::BeginJob() {

				// register data blocks

  RegisterDataBlock("GenpBlock","TGenpBlock",&fGenpBlock);

					// book histograms
  BookHistograms();

  return 0;
}

//_____________________________________________________________________________
int TStnPhotosAnaModule::BeginRun() {
  // initialize run-dependent calibration constants
  
  return 0;
}

//_____________________________________________________________________________
int TStnPhotosAnaModule::Event(int ientry) {
//-----------------------------------------------------------------------------
// in case of a TChain, ientry is the entry number in the current file
//-----------------------------------------------------------------------------
  TGenParticle *genp;

  double const mE (0.511e-3);     // in GeV
  
  fGenpBlock->GetEntry(ientry);

  fNPhotons = 0;

  fWeight = fPhotos->GetWeight();
  fP      = fPhotos->GetP();
  fE      = sqrt(fP*fP+mE*mE);
  
  int np = fGenpBlock->NParticles();
  for (int i=0; i<np; i++) {
    genp = fGenpBlock->Particle(i);
    if (genp->IsPhoton()) fNPhotons++;
  }
//-----------------------------------------------------------------------------
//  fill histograms
//-----------------------------------------------------------------------------
  FillHistograms();
//-----------------------------------------------------------------------------
//  debugging part
//-----------------------------------------------------------------------------
  if (PrintLevel() == 1) {
    fGenpBlock->Print();
  }

  Debug();
  return 0;			       
}

//-----------------------------------------------------------------------------
void TStnPhotosAnaModule::FillHistograms() {

  FillEventHistograms(fHist.fEvent[0]);

//-----------------------------------------------------------------------------
// fill GENP histograms
//-----------------------------------------------------------------------------
  TGenParticle* genp;
  
  int np = fGenpBlock->NParticles();

  for (int i=0; i<np; i++) {
    genp = fGenpBlock->Particle(i);
    if (genp->IsElectron()) {
      FillGenpHistograms(fHist.fGenp[0],genp);
    }
    else if (genp->IsPhoton()) {
      FillGenpHistograms(fHist.fGenp[1],genp);
    }
  }
}

//_____________________________________________________________________________
int TStnPhotosAnaModule::Debug() {

  return 0;
}

//_____________________________________________________________________________
int TStnPhotosAnaModule::EndRun() {
  return 0;
}

//_____________________________________________________________________________
int TStnPhotosAnaModule::EndJob() {
  printf("----- end job: ---- %s\n",GetName());
  return 0;
}

//_____________________________________________________________________________
void TStnPhotosAnaModule::Print(Option_t* opt) const {
}

