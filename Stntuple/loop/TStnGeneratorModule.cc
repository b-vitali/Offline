//-----------------------------------------------------------------------------
//  Dec 28 2000 P.Murat: Run II STNTUPLE input module
//-----------------------------------------------------------------------------
#include "TROOT.h"
#include "TChain.h"
#include "TFile.h"
#include "TSystem.h"

#include "mc/base/TPrimaryInteraction.hh"
#include "mc/base/TMcGenEvent.hh"
#include "mc/base/TG3Generator.hh"

#include "Stntuple/loop/TStnAna.hh"

#include "Stntuple/obj/TStnNode.hh"
#include "Stntuple/obj/TStnEvent.hh"
#include "Stntuple/obj/TStnHeaderBlock.hh"

#include "Stntuple/loop/TStnGeneratorModule.hh"

Int_t  StnGenInitGenpBlock  (TStnDataBlock*, TStnEvent*, Int_t);
Int_t  StnGenInitHeaderBlock(TStnDataBlock*, TStnEvent*, Int_t);

ClassImp(TStnGeneratorModule)

//_____________________________________________________________________________
TStnGeneratorModule::TStnGeneratorModule(TG3Generator* Gen): 
  TStnInputModule("GeneratorModule","MC Generator Module") 
{
//-----------------------------------------------------------------------------
//  assume to start with that TStnAna object has already been created
//-----------------------------------------------------------------------------
  fCurrent          = -1;
  fListOfGenerators = new TObjArray();
  if (Gen) fListOfGenerators->Add(Gen);
  fFrame       = "CMS";
  fBeam        = "P";
  fTarget      = "PBAR";
  fEnergy      = 1960.;
  fRunNumber   = 1;

  fMcEvent          = new TMcGenEvent();
  fFolder->Add(fMcEvent);
}

//_____________________________________________________________________________
TStnGeneratorModule::~TStnGeneratorModule() {
  // destructor: module owns its histograms

  fListOfGenerators->Delete();
  delete fListOfGenerators;

  delete fMcEvent;
}

//_____________________________________________________________________________
TStnNode* TStnGeneratorModule::GetNode(const char* BranchName,
				       const char* ClassName)
{
  char message[100];
				// check if a node with the name exists
  TStnNode* node;

  TStnEvent* ev = fAna->GetEvent();
  node = ev->FindNode(BranchName);

  if (node) return node;
				// make sure class is known to ROOT

  TClass* cl = gROOT->GetClass(ClassName);
  if (!cl || !cl->InheritsFrom("TStnDataBlock")) {
    sprintf(message,"wrong class name %s",ClassName);
    Error("GetNode",message);
    return NULL;
  }

  Int_t (*func)(TStnDataBlock *, TStnEvent *, Int_t);

  if      (strcmp(BranchName,"GenpBlock") == 0) {
    func = StnGenInitGenpBlock;
  }
  else if (strcmp(BranchName,"HeaderBlock") == 0) {
    func = StnGenInitHeaderBlock;
  }

  node = new TStnNode(BranchName,cl,ev,func);
  ev->GetListOfNodes()->Add(node);
  return node;
}

//_____________________________________________________________________________
int TStnGeneratorModule::BeginJob() {
  // 
  TG3Generator* gen;

  TIter it(fListOfGenerators);
  while((gen = (TG3Generator*) it.Next())) {
    gen->Initialize(fFrame.Data(),fBeam.Data(),fTarget.Data(),fEnergy);
  }

  TStnEvent* ev = new TStnEvent();
  fAna->SetEvent(ev);
  return 0;
}

//_____________________________________________________________________________
int TStnGeneratorModule::BeginRun() {
  return 0;
}

//_____________________________________________________________________________
int TStnGeneratorModule::Event(Int_t i) {
  return 0;
}

//_____________________________________________________________________________
int TStnGeneratorModule::NextEvent(Int_t IEntry) {

  TStnEvent* event  = fAna->GetEvent();

  TPrimaryInteraction* pi;

  TG3Generator* gen;
  TIter it(fListOfGenerators);

  fMcEvent->Clear();

  while((gen = (TG3Generator*) it.Next())) {
//-----------------------------------------------------------------------------
//  assume for the moment that every generator generates only one primary
//  interaction per call (init = 1)
//-----------------------------------------------------------------------------
    int nint = gen->NInteractions();
    for (int i=0; i<nint; i++) {
      pi = fMcEvent->NextInteraction();
      gen->GenerateEvent(pi);
    }
  }
  event->SetCurrentEntry(IEntry);

  GetHeaderBlock()->fEventNumber = IEntry;
  GetHeaderBlock()->fRunNumber   = fRunNumber;

  return IEntry;
}

//_____________________________________________________________________________
int TStnGeneratorModule::RegisterInputBranches(TStnEvent* Event)
{
  TClass*    cl;
  TStnNode*  node;

  TObjArray* list_of_nodes = Event->GetListOfInputNodes();

  //  Int_t (*func)(TStnDataBlock *, TStnEvent *, Int_t);

  cl   = gROOT->GetClass("TStnHeaderBlock");
  node = new TStnNode("HeaderBlock",cl,Event,StnGenInitHeaderBlock);
  list_of_nodes->Add(node);
  
  cl   = gROOT->GetClass("TGenpBlock");
  node = new TStnNode("GenpBlock",cl,Event,StnGenInitGenpBlock);
  list_of_nodes->Add(node);
  
  return 0;
}

//_____________________________________________________________________________
int TStnGeneratorModule::EndRun() {
  return 0;
}

//_____________________________________________________________________________
int TStnGeneratorModule::EndJob() {
  TG3Generator* gen;
  TIter it(fListOfGenerators);

  while((gen = (TG3Generator*) it.Next())) {
//-----------------------------------------------------------------------------
//  print final statistics
//-----------------------------------------------------------------------------
    gen->PrintStat(1);
  }
  return 0;
}

