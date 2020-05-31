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

//--------------------------ConstMinusFunction------------------------------//
//                                                                          //
// ConstMinusFunction, result of subtracting a function from constant       //
//                                                                          //
//--------------------------------------------------------------------------//

#ifndef ConstMinusFunction_hh
#define ConstMinusFunction_hh 
#include "QatGenericFunctions/AbsFunction.h"

namespace Genfun {

  class ConstMinusFunction : public AbsFunction  {

    FUNCTION_OBJECT_DEF(ConstMinusFunction)
  
      public:
  
    // Constructor
    ConstMinusFunction(double constant, const AbsFunction *arg);
  
    // Copy constructor
    ConstMinusFunction(const ConstMinusFunction &right);

    // Destructor
    virtual ~ConstMinusFunction();
  
    // Retreive function value
    virtual double operator ()(double argument) const;
    virtual double operator ()(const Argument & a) const;

    // Dimensionality 
    virtual unsigned int dimensionality() const;

    // Derivative.  
    Derivative partial (unsigned int) const;

    // Does this function.hhave an analytic derivative?
    virtual bool hasAnalyticDerivative() const {return true;}

  private:

    // It is illegal to assign a ConstMinusFunction
    const ConstMinusFunction & operator=(const ConstMinusFunction &right);

    double             _constant;
    const AbsFunction *_arg;
  };
} // namespace Genfun
#endif
