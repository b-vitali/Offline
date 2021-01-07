//
#include <iostream>

#include "TMatrix.h"
#include "obj/TStnTrackSeed.hh"

// #include "RecoDataProducts/inc/TrackSeed.hh"

namespace {
  //  const double BF = 1.4116 ;  // CDF case
  const double BF = 1.0 ;         // MU2E case
}

//namespace murat {

ClassImp(TStnTrackSeed)
void TStnTrackSeed::ReadV1(TBuffer &R__b) {

  struct TStnTrackSeedDataV1_t {
    int                       fNHits;
    int                       fHelixIndex;
    int                       fTrackIndex;
//-----------------------------------------------------------------------------
// floats
//-----------------------------------------------------------------------------
    float                     fT0;    
    float                     fT0Err; 
    float                     fD0;    
    float                     fZ0;    
    float                     fP;  
    float                     fPt; 
    float                     fTanDip;
    float                     fFlt0;// flight distance where the track crosses the tracker midplane (z=0)
    float                     fChi2;
    float                     fFitCons;

    float                     fClusterTime;   
    float		      fClusterEnergy; 
    float		      fClusterX;      
    float		      fClusterY;      
    float		      fClusterZ;      
 
    const mu2e::KalSeed*      fTrackSeed;  //!
    const mu2e::CaloCluster*  fCluster;    //!
  };

  TStnTrackSeedDataV1_t data;

  int            nwi, nwf;
  
  nwi      = ((int*  ) &data.fT0            ) - &data.fNHits;
  nwf      = ((float*) &data.fTrackSeed     ) - &data.fT0;
    
  R__b.ReadFastArray(&data.fNHits ,nwi);
  R__b.ReadFastArray(&data.fT0    ,nwf);

  fMom1.SetPxPyPzE(0.,0.,0.,0.);
  fOrigin1.SetXYZT(0.,0.,0.,0.);

  fMom2.SetPxPyPzE(0.,0.,0.,0.);
  fOrigin2.SetXYZT(0.,0.,0.,0.);


  fNHits             = data.fNHits;	     
  fHelixIndex        = data.fHelixIndex; 
  fTrackIndex        = data.fTrackIndex; 
  fSimpPDG1          = -1;         // added in v2
  fSimpPDGM1         = -1;         // added in v2
  fSimpId1Hits       = -1;         // added in v2
  fSimpPDG2          = -1;         // added in v2
  fSimpPDGM2         = -1;         // added in v2
  fSimpId2Hits       = -1;         // added in v2


  fT0                = data.fT0;    	  
  fT0Err             = data.fT0Err; 	  
                
  fD0                = data.fD0;    
  fZ0               = data.fZ0;   
  fP                 = data.fP;
  fPt                = data.fPt;
  fTanDip            = data.fTanDip;
  fFlt0              = data.fFlt0;
  fChi2              = data.fChi2;
  fFitCons           = data.fFitCons;

  fClusterTime       = data.fClusterTime; 
  fClusterEnergy     = data.fClusterEnergy; 
  fClusterX          = data.fClusterX;      
  fClusterY          = data.fClusterY;    
  fClusterZ          = data.fClusterZ;    

  fTrackSeed         = data.fTrackSeed;
  fCluster           = data.fCluster;
}

