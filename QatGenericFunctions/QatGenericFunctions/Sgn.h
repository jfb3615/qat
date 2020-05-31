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

//----------------------Sgn---------- --------------------------------------//
//                                                                          //
//  Class Sgn                                                               //
//                                                                          //
//  Sgn is a function that returns the sign its argument                    //
//--------------------------------------------------------------------------//
#ifndef Sgn_h
#define Sgn_h 1
#include "QatGenericFunctions/AbsFunction.h"
namespace Genfun {
  class Sgn : public AbsFunction  {

    FUNCTION_OBJECT_DEF(Sgn)
  
      public:

    // Constructor
    Sgn();
  
    // Copy constructor
    Sgn(const Sgn &right);
  
    // Destructor
    virtual ~Sgn();
  
    // Retrieve function value
    virtual double operator ()(double argument) const; 
    virtual double operator ()(const Argument & a) const {return operator() (a[0]);}
  
  private:

    // It is illegal to assign a fixed constant
    const Sgn & operator=(const Sgn &right);

  };
}
#include "QatGenericFunctions/Sgn.icc"
#endif
