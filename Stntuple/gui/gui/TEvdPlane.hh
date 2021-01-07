///////////////////////////////////////////////////////////////////////////////
// vis node displays one wedge
///////////////////////////////////////////////////////////////////////////////
#ifndef TEvdPlane_hh
#define TEvdPlane_hh

#include "Gtypes.h"
#include "TClonesArray.h"
#include "TH1.h"
#include "TPad.h"
#include "TArc.h"

namespace mu2e {
  class Plane;
};

class TEvdStation;
class TEvdPanel;

class TEvdPlane: public TObject {
public:
  
protected:
  int                 fID;
  int                 fNPanels;
  TObjArray*          fListOfPanels;

  TEvdStation*        fStation; 		// backward pointers
  const mu2e::Plane*  fPlane;

public:
//-----------------------------------------------------------------------------
// constructors and destructor
//-----------------------------------------------------------------------------
  TEvdPlane();
  TEvdPlane(int Number, const mu2e::Plane* Sector, TEvdStation* Station); 

  virtual ~TEvdPlane();
//-----------------------------------------------------------------------------
// accessors
//-----------------------------------------------------------------------------
  int        NPanels    () { return fNPanels;  }
  TEvdPanel* Panel (int I) { return (TEvdPanel*) fListOfPanels->UncheckedAt(I); }
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

  ClassDef(TEvdPlane,0)
};


#endif
