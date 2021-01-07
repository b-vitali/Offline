#ifndef STNTUPLE_ALG_TStntuple_hh
#define STNTUPLE_ALG_TStntuple_hh

#include <math.h>
#include "TClonesArray.h"
#include "TVector2.h"
#include "TVector3.h"

class TStnTrack;
class TStnCluster;
class TStnElectron;

class TStntuple: public TObject {
protected:
  static TStntuple*  fgInstance;

  static Int_t             fgRunNumber;
  static Float_t           fgEventVertex;

  class  Cleaner {
  public:
    Cleaner ();
    ~Cleaner();
  };
  friend class Cleaner;
//-----------------------------------------------------------------------------
//  methods
//-----------------------------------------------------------------------------
public:
                                // don't use constructor! use Instance()
                                // instead (if ever necessary...)
  TStntuple();
  virtual ~TStntuple();

  static TStntuple*  Instance();

  static Int_t     Init(Int_t RunNumber);

//-----------------------------------------------------------------------------
// parameterization of the DIO spectrum on Al from 
// Czarnecki et al, Phys.Rev.D84:013006,2011 (http://www.arxiv.org/abs/1106.4756)
//-----------------------------------------------------------------------------
  static double DioWeightAl   (double P);

//-----------------------------------------------------------------------------
// parameterization of the LO and LL DIO spectra on Al from 
// from mu2e-6309 (by R.Szafron)
//-----------------------------------------------------------------------------
  static double DioWeightAl_LO(double P);
  static double DioWeightAl_LL(double P);

  static double DioWeightTi   (double P);

  static double RMC_ClosureAppxWeight (double K, double KMax);
  static double RPC_PhotonEnergyWeight(double E);
//-----------------------------------------------------------------------------
// print routines - sometimes it is not possible to do it from a single block
//-----------------------------------------------------------------------------
  // static int  PrintElectron(TStnElectron*       Ele,
  // 			    TStnElectronBlock*  ElectronBlock,
  // 			    TCalDataBlock*      CalDataBlock);
  ClassDef(TStntuple,0)
};

#endif

