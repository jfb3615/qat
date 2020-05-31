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

#ifndef _SimpleRKStepper_h_
#define _SimpleRKStepper_h_
#include "QatGenericFunctions/RKIntegrator.h"
//
// This is a stepper that steps with stepsize h using an
// Explicit Runge-Kutte method determined by a Butcher
// tableau. 
//
// The stepsize is therefore not adaptively determined;
// so this type of stepper is mostly useful in attempting
// to categorize the different explicit RK Algorithms
// and probably not for real-life problems unless one has
// some a priori knowledge about how to set the stepsize.
//
#include "QatGenericFunctions/ButcherTableau.h"
namespace Genfun {
  class SimpleRKStepper:public RKIntegrator::RKStepper{
    
  public:
    
    // Constructor:
    SimpleRKStepper(const ButcherTableau & tableau,
		    double stepsize);

    // Destructor:
    virtual ~SimpleRKStepper();

    // Take a step:
    virtual void step (const std::shared_ptr<RKIntegrator::RKData> & data,  // functions 
		       const RKIntegrator::RKData::Data & sdata, // start point
		       RKIntegrator::RKData::Data       & ddata, // end point
		       double                         timeLimit  // time limit 
		       ) const ;
    // Clone:
    virtual SimpleRKStepper *clone() const;
    
private:

  ButcherTableau tableau;
  double stepsize;

  };
}

#endif
