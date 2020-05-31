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

#ifndef _AdaptiveRKStepper_h_
#define _AdaptiveRKStepper_h_
#include "QatGenericFunctions/RKIntegrator.h"
//
// This is the default RKStepper routine, used within the RKIntegrator
// when no other stepper is provided.  
//
namespace Genfun {

  class AdaptiveRKStepper:public RKIntegrator::RKStepper {
    
  public:
    
    class EEStepper;

    // Constructor. A default stepper is created if none
    // is specified--it is an Embedded Runge Kutta stepper
    // using a Cash-Karp extended Butcher Tableau, which
    // is fourth-order.
    AdaptiveRKStepper(const EEStepper *eeStepper=NULL);

    // Copy constructor:
    AdaptiveRKStepper(const AdaptiveRKStepper & right);

    // Destructor:
    virtual ~AdaptiveRKStepper();

    // Takes *multiple* steps if needed up to or exceeding the
    // specified time.  This called from the RKFunction and not
    // by users.    If a time limit is specified, only one step
    // is taken--this mode is used to interpolate between cached
    // meshpoints.

    // Input:  data contains diffEqn information and cache:
    //         sData a summary of starting point information
    //         timeLimit (if nonzero) time limit for step 
    // Output  dData a summary of into at the ending point.
    // 

    virtual void step (const std::shared_ptr<RKIntegrator::RKData>       & data, 
		       const RKIntegrator::RKData::Data & sdata, 
		       RKIntegrator::RKData::Data       & ddata, 
		       double                             timeLimit) const ;
    // Clone
    virtual AdaptiveRKStepper *clone() const;

    // Accessors and modifiers to algorithmic parameters.  Roughly
    // speaking these are ordered according to importance: the user
    // will often wish to modify the tolerance and the starting 
    // stepsize, but rarely should need to touch any of the others.
    
    // The tolerance:
    double & tolerance();
    const double & tolerance() const;

    // The starting stepsize:
    double & startingStepsize();
    const double & startingStepsize() const;

    // The safety factor.  Step size increases are moderated by this
    // factor:
    double & safetyFactor();
    const double & safetyFactor() const;

    // The minimum amount by which a step size is decreased:
    double & rmin();
    const double & rmin() const;

    // The maximum amount by which a step size is increased:
    double & rmax();
    const double & rmax() const;
    
    
  private:
    
    // It is illegal to assign an AdaptiveRKStepper:
    AdaptiveRKStepper & operator=(const AdaptiveRKStepper & right);

    const        EEStepper   *eeStepper;
    double       T;
    double       sStepsize;
    double       S;
    double       Rmin;
    double       Rmax;
    mutable      double       stepsize;
  };


  // 
  // An abstract base class for steppers that return an error
  // estimate at each step:
  //
  class AdaptiveRKStepper::EEStepper {
  public:
    
    virtual ~EEStepper();
    virtual void step   (const std::shared_ptr<RKIntegrator::RKData>       & data, 
			 const RKIntegrator::RKData::Data & sdata, 
			 RKIntegrator::RKData::Data       & ddata, 
			 std::vector<double> & errors) const = 0;
    virtual EEStepper *clone() const=0;
    virtual unsigned int order() const=0;
  };
}

#endif
