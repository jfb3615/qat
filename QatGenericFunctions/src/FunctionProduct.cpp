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

#include "QatGenericFunctions/FunctionProduct.h"
#include "QatGenericFunctions/KVector.h"
#include <stdexcept>

namespace Genfun {
FUNCTION_OBJECT_IMP(FunctionProduct)

FunctionProduct::FunctionProduct(const AbsFunction *arg1, const AbsFunction *arg2):
_arg1(arg1->clone()),
_arg2(arg2->clone())
{
  if (arg1->dimensionality()!=arg2->dimensionality()) {
    throw std::runtime_error ("FunctionProduct:  dimension mismatch");
  }
}

FunctionProduct::FunctionProduct(const FunctionProduct & right) :
  AbsFunction(right),
  _arg1(right._arg1->clone()),
  _arg2(right._arg2->clone())
{
}

FunctionProduct::~FunctionProduct()
{
  delete _arg1;
  delete _arg2;
}


unsigned int FunctionProduct::dimensionality() const {
  return _arg1->dimensionality();
}

double FunctionProduct::operator ()(double x) const
{
  return (*_arg1)(x)*(*_arg2)(x);
}

double FunctionProduct::operator ()(const Argument & x) const
{
  return (*_arg1)(x)*(*_arg2)(x);
}

Derivative FunctionProduct::partial(unsigned int index) const {
  const Derivative & d1=_arg1->partial(index);
  const Derivative & d2=_arg2->partial(index);
  const AbsFunction & fPrime =  (*_arg1)*d2 + d1*(*_arg2);
  return Derivative(&fPrime);
}

} // namespace Genfun
