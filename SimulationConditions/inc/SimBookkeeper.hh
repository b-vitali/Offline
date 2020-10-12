#ifndef SimulationConditions_SimBookkeeper_hh
#define SimulationConditions_SimBookkeeper_hh

//
// SimBookkeeper is the ProditionsEntry for
//

// Mu2e includes
#include "Mu2eInterfaces/inc/ProditionsEntity.hh"
#include "fhiclcpp/ParameterSet.h"

namespace mu2e {

  class SimBookkeeper : public ProditionsEntity {

  public:
    SimBookkeeper() : _name("SimBookkeeper") {}
    // accessors
    std::string const& name() const { return _name; }
    double const getEff(const std::string& name) const {
      for (const auto& i_eff : _effs) {
        if (i_eff.first == name) {
          return i_eff.second;
        }
      }
      return -1;
    }

    // setters
    void addEff(std::string name, double new_val) {
      _effs[name] = new_val;
    }

    const std::string print() const {
      std::stringstream out;
      print(out);
      return out.str();
    }

    void print(std::ostream& os) const {
      os << "Efficiencies in " << _name << ":" << std::endl;
      for (const auto& i_eff : _effs) {
        os << i_eff.first << " = " << i_eff.second << std::endl;
      }
      os << std::endl;
    }

    // typedefs
    typedef std::shared_ptr<SimBookkeeper> ptr_t;
    typedef std::shared_ptr<const SimBookkeeper> cptr_t;

  private:
    // data
    std::string _name;
    std::map<std::string, double> _effs;
  };
}

#endif
