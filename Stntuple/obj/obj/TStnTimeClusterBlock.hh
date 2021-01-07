#ifndef STNTUPLE_TStnTimeClusterBlock
#define STNTUPLE_TStnTimeClusterBlock
//-----------------------------------------------------------------------------
//  definition of the cluster block for MU2E analysis
//  Author:    G. Pezzullo
//  Date:      April 11 2016
//-----------------------------------------------------------------------------
#include "TClonesArray.h"

#include "Stntuple/obj/TStnDataBlock.hh"
#include "Stntuple/obj/TStnTimeCluster.hh"
#include "TBuffer.h"

class TStnTimeClusterBlock: public TStnDataBlock {

  friend Int_t StntupleInitMu2eTimeClusterBlock     (TStnDataBlock*, AbsEvent* , int);
  friend Int_t StntupleInitMu2eTimeClusterBlockLinks(TStnDataBlock*, AbsEvent* , int);
public:
//----------------------------------------------------------------------------
//  data members
//-----------------------------------------------------------------------------
  Int_t          fNTimeClusters;
  TClonesArray*  fListOfTimeClusters;
//----------------------------------------------------------------------------
//  functions
//----------------------------------------------------------------------------
public:
					// ****** constructors and destructor
  TStnTimeClusterBlock();
  virtual ~TStnTimeClusterBlock();

  TStnTimeCluster* NewTimeCluster() {
    TStnTimeCluster* trackSeed = new ((*fListOfTimeClusters)[fNTimeClusters]) TStnTimeCluster();
    fNTimeClusters++;
    return trackSeed;
  }
//-----------------------------------------------------------------------------
// accessors
//-----------------------------------------------------------------------------
  Int_t              NTimeClusters     () { return fNTimeClusters;   }
  TClonesArray*      ListOfTimeClusters() { return fListOfTimeClusters; }

  TStnTimeCluster*   TimeCluster(int I) {
    return (TStnTimeCluster*) fListOfTimeClusters->UncheckedAt(I); 
  }

//-----------------------------------------------------------------------------
// overloaded functions of TObject
//-----------------------------------------------------------------------------
  void Clear(Option_t* opt="");
  void Print(Option_t* opt="") const;

  ClassDef(TStnTimeClusterBlock,1)
};

#endif
