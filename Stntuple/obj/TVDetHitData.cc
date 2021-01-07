///////////////////////////////////////////////////////////////////////////////
//  2014-07-15 TVDetHitData
///////////////////////////////////////////////////////////////////////////////
#include "TString.h"

#include "Stntuple/obj/TVDetHitData.hh"

ClassImp(TVDetHitData)

//-----------------------------------------------------------------------------
void TVDetHitData::ReadV1(TBuffer &R__b) {
  struct TVDetHitDataV1_t {
    int     fIndex;
    float   fTime;
    float   fMass;
    float   fEnergyKin;
    float   fEnergy;
    float   fMcMomentum;		
    float   fMcMomentumX;		
    float   fMcMomentumY;		
    float   fMcMomentumZ;		
    float   fMcPositionX;			
    float   fMcPositionY;			
    float   fMcPositionZ;			
  };

  TVDetHitDataV1_t data;

  int nwf_v1 = 11;

  R__b >> fIndex;
  R__b.ReadFastArray(&data.fTime,nwf_v1);

  fTime          = data.fTime;
  fMass          = data.fMass;
  fEnergyKin     = data.fEnergyKin;
  fEnergy        = data.fEnergy;
  fMcMomentum    = data.fMcMomentum;		
  fMcMomentumX   = data.fMcMomentumX;		
  fMcMomentumY   = data.fMcMomentumY;		
  fMcMomentumZ   = data.fMcMomentumZ;		
  fMcPositionX   = data.fMcPositionX;			
  fMcPositionY   = data.fMcPositionY;			
  fMcPositionZ   = data.fMcPositionZ;
  

}



//_____________________________________________________________________________
void TVDetHitData::Streamer(TBuffer &R__b) {

  int nwi = ((int*) &fTime) - &fIndex;
  int nwf = &fMcPositionZ - &fTime +1;
  
  if(R__b.IsReading()) {
    Version_t R__v = R__b.ReadVersion();
    if (R__v == 1) ReadV1(R__b);
    else {
//-----------------------------------------------------------------------------
// curent version: V2
//-----------------------------------------------------------------------------
      R__b.ReadFastArray(&fIndex,nwi);
      R__b.ReadFastArray(&fTime ,nwf);
    }
  }
  else {
    R__b.WriteVersion(TVDetHitData::IsA());
    R__b.WriteFastArray(&fIndex,nwi);
    R__b.WriteFastArray(&fTime ,nwf);
  } 
}

//_____________________________________________________________________________
TVDetHitData::TVDetHitData(): TObject() {
  Clear();
}

//_____________________________________________________________________________
TVDetHitData::~TVDetHitData() {
}

//_____________________________________________________________________________
void TVDetHitData::Set(int Index, float Time,  float Mass, float EnergyKin,
			  float Energy,
			  int PdgID, int GenCode,
			  float McMomentum, 
			  float McMomentumX, float McMomentumY, float McMomentumZ,
			  float McPositionX, float McPositionY, float McPositionZ)
{
  fIndex         = Index; 
  fTime          = Time; 
  fMass          = Mass;
  fEnergyKin     = EnergyKin;
  fEnergy        = Energy;
  fPdgCode       = PdgID;
  fGeneratorCode = GenCode;
  fMcMomentum    = McMomentum;
  fMcMomentumX   = McMomentumX;
  fMcMomentumY   = McMomentumY;
  fMcMomentumZ   = McMomentumZ;
  fMcPositionX   = McPositionX;
  fMcPositionY   = McPositionY;
  fMcPositionZ   = McPositionZ;


  
}

//_____________________________________________________________________________
void TVDetHitData::Clear(Option_t* opt) {
  fIndex         = -1;
  fTime          = 1.e6;
  fMass          = -1;
  fEnergyKin     = -1;
  fEnergy        = -1;
  fPdgCode       = -1;
  fGeneratorCode = -1;
  fMcMomentum    = -1;
  fMcMomentumX   = -1;
  fMcMomentumY   = -1;
  fMcMomentumZ   = -1;
  fMcPositionX   = -1;
  fMcPositionY   = -1;
  fMcPositionZ   = -1;
}

//_____________________________________________________________________________
void TVDetHitData::Print(Option_t* Option) const {
  // print Virtual hit properties
  //  printf("Superlayer: %d, Wire: %d, Cell: %d,\n",fSuperLayer,fWire,fCell);
  
  TString opt = Option;
  opt.ToLower();

  if ((opt == "") || (opt.Index("banner") >= 0)) {
    printf("-------------------------------------------------------------------------------------");
    printf("-------------------------------------------------------\n");
    printf("  Index    Time       Mass    EKin   Energy       Pdg   GenCode     McMom      McMomX");
    printf("     McMomY     McMomZ     McPosX     McPosY     McPosZ\n");
    printf("-------------------------------------------------------------------------------------");
    printf("-------------------------------------------------------\n");
  }

  if (opt == "banner") return;
  
  printf("%6i %10.3f %8.3f %8.3f %8.3f %8i  %8i %10.3f %10.3f %10.3f %10.3f %10.3f %10.3f %10.3f\n",
	 fIndex, 
	 fTime,
	 fMass,
	 fEnergyKin,
	 fEnergy,
	 fPdgCode,
	 fGeneratorCode,
	 fMcMomentum,
	 fMcMomentumX,
	 fMcMomentumY,
	 fMcMomentumZ,
	 fMcPositionX,
	 fMcPositionY,
	 fMcPositionZ);
  
}
