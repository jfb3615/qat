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

//---------------------Bessel-------------------------------------------------//
//                                                                            //
// Class Bessel, providing Bessel Functions  The Namespace "FractionalORder"  //
// and "Integral order" are nested here, so that you fully specify the class  //
// like this:                                                                 //
//                                                                            //
//                 Genfun::FractionalOrder::Bessel                            //
//                                                                            //
//                              or                                            //
//                                                                            //
//                 Genfun::IntegralOrder::Bessel                              //
//                                                                            //
//                                                                            //
//--------------------------------------------------------------------------  //
#ifndef Bessel_h
#define Bessel_h 1
#include "QatGenericFunctions/AbsFunction.h"
#include "QatGenericFunctions/Parameter.h"
namespace Genfun {

namespace FractionalOrder { 

  class Bessel : public AbsFunction  {

    FUNCTION_OBJECT_DEF(Bessel)

      public:

    // Enumerated type:
    enum Type {FirstKind, SecondKind, ModifiedFirstKind, ModifiedSecondKind};

    // Constructor:  Use this one and you will get a Bessel function of
    // integer order
    Bessel (Type type);

    // Copy constructor
    Bessel(const Bessel &right);
  
    // Destructor
    virtual ~Bessel();
  
    // Retreive function value
    virtual double operator ()(double argument) const;
    virtual double operator ()(const Argument & a) const {return operator() (a[0]);}
  
    // Get the order of the Bessel Function.  Default value, 0.0.  If modified the
    // Bessel function 
    Parameter & n(); 
    const Parameter & n() const; 
    
    // Derivative.  
    Derivative partial (unsigned int) const;

    // Does this function have an analytic derivative?
    virtual bool hasAnalyticDerivative() const {return true;}

  private:

    // It is illegal to assign an adjustable constant
    const Bessel & operator=(const Bessel &right);

    // The type and order of the Bessel function
    Type      _type;
    Parameter _n; // the fractional order:
  
  };
} // namespace FractionalOrder

namespace IntegralOrder { 

  class Bessel : public AbsFunction  {

    FUNCTION_OBJECT_DEF(Bessel)

      public:

    // Enumerated type:
    enum Type {FirstKind, SecondKind, ModifiedFirstKind, ModifiedSecondKind};

    // Constructor:  Use this one and you will get a Bessel function of
    // integer order
    Bessel (unsigned int n, Type type);

    // Copy constructor
    Bessel(const Bessel &right);
  
    // Destructor
    virtual ~Bessel();
  
    // Retreive function value
    virtual double operator ()(double argument) const;
    virtual double operator ()(const Argument & a) const {return operator() (a[0]);}
  
    // Derivative.  
    Derivative partial (unsigned int) const;
    
    // Does this function have an analytic derivative?
    virtual bool hasAnalyticDerivative() const {return true;}

  private:

    // It is illegal to assign an adjustable constant
    const Bessel & operator=(const Bessel &right);

    // The type and order of the Bessel function
    Type                _type;
    unsigned  int       _n; 

  };
} // namespace IntegralOrder

} // namespace Genfun


#include "QatGenericFunctions/Bessel.icc"
#endif
