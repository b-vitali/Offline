///////////////////////////////////////////////////////////////////////////////
//  2014-01-26 P.Murat TTrackStrawHitData
///////////////////////////////////////////////////////////////////////////////
#include "TString.h"

#include "Stntuple/obj/TTrackStrawHitData.hh"

ClassImp(TTrackStrawHitData)

//-----------------------------------------------------------------------------
void TTrackStrawHitData::ReadV1(TBuffer &R__b) {
  struct TTrackStrawHitDataV1_t {
    int     fIndex;
    float   fTime;
    float   fDt;
    float   fEnergy;
  };

  TTrackStrawHitDataV1_t data;

  int nwf_v1 = 3;

  R__b >> fIndex;
  R__b.ReadFastArray(&data.fTime,nwf_v1);

  fTime   = data.fTime;
  fDt     = data.fDt;
  fEnergy = data.fEnergy;
}



//_____________________________________________________________________________
void TTrackStrawHitData::Streamer(TBuffer &R__b) {

  int nwi = ((int*) &fTime) - &fIndex;
  int nwf = &fMcMomentum - &fTime +1;
  
  if(R__b.IsReading()) {
//     Version_t R__v = R__b.ReadVersion();
    R__b.ReadVersion();
//-----------------------------------------------------------------------------
// curent version: V1
//-----------------------------------------------------------------------------
    R__b.ReadFastArray(&fIndex,nwi);
    R__b.ReadFastArray(&fTime ,nwf);
  }
  else {
    R__b.WriteVersion(TTrackStrawHitData::IsA());
    R__b.WriteFastArray(&fIndex,nwi);
    R__b.WriteFastArray(&fTime ,nwf);
  } 
}

//_____________________________________________________________________________
TTrackStrawHitData::TTrackStrawHitData(): TObject() {
  Clear();
}

//_____________________________________________________________________________
TTrackStrawHitData::~TTrackStrawHitData() {
}

//_____________________________________________________________________________
void TTrackStrawHitData::Set(int Index, float Time, float Dt, float EnergyDep,
			     int Active, int Ambig, float DriftRadius,
			     int PdgCode, int MotherPdgCode, int GenCode, int SimID, 
			     float McDoca, float McMomentum) 
{
  fIndex         = Index; 
  fTime          = Time; 
  fDt            = Dt; 
  fEnergy        = EnergyDep;

  fActive        = Active;
  fAmbig         = Ambig;
  fDriftRadius   = DriftRadius;

  fPdgCode       = PdgCode;
  fMotherPdgCode = MotherPdgCode;
  fGeneratorCode = GenCode;
  fSimID         = SimID;

  fMcDoca        = McDoca;
  fMcMomentum    = McMomentum;
  
}

//_____________________________________________________________________________
void TTrackStrawHitData::Clear(Option_t* opt) {
  fIndex         = -1;
  fTime          = 1.e6;
  fDt            = 1.e6;
  fEnergy        = -1;

  fActive        = -1;
  fAmbig         = -99;
  fDriftRadius   = 1.e6;

  fPdgCode       = -1;
  fMotherPdgCode = -1;
  fGeneratorCode = -1;
  fSimID         = -1;

  fMcDoca        = 1.e6;
  fMcMomentum    = -1;
}

//_____________________________________________________________________________
void TTrackStrawHitData::Print(Option_t* Option) const {
  // print Straw hit properties
  //  printf("Superlayer: %d, Wire: %d, Cell: %d,\n",fSuperLayer,fWire,fCell);
  
  TString opt = Option;
  opt.ToLower();

  if ((opt == "") || (opt.Index("banner") >= 0)) {
    printf("------------------------------------------------------------------------------------------------------------\n");
    printf("  Index  Active Ambig  Radius Time   Dt   Energy  PdgCode  PdgCode(M)  GenCode  SimID  McDoca     McMom     \n");
    printf("------------------------------------------------------------------------------------------------------------\n");
  }

  if (opt == "banner") return;
  
  printf("%6i %6i %6i %10.3f %10.3f %10.3f %10.5f %8i %8i %8i %8i %10.3f %10.3f\n",
	 fIndex, 
	 fActive,
	 fAmbig,
	 fDriftRadius,
	 fTime,
	 fDt,
	 fEnergy,
	 fPdgCode,
	 fMotherPdgCode,
	 fGeneratorCode,
	 fSimID,
	 fMcDoca,
	 fMcMomentum);
  
}
