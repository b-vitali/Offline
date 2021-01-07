///////////////////////////////////////////////////////////////////////////////
// Filter events passing only the ones from the provided event list
//
// event list contains 3 numbers per event : run, subrun, event
// if the event list is empty, all events are passed through
// set _printFreq to get the diagnostics printout


#include "MCDataProducts/inc/GenParticleCollection.hh"
#include "MCDataProducts/inc/SimParticleCollection.hh"

#include "art/Framework/Core/EDFilter.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Provenance.h"

#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/ParameterSetRegistry.h"

#include <iostream>

using namespace std;

namespace mu2e {

  class EventFilter : public art::EDFilter {
  protected: 
    vector<int>       _eventList;  // 3 numbers per event: [run, subrun, event]
    int               _printFreq;

    int               _nevents;
    int               _npassed;
  public:
    explicit EventFilter(fhicl::ParameterSet const& pset);

    virtual bool filter(art::Event& anEvent) override;

  private:

    void printProvenance( art::Provenance const& );

  };

  EventFilter::EventFilter(fhicl::ParameterSet const& pset)
    : art::EDFilter(pset),
      _eventList(pset.get<vector<int>>("eventList")),
      _printFreq(pset.get<int>        ("printFreq"))
  {
    _nevents = _eventList.size()/3;
    _npassed = 0;
  }


//-----------------------------------------------------------------------------
  bool EventFilter::filter(art::Event&  anEvent) {

    const char oname [] = "EventFilter::filter";

    uint   eventNumber, runNumber, subRunNumber, rn, srn, evn;
    int    found(1);

    rn  = anEvent.run();
    srn = anEvent.subRun();
    evn = anEvent.event();

    if (_nevents > 0) {
      found = 0;

      for (int i=0; i<_nevents; i++) {
	runNumber    = _eventList[3*i+0];
	subRunNumber = _eventList[3*i+1];
	eventNumber  = _eventList[3*i+2];
	
	if ((evn == eventNumber) && (rn == runNumber) && (srn == subRunNumber)) {
	  found = 1;
	  break;
	}
      }
    }

    if (found) _npassed += 1;

    if ((evn % _printFreq) == 0) {
      printf("[%s] : run, subrun event, found, n(passed): %8i %8i %8i %2i %10i\n",
	     oname,rn,srn,evn,found,_npassed);
    }


    return found;
  } 

} // end namespace mu2e

using mu2e::EventFilter;
DEFINE_ART_MODULE(EventFilter);
