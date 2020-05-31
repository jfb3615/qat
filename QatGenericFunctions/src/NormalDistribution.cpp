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

#include "QatGenericFunctions/NormalDistribution.h"
#include "QatGenericFunctions/Variable.h"
#include <cmath>     
#include <stdexcept>

namespace Genfun {
FUNCTION_OBJECT_IMP(NormalDistribution)

NormalDistribution::NormalDistribution():
  _mean("Mean", 0.0,-10,10),
  _sigma("Sigma",1.0,0, 10)
{}

NormalDistribution::~NormalDistribution() {
}

NormalDistribution::NormalDistribution(const NormalDistribution & right):
AbsFunction(right),
_mean(right._mean),
_sigma(right._sigma)
{
}

double NormalDistribution::operator() (double x) const {
  double s   = _sigma.getValue();
  double x0  = _mean.getValue();
  return (1.0/(sqrt(2*M_PI)*s))*
	  exp(-(x-x0)*(x-x0)/(2.0*s*s));
}

Parameter & NormalDistribution::mean() {
  return _mean;
}

Parameter & NormalDistribution::sigma() {
  return _sigma;
}

const Parameter & NormalDistribution::mean() const {
  return _mean;
}

const Parameter & NormalDistribution::sigma() const {
  return _sigma;
}



Derivative NormalDistribution::partial(unsigned int index) const {

  if (index!=0) throw std::range_error("NormalDistribution: partial derivative index out of range");

  Variable x;
  const AbsFunction & fPrime  = (*this)*(_mean-x)/_sigma/_sigma;
  return Derivative(&fPrime);
}

} // namespace Genfun
