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

#include "QatGenericFunctions/FunctionNegation.h"
#include "QatGenericFunctions/KVector.h"

namespace Genfun {
FUNCTION_OBJECT_IMP(FunctionNegation)

FunctionNegation::FunctionNegation(const AbsFunction *arg1):
  _arg1(arg1->clone())
{
}

FunctionNegation::FunctionNegation(const FunctionNegation & right):
  AbsFunction(right),
  _arg1(right._arg1->clone())
{
}


FunctionNegation::~FunctionNegation()
{
  delete _arg1;
}

unsigned int FunctionNegation::dimensionality() const {
  return _arg1->dimensionality();
}


double FunctionNegation::operator ()(double x) const
{
  return -((*_arg1)(x));
}

double FunctionNegation::operator ()(const Argument & x) const
{
  return -((*_arg1)(x));
}


Derivative FunctionNegation::partial(unsigned int index) const {
  const Derivative & d = _arg1->partial(index);
  const AbsFunction & fPrime  = -d;
  return Derivative(&fPrime);
}

  
} // namespace Genfun
