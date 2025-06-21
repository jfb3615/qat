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

#include "QatGenericFunctions/ConstOverFunction.h"
#include "QatGenericFunctions/KVector.h"

namespace Genfun {
FUNCTION_OBJECT_IMP(ConstOverFunction)

ConstOverFunction::ConstOverFunction(double constant, const std::shared_ptr<const AbsFunction> &arg):
  _constant(constant),
  _arg(arg)
{
}

ConstOverFunction::ConstOverFunction(const ConstOverFunction & right) :
AbsFunction(right),
_constant(right._constant),
_arg(right._arg)
{}

unsigned int ConstOverFunction::dimensionality() const {
  return _arg->dimensionality();
}

ConstOverFunction::~ConstOverFunction()
{
}



double ConstOverFunction::operator ()(double x) const
{
  return _constant / (*_arg)(x);
}


double ConstOverFunction::operator ()(const Argument & x) const
{
  return _constant / (*_arg)(x);
}


Derivative ConstOverFunction::partial(unsigned int index) const {
  // d/dx (k/f) = -(k/f^2)(df/dx)
  Derivative d=_arg->partial(index);
  const AbsFunction & fPrime = -_constant/(*_arg)/(*_arg)*(d);

  std::shared_ptr<const AbsFunction> deriv{fPrime.clone()};
  return Derivative(deriv);

  }


} // namespace Genfun
