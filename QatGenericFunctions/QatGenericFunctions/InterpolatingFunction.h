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

// -------------------------------------------------------------------------//
// This class represents interpolating functions.  It uses, and requires,   //
// the GNU scientific library (GSL).  The function is defined by data       //
// points which are provided using the addPoint method.  Six different      //
// types of interpolation are provided:                                     //
//                                                                          //
// Polynomial   :  uses Lagrange's interpolating polynomial.  Default.      //
// Linear       :  straight line interpolation between points.              //
// Cubic Spline :  interpolates with a natural cubic spline                 //
// Akima        :  Non-rounded natural akima spline                         //
// Period cubic spline:  cubic spline with periodic boundary conditions     //
// Periodic akima spline: akima spline with periodic boundary conditions    //
//                                                                          //
//--------------------------------------------------------------------------//
#ifndef _InterpolatingFunction_h_
#define _InterpolatingFunction_h_ 
#include "QatGenericFunctions/AbsFunction.h"
#include "QatGenericFunctions/Parameter.h"
#include <vector>
namespace Genfun {

  //
  // Six types of interpolating functions:
  //
  enum InterpolationType {POLYNOMIAL, 
			  CUBIC_SPLINE, 
			  CUBIC_SPLINE_PERIODIC, 
			  AKIMA_SPLINE, 
			  AKIMA_SPLINE_PERIODIC,
			  LINEAR};

  class InterpolatingFunction: public AbsFunction {

    FUNCTION_OBJECT_DEF(InterpolatingFunction)

      public:

    // Constructor
    InterpolatingFunction(InterpolationType=POLYNOMIAL);

    // Copy constructor
    InterpolatingFunction(const InterpolatingFunction &right);
  
    // Destructor:
    virtual ~InterpolatingFunction();

    // Retreive function value
    virtual double operator ()(double argument) const;
    virtual double operator ()(const Argument & a) const {return operator() (a[0]);}
  
    // Add another point through which to interpolate:
    void addPoint(double x, double y);

    // Get the range:
    void getRange(double & min, double & max) const;

    // Get extreme points max/min abcissa + ordinates of these points.
    void getExtreme(double & xmin, double & xmax, double &ymin, double &ymax) const;

  private:
  
    // It is illegal to assign an adjustable constant
    const InterpolatingFunction & operator=(const InterpolatingFunction &right);

    class Clockwork;
    Clockwork *c;
  };
} // namespace Genfun

#include "QatGenericFunctions/InterpolatingFunction.icc"

#endif
