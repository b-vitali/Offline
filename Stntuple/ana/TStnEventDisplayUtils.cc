//
// - Serves as the link between ROOT "events" (e.g. mouse-clicks) and the ART
//   event display service by providing a receiver slot for signals generated
//   by the ROOT events.  A ROOT dictionary needs to be generated for this.
//


#include "Stntuple/ana/StnNavState.hh"
#include "Stntuple/ana/TStnEventDisplayUtils.hh"
#include <string>

ClassImp(TStnEventDisplayUtils)

//-----------------------------------------------------------------------------
TStnEventDisplayUtils::TStnEventDisplayUtils(): TObject() {
  fTbRun = 0;
  fTbEvt = 0;
}

//-----------------------------------------------------------------------------
TStnEventDisplayUtils::~TStnEventDisplayUtils() {
}

void TStnEventDisplayUtils::PrevEvent(){
  StnNavState::Set(kPREV_EVENT);
}

void TStnEventDisplayUtils::NextEvent(){
  StnNavState::Set(kNEXT_EVENT);
}

void TStnEventDisplayUtils::GotoEvent(){
  int run   = std::stoi(fTbRun->GetString());
  int event = std::stoi(fTbEvt->GetString());
  
  StnNavState::SetTarget(run, event);
  StnNavState::Set(kGOTO_EVENT);
}
