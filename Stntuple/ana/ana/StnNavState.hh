//
// - Sets the navigation state to a value that the event display service
//   interprets as an action to perform when TApplication returns after
//   receiving a ROOT signal.  Based on Mark Messier's code for Nova's
//   event display service.
//

#ifndef __murat_ana_StnNavState_hh__
#define __murat_ana_StnNavState_hh__

enum nav_states_ {
  kNEXT_EVENT  ,
  kPREV_EVENT  ,
  kRELOAD_EVENT,
  kGOTO_EVENT
};


class StnNavState {
public:
  static int  Which      ();
  static void Set        (int which);
  static void SetTarget  (int run, int event);
  static int  TargetRun  ();
  static int  TargetEvent();
private:
  StnNavState() { }
};

#endif
