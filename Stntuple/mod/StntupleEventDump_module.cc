///////////////////////////////////////////////////////////////////////////////
// A half-interactive 2D event display. 
//
// $Id: MuHitDisplay_module.cc,v 1.6 2014/09/20 17:54:06 murat Exp $
// $Author: murat $
// $Date: 2014/09/20 17:54:06 $
//
// Contact person:  Pavel Murat, Gianantonio Pezzulo
//
// What this event display shows: 2D view of the straw hits, tracks, and calorimeter clusters
//
// Straw hit display mode: 
// -----------------------
// displayBackgroundHits : false - respect hit flags set by FlagStrawHits_module (default)
//                         the timing hit flag check is commented out - I didn't have a MC file
//                         in hands to check
//                       : true  - display all hits
// useStereoHits         : true : displayed hit position errors are defined by the StrawHitPositionCollection
//                       : false: semi-random assignment (sigr=5mm, sigp = strawTimeDivisionErr/2.)
//
// small black triangles: MC truth on the trajectory
//
// red   hits: hits on a track reconstructed as a downstream-moving conversion electron within the time window
//             size of the timing window can be redefined via  talk-to (input .fcl file)
// blue  hits: hits of the conversion electron track, which fall outside the time window
// black hits: hits produced by anything, but the conversion electron
//
// a hit is displayed by a cross with the radial error bar of 5mm and the resolution 
// along the straw of sigma(time division)/2, to better guide the eye
//
// green circles - contour of the disk-based calorimeter
// clusters on the 1st disk are shown in red, on the second disk - in pink
// 
// there are few other features which we need to document
//
// .fcl file to use: Analyses/test/hitDisplay.fcl
///////////////////////////////////////////////////////////////////////////////

// C++ includes.
#include <map>
#include <iostream>
#include <string>

// Framework includes.
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art_root_io/TFileService.h"
#include "art/Framework/Principal/Selector.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

// Mu2e includes.

// ROOT includes
#include "TApplication.h"
#include "TDirectory.h"

#include "Stntuple/mod/TAnaDump.hh"
#include "Stntuple/mod/TModule.hh"

using namespace std;
using CLHEP::Hep3Vector;

namespace mu2e {

  class StntupleEventDump : public TModule {
  private:
//-----------------------------------------------------------------------------
// Input parameters: Module labels 
//-----------------------------------------------------------------------------
    std::string        _moduleLabel;	             // this module label
    std::string        _processName;

    std::string        _producerName;
//-----------------------------------------------------------------------------
// end of input parameters
// Options to control the display
// hit flag bits which should be ON and OFF
//-----------------------------------------------------------------------------
    TApplication*                               fApplication;

  public:

    explicit StntupleEventDump(fhicl::ParameterSet const& pset);
    virtual ~StntupleEventDump();

    int      getData(const art::Event* Evt);
    void     Init   (art::Event* Evt);
//-----------------------------------------------------------------------------
// overloaded virtual methods of the base class
//-----------------------------------------------------------------------------
    virtual void     beginJob();
    virtual bool     beginRun(art::Run& aRun);
    virtual bool     filter(art::Event& Evt);
  };


//-----------------------------------------------------------------------------
  StntupleEventDump::StntupleEventDump(fhicl::ParameterSet const& pset) :
    TModule(pset, "StntupleEventDump"),
    _moduleLabel(pset.get<std::string>("module_label")),
    _processName(pset.get<string>("processName", ""))
  {
    fApplication = 0;
  }

//-----------------------------------------------------------------------------
  StntupleEventDump::~StntupleEventDump() {
    if (fApplication) delete fApplication;
  }


//-----------------------------------------------------------------------------
  void StntupleEventDump::beginJob() {
    //     const char oname[] = "StntupleEventDump::beginJob";
    int    tmp_argc(0);
    char** tmp_argv(0);

    if (!gApplication) {
      fApplication = new TApplication("StntupleEventDump_module", &tmp_argc, tmp_argv);
    }
  }

//-----------------------------------------------------------------------------
  bool StntupleEventDump::beginRun(art::Run& Run) {
    return true;
  }

//-----------------------------------------------------------------------------
// get data from the event record
//-----------------------------------------------------------------------------
  int StntupleEventDump::getData(const art::Event* Evt) {
    //    const char* oname = "StntupleEventDump::getData";
    return 0;
  }

//-----------------------------------------------------------------------------
  bool StntupleEventDump::filter(art::Event& Evt) {
    //    const char* oname = "StntupleEventDump::filter";
//-----------------------------------------------------------------------------
// go into interactive mode, till '.q' is pressed
//-----------------------------------------------------------------------------
    TModule::filter(Evt);
    return true;
  } 
}

using mu2e::StntupleEventDump;
DEFINE_ART_MODULE(StntupleEventDump);
