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

#include "QatDataAnalysis/TupleCut.h"
#include <set>
TupleCut::TupleCut (const TupleCut & right):
  Cut<Tuple>(),
  _f(right._f->clone()),
  _t(right._t),
  _v1(right._v1),
  _v2(right._v2)
{}

TupleCut::TupleCut(Genfun::GENFUNCTION f, double min, double max):
  _f(f.clone()),
  _t(NA),
  _v1(min),
  _v2(max) {}

TupleCut::TupleCut(Genfun::GENFUNCTION f, Type t, double val):
_f(f.clone()),
_t(t),
_v1(val),
_v2(0)
{
}

TupleCut::~TupleCut() {
  delete _f;
}

TupleCut *TupleCut::clone(void) const {
  return new TupleCut(*this);
}

bool TupleCut::operator () (const Tuple & t) const {

  double fx = (*_f)(t.asDoublePrec());
  switch (_t) {
  case NA:
    return fx >= _v1 && fx <= _v2;
  case GT:
    return fx > _v1;
  case LT:
    return fx < _v1;
  case GE:
    return fx >= _v1;
  case LE:
    return fx <= _v1;
  }
  return false;
} 
