#ifndef RecoDataProducts_LumiInfo_hh
#define RecoDataProducts_LumiInfo_hh
#include <float.h>
#include <math.h>
//
// This object is to contain the numbers to monitor the luminosity
// The aim is to have a unit16_t * lumi = new unit16_t
// lumi[0] : 8 bit for the version, 8 bit for the number of entries
// lumi[i] : 16 bit to indicate the i-number for the lumi (proton TC, calo energy etc)
//
// bvitali Dec 2020


namespace mu2e {

  struct LumiInfo {
    
    LumiInfo(){
      _lumi[0] = 0;
      _lumi[1] = 0;
    }    

    unsigned short _lumi[2];

  };
}

#endif


