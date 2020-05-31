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

#include "QatGenericFunctions/AdaptiveRKStepper.h"
#include "QatGenericFunctions/EmbeddedRKStepper.h"
#include <cmath>
#include <stdexcept>
namespace Genfun {

  AdaptiveRKStepper::AdaptiveRKStepper(const EEStepper *stepper):
    eeStepper(stepper ? stepper->clone():new EmbeddedRKStepper()),
    T(1.0E-6),
    sStepsize(0.01),
    S(0.9),
    Rmin(0.0),
    Rmax(5.0),
    stepsize(sStepsize)
  {
  }

  AdaptiveRKStepper::AdaptiveRKStepper(const AdaptiveRKStepper & right):
    RKStepper(right),
    eeStepper(right.eeStepper->clone()),
    T(right.T),
    sStepsize(right.sStepsize),
    S(right.S),
    Rmin(right.Rmin),
    Rmax(right.Rmax),
    stepsize(right.sStepsize)
  {
  }


  void AdaptiveRKStepper::step(const std::shared_ptr<RKIntegrator::RKData>   & data, 
			      const RKIntegrator::RKData::Data & s, 
			      RKIntegrator::RKData::Data       & d,
			      double                             timeLimit) const {
    //
    // Adaptive stepsize control
    //
    if (s.time==0.0) {
      stepsize=sStepsize;
    }
    const unsigned int p = eeStepper->order();              // Order of the stepper
    const double deltaMax = T*std::pow(S/Rmax, (int)(p+1)); // Maximum error 4 adjustment.
    const double TINY   = 1.0E-30;                          // Denominator regularization
    double hnext;
    //
    // Time limited step ? 
    // 
    d.time= timeLimit==0? s.time+stepsize : timeLimit;

    //--------------------------------------//
    // Take one step, from s to d:    //
    //--------------------------------------//
    double h = d.time-s.time;
    while (1) {
      std::vector<double> errors;
      eeStepper->step(data, s, d, errors);
      if (timeLimit!=0.0) return;
      
      // Take absolute value:
      for (size_t e=0;e<errors.size();e++) errors[e] = fabs(errors[e]);
    
      // Select the largest:
      double delta = (*std::max_element(errors.begin(),errors.end()));
      if (delta > T) {
	//
	// Bail out and try a smaller step.
	//
	h = std::max(S*h*std::pow(T/(delta + TINY), 1.0/(p+1)),Rmin*h);
	if  (!(((double) (s.time+h) - (double) s.time) > 0) ) {
	  throw std::runtime_error("Warning, RK Integrator step underflow");
	}
	d.time = s.time+h;
	hnext=h;
	continue;
      }
      else {
	if (delta < deltaMax) {
	  hnext = S*h*std::pow(T/(delta + TINY),1.0/(p+1));
	  // stepsize is supposed to increase;
	  if (hnext<h) hnext=h;
	}
	else {
	  hnext = Rmax*h;
	}
      }
      break;
    }
    stepsize=hnext;
    return;
  }
  


  AdaptiveRKStepper::~AdaptiveRKStepper(){
    delete eeStepper;
  }
  
  AdaptiveRKStepper *AdaptiveRKStepper::clone() const {
    return new AdaptiveRKStepper(*this);
  }

  AdaptiveRKStepper::EEStepper::~EEStepper() {
  }
 
  double & AdaptiveRKStepper::tolerance(){
    return T;
  }

  const double & AdaptiveRKStepper::tolerance() const{
    return T;
  }
  
  double & AdaptiveRKStepper::startingStepsize(){
    return sStepsize;
  }
  const double & AdaptiveRKStepper::startingStepsize() const{
    return sStepsize;
  }
  
  double & AdaptiveRKStepper::safetyFactor(){
    return S;
  }

  const double & AdaptiveRKStepper::safetyFactor() const{
    return S;
  }
  
  double & AdaptiveRKStepper::rmin(){
    return Rmin;
  }
  const double & AdaptiveRKStepper::rmin() const{
    return Rmin;
  }
  
  double & AdaptiveRKStepper::rmax(){
    return Rmax;
  }
  const double & AdaptiveRKStepper::rmax() const{
    return Rmax;
  }

}
