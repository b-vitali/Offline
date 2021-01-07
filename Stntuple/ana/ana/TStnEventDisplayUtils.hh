//
// - Serves as the link between ROOT "events" (e.g. mouse-clicks) and the ART
//   event display service by providing a receiver slot for signals generated
//   by the ROOT events.  A ROOT dictionary needs to be generated for this.
//

#ifndef __murat_ana_TStnEventDisplayUtils_hh__
#define __murat_ana_TStnEventDisplayUtils_hh__

#include <TObject.h>
#include <TApplication.h>
#include <TGTextBuffer.h>
#include <iostream>

class TStnEventDisplayUtils: public TObject {
public:

  TGTextBuffer *fTbRun;
  TGTextBuffer *fTbEvt;

  TStnEventDisplayUtils();
  ~TStnEventDisplayUtils();
  
  void     PrevEvent         ();
  void     NextEvent         ();
  void     GotoEvent         ();

  ClassDef(TStnEventDisplayUtils,0)
};

#endif 
