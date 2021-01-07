//
#include <iostream>

#include "obj/TStnPid.hh"

ClassImp(TStnPid)

//-----------------------------------------------------------------------------
// Read an object of class TStnPid (version 1).
// V1 didn't store the cluster asymmetry data
//-----------------------------------------------------------------------------
// void TStnPid::ReadV1(TBuffer &R__b) {
// }

//-----------------------------------------------------------------------------
void TStnPid::Streamer(TBuffer& R__b) {
  int nwi, nwf;

  nwi = (int*  ) &fInt   - &fTrkNumber      + kNFreeInts;
  nwf = (float*) &fFloat - &fLogDedxProbEle + kNFreeFloats ;

  if (R__b.IsReading()) {
    //    Version_t R__v = R__b.ReadVersion(); 
    R__b.ReadVersion(); 
					// current version: V1
    R__b.ReadFastArray(&fTrkNumber     ,nwi);
    R__b.ReadFastArray(&fLogDedxProbEle,nwf);
  }
  else {
    R__b.WriteVersion(TStnPid::IsA());

    R__b.WriteFastArray(&fTrkNumber     ,nwi);
    R__b.WriteFastArray(&fLogDedxProbEle,nwf);
  }
}

//_____________________________________________________________________________
TStnPid::TStnPid(Int_t Number) {
  // 'Number' can be -1 ...
  
  fTrkNumber = Number;
}

//_____________________________________________________________________________
TStnPid::~TStnPid() {
}

//-----------------------------------------------------------------------------
void TStnPid::Clear(Option_t* opt) {
  Error("Print", "Not implemented yet");
}

//-----------------------------------------------------------------------------
void TStnPid::Print(Option_t* Option) const {

  TString opt(Option);

  opt.ToLower();
					// "non-const *this" for printing purposes
  //  TStnPid* pid = (TStnPid*) this;

  if ((opt == "") || (opt.Index("banner") >= 0)) {
//-----------------------------------------------------------------------------
// print banner
//-----------------------------------------------------------------------------
    printf("---------------------------------------------------------------------------------------------------");
    printf("-----------------------------------\n");
    printf(" i  nm nma nuosh nussh nuosd  llhr_dedx(e/m)     drds        err_drds      drds_os  err_drds_os    ");
    printf("sum_avik     sq2_avik   sum_avik_os\n");
    printf("---------------------------------------------------------------------------------------------------");
    printf("-----------------------------------\n");
  }

  double llhr_dedx = (fLogDedxProbEle-fLogDedxProbMuo);
  if ((opt == "") || (opt.Index("data") >= 0)) {
    printf("%2i %3i %3i %5i %5i %5i", fTrkNumber, fNMatched, fNMatchedAll, fNUsedOsH, fNUsedSsH, fNUsedOsD);
    printf("   %12.5e %12.5e %12.5e %12.5e %12.5e %12.5e %12.5e %12.5e",
	   llhr_dedx, 
	   fDrdsVadim, fDrdsVadimErr,
	   fDrdsOs, fDrdsOsErr,
	   fSumAvik, fSq2Avik, fSumAvikOs);
    printf("\n");
  }
  
}

// } // end namespace




