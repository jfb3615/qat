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

//---------------------BetaDistribution-------------------------------------//
//                                                                          //
// Class BetaDistribution:                                                  //
//                                                                          //
// x^(a-1)(1-x)^beta-1 Gamma(a+b)/Gamma(a)/Gamma(b);                        //
//                                                                          //
//--------------------------------------------------------------------------//
#ifndef _BetaDistribution_h
#define _BetaDistribution_h 1
#include "QatGenericFunctions/AbsFunction.h"
#include "QatGenericFunctions/Parameter.h"
namespace Genfun {

  class BetaDistribution : public AbsFunction  {
    
    FUNCTION_OBJECT_DEF(BetaDistribution)
      
      public:
    
    // Constructor
    BetaDistribution();
    
    // Copy constructor
    BetaDistribution(const BetaDistribution &right);
    
    // Destructor
    virtual ~BetaDistribution();
    
    // Retreive function value
    virtual double operator ()(double argument) const;
    virtual double operator ()(const Argument & a) const {return operator() (a[0]);}
    
    // Get the paramter alpha
    Parameter & alpha(); 
    
    // Get the paramter alpha
    Parameter & beta(); 
    
  private:
    
    // It is illegal to assign an adjustable constant
    const BetaDistribution & operator=(const BetaDistribution &right);
    
    // Here are the two parameters alpha and beta:
    
    Parameter _alpha;
    Parameter _beta;
    
    
  };
} // namespace Genfun
#endif
