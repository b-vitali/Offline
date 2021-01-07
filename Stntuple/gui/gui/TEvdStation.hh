///////////////////////////////////////////////////////////////////////////////
// vis node displays one wedge
///////////////////////////////////////////////////////////////////////////////
#ifndef TEvdStation_hh
#define TEvdStation_hh

#include "Gtypes.h"
#include "TClonesArray.h"
#include "TH1.h"
#include "TPad.h"
#include "TArc.h"

#ifndef __CINT__
#include "TrackerGeom/inc/Tracker.hh"
#else
namespace mu2e {
  class   Tracker;
};
#endif

class TEvdStation;
class TEvdPlane;

class TEvdStation: public TObject {
public:
  
protected:
  int                   fID;
  int                   fNPlanes;
  TObjArray*            fListOfPlanes;

  //  const mu2e::Station*  fStation;  // backward pointer

public:
//-----------------------------------------------------------------------------
// constructors and destructor
//-----------------------------------------------------------------------------
  TEvdStation();
  TEvdStation(int ID, const mu2e::Tracker* Tracker); 

  virtual ~TEvdStation();
//-----------------------------------------------------------------------------
// accessors
//-----------------------------------------------------------------------------
  int          NPlanes     () { return fNPlanes;      }
  TObjArray*   ListOfPlanes() { return fListOfPlanes; }

  TEvdPlane* Plane  (int I) { 
    return (TEvdPlane*) fListOfPlanes->UncheckedAt(I); 
  }
//-----------------------------------------------------------------------------
// modifiers
//-----------------------------------------------------------------------------

  //  virtual void  Draw    (Option_t* option = "");

  virtual void  Paint   (Option_t* option = "");
          void  PaintXY (Option_t* option = "");
          void  PaintRZ (Option_t* option = "");

  //  virtual void  ExecuteEvent(Int_t event, Int_t px, Int_t py);

  virtual Int_t DistancetoPrimitive  (Int_t px, Int_t py);
  virtual Int_t DistancetoPrimitiveXY(Int_t px, Int_t py);
  virtual Int_t DistancetoPrimitiveRZ(Int_t px, Int_t py);

  //  virtual void   Print(const char* Opt = "") const ; // **MENU**

  ClassDef(TEvdStation,0)
};


#endif
