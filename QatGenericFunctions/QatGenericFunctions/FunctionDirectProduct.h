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

//--------------------------FunctionDirectProduct---------------------------//
//                                                                          //
// FunctionDirectProduct, result of multiplication of taking the direct     //
// product of two functions:  f(x)*g(y)= h(x,y).  The direct product always //
// gives a function taking an argument of higher dimensionality than the    //
// arguments.                                                               //
//                                                                          //
//--------------------------------------------------------------------------//
#ifndef FunctionDirectProduct_h
#define FunctionDirectProduct_h 1
#include "QatGenericFunctions/AbsFunction.h"

namespace Genfun {

  class FunctionDirectProduct : public AbsFunction {

    FUNCTION_OBJECT_DEF(FunctionDirectProduct)

      public:

    // Constructor
    FunctionDirectProduct(const AbsFunction *arg1, const AbsFunction *arg2);

    // Copy constructor
    FunctionDirectProduct(const FunctionDirectProduct &right);
  
    // Destructor
    virtual ~FunctionDirectProduct();

    // Retreive function value
    virtual double operator ()(double argument) const;    // Gives an error.
    virtual double operator ()(const Argument & a) const; // Must use this one

    // Dimensionality
    virtual unsigned int dimensionality() const;

    // Derivative.  
    Derivative partial (unsigned int) const;

    // Does this function have an analytic derivative?
    virtual bool hasAnalyticDerivative() const {return true;}

  private:

    // It is illegal to assign a FunctionDirectProduct
    const FunctionDirectProduct & operator=(const FunctionDirectProduct &right);

    AbsFunction *_arg1;
    AbsFunction *_arg2;
    unsigned int _m;  // dimension of arg1
    unsigned int _n;  // dimension of arg2

  };
} // namespace Genfun
#endif
