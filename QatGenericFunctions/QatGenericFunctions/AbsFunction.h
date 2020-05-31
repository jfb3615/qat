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

//------------------------AbsFunction-----------------------------------//
//                                                                      //
//  AbsFunction, base class for function objects                        //
//                                                                      //
//----------------------------------------------------------------------//
#ifndef AbsFunction_h
#define AbsFunction_h 1
#include "QatGenericFunctions/Argument.h"

namespace Genfun {

  class AbsParameter;

  //-----------------------------------------------------------------------//
  // Exact return type of arithmentic operations.  To the user, the return //
  // type is GENFUNCTION, or const AbsFunction &.                          //
  //-----------------------------------------------------------------------//

  class FunctionProduct;
  class FunctionSum;
  class FunctionDifference;
  class FunctionQuotient;
  class FunctionNegation;
  class FunctionDirectProduct;
  class FunctionComposition;
  class ConstPlusFunction;
  class ConstTimesFunction;
  class ConstMinusFunction;
  class ConstOverFunction;
  class FunctionPlusParameter;
  class FunctionTimesParameter;
  class FunctionNumDeriv;
  class Variable;
  class FunctionNoop;
  class ParameterComposition;

  typedef FunctionNoop Derivative;

  class AbsFunction {
  
  public:
  
    // Default Constructor
    AbsFunction();
  
    // Copy Constructor:
    AbsFunction(const AbsFunction &right);
  
    // Destructor
    virtual ~AbsFunction();
  
    // Function value:  N-dimensional functions must override these:
    virtual unsigned int dimensionality() const ;      // returns 1;

    // Function value
    virtual double operator() (double argument)          const=0;   
    virtual double operator() (const Argument &argument) const=0; 

    // Every function must override this:
    virtual AbsFunction * clone() const=0;
  
    // Function composition.  Do not attempt to override:
    virtual FunctionComposition operator () (const AbsFunction &f) const;
    
    // Parameter composition.  Do not attempt to override:
    virtual ParameterComposition operator() ( const AbsParameter &p) const;

    // Derivative, (All functions)  (do not override)
    Derivative partial(const Variable &v) const;

    // Derivative (1D functions only) (do not override)
    Derivative prime() const;

    // Does this function have an analytic derivative?
    virtual bool hasAnalyticDerivative() const {return false;}

    // Derivative.  Overriders may be provided, numerical method by default!
    virtual Derivative partial(unsigned int) const;
  
  private:

    // It is illegal to assign a function.
    const AbsFunction & operator=(const AbsFunction &right);
  
  };

FunctionProduct           operator * (const AbsFunction &op1, const AbsFunction &op2);
FunctionSum               operator + (const AbsFunction &op1, const AbsFunction &op2);
FunctionDifference        operator - (const AbsFunction &op1, const AbsFunction &op2);
FunctionQuotient          operator / (const AbsFunction &op1, const AbsFunction &op2);
FunctionNegation          operator - (const AbsFunction &op1);

ConstTimesFunction        operator * (double c, const AbsFunction &op2);
ConstPlusFunction         operator + (double c, const AbsFunction &op2);
ConstMinusFunction        operator - (double c, const AbsFunction &op2);
ConstOverFunction         operator / (double c, const AbsFunction &op2);

ConstTimesFunction        operator * (const AbsFunction &op2, double c);
ConstPlusFunction         operator + (const AbsFunction &op2, double c);
ConstPlusFunction         operator - (const AbsFunction &op2, double c);
ConstTimesFunction        operator / (const AbsFunction &op2, double c);

FunctionTimesParameter    operator * (const AbsFunction &op1, const AbsParameter &op2);
FunctionPlusParameter     operator + (const AbsFunction &op1, const AbsParameter &op2);
FunctionPlusParameter     operator - (const AbsFunction &op1, const AbsParameter &op2);
FunctionTimesParameter    operator / (const AbsFunction &op1, const AbsParameter &op2);

FunctionTimesParameter    operator * (const AbsParameter   &op1, const AbsFunction &op2);
FunctionPlusParameter     operator + (const AbsParameter   &op1, const AbsFunction &op2);
FunctionPlusParameter     operator - (const AbsParameter   &op1, const AbsFunction &op2);
FunctionTimesParameter    operator / (const AbsParameter   &op1, const AbsFunction &op2);

FunctionDirectProduct     operator % (const AbsFunction &op1, const AbsFunction &op2);

typedef const AbsFunction & GENFUNCTION;

} // namespace Genfun


#define FUNCTION_OBJECT_DEF(classname) \
public:                                \
 using AbsFunction::operator();		       \
  virtual classname *clone() const;            \
private:                               

//----------------------------------------------------------------------------
//
//  This macro implements the ugly boilerplate
//
  
#define FUNCTION_OBJECT_IMP(classname)       \
classname *classname::clone() const          \
{                                            \
  return new classname(*this);               \
}


#define FUNCTION_OBJECT_IMP_INLINE(classname)       \
inline classname *classname::clone() const          \
{                                            \
  return new classname(*this);               \
}


//----------------------------------------------------------------------------


#include "QatGenericFunctions/FunctionProduct.h"
#include "QatGenericFunctions/FunctionSum.h"
#include "QatGenericFunctions/FunctionDifference.h"
#include "QatGenericFunctions/FunctionQuotient.h"
#include "QatGenericFunctions/FunctionNegation.h"
#include "QatGenericFunctions/FunctionDirectProduct.h"
#include "QatGenericFunctions/FunctionComposition.h"
#include "QatGenericFunctions/ConstPlusFunction.h"
#include "QatGenericFunctions/ConstTimesFunction.h"
#include "QatGenericFunctions/ConstMinusFunction.h"
#include "QatGenericFunctions/ConstOverFunction.h"
#include "QatGenericFunctions/FunctionPlusParameter.h"
#include "QatGenericFunctions/FunctionTimesParameter.h"
#include "QatGenericFunctions/FunctionNoop.h"
#include "QatGenericFunctions/ParameterComposition.h"

#endif
