// Printout ExtMonFNAL raw hits
//
// Andrei Gaponenko, 2012

#include <iostream>

#include "cetlib/exception.h"
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Run.h"

#include "RecoDataProducts/inc/ExtMonFNALRawHit.hh"
#include "RecoDataProducts/inc/ExtMonFNALRawHitCollection.hh"

#include "GeometryService/inc/GeomHandle.hh"
#include "ExtinctionMonitorFNAL/Geometry/inc/ExtMonFNAL.hh"
#include "ExtinctionMonitorFNAL/Geometry/inc/ExtMonFNALSensor.hh"
#include "ExtinctionMonitorFNAL/Geometry/inc/ExtMonFNALPixelChip.hh"

#include "art/Framework/Core/ModuleMacros.h"

namespace mu2e {

  class EMFRawHitsValidator : public art::EDAnalyzer {
    std::string inModuleLabel_;
    std::string inInstanceName_;
    std::string geomModuleLabel_;
    std::string geomInstanceName_;
    const ExtMonFNAL::ExtMon *extmon_;
    unsigned int numSeenHits_;
    unsigned int numPassedHits_;
  public:
    explicit EMFRawHitsValidator(const fhicl::ParameterSet& pset)
      : inModuleLabel_(pset.get<std::string>("inputModuleLabel"))
      , inInstanceName_(pset.get<std::string>("inputInstanceName"))
      , geomModuleLabel_(pset.get<std::string>("geomModuleLabel"))
      , geomInstanceName_(pset.get<std::string>("geomInstanceName"))
      , numSeenHits_(0)
      , numPassedHits_(0)
    {}

    virtual void beginRun(const art::Run& run);
    virtual void analyze(const art::Event& event);
    virtual void endJob();
  };

  //================================================================
  void EMFRawHitsValidator::beginRun(const art::Run& run) {
    try {
      art::Handle<ExtMonFNAL::ExtMon> extmonh;
      run.getByLabel(geomModuleLabel_, geomInstanceName_, extmonh);
      extmon_ = &*extmonh;
      std::cout<<"EMFRawHitsValidator::beginRun(): got ExtMonFNAL: detectorCenterInMu2e = "<<extmon_->detectorCenterInMu2e()<<std::endl;
    }
    catch(cet::exception& e) {
      std::cerr<<"WARNING: EMFRawHitsValidator: could not retrieve ExtMonFNAL::ExtMon from Run, falling back to GeometryService"<<std::endl;
      extmon_ = &*GeomHandle<ExtMonFNAL::ExtMon>();
      std::cout<<"EMFRawHitsValidator::beginRun(): from GeometryService: ExtMonFNAL detectorCenterInMu2e = "<<extmon_->detectorCenterInMu2e()<<std::endl;
    }
  }

  //================================================================
  void EMFRawHitsValidator::analyze(const art::Event& event) {

    art::Handle<ExtMonFNALRawHitCollection> ih;
    event.getByLabel(inModuleLabel_, inInstanceName_, ih);

    const ExtMonFNALRawHitCollection& inputs(*ih);

    for(ExtMonFNALRawHitCollection::const_iterator i=inputs.begin(); i!=inputs.end(); ++i) {
      ++numSeenHits_;

      if(i->pixelId().chip().sensor().plane() >= extmon_->up().nplanes() + extmon_->dn().nplanes()) {
        throw cet::exception("BUG")<<*i<<": invalid plane number\n";
      }

      if(i->pixelId().chip().chipCol() >= extmon_->sensor().nxChips()) {
        throw cet::exception("BUG")<<*i<<": invalid chipCol\n";
      }

      if(i->pixelId().chip().chipRow() >= extmon_->sensor().nyChips()) {
        throw cet::exception("BUG")<<*i<<": invalid chipRow\n";
      }

      if(i->pixelId().col() >= extmon_->chip().nColumns()) {
        throw cet::exception("BUG")<<*i<<": invalid col\n";
      }

      if(i->pixelId().row() >= extmon_->chip().nRows()) {
        throw cet::exception("BUG")<<*i<<": invalid row\n";
      }

      ++numPassedHits_;
    }

  }

  //================================================================
  void EMFRawHitsValidator::endJob() {
    std::cout<<"EMFRawHitsValidator: found "<<(numSeenHits_ - numPassedHits_)
             <<" problem having checked "<<numSeenHits_<<" hits"
             <<std::endl;
  }

  //================================================================
} // namespace mu2e

DEFINE_ART_MODULE(mu2e::EMFRawHitsValidator);
