//
// An EDAnalyzer module that reads the Trigger Info 
//
// Original author G. Pezzullo
//

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "art_root_io/TFileService.h"
#include "art_root_io/TFileDirectory.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Selector.h"
#include "art/Framework/Principal/Provenance.h"
#include "canvas/Persistency/Common/TriggerResults.h"
#include "art/Framework/Services/System/TriggerNamesService.h"
#include "cetlib_except/exception.h"
#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/ParameterSetRegistry.h"

#include "messagefacility/MessageLogger/MessageLogger.h"
// #include "canvas/Utilities/InputTag.h"
#include "BFieldGeom/inc/BFieldManager.hh"
#include "GeometryService/inc/GeomHandle.hh"
#include "GeometryService/inc/DetectorSystem.hh"
#include "TrackerGeom/inc/Tracker.hh"

//Conditions
#include "ConditionsService/inc/AcceleratorParams.hh"
#include "ConditionsService/inc/ConditionsHandle.hh"

//Dataproducts
#include "RecoDataProducts/inc/CaloCluster.hh"
#include "RecoDataProducts/inc/CaloTrigSeed.hh"
#include "RecoDataProducts/inc/HelixSeed.hh"
#include "RecoDataProducts/inc/KalSeed.hh"
#include "RecoDataProducts/inc/TrkQual.hh"
#include "RecoDataProducts/inc/TriggerInfo.hh"
#include "RecoDataProducts/inc/ComboHit.hh"
#include "RecoDataProducts/inc/StrawDigi.hh"
#include "RecoDataProducts/inc/StrawDigiCollection.hh"
#include "RecoDataProducts/inc/CaloDigi.hh"
#include "DataProducts/inc/XYZVec.hh"

#include "RecoDataProducts/inc/LumiInfo.hh"                   //bvitali


//MC dataproducts
#include "MCDataProducts/inc/SimParticle.hh"
#include "MCDataProducts/inc/SimParticleCollection.hh"
#include "MCDataProducts/inc/StrawDigiMC.hh"
#include "MCDataProducts/inc/StrawDigiMCCollection.hh"
#include "MCDataProducts/inc/StepPointMC.hh"
#include "MCDataProducts/inc/StepPointMCCollection.hh"
#include "MCDataProducts/inc/ProtonBunchIntensity.hh"

#include "GlobalConstantsService/inc/GlobalConstantsHandle.hh"
#include "GlobalConstantsService/inc/ParticleDataTable.hh"

//Utilities
#include "Mu2eUtilities/inc/TriggerResultsNavigator.hh"
#include "Mu2eUtilities/inc/HelixTool.hh"

//ROOT
#include "TH1F.h"
#include "TH2F.h"

#include <cmath>
// #include <iostream>
#include <string>
// #include <map>
#include <vector>


namespace mu2e {


  class ReadLumiInfo : public art::EDAnalyzer {

  public:

    struct  summaryInfoHist_  {
      TH1F *_hETot  ;
      TH2F *_hETotVsNPOT;

      TH2F *_hPTCVsNPOT;  //bvitali protonTimeClusters
      TH2F *_hPTCVsNPOT_2;  //bvitali protonTimeClusters

      
      summaryInfoHist_() {
	_hETot        = NULL;
	_hETotVsNPOT  = NULL;

	_hPTCVsNPOT  = NULL;
	_hPTCVsNPOT_2  = NULL;

      }
    };
      
    explicit ReadLumiInfo(fhicl::ParameterSet const& pset);
    virtual ~ReadLumiInfo() { }

    virtual void beginJob();
    virtual void endJob();
    virtual void endSubRun(const art::SubRun& sr);

    // This is called for each event.
    virtual void analyze(const art::Event& e);
    virtual void beginRun(const art::Run & run);

    void     bookHistograms  ();
    void     bookTrigInfoHist(art::ServiceHandle<art::TFileService> & Tfs, summaryInfoHist_  &Hist);

    void     fillCaloLumiInfo();
  private:

    int                       _diagLevel;
    art::InputTag             _sdTag;
    art::InputTag             _cdTag;
    art::InputTag             _clTag;
    art::InputTag             _lumiTag;             //bvitali
    art::InputTag             _evtWeightTag;


    summaryInfoHist_          _sumHist;

    const mu2e::Tracker*      _tracker;
    double                    _nPOT;
    
    //the following pointer is needed to navigate the MC truth info of the strawHits
    const mu2e::StrawDigiCollection*   _sdCol;
    const mu2e::CaloDigiCollection*    _cdCol;
    const mu2e::CaloClusterCollection* _clCol;
    const mu2e::LumiInfo*               _lumi;      //bvitali
    const art::Event*                  _event;


  };

