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

#include "QatGenericFunctions/BetaDistribution.h"
#include <cmath>
using namespace std;

namespace Genfun {
FUNCTION_OBJECT_IMP(BetaDistribution)


BetaDistribution::BetaDistribution():
_alpha("a", 1.0, 0.0, 100),
  _beta ("beta", 1.0, 0.0, 100)
{}
  
  BetaDistribution::BetaDistribution(const BetaDistribution & right):
    AbsFunction( right ),
    _alpha(right._alpha),
    _beta (right._beta)
  {
  }
  
  BetaDistribution::~BetaDistribution() {
  }
  
  double BetaDistribution::operator() (double x) const {
    double a = _alpha.getValue(),b=_beta.getValue();
    return pow(x,a-1)*pow((1-x),b-1)*
      exp(lgamma(a+b)-lgamma(a)-lgamma(b));
    
  }
  
  Parameter & BetaDistribution::alpha() {
    return _alpha;
  }
  
  
  Parameter & BetaDistribution::beta() {
    return _beta;
  }
  
  
  
} // namespace Genfun
