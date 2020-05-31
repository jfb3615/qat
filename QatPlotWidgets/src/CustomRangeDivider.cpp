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

#include "QatPlotWidgets/CustomRangeDivider.h"
// Constructor:
CustomRangeDivider::CustomRangeDivider():
  _min(0),_max(0){
}

// Destructor:
CustomRangeDivider::~CustomRangeDivider(){
}

// Insert a subdivision:
void CustomRangeDivider::add (double x, QTextDocument *label){
  _subdivision.push_back(RangeDivision(x));
  if (label) _subdivision.back().setLabel(label); // internally this copies. 
}


void CustomRangeDivider::add (double x, const std::string & label, const QFont *font) {
  QTextDocument doc;
  doc.setPlainText(label.c_str());
  if (font) doc.setDefaultFont(*font);
  add(x, &doc);
}


// Get the number of subdivision:
int CustomRangeDivider::getNumSubdivisions() const{
  return _subdivision.size();
}

// Get the location of each subdivision:
const RangeDivision & CustomRangeDivider::getSubdivision ( int i) const{
  return _subdivision[i];
}


// Set the range:
void CustomRangeDivider::setRange(double min, double max){

  // This protection insures 
  if (min==max) return;
  _min=min;
  _max=max;

}

// Set the range:
void CustomRangeDivider::setMin(double min){
  setRange(min,_max);
}

// Set the range:
void CustomRangeDivider::setMax(double max){
  setRange(_min,max);
}

// Get the validity of each subdivision:
bool CustomRangeDivider::isValid(int i) const {
  const RangeDivision & rd=getSubdivision(i);
  return (rd.x()>=_min && rd.x()<=_max);
}
