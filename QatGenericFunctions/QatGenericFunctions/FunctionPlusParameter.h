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

//--------------------------FunctionPlusParameter---------------------------//
//                                                                          //
// ConstOverFunction, result of adding a function and a parameter           //
//                                                                          //
//--------------------------------------------------------------------------//

#ifndef FunctionPlusParameter_h
#define FunctionPlusParameter_h 1
#include "QatGenericFunctions/AbsFunction.h"

namespace Genfun {
  
  class FunctionPlusParameter : public AbsFunction  {
    
    FUNCTION_OBJECT_DEF(FunctionPlusParameter)
      
      public:
    
    // Constructor
    FunctionPlusParameter(const AbsParameter *parameter, const AbsFunction *function);
  
    // Copy constructor
    FunctionPlusParameter(const FunctionPlusParameter &right);

    // Destructor
    virtual ~FunctionPlusParameter();
  
    // Retreive function value
    virtual double operator ()(double argument) const;
    virtual double operator ()(const Argument & a) const;

    // Dimensionality 
    virtual unsigned int dimensionality() const;

    // Derivative.  
    Derivative partial (unsigned int) const;

    // Does this function have an analytic derivative?
    virtual bool hasAnalyticDerivative() const {return true;}

  private:

    // It is illegal to assign a FunctionPlusParameter
    const FunctionPlusParameter & operator=(const FunctionPlusParameter &right);

    const AbsFunction  *_function;
    AbsParameter       *_parameter;

  };
} // namespace Genfun
#endif
