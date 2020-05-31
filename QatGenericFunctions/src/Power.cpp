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

#include "QatGenericFunctions/Power.h"
#include <cmath>      // for pow()
#include <stdexcept>
namespace Genfun {
FUNCTION_OBJECT_IMP(Power)

Power::Power(int n):
    _intPower(n),
    _asInteger(true)
{}

Power::Power(unsigned int n):
    _intPower(n),
    _asInteger(true)
{}

Power::Power(double n):
    _doublePower(n),
    _asInteger(false)
{}

Power::Power(const Power & right)
    : AbsFunction(right),
      _doublePower(right._doublePower),
      _intPower(right._intPower),
      _asInteger(right._asInteger)
{}

Power::~Power() {
}

double Power::operator() (double x) const {
    if (_asInteger) {
	if (_intPower==0) {
	    return 1;
	}
	else if (_intPower>0) {
	    double f = 1;
	    for (int i=0;i<_intPower;i++) {
		f *=x;
	    }
	    return f;
	}
	else {
	    double f = 1;
	    for (int i=0;i<-_intPower;i++) {
		f /=x;
	    }
	    return f;
	}	    
    }
    else {
	return std::pow(x,_doublePower);
    }

}



Derivative Power::partial(unsigned int index) const {
  if (index!=0) throw std::range_error("Power:  partial derivative index out of range");
  if (_asInteger) {
    const AbsFunction & fPrime = _intPower*Power(_intPower-1);
    return Derivative(&fPrime);
  }
  else {
    const AbsFunction & fPrime = _doublePower*Power(_doublePower-1);
    return Derivative(&fPrime);
  }

}


} // namespace Genfun
