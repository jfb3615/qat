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

#include "QatGenericFunctions/FunctionDirectProduct.h"
#include "QatGenericFunctions/KVector.h"
#include <stdexcept>

namespace Genfun {
FUNCTION_OBJECT_IMP(FunctionDirectProduct)

FunctionDirectProduct::FunctionDirectProduct(const AbsFunction *arg1, const AbsFunction *arg2):
_arg1(arg1->clone()),
_arg2(arg2->clone()),
_m(arg1->dimensionality()),
_n(arg2->dimensionality())
{
}

FunctionDirectProduct::FunctionDirectProduct(const FunctionDirectProduct & right):
AbsFunction(right),
_arg1(right._arg1->clone()),
_arg2(right._arg2->clone()),
_m(right._m),
_n(right._n)
{
}


FunctionDirectProduct::~FunctionDirectProduct()
{
  delete _arg1;
  delete _arg2;
}


double FunctionDirectProduct::operator() (const Argument & a) const {
  unsigned int P = a.dimension();
  Argument x1(_m);
  Argument x2(_n);
  if (_m+_n != P) {
    throw std::runtime_error("FunctionDirectProduct:  dimension mismatch");
    return 0;
  }
  for (unsigned int i = 0; i<_m;i++) {
    x1[i]=a[i];
  }
  for (unsigned int j = 0;j<_n;j++) {
    x2[j]=a[j+_m];
  }
  return (*_arg1)(x1) * (*_arg2)(x2);
}

unsigned int FunctionDirectProduct::dimensionality() const {
  return _m+_n;
}

double FunctionDirectProduct::operator ()(double) const
{
  throw std::runtime_error("FunctionDirectProduct:  evaluation with scalar argument");
  return 0;
}



Derivative FunctionDirectProduct::partial(unsigned int index) const {
  if (index>=(_m+_n)) throw std::range_error ("FunctionDirectProduct:  partial derivative index out of range");
  if (index<_m) {
    const Derivative & d1=_arg1->partial(index);
    const AbsFunction & fPrime = d1%(*_arg2);
    return Derivative(&fPrime);
  }
  else {
    const Derivative &d2=_arg2->partial(index-_m);
    const AbsFunction & fPrime = (*_arg1)%d2;
    return Derivative(&fPrime);
  }
}


} // namespace Genfun
