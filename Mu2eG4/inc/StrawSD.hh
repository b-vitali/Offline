#ifndef StrawSD_h
#define StrawSD_h 1
//
// Define a sensitive detector for Straws.
// ( Not sure yet if I can use this for both LTracker and TTracker?)
// 
// $Id: StrawSD.hh,v 1.3 2010/04/06 23:07:07 kutschke Exp $
// $Author: kutschke $ 
// $Date: 2010/04/06 23:07:07 $
//
// Original author Rob Kutschke
//

// Mu2e includes
#include "Mu2eG4/inc/StepPointG4.hh"
#include "Mu2eG4/inc/EventNumberList.hh"

// G4 includes
#include "G4VSensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;

namespace mu2e {

  // Forward declarations in mu2e namespace
  class SimpleConfig;

  class StrawSD : public G4VSensitiveDetector{

  public:
    StrawSD(G4String, const SimpleConfig& config);
    ~StrawSD();
    
    void Initialize(G4HCofThisEvent*);
    G4bool ProcessHits(G4Step*, G4TouchableHistory*);
    void EndOfEvent(G4HCofThisEvent*);
  
  private:
    StepPointG4Collection* _collection;

    // List of events for which to enable debug printout.
    EventNumberList _debugList;

    
  };

} // namespace mu2e

#endif
