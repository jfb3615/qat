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

//---------------------NormalDistribution-----------------------------------//
//                                                                          //
// Class NormalDistribution                                                 //
//                                                                          //
//--------------------------------------------------------------------------//
#ifndef NormalDistribution_h
#define NormalDistribution_h 1
#include "QatGenericFunctions/AbsFunction.h"
#include "QatGenericFunctions/Parameter.h"

namespace Genfun {

  class NormalDistribution : public AbsFunction  {

    FUNCTION_OBJECT_DEF(NormalDistribution)

      public:

    // Constructor
    NormalDistribution();

    // Copy constructor
    NormalDistribution(const NormalDistribution &right);
  
    // Destructor
    virtual ~NormalDistribution();
  
    // Retreive function value
    virtual double operator ()(double argument) const;
    virtual double operator ()(const Argument & a) const {return operator() (a[0]);}
  
    // Get the mean of the NormalDistribution
    Parameter & mean(); 
    const Parameter & mean() const; 

    // Get the sigma of the NormalDistribution
    Parameter & sigma();
    const Parameter & sigma() const;

    // Derivative.  
    Derivative partial (unsigned int) const;

    // Does this function have an analytic derivative?
    virtual bool hasAnalyticDerivative() const {return true;}

  
  private:

    // It is illegal to assign an adjustable constant
    const NormalDistribution & operator=(const NormalDistribution &right);

    // Here is the decay constant
    Parameter _mean;

    // Here is the sigma
    Parameter _sigma;
  };
} // namespace Genfun

#endif
