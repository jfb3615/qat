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

//--------------------AssociatedLaguerrePolynomial--------------------------//
//                                                                          //
// Class AssociatedLaguerrePolynomial.  An associated legendre polynomial.  // 
// Relies on, must be linked with, the gnu scientific library.              //
//                                                                          //
//--------------------------------------------------------------------------//
#ifndef AssociatedLaguerrePolynomial_h
#define AssociatedLaguerrePolynomial_h 1
#include "QatGenericFunctions/AbsFunction.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/Defs.h"
namespace Genfun {

  class AssociatedLaguerrePolynomial : public AbsFunction  {
  
    FUNCTION_OBJECT_DEF(AssociatedLaguerrePolynomial)

      public:

    // Constructor

    // STD means a normal Associated LaguerrePolynomial.  TWIDDLE
    // means that the factor sqrt((n+a)!/n!) is included
    // already, to obtain normalized functions.    

    AssociatedLaguerrePolynomial(unsigned int n, double a, NormalizationType type=STD);

    // Copy constructor
    AssociatedLaguerrePolynomial(const AssociatedLaguerrePolynomial &right);
  
    // Destructor
    virtual ~AssociatedLaguerrePolynomial();
  
    // Retreive function value
    virtual double operator ()(double argument) const;
    virtual double operator ()(const Argument & a) const {return operator() (a[0]);}
  
    // Get n
    unsigned int n() const;

    // Get a
    double a() const;

    // Derivative.  
    Derivative partial (unsigned int) const;
    
    // Does this function have an analytic derivative?
    virtual bool hasAnalyticDerivative() const {return true;}

  private:

    // It is illegal to assign an adjustable constant
    const AssociatedLaguerrePolynomial & operator=(const AssociatedLaguerrePolynomial &right);

    // Here is the decay constant
    unsigned int _n;

    // Here is the sigma
    double _a;

    // Normalization type:
    NormalizationType _normType;

  };

} 

#include "QatGenericFunctions/AssociatedLaguerrePolynomial.icc"


#endif


