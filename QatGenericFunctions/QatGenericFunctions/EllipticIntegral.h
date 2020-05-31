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

//---------------------EllipticIntegral---------------------------------------//
//                                                                            //
//  Three classes:  Genfun::EllipticIntegral::FirstKind                       //
//                  Genfun::EllipticIntegral::SecondKind                      //
//                  Genfun::EllipticIntegral::ThirdKind                       //
//                                                                            //
//  Relies on, must be linked with, the gnu scientific library                //
//--------------------------------------------------------------------------  //
#ifndef EllipticIntegral_h
#define EllipticIntegral_h 1
#include "QatGenericFunctions/AbsFunction.h"
#include "QatGenericFunctions/Parameter.h"

namespace Genfun {
namespace EllipticIntegral {

  class FirstKind : public AbsFunction  {

    FUNCTION_OBJECT_DEF(FirstKind)

      public:

    // Constructor:  
    FirstKind ();

    // Copy constructor
    FirstKind(const FirstKind &right);
  
    // Destructor
    virtual ~FirstKind();
  
    // Retreive function value
    virtual double operator ()(double argument) const;
    virtual double operator ()(const Argument & a) const {return operator() (a[0]);}
  
    // Get the k-parameter.  Default value = 1.0.
    Parameter & k(); 
    const Parameter & k() const; 

  private:

    // It is illegal to assign an adjustable constant
    const FirstKind & operator=(const FirstKind &right);

    Parameter _k;    // the k parameter
  
  };

  class SecondKind : public AbsFunction  {

    FUNCTION_OBJECT_DEF(SecondKind)

      public:

    // Constructor:  
    SecondKind ();

    // Copy constructor
    SecondKind(const SecondKind &right);
  
    // Destructor
    virtual ~SecondKind();
  
    // Retreive function value
    virtual double operator ()(double argument) const;
    virtual double operator ()(const Argument & a) const {return operator() (a[0]);}
  
    // Get the k-parameter.  Default value = 1.0.
    Parameter & k(); 
    const Parameter & k() const; 

  private:

    // It is illegal to assign an adjustable constant
    const SecondKind & operator=(const SecondKind &right);

    Parameter _k;    // the k parameter
  
  };

  class ThirdKind : public AbsFunction  {

    FUNCTION_OBJECT_DEF(ThirdKind)

      public:

    // Constructor:  
    ThirdKind ();

    // Copy constructor
    ThirdKind(const ThirdKind &right);
  
    // Destructor
    virtual ~ThirdKind();
  
    // Retreive function value
    virtual double operator ()(double argument) const;
    virtual double operator ()(const Argument & a) const {return operator() (a[0]);}
  
    // Get the k-parameter.  Default value = 1.0.
    Parameter & k(); 
    const Parameter & k() const; 

    // Get the n-parameter.  Default value = 1.0.
    Parameter & n(); 
    const Parameter & n() const; 

  private:

    // It is illegal to assign an adjustable constant
    const ThirdKind & operator=(const ThirdKind &right);

    Parameter _k;    // the k parameter
    Parameter _n;    // the n parameter
  
  };

} // end namespace EllipticIntegral 
} // end namespace Genfun

#include "QatGenericFunctions/EllipticIntegral.icc"
#endif
