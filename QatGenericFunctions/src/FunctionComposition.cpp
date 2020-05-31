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

#include "QatGenericFunctions/FunctionComposition.h"
#include <stdexcept>
#include "QatGenericFunctions/KVector.h"

namespace Genfun {
FUNCTION_OBJECT_IMP(FunctionComposition)

FunctionComposition::FunctionComposition(const AbsFunction *arg1, const AbsFunction *arg2):_arg1(arg1->clone()),_arg2(arg2->clone())
{
  if (arg1->dimensionality()!=1) {
    throw std::runtime_error("Dimension mismatch in function composition");
  }
}

FunctionComposition::FunctionComposition(const FunctionComposition & right):
AbsFunction(right),
_arg1(right._arg1->clone()),
_arg2(right._arg2->clone())
{}

FunctionComposition::~FunctionComposition()
{
  delete _arg1;
  delete _arg2;
}

unsigned int FunctionComposition::dimensionality() const {
  return _arg2->dimensionality();
}

double FunctionComposition::operator ()(double argument) const {
  if (dimensionality()!=1) {
    throw std::runtime_error("FunctionComposition: dimension mismatch");
    return 0;
  }
  else {
    return (*_arg1)((*_arg2)(argument));
  }
}

double FunctionComposition::operator() (const Argument &  v) const {
  if (v.dimension()!=_arg2->dimensionality()) {
    throw std::runtime_error("FunctionComposition: dimension mismatch");
    return 0;
  }
  else {
    return (*_arg1)((*_arg2)(v));
  }
}


Derivative FunctionComposition::partial(unsigned int index) const {
  const Derivative & d1=_arg1->partial(0);
  const Derivative & d2=_arg2->partial(index);
  const AbsFunction & fPrime = d1(*_arg2)*d2;
  return Derivative(&fPrime);
}


} // namespace Genfun
