#ifndef TrackerGeom_Panel_hh
#define TrackerGeom_Panel_hh
//
// Holds information about one panel in a tracker.
//

//
// $Id: Panel.hh,v 1.14 2013/03/26 23:28:23 kutschke Exp $
// $Author: kutschke $
// $Date: 2013/03/26 23:28:23 $
//
// Original author Rob Kutschke
//

#include <algorithm>
#include <vector>

#include "TrackerGeom/inc/Layer.hh"
#include "DataProducts/inc/PanelId.hh"

#include "CLHEP/Vector/ThreeVector.h"
#ifndef __CINT__
#include "boost/bind.hpp"
#endif


namespace mu2e {

  class Tracker;

  class Panel{


    friend class Device;
    friend class TTracker;
    friend class TTrackerMaker;

  public:

    Panel():_id(PanelId(-1,-1)){}
    Panel( const PanelId& id ):_id(id){}

    // Accept the compiler generated destructor, copy constructor and assignment operators

    const PanelId& id() const { return _id;}

    const std::vector<Layer>& getLayers() const{
      return _layers;
    }

    int nLayers() const{
      return _layers.size();
    }

    const Layer& getLayer ( int n ) const {
      return _layers.at(n);
    }

    const Layer& getLayer ( const LayerId& lid) const {
      return _layers.at(lid.getLayer());
    }

    const Straw& getStraw ( const StrawId& sid ) const{
      return _layers.at(sid.getLayer()).getStraw(sid);
    }

    // Mid-point position of the average (over the layers) of the primary 
    // straws, and (collective) straw direction.
    // (The primary straw of each layer is the straw used to establish position. 
    //  In the TTracker the primary straw is the innermost straw.) 
    // *** In a multi-layer geometry, the straw0MidPoint *** 
    // ***        need not lie on any actaul straw       ***
    CLHEP::Hep3Vector straw0MidPoint()  const { return _straw0MidPoint;  }
    CLHEP::Hep3Vector straw0Direction() const { return _straw0Direction; }

    // Formatted string embedding the id of the panel.
    std::string name( std::string const& base ) const;

    const std::vector<double>& boxHalfLengths() const { return _boxHalfLengths; }

    double         boxRxAngle()     const { return _boxRxAngle;     }
    double         boxRyAngle()     const { return _boxRyAngle;     }
    double         boxRzAngle()     const { return _boxRzAngle;     }
    const CLHEP::Hep3Vector&    boxOffset()      const { return _boxOffset;      }

    std::vector<CLHEP::Hep3Vector> const& getBasePosition() const{
      return _basePosition;
    }

    CLHEP::Hep3Vector const& getBaseDelta() const{
      return _baseDelta;
    }

    // On readback from persistency, recursively recompute mutable members.
    void fillPointers ( const Tracker& tracker ) const;

#ifndef __CINT__

    template <class F>
    void for_each_layer( F f) const{
      std::for_each ( _layers.begin(),
                      _layers.end(),
                      f);
    }

    template <class F>
    void for_each_straw( F f) const {
      for_each_layer( boost::bind( Layer::for_each<F>, _1, f));
    }

    // Loop over all straws and call F.
    // F can be a class with an operator() or a free function.
    template <class F>
    inline void forAllStraws ( F& f) const{
      for ( std::vector<Layer>::const_iterator i=_layers.begin(), e=_layers.end();
            i !=e; ++i){
        i->forAllStraws(f);
      }
    }

    template <class F>
    inline void forAllLayers ( F& f) const{
      for ( std::vector<Layer>::const_iterator i=_layers.begin(), e=_layers.end();
            i !=e; ++i){
        f(*i);
      }
    }


#endif

  protected:

    PanelId _id;
    std::vector<Layer> _layers;

    // Vertices of enclosing polygon.
    std::vector<CLHEP::Hep3Vector> corners;

    // Properties of the enclosing logical volume (box).

    // Half lengths of the logical box.
    std::vector<double> _boxHalfLengths;

    std::vector<CLHEP::Hep3Vector> _basePosition;
    CLHEP::Hep3Vector _baseDelta;

    // Rotations and offsets to place the logical box.
    // placedshape = ( offset + RZ*RX*RY*shape );
    //
    double _boxRxAngle;
    double _boxRyAngle;
    double _boxRzAngle;
    CLHEP::Hep3Vector _boxOffset;

    // Position (in tracker coordinates) of the midpoint, and direction 
    // of the average of the primary straw.  Mutable because these are set
    // by fillPointers.
    // TODO -- there is clearly a way to design this such that these mutable
    //         declarations can go away.
    mutable CLHEP::Hep3Vector _straw0MidPoint;
    mutable CLHEP::Hep3Vector _straw0Direction;

 
  };

}  //namespace mu2e
#endif /* TrackerGeom_Panel_hh */