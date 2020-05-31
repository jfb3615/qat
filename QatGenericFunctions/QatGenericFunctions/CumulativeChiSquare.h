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

//---------------------CumulativeChiSquare----------------------------------//
//                                                                          //
// Class CumulativeChiSquare P(chi^2 | N)                                   //
// Joe Boudreau, October 2000                                               //
//                                                                          //
//--------------------------------------------------------------------------//
#ifndef CumulativeChiSquare_h
#define CumulativeChiSquare_h 1
#include "QatGenericFunctions/AbsFunction.h"
#include "QatGenericFunctions/Parameter.h"
namespace Genfun {

  class CumulativeChiSquare : public AbsFunction  {

    FUNCTION_OBJECT_DEF(CumulativeChiSquare)

      public:

    // Constructor
    CumulativeChiSquare(unsigned int nDof);

    // Copy constructor
    CumulativeChiSquare(const CumulativeChiSquare &right);
  
    // Destructor
    virtual ~CumulativeChiSquare();
  
    // Retreive function value
    virtual double operator ()(double argument) const;
    virtual double operator ()(const Argument & a) const {return operator() (a[0]);}
  
    // Get the number of degrees of freedom
    unsigned int nDof() const;

  private:

    // It is illegal to assign an adjustable constant
    const CumulativeChiSquare & operator=(const CumulativeChiSquare &right);

    // Number of degrees fo freedom.
    unsigned int _nDof;
  
    // Here is the "work function"
    const AbsFunction *_function;

    // This function is needed in all constructors:
    void create(); 

  };
} // namespace Genfun
#include "QatGenericFunctions/CumulativeChiSquare.icc"
#endif
