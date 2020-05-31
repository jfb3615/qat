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

#include "QatPlotWidgets/LogRangeDivider.h"
#include <cmath>
#include <sstream>
#include <algorithm>
inline double ciel(double x) { 
  double fx = floor(x);
  if (x==fx) return x;
  else return x+1.0;
}
LogRangeDivider::LogRangeDivider():
  _min(0.0),_max(1.0)
{
}

LogRangeDivider::~LogRangeDivider() {
}



// Set the range:
void LogRangeDivider::setRange(double min, double max){

  // This protection insures 
  if (min==max) return;
  _min=std::min(min,max);
  _max=std::max(min,max);

  if (_max<0) {
    _max=1;
    _min=0.1;
  }
  if (_min<0) {
    _min=_max/10.0;
  }
  
  _recompute();




}

// Set the range:
void LogRangeDivider::setMin(double min){
  setRange(min,_max);
}

// Set the range:
void LogRangeDivider::setMax(double max){
  setRange(_min,max);
}


// Get the number of subdivisions:
int LogRangeDivider::getNumSubdivisions() const{
  return _subdivision.size();
}

// Get the location of each subdivision:
const RangeDivision & LogRangeDivider::getSubdivision(int i) const{
  return _subdivision[i];
}

void LogRangeDivider::_recompute() {
  // Clean out old subdivisions:
  _subdivision.erase(_subdivision.begin(),_subdivision.end());

  
  double exponentMin = floor(log10(_min));
  double multMin = pow(10,exponentMin);
  unsigned int NMin = (unsigned int) (_min/multMin+1);
  if (NMin==10) {
    NMin=1;
    multMin*=10;
  }

  double nDecades = log10(_max/_min);
  static unsigned int valueSet3[]={1};
  static unsigned int valueSet2[]={1,5};
  static unsigned int valueSet1[]={1,2,5};
  static unsigned int valueSet0[]={1,2,3,4,5,6,7,8,9};
  static unsigned int *valueSet=NULL;
  unsigned int valueSetSize=0;
  unsigned int nJump = (unsigned int) (nDecades/8 + 1);
  if     (nDecades<1) {
    valueSet=valueSet0;
    valueSetSize=sizeof(valueSet0)/sizeof(int);
  }
  else if (nDecades<2) {
    valueSet=valueSet1;
    valueSetSize=sizeof(valueSet1)/sizeof(int);
  }
  else if (nDecades<3) {
    valueSet=valueSet2;
    valueSetSize=sizeof(valueSet2)/sizeof(int);
  }
  else {
    valueSet=valueSet3;
    valueSetSize=sizeof(valueSet3)/sizeof(int);
  }

  while (1) {

    if (std::find(valueSet,valueSet+valueSetSize,NMin)!=valueSet+valueSetSize) {
      _subdivision.push_back(NMin*multMin);
      std::ostringstream label;
      label << NMin*multMin;
      QString qstr = label.str().c_str();
      _subdivision.back().label()->setPlainText(qstr);
    }

    NMin++;
    if (NMin==10) {
      NMin=1;
      multMin*=pow(10,nJump);
    }
    if (NMin*multMin > _max) break;
  }

}

// Get the validity of each subdivision:
bool LogRangeDivider::isValid(int i) const {
  const RangeDivision & rd=getSubdivision(i);
  return (rd.x()>=_min && rd.x()<=_max);
}

//if (fabs(xValue)/xMultiplier < 1.0E-6) xValue=0;
