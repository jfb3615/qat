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

//---------------------Hermite----------------------------------------------//
//                                                                          //
// Class HermitePolynomial.  (Hermite polynomials)                          //
//                                                                          //
// If the normalization type is "TWIDDLE" these polynomials are normalized. //
// Otherwise, their inner product <H_n|H_m>=sqrt{pi}*2^n n!delta_{,n}       //
//                                                                          //
//--------------------------------------------------------------------------//
#ifndef HermitePolynomial_h_
#define HermitePolynomial_h_ 1
#include "QatGenericFunctions/AbsFunction.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/Defs.h"

namespace Genfun {

  class HermitePolynomial : public AbsFunction  {
  
    FUNCTION_OBJECT_DEF(HermitePolynomial)

      public:

    // Constructor
    HermitePolynomial(unsigned int n, NormalizationType type=STD);

    // Copy constructor
    HermitePolynomial(const HermitePolynomial &right);
  
    // Destructor
    virtual ~HermitePolynomial();
  
    // Retreive function value
    virtual double operator ()(double argument) const;
    virtual double operator ()(const Argument & a) const {return operator() (a[0]);}
  
    // Get the order, n
    unsigned int n() const;

    // Derivative.  
    Derivative partial (unsigned int) const;

    // Does this function have an analytic derivative?
    virtual bool hasAnalyticDerivative() const {return true;}
  

  private:

    // It is illegal to assign a Hermite Polynomial
    const HermitePolynomial & operator=(const HermitePolynomial &right);

    // Here is the sigma
    unsigned int _n;

    // Normalization (STD or TWIDDLE)
    NormalizationType _type;

  };

} // namespace Genfun

#endif


