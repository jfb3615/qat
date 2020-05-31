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

// -------------------------------------------------------------------//
//                                                                    //
// AbsParameter, a base class for all parameters, which are           //
// essentially adjustable constants.                                  //
//                                                                    //
//--------------------------------------------------------------------//
#ifndef _AbsParameter_h_
#define _AbsParameter_h_

namespace Genfun {
  class Parameter;
  class ParameterSum;
  class ParameterDifference;
  class ParameterProduct;
  class ParameterQuotient;
  class ParameterNegation;
  class ConstPlusParameter;
  class ConstMinusParameter;
  class ConstTimesParameter;
  class ConstOverParameter;

  class AbsParameter {
    
  public:
    
    // Default Constructor
    AbsParameter();
    
    // Copy constructor:
    AbsParameter(const AbsParameter &);

    // Destructor
    virtual ~AbsParameter();
  
    // Parameter value
    virtual double getValue()  const=0;   

    // Every parameter must override this:
    AbsParameter * clone() const;

    // Extra lingual type information:
    virtual Parameter *parameter() {return 0;}
    virtual const Parameter *parameter() const {return 0;}

  private:

    // Cannot assign an AbsParameter:
    AbsParameter & operator= (const AbsParameter &);

    virtual AbsParameter *_clone() const=0;

  };

// Now for some additional operations:


ConstTimesParameter               operator * (double c, const AbsParameter &op2);
ConstPlusParameter                operator + (double c, const AbsParameter &op2);
ConstMinusParameter               operator - (double c, const AbsParameter &op2);
ConstOverParameter                operator / (double c, const AbsParameter &op2);

ConstTimesParameter               operator * (const AbsParameter &op2, double c);
ConstPlusParameter                operator + (const AbsParameter &op2, double c);
ConstPlusParameter                operator - (const AbsParameter &op2, double c);
ConstTimesParameter               operator / (const AbsParameter &op2, double c);


ParameterProduct                  operator * (const AbsParameter &op1, const AbsParameter &op2);
ParameterSum                      operator + (const AbsParameter &op1, const AbsParameter &op2);
ParameterDifference               operator - (const AbsParameter &op1, const AbsParameter &op2);
ParameterQuotient                 operator / (const AbsParameter &op1, const AbsParameter &op2);
ParameterNegation                 operator - (const AbsParameter &op1);

typedef const AbsParameter & GENPARAMETER;

} // namespace Genfun   

// Poor man's covariant return type:
#define PARAMETER_OBJECT_DEF(classname)              \
public:                                              \
  classname *clone() const;                          \
private:                                             \
  virtual AbsParameter* _clone() const;

// Poor man's covariant return type:  
#define PARAMETER_OBJECT_IMP(classname)              \
classname *classname::clone () const {               \
  return (classname *) _clone();                     \
}                                                    \
AbsParameter *classname::_clone () const {           \
  return new classname(*this);                       \
}


#include "QatGenericFunctions/ParameterProduct.h"
#include "QatGenericFunctions/ParameterSum.h"
#include "QatGenericFunctions/ParameterDifference.h"
#include "QatGenericFunctions/ParameterQuotient.h"
#include "QatGenericFunctions/ParameterNegation.h"
#include "QatGenericFunctions/ConstPlusParameter.h"
#include "QatGenericFunctions/ConstMinusParameter.h"
#include "QatGenericFunctions/ConstTimesParameter.h"
#include "QatGenericFunctions/ConstOverParameter.h"

#endif
