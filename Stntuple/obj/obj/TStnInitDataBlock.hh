#ifndef STNTUPLE_TStnInitDataBlock
#define STNTUPLE_TStnInitDataBlock
//-----------------------------------------------------------------------------
//  base class for initialization of STNTUPLE data block
//  Author:    Pasha Murat (Mu2e/FNAL)
//  Date:      Feb 10 2019
//-----------------------------------------------------------------------------
#include "Stntuple/obj/AbsEvent.hh"

// class TStnEvent;
class TStnDataBlock;

class TStnInitDataBlock {
public:
//-----------------------------------------------------------------------------
//  constructors and destructor
//-----------------------------------------------------------------------------
  TStnInitDataBlock();
  virtual ~TStnInitDataBlock();
//-----------------------------------------------------------------------------
//  data block initialization functions
//-----------------------------------------------------------------------------
  virtual int InitDataBlock(TStnDataBlock*, AbsEvent*, int Mode); 
  virtual int ResolveLinks (TStnDataBlock*, AbsEvent*, int Mode); 
};
#endif
