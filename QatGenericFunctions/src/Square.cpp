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

#include "QatGenericFunctions/Square.h"
#include "QatGenericFunctions/Variable.h"
#include <stdexcept>

namespace Genfun {
FUNCTION_OBJECT_IMP(Square)

Square::Square()
{}

Square::Square(const Square & right) : AbsFunction(right)
{
}

Square::~Square() {
}

double Square::operator() (double x) const {
  return x*x;
}



Derivative Square::partial(unsigned int index) const {
  if (index!=0) throw std::range_error("Square: partial derivative index out of range"); 
  Variable x;
  const AbsFunction & fPrime = 2*x;
  std::shared_ptr<const AbsFunction> deriv{fPrime.clone()};
  return Derivative(deriv);
}


} // namespace Genfun
