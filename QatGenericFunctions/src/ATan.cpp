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

#include "QatGenericFunctions/ATan.h"
#include "QatGenericFunctions/Square.h"
#include <stdexcept>
#include <cmath>    

namespace Genfun {
FUNCTION_OBJECT_IMP(ATan)

ATan::ATan()
{}

ATan::~ATan() {
}

ATan::ATan(const ATan & right) : AbsFunction(right)
{  }


double ATan::operator() (double x) const {
  return atan(x);
}



Derivative ATan::partial(unsigned int index) const {
  if (index!=0) throw std::range_error("ATan: partial derivative index out of range");

  Square square;
  
  const AbsFunction & fPrime=1.0/(1.0+square);;
  return Derivative(& fPrime);
}

} // end namespace Genfun 
