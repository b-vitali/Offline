///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#ifndef __STNTUPLE_InitSimpBlock__
#define __STNTUPLE_InitSimpBlock__

#include <string.h>

#include "canvas/Utilities/InputTag.h"

#include "Stntuple/obj/TStnInitDataBlock.hh"
#include "Stntuple/obj/TSimpBlock.hh"

class StntupleInitSimpBlock : public TStnInitDataBlock {
public:
  art::InputTag   fSimpCollTag;
  art::InputTag   fStrawHitsCollTag;
  art::InputTag   fStrawDigiMCCollTag;
  art::InputTag   fVDHitsCollTag;
  float           fMinSimpEnergy;
  int             fGenProcessID;
//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
public:

  void   SetSimpCollTag       (std::string& Tag ) { fSimpCollTag        = art::InputTag(Tag); }
  void   SetStrawHitsCollTag  (std::string& Tag ) { fStrawHitsCollTag   = art::InputTag(Tag); }
  void   SetStrawDigiMCCollTag(std::string& Tag ) { fStrawDigiMCCollTag = art::InputTag(Tag); }
  void   SetVDHitsCollTag     (std::string& Tag ) { fVDHitsCollTag      = art::InputTag(Tag); }
  void   SetMinSimpEnergy     (double       MinE) { fMinSimpEnergy      = MinE              ; }
  void   SetGenProcessID      (int          ID  ) { fGenProcessID       = ID                ; }

  virtual int InitDataBlock(TStnDataBlock* Block, AbsEvent* Evt, int Mode);
  //  virtual int ResolveLinks (TStnDataBlock* Block, AbsEvent* Evt, int Mode);

};

#endif
