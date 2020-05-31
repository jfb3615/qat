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

//-------------------------------------------------------------//
//                                                             //
// This functional performs Romberg integration on a function  //
// between lower bound and upper bound b.                      //
//                                                             //
// Two types:  OPEN: use open quadrature formula               //
//                   for improper integrals                    //
//             CLOSED (default) use closed quadrature          //
//                   formula.                                  //
//                                                             //
// Two types of convergence criterion:                         //
//             ABSOLUTE: convergence when value of integral    //
//                       changes by less than some amount.     //
//             RELATIVE: convergence when the value of         //
//                       integral changes by less than some    //
//                       fraction (default)                    //
//                                                             //
//-------------------------------------------------------------//

#ifndef _RombergIntegrator_h_
#define _RombergIntegrator_h_
#include "QatGenericFunctions/AbsFunctional.h"

namespace Genfun {

  class RombergIntegrator:public AbsFunctional {

  public:
  
    // Type definition:
    typedef enum {CLOSED, OPEN} Type;

    // Type definition:
    typedef enum {ABSOLUTE, RELATIVE} ConvergenceCriterion;

    // Constructor:
    RombergIntegrator(double a, double b, Type=CLOSED, ConvergenceCriterion=RELATIVE);

    // Copy Constructor:
    RombergIntegrator(const RombergIntegrator &);

    // Assignment Operator:
    RombergIntegrator & operator=(const RombergIntegrator &) ;

    // Destructor:
    ~RombergIntegrator();

    // Take the definite integral of a function between the bounds:
    virtual double operator () (const AbsFunction & function) const;

    // Retrieve the number of function calls for the last operation:
    unsigned int numFunctionCalls() const;

    // Algorithmic parameters:

    // Desired precision (default 1.0E-06)
    void setEpsilon(double eps);

    // Maximum number of iterations (default 20(closed) 14 (open))
    void setMaxIter (unsigned int maxIter);

    // Minimum order:
    void setMinOrder (unsigned int order);


  private:

    class Clockwork;
    Clockwork *c;

  };
} // namespace Genfun
#endif
