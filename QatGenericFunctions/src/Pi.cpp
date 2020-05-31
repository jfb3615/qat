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

#include "QatGenericFunctions/Pi.h"
#include "QatGenericFunctions/Sigma.h"
#include "QatGenericFunctions/FixedConstant.h"
#include <stdexcept>

namespace Genfun {
FUNCTION_OBJECT_IMP(Pi)

void Pi::accumulate( const AbsFunction & fcn) {
  _fcn.push_back(fcn.clone());
}

Pi::Pi()
{
}

Pi::Pi(const Pi & right) : AbsFunction(right)
{
  for (size_t i=0; i<right._fcn.size();i++) {
    _fcn.push_back(right._fcn[i]->clone());
  }
}

unsigned int Pi::dimensionality() const {
  if (_fcn.size()==0) throw std::runtime_error("Cannot determine dimensionality from empty product");
  return _fcn[0]->dimensionality();
}

Pi::~Pi()
{
  for (size_t i=0; i<_fcn.size();i++) {
    delete _fcn[i];
  }
}

double Pi::operator ()(double x) const
{
  double retVal=1.0;
  for (size_t i=0;i<_fcn.size();i++) retVal *= (*_fcn[i])(x);
  return retVal;
}


double Pi::operator ()(const Argument & x) const
{
  double retVal=1.0;
  for (size_t i=0;i<_fcn.size();i++) retVal *= (*_fcn[i])(x);
  return retVal;
}



Derivative Pi::partial(unsigned int index) const {

  Sigma fPrime;
  fPrime.accumulate(FixedConstant(0.0));
  for (size_t i=0;i<_fcn.size();i++) {
    Pi subproductI;
    subproductI.accumulate(FixedConstant(1.0));
    for (size_t j=0;j<_fcn.size();j++) {
      if (j!=i) subproductI.accumulate(*_fcn[i]);
    }
    fPrime.accumulate(_fcn[i]->partial(index)*subproductI);
  }
  return Derivative(&fPrime);
}



} // namespace Genfun
