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

//----------------------------------- --------------------------------------//
//                                                                          //
//  Class KVector for internal use                                          //
//                                                                          //
//--------------------------------------------------------------------------//
#ifndef KVector_h
#define KVector_h 1
#include "QatGenericFunctions/AbsFunction.h"
namespace Genfun {

  class KVector : public AbsFunction  {
  
    FUNCTION_OBJECT_DEF(KVector)

      public:

    // Constructor
    KVector(unsigned int dimensionality=1,
	    double       value=1);
  
    // Copy constructor
    KVector(const KVector &right);
  
    // Destructor
    virtual ~KVector();
  
    // Retrieve function value
    virtual double operator ()(double argument) const; 
    virtual double operator ()(const Argument & a) const;

    // Get the dimensionality, as specified in the constructor:
    virtual unsigned int dimensionality() const ;  
  
    // Derivative.  
    Derivative partial (unsigned int) const;

    // Does this function have an analytic derivative?
    virtual bool hasAnalyticDerivative() const {return true;}

  private:

    // It is illegal to assign a fixed constant
    const KVector & operator=(const KVector &right);


    // The dimensionality, length of the argument list:
    unsigned int _dimensionality;

    // The value of the index() component...
    double _value;
  };

} // namespace Genfun
#endif