//-----------------------------------------------------------------------------
void TStnTrackSeed::ReadV2(TBuffer &R__b) {

  struct TStnTrackSeedDataV2_t {
    TLorentzVector            fMom1;  
    TLorentzVector            fOrigin1;
    TLorentzVector            fMom2;  
    TLorentzVector            fOrigin2;
//-----------------------------------------------------------------------------
// int
//-----------------------------------------------------------------------------
    int                       fNHits;
    int                       fHelixIndex;
    int                       fTrackIndex;
    int                       fSimpPDG1;          // added in v2
    int                       fSimpPDGM1;         // added in v2
    int                       fSimpId1Hits;       // added in v2
    int                       fSimpPDG2;          // added in v2
    int                       fSimpPDGM2;         // added in v2
    int                       fSimpId2Hits;       // added in v2
    int                       fInt[kNFreeIntsV2];
//-----------------------------------------------------------------------------
// floats
//-----------------------------------------------------------------------------
    float                     fT0;    
    float                     fT0Err; 
    float                     fD0;    
    float                     fZ0;    
    float                     fP;  
    float                     fPt; 
    float                     fTanDip;
    float                     fFlt0;// flight distance where the track crosses the tracker midplane (z=0)
    float                     fChi2;
    float                     fFitCons;

    float                     fClusterTime;   
    float		      fClusterEnergy; 
    float		      fClusterX;      
    float		      fClusterY;      
    float		      fClusterZ;      
    float                     fFloat[kNFreeFloatsV2]; // provision for future I/O expansion

    const mu2e::KalSeed*      fTrackSeed;  //!
    const mu2e::CaloCluster*  fCluster;    //!
  };

  TStnTrackSeedDataV2_t data;

  int            nwi, nwf;
  fMom1.Streamer(R__b);
  fOrigin1.Streamer(R__b);
  fMom2.Streamer(R__b);
  fOrigin2.Streamer(R__b);

  nwi      = ((int*  ) &data.fT0            ) - &data.fNHits;
  nwf      = ((float*) &data.fTrackSeed     ) - &data.fT0;
    
  R__b.ReadFastArray(&data.fNHits ,nwi);
  R__b.ReadFastArray(&data.fT0    ,nwf);

  fNHits             = data.fNHits;	     
  fHelixIndex        = data.fHelixIndex; 
  fTrackIndex        = data.fTrackIndex; 
  fSimpPDG1          = data.fSimpPDG1   ;         
  fSimpPDGM1         = data.fSimpPDGM1  ;         
  fSimpId1Hits       = data.fSimpId1Hits;         
  fSimpPDG2          = data.fSimpPDG2   ;         
  fSimpPDGM2         = data.fSimpPDGM2  ;         
  fSimpId2Hits       = data.fSimpId2Hits;         
  fNLoops            = 0;                         // added in v3

  fT0                = data.fT0;    	  
  fT0Err             = data.fT0Err; 	  
                
  fD0                = data.fD0;    
  fZ0                = data.fZ0;   
  fP                 = data.fP;
  fPt                = data.fPt;
  fTanDip            = data.fTanDip;
  fFlt0              = data.fFlt0;
  fChi2              = data.fChi2;
  fFitCons           = data.fFitCons;

  fClusterTime       = data.fClusterTime; 
  fClusterEnergy     = data.fClusterEnergy; 
  fClusterX          = data.fClusterX;      
  fClusterY          = data.fClusterY;    
  fClusterZ          = data.fClusterZ;    

  fTrackSeed         = data.fTrackSeed;
  fCluster           = data.fCluster;
}

//-----------------------------------------------------------------------------
void TStnTrackSeed::Streamer(TBuffer& R__b) {
  int   nwi, nwf;
  
  nwi      = ((int*  ) &fT0            ) - &fNHits;
  nwf      = ((float*) &fTrackSeed     ) - &fT0;

  if (R__b.IsReading()) {
    Version_t R__v = R__b.ReadVersion(); 
    if (R__v == 1) ReadV1 (R__b);
    if (R__v == 2) ReadV2 (R__b);
    else {
      fMom1.Streamer(R__b);
      fOrigin1.Streamer(R__b);
      fMom2.Streamer(R__b);
      fOrigin2.Streamer(R__b);
					// current version: V2
      R__b.ReadFastArray(&fNHits, nwi);
      R__b.ReadFastArray(&fT0   , nwf);
    }
   }
  else {
    R__b.WriteVersion(TStnTrackSeed::IsA());
    fMom1.Streamer(R__b);
    fOrigin1.Streamer(R__b);
    fMom2.Streamer(R__b);
    fOrigin2.Streamer(R__b);

    R__b.WriteFastArray(&fNHits, nwi);
    R__b.WriteFastArray(&fT0    , nwf);
  }
}

//-----------------------------------------------------------------------------
TStnTrackSeed::TStnTrackSeed(int Number) {
  fMom1.SetPxPyPzE(0.,0.,0.,0.);
  fOrigin1.SetXYZT(0.,0.,0.,0.);
  
  fMom2.SetPxPyPzE(0.,0.,0.,0.);
  fOrigin2.SetXYZT(0.,0.,0.,0.);
 
  fNumber      = Number;
  fNHits       = 0;
  fHelixIndex  = -1;
  fTrackIndex  = -1;
  fSimpPDG1    = 0;   
  fSimpPDGM1   = 0;  
  fSimpId1Hits = 0;
  fSimpPDG2    = 0;   
  fSimpPDGM2   = 0;  
  fSimpId2Hits = 0;
  
  fT0      = 0;    
  fT0Err   = 0; 
  fD0      = 0;    
  fZ0      = 0;    
  fP       = 0;
  fPt      = 0;
  fTanDip  = 0;
  fFlt0    = 0;
  fChi2    = 0;
  fFitCons = 0;

  fClusterTime   = 0;  
  fClusterEnergy = 0;
  fClusterX      = 0;     
  fClusterY      = 0;     
  fClusterZ      = 0;     
  
  fNLoops        = 0;

  fTrackSeed = NULL;
  fCluster   = NULL;
}

//_____________________________________________________________________________
TStnTrackSeed::~TStnTrackSeed() {
}


//-----------------------------------------------------------------------------
void TStnTrackSeed::Clear(Option_t* opt) {
  Error("Print", "Not implemented yet");
}

//-----------------------------------------------------------------------------
void TStnTrackSeed::Print(Option_t* opt) const {
  Error("Print", "Not implemented yet");
}

// } // end namespace




