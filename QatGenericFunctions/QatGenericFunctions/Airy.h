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

//---------------------LegendrePolynomial-----------------------------------//
//                                                                          //
// Class Airy.  An Airy function. Relies on,                                //
// must be linked with, the gnu scientific library.                         //
//                                                                          //
//--------------------------------------------------------------------------//
#ifndef Airy_h
#define Airy_h 1
#include "QatGenericFunctions/AbsFunction.h"
namespace Genfun {
  
  class Airy : public AbsFunction  {
    
    FUNCTION_OBJECT_DEF(Airy)

    public:

    enum Type {Ai, Bi};

    // Constructor:  Use this one and you will get a Airy function Ai or Bi
    Airy (Type type);

    // Copy constructor
    Airy(const Airy &right);
  
    // Destructor
    virtual ~Airy();
  
    // Retreive function value
    virtual double operator ()(double argument) const;
    virtual double operator ()(const Argument & a) const {return operator() (a[0]);}
  
  private:

    // It is illegal to assign Airy function
    const Airy & operator=(const Airy &right);

    // The type and order of the Airy function
    Type      _type;
  
  };

} // namespace Genfun


#include "QatGenericFunctions/Airy.icc"
#endif
