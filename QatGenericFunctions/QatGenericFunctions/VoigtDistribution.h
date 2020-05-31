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

//---------------------VoigtDistribution------------------------------------//
//                                                                          //
// This function represents a Cauchy (Lorentian, Breit-Wigner) convolved    //
// with a Gaussian                                                          //
//                                                                          //
//--------------------------------------------------------------------------//
#ifndef VoigtDistribution_h
#define VoigtDistribution_h 1
#include "QatGenericFunctions/AbsFunction.h"
#include "QatGenericFunctions/Parameter.h"
namespace Genfun {

  class VoigtDistribution : public AbsFunction  {
    
    FUNCTION_OBJECT_DEF(VoigtDistribution)
      
      public:
    
    // Constructor
    VoigtDistribution();
    
    // Copy constructor
    VoigtDistribution(const VoigtDistribution &right);
    
    // Destructor
    virtual ~VoigtDistribution();
    
    // Retreive function value
    virtual double operator ()(double argument) const;
    virtual double operator ()(const Argument & a) const {return operator() (a[0]);}
    
    // Get the parameter delta (width of Cauchy/Lorentian/Breit-Wigner)
    Parameter & delta(); 

    // Get the parameter (width of Gaussian/normal)
    Parameter & sigma(); 

    
  private:
    
    // It is illegal to assign an adjustable constant
    const VoigtDistribution & operator=(const VoigtDistribution &right);
    
    // Here are the two parameters delta and sigma:

    Parameter       _delta;
    Parameter       _sigma;


  };
} // namespace Genfun
#endif
