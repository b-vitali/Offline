#ifndef __StntupleInitGenpBlock_hh__
#define __StntupleInitGenpBlock_hh__

#include "TObject.h"

#include "Stntuple/obj/TStnDataBlock.hh"

//-----------------------------------------------------------------------------
class StntupleInitDataBlock: public TObject {
  virtual int InitBlock(TStnDataBlock* block, AbsEvent* AnEvent, int Mode);
  virtual int InitLinks(TStnDataBlock* block, AbsEvent* AnEvent, int Mode);
}



class StntupleInitGenpBlock: public StntupleInitDataBlock {
//-----------------------------------------------------------------------------
// data members - parameters
//-----------------------------------------------------------------------------
  int      fGenID;               // signal process Generator id
  TString  fGenpCollTag;         // collection tag

//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
  StntupleInitGenpBlock();
  ~StntupleInitGenpBlock();


  InitBlock(TStnDataBlock* block, AbsEvent* AnEvent, int Mode);
  
}
#endif
