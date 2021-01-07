///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#include <cstdlib>
#include "Stntuple/obj/TStepPointMCBlock.hh"


ClassImp(TStepPointMCBlock)

//______________________________________________________________________________
void TStepPointMCBlock::Streamer(TBuffer &R__b) 
{
   // Stream an object of class TStepPointMCBlock as compact, as possible

  if (R__b.IsReading()) {
    Version_t R__v = R__b.ReadVersion(); if (R__v) { }
    R__b >> fNStepPoints;
    fListOfStepPoints->Streamer(R__b);
  } 
  else {
    R__b.WriteVersion(TStepPointMCBlock::IsA());
    R__b << fNStepPoints;
    fListOfStepPoints->Streamer(R__b);
  }
}

//_____________________________________________________________________________
TStepPointMCBlock::TStepPointMCBlock() {
  fGenProcessID     = -1;
  fNStepPoints      =  0;
  fListOfStepPoints = new TClonesArray("TStepPointMC",10);
  fListOfStepPoints->BypassStreamer(kFALSE);
}


//_____________________________________________________________________________
TStepPointMCBlock::~TStepPointMCBlock() {
  fListOfStepPoints->Delete();
  delete fListOfStepPoints;
}


//_____________________________________________________________________________
void TStepPointMCBlock::Clear(const char* opt) {
  fNStepPoints    = 0;
					// don't modify cut values at run time
  fListOfStepPoints->Clear(opt);

  f_EventNumber       = -1;
  f_RunNumber         = -1;
  f_SubrunNumber      = -1;
  fLinksInitialized   =  0;
}

//_____________________________________________________________________________
TStepPointMC* 
TStepPointMCBlock::NewStepPointMC(int VolumeID, int GenIndex, 
				  int SimID   , int PDGCode, 
				  int ParentSimID, int ParentPDGCode,
				  int CreationCode, int EndProcessCode, 
				  float EDepTot, float EDepNio,
				  float Time, float StepLength,
				  float  X, Float_t  Y, Float_t  Z,
				  float Px, Float_t Py, Float_t Pz
				  )
{
  // add new particle to the block. Block is filled sequentially, so 
  // assume that this is the last particle (not necessarily!) and it is 
  // added to the last primary interaction

  TStepPointMC* sp;

  sp = new ((*fListOfStepPoints)[fNStepPoints]) TStepPointMC(VolumeID,GenIndex,
							     SimID, PDGCode,
							     ParentSimID, ParentPDGCode,
							     CreationCode, EndProcessCode,
							     EDepTot,EDepNio,
							     Time,StepLength,
							     X,Y,Z,Px,Py,Pz);
  fNStepPoints += 1;

  return sp;
}


//_____________________________________________________________________________
void TStepPointMCBlock::Print(const char* Opt) const {
  // opt: /c : comment lines only, useful for printing the hard interaction only

  int banner_printed(0), i1, i2 /*, comments_only(0)*/;

  //  if (strstr(Opt,"/c") != 0) comments_only = 1;

  TStepPointMCBlock* block = (TStepPointMCBlock*) this;

  i1 = 0;
  i2 = block->NStepPoints();

  for (int i=i1; i<i2; i++) {
    TStepPointMC* p = block->StepPointMC(i);
    if (! banner_printed) {
      p->Print("banner");
      banner_printed = 1;
    }
    p->Print("data");
  }
}
