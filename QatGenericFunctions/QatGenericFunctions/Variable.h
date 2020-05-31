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

//----------------------X------------ --------------------------------------//
//                                                                          //
//  Class Variable                                                          //
//                                                                          //
//  Variable is a function that returns the variable itself. Dimensionality //
//  is specified in the constructor;  if it is greater than one (default)   //
//  it means that the variable is part of a vector. Which part is specified //
//  by the "selection index" argument to the constructor.                   //
//                                                                          //
//                                                                          //
//--------------------------------------------------------------------------//
#ifndef Variable_h
#define Variable_h 1
#include "QatGenericFunctions/AbsFunction.h"
namespace Genfun {

  class Variable : public AbsFunction  {
  
    FUNCTION_OBJECT_DEF(Variable)

      public:

    // Constructor
    Variable(unsigned int selectionIndex=0,
             unsigned int dimensionality=1);
  
    // Copy constructor
    Variable(const Variable &right);
  
    // Destructor
    virtual ~Variable();
  
    // Retrieve function value
    virtual double operator ()(double argument) const; 
    virtual double operator ()(const Argument & a) const;

    // Get the dimensionality, as specified in the constructor:
    virtual unsigned int dimensionality() const ;  
  
    // Get the selectionIndex:
    unsigned int index() const;

    // Derivative.  
    Derivative partial (unsigned int) const;

    // Does this function have an analytic derivative?
    virtual bool hasAnalyticDerivative() const {return true;}

  private:

    // It is illegal to assign a fixed constant
    const Variable & operator=(const Variable &right);

    // The selection index is used to select a particular element from
    // the argument.  By default it is zero.....
    unsigned int _selectionIndex;

    // The dimensionality, length of the argument list:
    unsigned int _dimensionality;

  };

} // namespace Genfun
#endif
