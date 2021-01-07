///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#include <cstdlib>
#include "Stntuple/obj/TGenpBlock.hh"


ClassImp(TGenpBlock)

//-----------------------------------------------------------------------------
void TGenpBlock::ReadV1(TBuffer &R__b) {
  R__b >> fNParticles;

  fListOfParticles->Streamer(R__b);
  for (int i=0; i<fNParticles; i++) {
    Particle(i)->SetUniqueID(i);
  }

  fGenProcessID = -1;                // added in V2
  fWeight       = 1.;                // added in V2
}

//______________________________________________________________________________
void TGenpBlock::Streamer(TBuffer &R__b)
{
   // Stream an object of class TGenpBlock as compact, as possible

  if (R__b.IsReading()) {
    Version_t R__v = R__b.ReadVersion(); 
    if (R__v == 1) { 
      ReadV1(R__b);
    }
    else {
//-----------------------------------------------------------------------------
//  current version - V2
//-----------------------------------------------------------------------------
      R__b >> fNParticles;
      R__b >> fGenProcessID; 		// added in V2
      R__b >> fWeight;			// added in V2
      fListOfParticles->Streamer(R__b);
      for (int i=0; i<fNParticles; i++) {
	Particle(i)->SetUniqueID(i);
      }
    }
  } 
  else {
    R__b.WriteVersion(TGenpBlock::IsA());
    R__b << fNParticles;
    R__b << fGenProcessID;
    R__b << fWeight;
    fListOfParticles->Streamer(R__b);
  }
}

//_____________________________________________________________________________
TGenpBlock::TGenpBlock() {
  fNParticles      = 0;
  fGenProcessID    = -1;
  fWeight          = 1.;
  fListOfParticles = new TClonesArray("TGenParticle",100);
  fListOfParticles->BypassStreamer(kFALSE);
}


//_____________________________________________________________________________
TGenpBlock::~TGenpBlock() {
  fListOfParticles->Delete();
  delete fListOfParticles;
}


//_____________________________________________________________________________
void TGenpBlock::Clear(const char* opt) {
  fNParticles    = 0;
  //  fGenProcessID  = -1; // fGenProcessID needs to stay
  fWeight        = 1.;
					// don't modify cut values at run time
  fListOfParticles->Clear(opt);

  f_EventNumber       = -1;
  f_RunNumber         = -1;
  f_SubrunNumber      = -1;
  fLinksInitialized   =  0;
}

//_____________________________________________________________________________
TGenParticle* 
TGenpBlock::NewParticle(Int_t PdgCode, Int_t GeneratorID, 
			Int_t m1, Int_t m2, Int_t d1, Int_t d2, 
			Float_t px, Float_t py, Float_t pz, Float_t e,
			Float_t vx, Float_t vy, Float_t vz, Float_t t,
			float ProperTime)
{
  // add new particle to the block. Block is filled sequentially, so 
  // assume that this is the last particle (not necessarily!) and it is 
  // added to the last primary interaction

  TGenParticle* p;

  p = new ((*fListOfParticles)[fNParticles]) TGenParticle();

  p->Init(fNParticles,PdgCode,GeneratorID,m1,m2,d1,d2,
	  px,py,pz,e,vx,vy,vz,t,ProperTime);

  fNParticles += 1;

  return p;
}


//_____________________________________________________________________________
void TGenpBlock::Print(const char* Opt) const {
  // opt: /c : comment lines only, useful for printing the hard interaction only

  int banner_printed(0), i1, i2, comments_only(0);

  if (strstr(Opt,"/c") != 0) comments_only = 1;

  TGenpBlock* genp = (TGenpBlock*) this;

  i1 = 0;
  i2 = genp->NParticles();

  for (int i=i1; i<i2; i++) {
    TGenParticle* p = genp->Particle(i);
    if ((p->GetStatusCode() == 3) || (comments_only == 0)) {
      if (! banner_printed) {
	p->Print("banner");
	banner_printed = 1;
      }
      p->Print("data");
    }
  }
}


