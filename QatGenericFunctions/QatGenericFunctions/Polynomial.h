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

#ifndef _Polynomial_h_
#define _Polynomial_h_
//-------------------------------------------------------//
// This one dimensional function takes its values from   //
// an array of coefficients, which it copies in.         //
//-------------------------------------------------------//
#include "QatGenericFunctions/AbsFunction.h"
#include "Eigen/Dense"
#include <vector>
#include <complex>
#include <initializer_list>
namespace Genfun {
class Polynomial : public AbsFunction  {
  
  FUNCTION_OBJECT_DEF(Polynomial)
    
    public:
  
  // Constructor
  template <typename ConstIterator>
  Polynomial(ConstIterator begin, ConstIterator end);

  // Initializer list constructor:
  Polynomial(std::initializer_list<double> values);
  
  // Destructor
  virtual ~Polynomial();
  
  // Copy constructor
  Polynomial(const Polynomial &right);
  
  // Retreive function value
  virtual double operator ()(double argument) const;
  virtual double operator ()(const Argument & a) const {return operator() (a[0]);}

  // Derivative.  
  Derivative partial (unsigned int) const;
  
  // Does this function have an analytic derivative?
  virtual bool hasAnalyticDerivative() const {return true;}

  // Get the roots of this polynomial:
  std::vector<std::complex<double>> getRoots() const;
  
  // Get the companion matrix of this polynomial:
  Eigen::MatrixXd getCompanionMatrix() const;

  // Get coefficients of the derivative:
  std::vector<double> coefficientsOfDerivative() const;
  
 private:
  
  // It is illegal to assign a Polynomial
  const Polynomial & operator=(const Polynomial &right);

  std::vector<double> _values;
};
}
#include "QatGenericFunctions/Polynomial.icc"
#endif
