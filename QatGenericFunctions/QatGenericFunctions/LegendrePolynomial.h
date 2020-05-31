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

//---------------------LegendrePolynomial-----------------------------------//
//                                                                          //
// Class LegendrePolynomial.  An legendre polynomial. Relies on,            //
// must be linked with, the gnu scientific library.                         //
//                                                                          //
//--------------------------------------------------------------------------//
#ifndef LegendrePolynomial_h
#define LegendrePolynomial_h 1
#include "QatGenericFunctions/AbsFunction.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/Defs.h"
namespace Genfun {

  class LegendrePolynomial : public AbsFunction  {
  
    FUNCTION_OBJECT_DEF(LegendrePolynomial)

      public:

    // Constructor

    // STD means a normal  LegendrePolynomial.  TWIDDLE means that the factor 
    // sqrt(2l+1)/2) is included already, making the set of legendres
    // orthonormal.   

    LegendrePolynomial(unsigned int l, NormalizationType type=STD);

    // Copy constructor
    LegendrePolynomial(const LegendrePolynomial &right);
  
    // Destructor
    virtual ~LegendrePolynomial();
  
    // Retreive function value
    virtual double operator ()(double argument) const;
    virtual double operator ()(const Argument & a) const {return operator() (a[0]);}
  
    // Get the integer variable l
    unsigned int l() const;

   // Derivative.  
    Derivative partial (unsigned int) const;

    // Does this function have an analytic derivative?
    virtual bool hasAnalyticDerivative() const {return true;}

  private:

    // It is illegal to assign an adjustable constant
    const LegendrePolynomial & operator=(const LegendrePolynomial &right);

    // Here is the decay constant
    unsigned int _l;

    // Normalization type:
    NormalizationType _normType;

  };

} // namespace Local

#include "QatGenericFunctions/LegendrePolynomial.icc"


#endif


