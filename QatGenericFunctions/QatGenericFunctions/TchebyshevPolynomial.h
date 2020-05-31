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

//---------------------TchebyshevPolynomial---------------------------------//
//                                                                          //
//--------------------------------------------------------------------------//
#ifndef TchebyshevPolynomial_h
#define TchebyshevPolynomial_h 1
#include "QatGenericFunctions/AbsFunction.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/Defs.h"
namespace Genfun {

  class TchebyshevPolynomial : public AbsFunction  {
  
    FUNCTION_OBJECT_DEF(TchebyshevPolynomial)

    public:

    // Type 
    enum Type {FirstKind, SecondKind};



    // Constructor

    // STD means a normal TchebyshevPolynomial.  TWIDDLE means that the factor 
    // sqrt(pi/2) for n=0, sqrt(pi) for n!=0 is included already, making 
    // the set orthonormal.   


    TchebyshevPolynomial(unsigned int l, Type type=FirstKind, NormalizationType normType=STD);

    // Copy constructor
    TchebyshevPolynomial(const TchebyshevPolynomial &right);
  
    // Destructor
    virtual ~TchebyshevPolynomial();
  
    // Retreive function value
    virtual double operator ()(double argument) const;
    virtual double operator ()(const Argument & a) const {return operator() (a[0]);}
  
    // Get the integer variable l
    unsigned int n() const;

    // Get the type
    Type type () const;

    // Derivative.  
    Derivative partial (unsigned int) const;

    // Does this function have an analytic derivative?
    virtual bool hasAnalyticDerivative() const {return true;}
  

  private:

    // It is illegal to assign an adjustable constant
    const TchebyshevPolynomial & operator=(const TchebyshevPolynomial &right);

    // The order:
    unsigned int _n;

    // The type
    Type _type;

    // The normalization
    NormalizationType _normType;

  };
} // namespace Genfun



#endif


