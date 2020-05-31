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

#include "QatPlotWidgets/LinearSubdivider.h"
#include <cmath>
#include <sstream>
#include <iostream>
inline double ciel(double x) { 
  double fx = floor(x);
  if (x==fx) return x;
  else return x+1.0;
}
LinearSubdivider::LinearSubdivider():
  _min(0.0),_max(1.0)
{
}

LinearSubdivider::~LinearSubdivider() {
}



// Set the range:
void LinearSubdivider::setRange(double min, double max){

  // This protection insures 
  if (min==max) return;
  _min=min;
  _max=max;
  _recompute();




}

// Set the range:
void LinearSubdivider::setMin(double min){
  setRange(min,_max);
}

// Set the range:
void LinearSubdivider::setMax(double max){
  setRange(_min,max);
}


// Get the number of subdivisions:
int LinearSubdivider::getNumSubdivisions() const{
  return _subdivision.size();
}

// Get the location of each subdivision:
const RangeDivision & LinearSubdivider::getSubdivision(int i) const{
  return _subdivision[i];
}


void LinearSubdivider::_recompute() {
  // Clean out old subdivisions:
  _subdivision.erase(_subdivision.begin(),_subdivision.end());


  double exponent = floor(log10(_max-_min));
  double multiplier = pow(10,exponent);
  int Ntyp = int (10*_max/multiplier-10*_min/multiplier+0.5); 
  // number from 10 to 100
  int intervals=1;
  if (Ntyp==12) {
    intervals=4;
  }
  else if (Ntyp==20) {
    intervals=5;
  }
  else if (Ntyp==30) {
    intervals=5;
  }
  else if (Ntyp==40) {
    intervals=4;
  }
  else if (Ntyp==50) {
    intervals=5;
  }
  else if (Ntyp==60) {
    intervals=5;
  }
  else if (Ntyp==70) {
    intervals=7;
  }
  else if (Ntyp==80) {
    intervals=4;
  }
  else if (Ntyp==90) {
    intervals=3;
  }
  else {
    intervals=5;
  }
  multiplier *= (Ntyp/10.0/intervals);

  int nTicks=int(((_max-_min)/multiplier) + 1.05);
  
  if (nTicks<50) {
    for (int i=0;i<nTicks;i++) {
      double sub = _min+i*multiplier;
      //if (fabs(xValue)/xMultiplier < 1.0E-6) xValue=0;
      if (sub>=_min && sub<=_max)   {
	_subdivision.push_back(sub);
	
      }
      
    }
  }
  else {
  
    {
      _subdivision.push_back(_min);
    }
    {
      _subdivision.push_back(_max);
    }
  }
}

// Get the validity of each subdivision:
bool LinearSubdivider::isValid(int i) const {
  const RangeDivision & rd=getSubdivision(i);
  return (rd.x()>=_min && rd.x()<=_max);
}

