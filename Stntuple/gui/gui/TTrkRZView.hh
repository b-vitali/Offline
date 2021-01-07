#ifndef TTrkRZView_hh
#define TTrkRZView_hh


#include "TMarker.h"
#include "TNamed.h"

class TTrkRZView: public TNamed {
protected:
  Int_t               fPx1;
  Int_t               fPy1;
  Int_t               fPx2;
  Int_t               fPy2;

  TMarker*            fCenter;

  int                 fCursorX;
  int                 fCursorY;

public:
  TTrkRZView();
  virtual ~TTrkRZView();
//-----------------------------------------------------------------------------
// overloaded functions of TObject
//-----------------------------------------------------------------------------
  virtual void  Paint              (Option_t* option = "");
  virtual void  ExecuteEvent       (Int_t event, Int_t px, Int_t py);
  virtual Int_t DistancetoPrimitive(Int_t px, Int_t py);

  virtual char* GetObjectInfo(Int_t Px, Int_t Py) const ;

  void    SetStations(int I1, int I2);   // *MENU* 
  void    SetTimeCluster(int I);            // *MENU* 

  ClassDef(TTrkRZView,0)
};

#endif
