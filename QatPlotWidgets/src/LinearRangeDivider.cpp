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

#include "QatPlotWidgets/LinearRangeDivider.h"
#include <cmath>
#include <sstream>
inline double ciel(double x) { 
  double fx = floor(x);
  if (x==fx) return x;
  else return x+1.0;
}
LinearRangeDivider::LinearRangeDivider():
  _min(0.0),_max(1.0)
{
}

LinearRangeDivider::~LinearRangeDivider() {
}



// Set the range:
void LinearRangeDivider::setRange(double min, double max){

  // This protection insures 
  if (min==max) return;
  _min=min;
  _max=max;
  _recompute();




}

// Set the range:
void LinearRangeDivider::setMin(double min){
  setRange(min,_max);
}

// Set the range:
void LinearRangeDivider::setMax(double max){
  setRange(_min,max);
}


// Get the number of subdivisions:
int LinearRangeDivider::getNumSubdivisions() const{
  return _subdivision.size();
}

// Get the location of each subdivision:
const RangeDivision & LinearRangeDivider::getSubdivision(int i) const{
  return _subdivision[i];
}


void LinearRangeDivider::_recompute() {
  // Clean out old subdivisions:
  _subdivision.erase(_subdivision.begin(),_subdivision.end());

  double lower, multiplier;
  int nTicks;

  double exponent = floor(log10(_max-_min));
  multiplier = pow(10,exponent);
  int Ntyp = int(10*_max/multiplier-10*_min/multiplier); 
  // number from 10 to 100
  double omult;
  if (Ntyp<12) {
    omult = .1*multiplier;
    multiplier = .2* multiplier;
  }
  else if (Ntyp<16) {
    omult = .1*multiplier;
    multiplier = .3*multiplier;
  }
  else if (Ntyp<25) {
    omult = .2*multiplier;
    multiplier = .4*multiplier;
  }
  else if (Ntyp<30) {
    omult = .1*multiplier;
    multiplier = .5*multiplier;
  }
  else if (Ntyp<40) {
    omult = .2*multiplier;
    multiplier = .6*multiplier;
  }
  else if (Ntyp<50) {
    omult = .2*multiplier;
    multiplier = .8*multiplier;
  }
  else if (Ntyp<60) {
    omult = multiplier;
    multiplier = 1.0*multiplier;
  }
  else if (Ntyp<75) {
    omult = 0.4*multiplier;
    multiplier = 1.2*multiplier;
  }
  else if (Ntyp<80) {
    omult = 0.5*multiplier;
    multiplier = 1.5*multiplier;
  }
  else {
    omult =      0.5*multiplier;
    multiplier = 2.0*multiplier;
  }
  int tk=(int) floor(_min/omult +0.5);

  lower  = tk*omult;
  nTicks=int(((_max-lower)/multiplier) + 1.05);
  
  if (nTicks<50) {
    for (int i=0;i<nTicks;i++) {
      double sub = lower+i*multiplier;
      //if (fabs(xValue)/xMultiplier < 1.0E-6) xValue=0;
      if (sub>=_min && sub<=_max)   {
	_subdivision.push_back(sub);
	
	std::ostringstream label;
	label << sub;
	QString qstr = label.str().c_str();
	_subdivision.back().label()->setPlainText(qstr);
      }
      
    }
  }
  else {
  
    {
      _subdivision.push_back(_min);
      
      std::ostringstream label;
      label << _min;
      QString qstr = label.str().c_str();
      _subdivision.back().label()->setPlainText(qstr);
    }
    {
      _subdivision.push_back(_max);
      
      std::ostringstream label;
      label << _max;
      QString qstr = label.str().c_str();
      _subdivision.back().label()->setPlainText(qstr);
    }
  }
}

// Get the validity of each subdivision:
bool LinearRangeDivider::isValid(int i) const {
  const RangeDivision & rd=getSubdivision(i);
  return (rd.x()>=_min && rd.x()<=_max);
}

