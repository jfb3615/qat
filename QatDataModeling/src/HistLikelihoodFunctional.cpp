//---------------------------------------------------------------------------//
// !!                                                                     !! //
//                                                                           //
//  Copyright (C) 2016 Joe Boudreau                                          //
//                                                                           //
//  This file is part of the QAT Toolkit for computational science           //
//                                                                           //
//  QAT is free software: you can redistribute it and/or modify              //
//  it under the terms of the GNU Lesser General Public License as           //
//  published by the Free Software Foundation, either version 3 of           //
//  the License, or (at your option) any later version.                      //
//                                                                           //
//  QAT is distributed in the hope that it will be useful,                   //
//  but WITHOUT ANY WARRANTY; without even the implied warranty of           //
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
//  GNU Lesser General Public License for more details.                      //
//                                                                           //
//  You should have received a copy of the GNU Lesser General Public         //
//  License along with QAT.  If not, see <http://www.gnu.org/licenses/>.     //
//                                                                           //
//---------------------------------------------------------------------------//

#include "QatDataModeling/HistLikelihoodFunctional.h"
#include "QatDataAnalysis/Hist1D.h"
#include "QatGenericFunctions/RombergIntegrator.h"
#include "QatGenericFunctions/AbsFunction.h"
#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <math.h>

HistLikelihoodFunctional::HistLikelihoodFunctional(const Hist1D * histogram,
				       double minValue,
				       double maxValue,
				       bool integrate):
  _histogram(histogram),
  _minValue(minValue),
  _maxValue(maxValue),
  _integrate(integrate),
  _nBins(_histogram->nBins()),
  _min(_histogram->min()),
  _max(_histogram->max()),
  _width(_histogram->binWidth())
{
  _delta=(_max-_min)/_nBins;
}

HistLikelihoodFunctional::~HistLikelihoodFunctional() {
}

HistLikelihoodFunctional::HistLikelihoodFunctional (const HistLikelihoodFunctional & right):
  Genfun::AbsFunctional(),
  _histogram(right._histogram),
  _minValue(right._minValue),
  _maxValue(right._maxValue),
  _integrate(right._integrate),
  _nBins(right._nBins),
  _min(right._min),
  _max(right._max),
  _delta(right._delta),
  _width(right._width)
{
}


double HistLikelihoodFunctional::operator() (const Genfun::AbsFunction & function) const{
  double logLik=0;
  for (int i = 0; i<_nBins;i++) {
    double a = _min +i*_delta;
    double b = a+_delta;
    double y = _histogram->bin(i);
    double e = _histogram->binError(i);
    if (e!=0) {
      if (b>_minValue && a < _maxValue) {
        double y0;
        if (_integrate) {
          Genfun::RombergIntegrator integral(a,b);
          y0=integral(function);
	  if (!std::isfinite(y0)) {
	    std::ostringstream message;
	    message << "HistLikelihoodFunctional, Function is infinite at " << (a+b)/2.0 << std::endl;
	    throw std::runtime_error(message.str());
	  }

        }
        else {
          y0 = function ((a+b)/2.0)*_histogram->binWidth();
	  if (!std::isfinite(y0)) {
	    std::ostringstream message;
	    message << "HistLikelihoodFunctional, Function is infinite at " << (a+b)/2.0 << std::endl;
	    throw std::runtime_error(message.str());
	  }
        }
        logLik += -2.0*(-y0 + y*log(y0));
	if (!std::isfinite(logLik)) {
	  std::ostringstream message;
	  message << "HistLikelihoodFunctional, Likelihood is infinite at " << (a+b)/2.0 << std::endl;
	  message << "Value of point is " << y0 << std::endl;
	  throw std::runtime_error(message.str());
	}
      }
    } 
  }
  return logLik;
}

