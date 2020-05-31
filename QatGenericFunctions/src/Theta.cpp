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

#include "QatGenericFunctions/Theta.h"
#include "QatGenericFunctions/FixedConstant.h"
#include <stdexcept>
namespace Genfun {
FUNCTION_OBJECT_IMP(Theta)

Theta::Theta()
{}

Theta::~Theta() {
}

Theta::Theta(const Theta & right) : AbsFunction(right)
{  }


double Theta::operator() (double x) const {
  return (x>=0) ? 1.0:0.0;
}



Derivative Theta::partial(unsigned int index) const {
  if (index!=0) throw std::runtime_error("Theta::Partial: index out of range");
  const AbsFunction & fPrime = FixedConstant(0.0);
  return Derivative(& fPrime);
}

} // namespace Genfun
