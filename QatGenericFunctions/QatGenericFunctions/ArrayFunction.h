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

#ifndef _ArrayFunction_h_
#define _ArrayFunction_h_
//-------------------------------------------------------//
// This one dimensional function takes its values from   //
// an array..which it copies in.                         //
//-------------------------------------------------------//
#include "QatGenericFunctions/AbsFunction.h"
#include <vector>
#include <initializer_list>
namespace Genfun {
class ArrayFunction : public AbsFunction  {
  
  FUNCTION_OBJECT_DEF(ArrayFunction)
    
    public:
  
  // Constructor
  ArrayFunction(const double *begin, const double *end);

  // Initializer list constructor:
  ArrayFunction(std::initializer_list<double> values);
  
  // Destructor
  virtual ~ArrayFunction();
  
  // Copy constructor
  ArrayFunction(const ArrayFunction &right);
  
  // Retreive function value
  virtual double operator ()(double argument) const;
  virtual double operator ()(const Argument & a) const {return operator() (a[0]);}

  // Derivative.  
  Derivative partial (unsigned int) const;
  
  // Does this function have an analytic derivative?
  virtual bool hasAnalyticDerivative() const {return true;}
    
 private:
  
  // It is illegal to assign a ArrayFunction
  const ArrayFunction & operator=(const ArrayFunction &right);

  std::vector<double> _values;
};
}
#endif
