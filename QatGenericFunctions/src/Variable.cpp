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

#include "QatGenericFunctions/Variable.h"
#include "QatGenericFunctions/KVector.h"
#include <stdexcept>
namespace Genfun {
FUNCTION_OBJECT_IMP(Variable)

Variable::Variable(unsigned int selectionIndex, unsigned int dmsnlty):
  _selectionIndex(selectionIndex),
  _dimensionality(dmsnlty)
{}

Variable::Variable(const Variable & right):
  AbsFunction(right),
  _selectionIndex(right._selectionIndex),
  _dimensionality(right._dimensionality)
{
}

Variable::~Variable() {
}

double Variable::operator() (double x) const {
  if (_selectionIndex!=0) throw std::runtime_error("Genfun::Variable: selection index !=0") ;
  return x;
}

double Variable::operator () (const Argument & a) const {
  if  (!(_selectionIndex<a.dimension())) throw std::runtime_error("Genfun::Varaible selection index out of bounds");
  return a[_selectionIndex];
}

unsigned int Variable::index() const {
  return _selectionIndex;
}


Derivative Variable::partial(unsigned int mindex) const {
  double kroneckerDelta = mindex==_selectionIndex ? 1.0 : 0.0;
  KVector vec(_dimensionality,kroneckerDelta);
  return Derivative(&vec);
}

unsigned int Variable::dimensionality() const {
  return _dimensionality;
} 

} // namespace Genfun
