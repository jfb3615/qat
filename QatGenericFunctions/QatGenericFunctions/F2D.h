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
// This class is an adaptor from any function                         //
//                                                                    //
//                   (double *f)(double x, double y)                  //
//                                                                    //
// of two real variables, to generic functions. This allows one       //
// to plot, differentiate, sum, compose the function.                 //
//                                                                    //
//--------------------------------------------------------------------//
#ifndef F2D_h
#define F2D_h 1
#include "QatGenericFunctions/AbsFunction.h"
namespace Genfun {

  class F2D : public AbsFunction  {
    
    FUNCTION_OBJECT_DEF(F2D)

      public:

    typedef double (*FcnPtr)(double,double);

    // Constructor
    F2D(FcnPtr fcn):p(fcn){};

    // Destructor
    virtual ~F2D(){};
  
    // Copy constructor
    F2D(const F2D &right):AbsFunction(),p(right.p){};
  
    // Retreive function value
    virtual double operator ()(double ) const {
      throw std::runtime_error("F2D: dimensionality error in operator ()"); 
    }

    virtual double operator ()(const Argument & a) const {return (*p)(a[0], a[1]);}

    virtual unsigned int dimensionality() const { return 2;} 

  private:

    // It is illegal to assign a F2D
    const F2D & operator=(const F2D &right);

    // Data:
    FcnPtr p;

  };
  FUNCTION_OBJECT_IMP_INLINE(F2D)
} // namespace Genfun
#endif
