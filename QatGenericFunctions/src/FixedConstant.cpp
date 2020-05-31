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

#include "QatGenericFunctions/FixedConstant.h"
#include <stdexcept>

namespace Genfun {
FUNCTION_OBJECT_IMP(FixedConstant)

FixedConstant::FixedConstant(double value):
_value(value)
{
}

FixedConstant::FixedConstant(const FixedConstant & right):
AbsFunction(right), _value(right._value) {
}

FixedConstant::~FixedConstant()
{
}

double FixedConstant::operator ()(double) const
{
  return _value;
}

Derivative FixedConstant::partial(unsigned int index) const {
  if (index!=0) throw std::range_error("FixedConstant: partial derivative index out of range");
  FixedConstant fPrime(0.0);
  return Derivative(&fPrime);
}

} // namespace Genfun
