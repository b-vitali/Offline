//
// - Sets the navigation state to a value that the event display service
//   interprets as an action to perform when TApplication returns after 
//   receiving a ROOT signal.  Based on Mark Messier's code for Nova's
//   event display service.
//

#include "Stntuple/ana/StnNavState.hh"
#include "TROOT.h"
#include "TApplication.h"

static int gsStnNavState = 0;
static int gsTargetRun   = 0;
static int gsTargetEvent = 0;
//......................................................................

int StnNavState::Which() { return gsStnNavState; }

//......................................................................

void StnNavState::Set(int which) {
  gsStnNavState = which;
  gROOT->GetApplication()->Terminate();
}

//......................................................................

void StnNavState::SetTarget(int run, int event) {
  gsTargetRun = run;
  gsTargetEvent = event;
}

//......................................................................

int StnNavState::TargetRun() {
  return gsTargetRun;
}

//......................................................................

int StnNavState::TargetEvent() {
  return gsTargetEvent;
}
