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

#include "QatGenericFunctions/ConstTimesFunction.h"
#include "QatGenericFunctions/KVector.h"
namespace Genfun {
FUNCTION_OBJECT_IMP(ConstTimesFunction)

ConstTimesFunction::ConstTimesFunction(double constant, const AbsFunction *arg):
  _constant(constant),
  _arg(arg->clone())
{
}

ConstTimesFunction::ConstTimesFunction(const ConstTimesFunction & right) :
AbsFunction(right),
_constant(right._constant),
_arg(right._arg->clone())
{}

unsigned int ConstTimesFunction::dimensionality() const {
  return _arg->dimensionality();
}

ConstTimesFunction::~ConstTimesFunction()
{
  delete _arg;
}



double ConstTimesFunction::operator ()(double x) const
{
  return _constant * (*_arg)(x);
}


double ConstTimesFunction::operator ()(const Argument & x) const
{
  return _constant * (*_arg)(x);
}

  Derivative ConstTimesFunction::partial(unsigned int index) const {
    // d/dx (k*f) = k*(df/dx)
    const Derivative & d=_arg->partial(index);
    const AbsFunction & fPrime = _constant*d;
    return Derivative(& fPrime);
  }
  
} // namespace Genfun
