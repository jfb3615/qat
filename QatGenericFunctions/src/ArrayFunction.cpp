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

#include "QatGenericFunctions/ArrayFunction.h"
#include "QatGenericFunctions/FixedConstant.h"

namespace Genfun {

FUNCTION_OBJECT_IMP(ArrayFunction)
  
// Constructor
ArrayFunction::ArrayFunction(const double *begin, const double *end) :
_values(begin,end) {}
  
// Initializer list constructor:
  ArrayFunction::ArrayFunction(std::initializer_list<double> values):
    _values(values)
  {
  }

// Destructor
ArrayFunction::~ArrayFunction(){}
  
// Copy constructor
ArrayFunction::ArrayFunction(const ArrayFunction &right):
  AbsFunction(right), _values(right._values) {}
  
  // Retreive function value
double ArrayFunction::operator ()(double argument) const {
  int i =  int (argument+0.5);
  if (i<0 || i>=int(_values.size())) return 0;
  else return _values[i];
}
Derivative ArrayFunction::partial(unsigned int index) const {
  if (index!=0) throw std::range_error("ArrayFunction: partial derivative index out of range");
  FixedConstant fPrime(0.0);
  return Derivative(&fPrime);
}

}