  //--------------------------------------------------------------------------------
  ReadLumiInfo::ReadLumiInfo(fhicl::ParameterSet const& pset) :
    art::EDAnalyzer(pset), 
    _diagLevel     (pset.get<int>          ("diagLevel", 0)),
    _sdTag         (pset.get<art::InputTag>("strawDigiCollection"  , "makeSD")),
    _cdTag         (pset.get<art::InputTag>("caloDigiCollection"   , "CaloDigiFromShower")),
    _clTag         (pset.get<art::InputTag>("caloClusterCollection", "CaloClusterFast")),
    _lumiTag       (pset.get<art::InputTag>("lumi","TimeClusterFinderDpP")),      //bvitali
    _evtWeightTag  (pset.get<art::InputTag>("protonBunchIntensity" , "protonBunchIntensity")){}
  

  
  //--------------------------------------------------------------------------------
  void     ReadLumiInfo::bookHistograms           (){
    art::ServiceHandle<art::TFileService> tfs;
    
    bookTrigInfoHist(tfs, _sumHist);
  }

  //--------------------------------------------------------------------------------//
  void     ReadLumiInfo::bookTrigInfoHist         (art::ServiceHandle<art::TFileService> & Tfs, summaryInfoHist_       &Hist){
    art::TFileDirectory trigInfoDir = Tfs->mkdir("trigInfo");
    int    nLumiBins(20), nETotBins(500);
    float  minNPOT(1e6), maxNPOT(1.2e8);
    float  minETot(0), maxETot(5e3);
    Hist._hETot         = trigInfoDir.make<TH1F>("hETot"        , "Total caloriemter energy; E_{tot} [MeV]", nETotBins, minETot, maxETot);
    Hist._hETotVsNPOT   = trigInfoDir.make<TH2F>("hETotVsNPOT"  , "E_{tot} vs nPOT; nPOT; E_{tot} [MeV]"   , nLumiBins, minNPOT, maxNPOT, nETotBins, minETot, maxETot);

    Hist._hPTCVsNPOT   = trigInfoDir.make<TH2F>("hPTCVsNPOT"  , "N Proton TimeClusters vs nPOT; nPOT; nPTC"   , nLumiBins, minNPOT, maxNPOT, 200, 0, 200); //bvitali 200 is a random guess
    Hist._hPTCVsNPOT_2   = trigInfoDir.make<TH2F>("hPTCVsNPOT_2"  , "N Proton TimeClusters vs nPOT 2; nPOT; nPTC"   , nLumiBins, minNPOT, maxNPOT, 200, 0, 200);
    
  }

  //--------------------------------------------------------------------------------//
  void ReadLumiInfo::beginJob(){
    bookHistograms();
  }

  //--------------------------------------------------------------------------------//
  void ReadLumiInfo::endJob(){}
  
  //================================================================
  void   ReadLumiInfo::beginRun(const art::Run & run){
    mu2e::GeomHandle<mu2e::Tracker> th;
    _tracker  = th.get();
  }

  //--------------------------------------------------------------------------------//
  void ReadLumiInfo::endSubRun(const art::SubRun& sr){}

  //--------------------------------------------------------------------------------
  void ReadLumiInfo::analyze(const art::Event& event) {

    //get the number of POT
    _nPOT  = -1.;
    art::Handle<ProtonBunchIntensity> evtWeightH;
    event.getByLabel(_evtWeightTag, evtWeightH);
    if (evtWeightH.isValid()){
      _nPOT  = (double)evtWeightH->intensity();
    }

    art::Handle<mu2e::CaloClusterCollection>  clH;
    event.getByLabel(_clTag, clH);
    if (clH.isValid()){
      _clCol = clH.product();
    }else{
      _clCol = NULL;
    }

    //get the StrawDigi Collection
    art::Handle<mu2e::StrawDigiCollection> sdH;
    event.getByLabel(_sdTag, sdH);
    if (sdH.isValid()) {
      _sdCol = sdH.product();
    }else {
      _sdCol = NULL;
    }

    //get the CaloDigi Collection
    art::Handle<mu2e::CaloDigiCollection> cdH;
    event.getByLabel(_cdTag, cdH);
    if (cdH.isValid()) {
      _cdCol = cdH.product();
    }else {
      _cdCol = NULL;
    }

    //get the Number of ProtonTimeClusters   //bvitali
    
    art::Handle<mu2e::LumiInfo> lumi;
    event.getByLabel(_lumiTag, lumi);
    //event.getByType(lumi);
    if (lumi.isValid()) {
      _lumi = lumi.product();
    }else {
      _lumi = NULL;
    }
    

    //now fill the lumi info
    fillCaloLumiInfo();

  }
  

  void   ReadLumiInfo::fillCaloLumiInfo(){
    double eTot(0);
    if (_clCol != NULL){
      if (_clCol->size() > 0) eTot = _clCol->at(0).energyDep();
    }
    _sumHist._hETot      ->Fill(eTot);
    _sumHist._hETotVsNPOT->Fill(_nPOT, eTot);

    
    if (_lumi != NULL){
      _sumHist._hPTCVsNPOT ->Fill(_nPOT, _lumi->_lumi[0]);   //bvitali 
      _sumHist._hPTCVsNPOT_2 ->Fill(_nPOT, _lumi->_lumi[1]);   //bvitali 
      //_sumHist._hPTCVsNPOT ->Fill(_nPOT, _lumi->_lumi1);   //bvitali 
      //_sumHist._hPTCVsNPOT_2 ->Fill(_nPOT, _lumi->_lumi2);   //bvitali
    }
  }
  
}  

DEFINE_ART_MODULE(mu2e::ReadLumiInfo);
