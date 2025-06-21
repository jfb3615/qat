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

#include "QatGenericFunctions/ASin.h"
#include "QatGenericFunctions/Sqrt.h"
#include "QatGenericFunctions/Square.h"
#include <cmath>     
#include <stdexcept>

namespace Genfun {
FUNCTION_OBJECT_IMP(ASin)

ASin::ASin()
{}

ASin::~ASin() {
}

ASin::ASin(const ASin & right) : AbsFunction(right)
{  }


double ASin::operator() (double x) const {
  return asin(x);
}



Derivative ASin::partial(unsigned int index) const {
  if (index!=0) throw std::range_error("ASin: partial derivative index out of range");

  Square square;
  Sqrt   root;

  const AbsFunction & fPrime =  1.0/root(1.0-square) ;
  std::shared_ptr<const AbsFunction> deriv{fPrime.clone()};
  return Derivative(deriv);
}

} // end namespace Genfun 
