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

#include "QatGenericFunctions/HermitePolynomial.h"
#include "QatGenericFunctions/FixedConstant.h"
#include <stdexcept>
#include <cmath>

namespace Genfun {
FUNCTION_OBJECT_IMP(HermitePolynomial)

HermitePolynomial::HermitePolynomial(unsigned int n, NormalizationType type)
:_n(n),_type(type)
{}

HermitePolynomial::~HermitePolynomial() {
}

HermitePolynomial::HermitePolynomial(const HermitePolynomial & right) 
  :AbsFunction(), _n(right._n), _type(right._type)
{  }


double HermitePolynomial::operator() (double x) const {
  const static double h00=0.0;
  const static double h0=pow(M_PI,-1/4.0);
  double p1=h0;
  double p2=h00;
  double p3=0;
  for (unsigned int i=1;i<=_n;i++) {
    p3=p2;
    p2=p1;
    p1=x*sqrt(2.0/i)*p2-sqrt((i-1.0)/i)*p3;
  }
  double pn=p1;
  return _type==TWIDDLE? pn:pn*pow(M_PI, 0.25)*pow(2.0,_n/2.0)*exp(lgamma(_n+1)/2.0);
}

unsigned int HermitePolynomial::n() const {
  return _n;
}

Derivative HermitePolynomial::partial(unsigned int index) const {
  if (index!=0) throw std::range_error("HermitePolynomial: partial derivative index out of range");
  if (_n>0) {
    const AbsFunction & fPrime = _type==TWIDDLE ? 
      2.0*_n*HermitePolynomial(_n-1,STD)/pow(M_PI, 0.25)/(pow(2.0,_n/2.0)*exp(lgamma(_n+1)/2.0))
      :
      2.0*_n*HermitePolynomial(_n-1,STD);
    return Derivative(& fPrime);
  }
  else {
    const AbsFunction & fPrime   = FixedConstant(0.0);
    return Derivative(& fPrime);
  }
}

} // namespace Genfun
