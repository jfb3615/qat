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

#include "QatPlotWidgets/RangeDivision.h"
QTextDocument* RangeDivision::label() const {
  return _label;
}

void RangeDivision::setLabel(QTextDocument *doc) {
  delete _label;
  _label=doc->clone();
}

const double & RangeDivision::x() const { 
  return _x;
}


RangeDivision::RangeDivision(double value): _x(value),_label(new QTextDocument) {
}
    
RangeDivision::~RangeDivision() {
  delete _label;
}

RangeDivision::RangeDivision ( const RangeDivision & right):
  _x(right._x),
  _label(right._label->clone())
{
}

RangeDivision & RangeDivision::operator=(const RangeDivision & right) {
  if (&right!=this) {
    if (_label) delete _label;
    _label=right._label->clone();
    _x=right._x;
  }
  return *this;
}

