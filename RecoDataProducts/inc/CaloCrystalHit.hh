#ifndef RecoDataProducts_CaloCrystalHit_hh
#define RecoDataProducts_CaloCrystalHit_hh

// $Id: CaloCrystalHit.hh,v 1.5 2014/05/14 18:14:21 murat Exp $
// $Author: murat $
// $Date: 2014/05/14 18:14:21 $
//
// Original author KLG

// C++ includes
#include <iostream>
#include <vector>

// Mu2e includes
#include "RecoDataProducts/inc/CaloHit.hh"

#include "RecoDataProducts/inc/RecoCaloDigi.hh"
#include "RecoDataProducts/inc/RecoCaloDigiCollection.hh"

#include "art/Persistency/Common/Ptr.h"

namespace mu2e {

  class CaloCrystalHit{

  public:

    typedef art::Ptr<CaloHit>      CaloHitPtr;
    typedef art::Ptr<RecoCaloDigi> RecoCaloDigiPtr;

    CaloCrystalHit():
      _crystalId(-1),
      _time(0.),
      _energyDep(0.),
      _energyDepTotal(0.),
      _numberOfROIdsUsed(0)
    {}
    
    CaloCrystalHit(const CaloCrystalHit &Other):
      _crystalId        (Other.id()),
      _time             (Other.time()),
      _energyDep        (Other.energyDep()),
      _energyDepTotal   (Other.energyDepTotal()),
      _numberOfROIdsUsed(Other.numberOfROIdsUsed()){
      
      //copy the CaloHits
      int     size = Other.readouts().size();
      
      for (int i=0; i<size; ++i){
	_readouts.push_back(Other.readouts().at(i));
      }

      //copy the recoCaloDigi
      size = Other.recoCaloDigis().size();
      
      for (int i=0; i<size; ++i){
	_recoCaloDigis.push_back(Other.recoCaloDigis().at(i));
      }

      
    }
    
    CaloCrystalHit(int  crystalId, CaloHit const & hit, CaloHitPtr const& chPtr );

    //constructor that uses the CaloDigi
    CaloCrystalHit(int  crystalId, std::vector<RecoCaloDigiPtr>  RecoCaloDigiCol);

    // Accessors

    int   id()                const { return _crystalId; }
    float time()              const { return _time;      }
    float energyDep()         const { return _energyDep; }
    float energyDepTotal()    const { return _energyDepTotal; }
    int   numberOfROIdsUsed() const { return _numberOfROIdsUsed; }

    std::vector<CaloHitPtr> const & readouts() const { return _readouts; }

    std::vector<RecoCaloDigiPtr> const & recoCaloDigis() const { return _recoCaloDigis;}

    // Accept compiler generated versions of d'tor, copy c'tor, assignment operator.

    // Print contents of the object.
    void print( std::ostream& ost = std::cout, bool doEndl = true ) const;

    // almost operator += CaloHit
    CaloCrystalHit& add( CaloHit const & hit, CaloHitPtr const& chPtr );

    void add(CaloCrystalHit* const hit);

    CaloCrystalHit& addEnergyToTot( CaloHit const & hit);

    // almost like one of the constructors, plays a role of a two
    // argument assignment operator
    void assign           (int crystalId, CaloHit const & hit, CaloHitPtr const& chPtr );

    void assignEnergyToTot(int crystalId, CaloHit const & hit);

    void setEnergyDep     (double energy);
    void setEnergyDepTotal(double energy);

    void setTime          (double Time);

  private:

    //   iv*ncrys + ic; // Crystal ID

    int              _crystalId;
    float            _time;               // (ns)
    float            _energyDep;          // (MeV)
    float            _energyDepTotal;     // (MeV)
    int              _numberOfROIdsUsed;  // roIds used to calculate _energyDep
    std::vector<CaloHitPtr>       _readouts;
    std::vector<RecoCaloDigiPtr>  _recoCaloDigis;

  };

  inline std::ostream& operator<<( std::ostream& ost,
                                   CaloCrystalHit const& hit){
    hit.print(ost,false);
    return ost;
  }

  inline std::ostream& operator<<( std::ostream& ost,
                                   CaloCrystalHit::CaloHitPtr const& hit){
    ost << "("
        << hit.id() << ","
        << hit.key() << ")";
    return ost;
  }

} // namespace mu2e

#endif /* RecoDataProducts_CaloCrystalHit_hh */
