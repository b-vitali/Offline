///////////////////////////////////////////////////////////////////////////////
//  2014-01-26 P.Murat TCalHitData
///////////////////////////////////////////////////////////////////////////////
#include "Stntuple/obj/TCalHitData.hh"

ClassImp(TCalHitData)

//-----------------------------------------------------------------------------
// frst version of the I/O with the schema evolution
//-----------------------------------------------------------------------------
void TCalHitData::ReadV1(TBuffer &R__b) {

  struct TCalHitData_t_V1 {
    int        fID;         // hit ID, cods disk,  x1, x2
    int        fNChannels;  // number of readout channels, 1 or 2 (kludge)
    float      fTime; 
    float      fEnergy;
    void*      fDummy;      // !
  } data;
  
  int nwi = ((int*  ) &data.fTime ) - &data.fID;
  int nwf = ((float*) &data.fDummy) - &data.fTime  ;
  
  R__b.ReadFastArray(&fID  ,nwi);
  R__b.ReadFastArray(&fTime,nwf);
}

//_____________________________________________________________________________
void TCalHitData::Streamer(TBuffer &R__b) {
  int nwi = 2;
  int nwf = 2;
  
  if(R__b.IsReading()) {
    Version_t R__v = R__b.ReadVersion();
    if (R__v == 1) ReadV1(R__b);
    else {
//-----------------------------------------------------------------------------
// read version > 1 ???
//-----------------------------------------------------------------------------
      printf(">>> ERROR: TCalHitData::Streamer read wrong version %i\n",R__v);
    }
  }
  else {
    R__b.WriteVersion(TCalHitData::IsA());
    R__b.WriteFastArray(&fID  ,nwi);
    R__b.WriteFastArray(&fTime,nwf);
  } 
}

//_____________________________________________________________________________
TCalHitData::TCalHitData(): TObject() {
  Clear();
}

//_____________________________________________________________________________
TCalHitData::~TCalHitData() {
}

//_____________________________________________________________________________
void TCalHitData::Clear(Option_t* opt) {
  fID        = -1;
  fNChannels = -1;
  fTime      = -1.;
  fEnergy    = -1.;
}

//_____________________________________________________________________________
void TCalHitData::Print(Option_t* opt) const {
  // print Straw hit properties
  //  printf("Superlayer: %d, Wire: %d, Cell: %d,\n",fSuperLayer,fWire,fCell);
}
