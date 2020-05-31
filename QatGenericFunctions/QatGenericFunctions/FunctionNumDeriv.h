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

//--------------------------FunctionNumDeriv--------------------------------//
//                                                                          //
// Class FunctionNumDeriv, providing numerical derivatives                  //
//                                                                          //
//--------------------------------------------------------------------------//
#ifndef FunctionNumDeriv_h
#define FunctionNumDeriv_h 1
#include "QatGenericFunctions/AbsFunction.h"

namespace Genfun {

  class FunctionNumDeriv : public AbsFunction {
  
    FUNCTION_OBJECT_DEF(FunctionNumDeriv)

      public:

    // Constructor
    FunctionNumDeriv(const AbsFunction *arg1, unsigned int index=0);
  
    // Copy constructor.
    FunctionNumDeriv(const FunctionNumDeriv &right);
  
    // Destructor
    virtual ~FunctionNumDeriv();
  
    // Retreive function value
    virtual double operator ()(double argument) const;
    virtual double operator ()(const Argument & a) const;
  
    // Dimensionality 
    virtual unsigned int dimensionality() const;

  private:
  
    // It is illegal to assign a FunctionNumDeriv
    const FunctionNumDeriv & operator=(const FunctionNumDeriv &right);

    // The function we're differntiating.
    const AbsFunction        *_arg1;
    const unsigned int       _wrtIndex;

    // helper data and methods to let us code Argument and double similarly
    double numericalDerivative 
	( double (FunctionNumDeriv::*f)(double)const, double x ) const;
    double f_x   (double x) const;
    double f_Arg (double x) const;
    mutable Argument 	    _xArg;

  };
} // namespace Genfun
#endif
