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

//--------------------------------------------------------------------//
// This class is an adaptor from any function (double *f)(double x)   //
// of one real variable, to generic functions. This allows one        //
// to plot, differentiate, sum, compose, etc, any standard C or       //
// C++ math function by converting it to a Generic Function.          //
//                                                                    //
//--------------------------------------------------------------------//
#ifndef F1D_h
#define F1D_h 1
#include "QatGenericFunctions/AbsFunction.h"
namespace Genfun {

  class F1D : public AbsFunction  {
    
    FUNCTION_OBJECT_DEF(F1D)

      public:

    typedef double (*FcnPtr)(double);

    // Constructor
    F1D(FcnPtr fcn):p(fcn){};

    // Destructor
    virtual ~F1D(){};
  
    // Copy constructor
    F1D(const F1D &right):AbsFunction(),p(right.p){};
  
    // Retreive function value
    virtual double operator ()(double x) const {
      return (*p)(x);
    }

    virtual double operator ()(const Argument & a) const {return operator() (a[0]);}

  private:

    // It is illegal to assign a F1D
    const F1D & operator=(const F1D &right);

    // Data:
    FcnPtr p;

  };
  FUNCTION_OBJECT_IMP_INLINE(F1D)
} // namespace Genfun
#endif
