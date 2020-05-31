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

#include "QatGenericFunctions/ACosh.h"
#include "QatGenericFunctions/Sqrt.h"
#include "QatGenericFunctions/Square.h"
#include <cmath>      
#include <stdexcept>

namespace Genfun {
FUNCTION_OBJECT_IMP(ACosh)

ACosh::ACosh()
{}

ACosh::~ACosh() {
}

ACosh::ACosh(const ACosh & right) : AbsFunction(right)
{  }


double ACosh::operator() (double x) const {
  return acosh(x);
}


Derivative ACosh::partial(unsigned int index) const {
 if (index!=0) throw std::range_error("ACosh: partial derivative index out of range");

  Square square;
  Sqrt   root;

  const AbsFunction & fPrime =  1.0/root(square-1) ;
  return Derivative(& fPrime);
}

} // end namespace Genfun 


