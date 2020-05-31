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

#include "QatGenericFunctions/Sigma.h"
#include <stdexcept>

namespace Genfun {
FUNCTION_OBJECT_IMP(Sigma)

void Sigma::accumulate( const AbsFunction & fcn) {
  _fcn.push_back(fcn.clone());
}

Sigma::Sigma()
{
}

Sigma::Sigma(const Sigma & right) : AbsFunction(right)
{
  for (size_t i=0; i<right._fcn.size();i++) {
    _fcn.push_back(right._fcn[i]->clone());
  }
}

unsigned int Sigma::dimensionality() const {
  if (_fcn.size()==0) throw std::runtime_error("Cannot determine dimensionality from empty sum");
  return _fcn[0]->dimensionality();
}

Sigma::~Sigma()
{
  for (size_t i=0; i<_fcn.size();i++) {
    delete _fcn[i];
  }
}

double Sigma::operator ()(double x) const
{
  double retVal=0.0;
  for (size_t i=0;i<_fcn.size();i++) retVal += (*_fcn[i])(x);
  return retVal;
}


double Sigma::operator ()(const Argument & x) const
{
  double retVal=0.0;
  for (size_t i=0;i<_fcn.size();i++) retVal += (*_fcn[i])(x);
  return retVal;
}



Derivative Sigma::partial(unsigned int index) const {
  Sigma fPrime;
  for (size_t i=0;i<_fcn.size();i++) {
    fPrime.accumulate(_fcn[i]->partial(index));
  }
  return Derivative(&fPrime);
}



} // namespace Genfun
