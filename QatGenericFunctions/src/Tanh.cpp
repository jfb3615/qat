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

#include "QatGenericFunctions/Tanh.h"
#include "QatGenericFunctions/Cosh.h"
#include <stdexcept>
#include <cmath>

namespace Genfun {
FUNCTION_OBJECT_IMP(Tanh)

Tanh::Tanh()
{}

Tanh::~Tanh() {
}

Tanh::Tanh(const Tanh & right) : AbsFunction(right)
{  }


double Tanh::operator() (double x) const {
  return tanh(x);
}



Derivative Tanh::partial(unsigned int index) const {

  if (index!=0) throw std::range_error("Tanh: partial derivative index out of range");

  const AbsFunction & fPrime = 1.0/Cosh()/Cosh();
  return Derivative(& fPrime);
}

} // namespace Genfun
