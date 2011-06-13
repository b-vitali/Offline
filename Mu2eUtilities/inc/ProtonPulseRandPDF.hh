#ifndef ProtonPulseRandPDF_hh
#define ProtonPulseRandPDF_hh

//  
// Constructor of a PDF to extract random times to describe the proton pulse
//
// $Id: ProtonPulseRandPDF.hh,v 1.2 2011/06/13 19:01:56 onoratog Exp $
// $Author: onoratog $
// $Date: 2011/06/13 19:01:56 $
//
// Original author Gianni Onorato
//
//

#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Random/RandomEngine.h"
#include "art/Framework/Services/Optional/RandomNumberGenerator.h"
#include <map>

namespace mu2e {

  class ProtonPulseRandPDF {

  public:

    ProtonPulseRandPDF(art::RandomNumberGenerator::base_engine_t& engine);

    ~ProtonPulseRandPDF();

    double fire();

  private:

    CLHEP:: RandFlat _randFlat;

    //PDF description
    double TripleGaussian(double x);

    //Parameters
    double binWidth, pulseRange, sigma1, sigma2, sigma3,
      A1, A2, A3, x01, x02, x03, Norm;

    std::map<double,double> _intPDF;


  };

}

#endif /* ProtonPulseRandPDF_hh */
