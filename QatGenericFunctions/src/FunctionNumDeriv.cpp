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

#include "QatGenericFunctions/FunctionNumDeriv.h"
#include <stdexcept>
#include <cmath>      // for pow()

namespace Genfun {
FUNCTION_OBJECT_IMP(FunctionNumDeriv)

FunctionNumDeriv::FunctionNumDeriv(const AbsFunction *arg1, unsigned int index):
  _arg1(arg1->clone()),
  _wrtIndex(index)
{
}

FunctionNumDeriv::FunctionNumDeriv(const FunctionNumDeriv & right):
  AbsFunction(right),
  _arg1(right._arg1->clone()),
  _wrtIndex(right._wrtIndex)
{
}


FunctionNumDeriv::~FunctionNumDeriv()
{
  delete _arg1;
}

unsigned int FunctionNumDeriv::dimensionality() const {
  return _arg1->dimensionality();
}

#define ROBUST_DERIVATIVES 
#ifdef  ROBUST_DERIVATIVES

double FunctionNumDeriv::f_x (double x) const   { 
  return (*_arg1)(x);
}


double FunctionNumDeriv::f_Arg (double x) const { 
  _xArg [_wrtIndex] = x;
  return (*_arg1)(_xArg);
}


double FunctionNumDeriv::operator ()(double x) const
{
  if (_wrtIndex!=0) throw std::runtime_error("FunctionNumDeriv:  dimension mismatch");
  return numericalDerivative ( &FunctionNumDeriv::f_x, x );
}

double FunctionNumDeriv::operator ()(const Argument & x) const
{
  if (_wrtIndex >= x.dimension()) throw std::runtime_error("FunctionNumDeriv:  dimension mismatch");
  _xArg = x;
  double xx = x[_wrtIndex];
  return numericalDerivative ( &FunctionNumDeriv::f_Arg, xx );
}


double FunctionNumDeriv::numericalDerivative
        ( double (FunctionNumDeriv::*f)(double)const, double x ) const {

  const double h0 = 5 * std::pow(2.0, -17);

  const double maxErrorA = .0012;    // These are the largest errors in steps 
  const double maxErrorB = .0000026; // A, B consistent with 8-digit accuracy.

  const double maxErrorC = .0003; // Largest acceptable validation discrepancy.

  // This value of gives 8-digit accuracy for 1250 > curvature scale < 1/1250.

  const int nItersMax = 6;
  int nIters;
  double bestError = 1.0E30;
  double bestAns = 0;

  const double valFactor  = std::pow(2.0, -16);

  const double w   = 5.0/8;
  const double wi2 = 64.0/25.0;
  const double wi4 = wi2*wi2;

  double size    = fabs((this->*f)(x));
  if (size==0) size = std::pow(2.0, -53);

  const double adjustmentFactor[nItersMax] = {
    1.0,
    std::pow(2.0, -17),
    std::pow(2.0, +17),
    std::pow(2.0, -34),
    std::pow(2.0, +34),
    std::pow(2.0, -51)  };

  for ( nIters = 0; nIters < nItersMax; ++nIters ) {

    double h = h0 * adjustmentFactor[nIters];

    // Step A: Three estimates based on h and two smaller values:

    double A1 = ((this->*f)(x+h) - (this->*f)(x-h))/(2.0*h);
//    size = max(fabs(A1), size);
    if (fabs(A1) > size) size = fabs(A1);

    double hh = w*h;
    double A2 = ((this->*f)(x+hh) - (this->*f)(x-hh))/(2.0*hh);
//    size = max(fabs(A2), size);
    if (fabs(A2) > size) size = fabs(A2);

    hh *= w; 
    double A3 = ((this->*f)(x+hh) - (this->*f)(x-hh))/(2.0*hh);
//    size = max(fabs(A3), size);
    if (fabs(A3) > size) size = fabs(A3);

    if ( (fabs(A1-A2)/size > maxErrorA) || (fabs(A1-A3)/size > maxErrorA) ) { 
      continue;
    }

    // Step B: Two second-order estimates based on h h*w, from A estimates

    double B1 = ( A2 * wi2 - A1 ) / ( wi2 - 1 );
    double B2 = ( A3 * wi2 - A2 ) / ( wi2 - 1 );
    if ( fabs(B1-B2)/size > maxErrorB ) { 
      continue;
    }

    // Step C: Third-order estimate, from B estimates:

    double ans = ( B2 * wi4 - B1 ) / ( wi4 - 1 );
    double err = fabs ( ans - B1 );
    if ( err < bestError ) {
      bestError = err;
      bestAns = ans;
    }

    // Validation estimate based on much smaller h value:

    hh = h * valFactor;
    double val = ((this->*f)(x+hh) - (this->*f)(x-hh))/(2.0*hh);
    if ( fabs(val-ans)/size > maxErrorC ) {
      continue;
    }

    // Having passed both apparent accuracy and validation, we are finished:
    break;
  }

  return bestAns;

}
#endif // ROBUST_DERIVATIVES



#ifdef SIMPLER_DERIVATIVES
double FunctionNumDeriv::operator ()(double x) const
{
  if (_wrtIndex !=0) throw std::runtime_error("FunctionNumDeriv:  dimension mismatch");
  const double h=1.0E-6;
  return ((*_arg1)(x+h) - (*_arg1)(x-h))/(2.0*h);
}

double FunctionNumDeriv::operator ()(const Argument & x) const
{
  if (_wrtIndex >= x.dimension()) throw std::runtime_error("FunctionNumDeriv:  dimension mismatch");
  const double h=1.0E-6;
  Argument x1=x, x0=x;
  x1[_wrtIndex] +=h;
  x0[_wrtIndex] -=h; 
  return ((*_arg1)(x1) - (*_arg1)(x0))/(2.0*h);
}
#endif // SIMPLER_DERIVATIVES

} // namespace Genfun
