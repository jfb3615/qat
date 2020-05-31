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

// ------------------------------------------------------------------------------//
// Lagrange's Interpolating Polynomial                                           //
//                                                                               //
//                                                                               //
// Joe Boudreau.                                                                 //
//                                                                               //
// ------------------------------------------------------------------------------//
#ifndef _InterpolatingPolynomial_h_
#define _InterpolatingPolynomial_h_ 
#include "QatGenericFunctions/AbsFunction.h"
#include "QatGenericFunctions/Parameter.h"
#include <vector>
namespace Genfun {

  class InterpolatingPolynomial: public AbsFunction {

    FUNCTION_OBJECT_DEF(InterpolatingPolynomial)

      public:

    // Constructor
    InterpolatingPolynomial();

    // Copy constructor
    InterpolatingPolynomial(const InterpolatingPolynomial &right);
  
    // Destructor:
    virtual ~InterpolatingPolynomial();

    // Retreive function value
    virtual double operator ()(double argument) const;
    virtual double operator ()(const Argument & a) const {return operator() (a[0]);}
  
    // Add a new point to the set.
    void addPoint(double x, double y);

    // Get the range:
    void getRange(double & min, double & max) const;

    // Get an estimate of the interpolation error. A positive number.
    double getDelta() const;

  private:
  
    // It is illegal to assign an adjustable constant
    const InterpolatingPolynomial & operator=(const InterpolatingPolynomial &right);

    std::vector<std::pair<double,double> > xPoints;
    mutable std::vector<double>            q; 
    mutable double                         delta;
  };
} // namespace Genfun
#endif
