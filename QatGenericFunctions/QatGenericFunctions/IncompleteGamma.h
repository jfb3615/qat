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

//--------------------------------------------------------------------------//
//                                                                          //
// Class IncompleteGamma, the incomplete Gamma function                     //
// Relies on, must be linked with, the GNU Scientific Library               //
//--------------------------------------------------------------------------//
#ifndef IncompleteGamma_h
#define IncompleteGamma_h 1
#include "QatGenericFunctions/AbsFunction.h"
#include "QatGenericFunctions/Parameter.h"

namespace Genfun {
  
  class IncompleteGamma : public AbsFunction  {
    
    FUNCTION_OBJECT_DEF(IncompleteGamma)
      
      public:
    
    enum Type {UPPER,LOWER,Q,P};
    
    // Constructor
    IncompleteGamma(Type type=UPPER);
    
    // Copy constructor
    IncompleteGamma(const IncompleteGamma &right);
    
    // Destructor
    virtual ~IncompleteGamma();
    
    // Retreive function value
    virtual double operator ()(double argument) const;
    virtual double operator ()(const Argument & arg) const {return operator() (arg[0]);}
    
    // Get the paramter a
    Parameter & a();     
    const Parameter & a() const;     

    // Derivative.  
    Derivative partial (unsigned int) const;
    
    // Does this function have an analytic derivative?
    virtual bool hasAnalyticDerivative() const {return true;}
    
  private:
    
    // It is illegal to assign 
    const IncompleteGamma & operator=(const IncompleteGamma &right);
    
    // Type 
    Type _type;

    // Here is the parameter of the Incomplete Gamma Function
    Parameter _a;
  };
} // namespace Genfun
#include "QatGenericFunctions/IncompleteGamma.icc"
#endif
