//
#include <iostream>

#include "TMatrix.h"
#include "obj/TStnTimeCluster.hh"

// #include "RecoDataProducts/inc/TimeCluster.hh"

namespace {
  //  const double BF = 1.4116 ;  // CDF case
  const double BF = 1.0 ;         // MU2E case
}

//namespace murat {

ClassImp(TStnTimeCluster)
void TStnTimeCluster::ReadV1(TBuffer &R__b) {

  struct TStnTimeClusterDataV1_t {
    int                       fNHits;
    int                       fHelixSeedIndex;

    float                     fT0;    
    float                     fT0Err; 

    float                     fPosX;
    float                     fPosY;
    float                     fPosZ;
 
    float                     fClusterTime;   
    float		      fClusterEnergy; 
    float		      fClusterX;      
    float		      fClusterY;      
    float		      fClusterZ;      
    
    const mu2e::TimeCluster*    fTimeCluster;  //!
  };

  TStnTimeClusterDataV1_t data;

  int            nwi, nwf;
  
  nwi      = ((int*  ) &data.fT0            ) - &data.fNHits;
  nwf      = ((float*) &data.fTimeCluster   ) - &data.fT0;
    
  R__b.ReadFastArray(&data.fNHits ,nwi);
  R__b.ReadFastArray(&data.fT0    ,nwf);

  fNHits             = data.fNHits;	     
  fHelixSeedIndex    = data.fHelixSeedIndex;

  fT0                = data.fT0;    	  
  fT0Err             = data.fT0Err; 	  
                
  fPosX              = data.fPosX; 
  fPosY              = data.fPosY; 
  fPosZ              = data.fPosZ; 

  fClusterEnergy     = data.fClusterEnergy; 
  fClusterX          = data.fClusterX;      
  fClusterY          = data.fClusterY;    
  fClusterZ          = data.fClusterZ;    
}

void TStnTimeCluster::Streamer(TBuffer& R__b) {
  int   nwi, nwf;
  
  nwi      = ((int*  ) &fT0            ) - &fNHits;
  nwf      = ((float*) &fTimeCluster   ) - &fT0;

  if (R__b.IsReading()) {
    Version_t R__v = R__b.ReadVersion(); 
    
    if (R__v == 1) ReadV1 (R__b);
    else {
                                        // current version: V2
      R__b.ReadFastArray(&fNHits, nwi);
      R__b.ReadFastArray(&fT0   , nwf);
    }
  }
  else {
    R__b.WriteVersion(TStnTimeCluster::IsA());

    R__b.WriteFastArray(&fNHits, nwi);
    R__b.WriteFastArray(&fT0   , nwf);
    
    
  }
}

TStnTimeCluster::TStnTimeCluster(int i) {

  fNHits  = 0;

  fNComboHits  = 0;


  fT0     = 0;    
  fT0Err  = 0; 

  fPosX   = 0;
  fPosY   = 0;
  fPosZ   = 0;

  fClusterTime   = 0;  
  fClusterEnergy = 0;
  fClusterX      = 0;     
  fClusterY      = 0;     
  fClusterZ      = 0;     

}

//_____________________________________________________________________________
TStnTimeCluster::~TStnTimeCluster() {
}


//-----------------------------------------------------------------------------
void TStnTimeCluster::Clear(Option_t* opt) {
  Error("Print", "Not implemented yet");
}

//-----------------------------------------------------------------------------
void TStnTimeCluster::Print(Option_t* opt) const {
  Error("Print", "Not implemented yet");
}

// } // end namespace




