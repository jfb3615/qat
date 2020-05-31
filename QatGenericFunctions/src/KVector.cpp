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

#include "QatGenericFunctions/KVector.h"
#include "QatGenericFunctions/FixedConstant.h"
#include <stdexcept>
namespace Genfun {
FUNCTION_OBJECT_IMP(KVector)

KVector::KVector(unsigned int dmsnlty, double val):
  _dimensionality(dmsnlty),
  _value(val)
{}

KVector::KVector(const KVector & right):
  AbsFunction(right),
  _dimensionality(right._dimensionality),
  _value(right._value)
{
}

KVector::~KVector() {
}

double KVector::operator() (double ) const {
  if (_dimensionality!=1) throw std::runtime_error("Genfun::KVector: dimensionality !=1") ;
  return _value;
}

double KVector::operator () (const Argument & a) const {
 
  if  (_dimensionality!=a.dimension()) {
    throw std::runtime_error("Genfun::KVector dimensionality mismatch");
  }
  return _value;
}



Derivative KVector::partial(unsigned int ) const {

  KVector vec(_dimensionality,0.0);
  return Derivative(&vec);
}

unsigned int KVector::dimensionality() const {
  return _dimensionality;
} 

} // namespace Genfun
