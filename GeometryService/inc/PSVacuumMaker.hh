#ifndef ProductionSolenoidGeom_PSVacuumMaker_hh
#define ProductionSolenoidGeom_PSVacuumMaker_hh
//
// Class to construct and return PSVacuum
//
// $Id: PSVacuumMaker.hh,v 1.2 2013/03/15 15:52:05 kutschke Exp $
// $Author: kutschke $
// $Date: 2013/03/15 15:52:05 $
//
// Original author Andrei Gaponenko
//

#include <string>
#include <memory>

namespace CLHEP { class Hep3Vector; }

namespace mu2e {

  class PSVacuum;
  class SimpleConfig;

  class ProductionSolenoid;
  class PSEnclosure;

  class PSVacuumMaker {
  public:

    static std::unique_ptr<PSVacuum>  make(const SimpleConfig& config,
                                         const ProductionSolenoid& ps,
                                         const PSEnclosure& pse,
                                         double zmax /*interface to TS*/
                                         );
  };

}  //namespace mu2e

#endif /* ProductionSolenoidGeom_PSVacuumMaker_hh */
