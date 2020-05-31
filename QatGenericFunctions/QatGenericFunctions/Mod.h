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

#ifndef Mod_h
#define Mod_h 1
#include "QatGenericFunctions/AbsFunction.h"
namespace Genfun {
  class Mod : public AbsFunction  {

    FUNCTION_OBJECT_DEF(Mod)
  
      public:

    // Constructor
    Mod(double y);
  
    // Copy constructor
    Mod(const Mod &right);
  
    // Destructor
    virtual ~Mod();
  
    // Retrieve function value

    virtual double operator ()(double argument) const; 
    virtual double operator ()(const Argument & a) const {return operator() (a[0]);}
  
  private:

    // It is illegal to assign a fixed constant
    const Mod & operator=(const Mod &right);

    double _y; // mod (as a double)

  };
} // namespace Genfun
#endif
