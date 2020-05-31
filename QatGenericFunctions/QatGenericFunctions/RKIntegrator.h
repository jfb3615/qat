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

//---------------------Runge-Kutte Integrator-------------------------------//
//                                                                          //
// Class RKIntegrator                                                       //
// Joe Boudreau, November 2002                                              //
//                                                                          //
// This is a Runge-Kutta Numerical integrator for a set of N autonomous     //
// first order differential equations in N variables. The point is to       //
// create one or more functions which are defined by A) the differential    //
// equations governing their time evolution, and B) their values at time    //
// t=0.                                                                     //
//                                                                          //
// You add differential eqns one at a time to this integrator.  Each one    //
// is a GENFUNCTION governing the time evolution of the i^th variable, and  //
// should depend on all of the N variables, but not on the time             //
// explicitly.  You should add N differential equations in all.   Each      //
// time you add a differential equation the integrator creates a parameter  //
// for you representing the starting value of the variable, and returns a   //
// pointer.  You may either set the values of that parameter to desired     //
// values or else connect it to an external parameter if you wish to vary   //
// the shape of the function by adjusting starting values.                  //
//                                                                          //
// In addition, you may request the integrator to create a control          //
// parameter.  The control parameter may also be set, or connected.         //
// It can be used in the equations that define the time evolution of the    //
// variables.                                                               //
//--------------------------------------------------------------------------//
#ifndef RKIntegrator_h
#define RKIntegrator_h 1
#include "QatGenericFunctions/AbsFunction.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/Variable.h"
#include <vector>
#include <set>
#include <memory>
namespace Genfun {

  class RKIntegrator {

  public:

    // Some helper classes:
    class RKFunction;
    class RKData;
    class RKStepper;

    // Constructor
    RKIntegrator(const RKStepper *stepper=NULL);

    // Destructor
    virtual ~RKIntegrator();
  
    // Add a differential equation governing the time evolution of the next variable.
    // Get back a parameter representing the starting value of that variable.  You 
    // can either arrange for that parameter to have the right starting value, or you
    // can connect it to another parameter so that you may change it. 
    Parameter * addDiffEquation (const AbsFunction  * diffEquation,
				 const std::string & variableName="anon",
				 double defStartingValue=0.0,
				 double startingValueMin=0.0,
				 double startingValueMax=0.0);


    // Create a control parameter.  You can then connnect this to some other
    // parameter.
    Parameter *createControlParameter (const std::string & variableName="anon",
				       double defStartingValue=0.0,
				       double startingValueMin=0.0,
				       double startingValueMax=0.0);

    // Get back a function. This function will now actually change as parameters
    // are changed; this includes both control parameters and starting value 
    // parameters.
    const RKFunction *getFunction(unsigned int i) const;
    const RKFunction *getFunction(const Variable & v) const;

  private:

    // It is illegal to assign an RKIntegrator
    const RKIntegrator & operator=(const RKIntegrator &right);

    // It is illegal to copy an RKIntegrator
    RKIntegrator(const RKIntegrator &right);
  
    // Here is the data, it belongs to the integrator and to the
    // functions, and is reference counted:
    std::shared_ptr<RKData>                          _data;


    // Here are the functions:
    std::vector<const RKFunction *> _fcn;


  };


  class RKIntegrator::RKData  {
    

  public:
    
    // Information about solution at each mesh point.
    struct Data{

      std::vector<double>         variable;             // Solution
      mutable std::vector<double> firstDerivative;      // It's first derivative
      double time;                                      // time
 
      Data(int size): variable(size), firstDerivative(size), time(0) {} 
      bool operator <  (const Data & right) const { return time < right.time; }
      bool operator == (const Data & right) const { return time==right.time; } 
    };

    RKData();
    ~RKData();
    void lock();
    void recache();

    std::vector<Parameter *>           _startingValParameter;
    std::vector<double>                _startingValParameterCache;

    std::vector <Parameter *>          _controlParameter;
    std::vector <double>               _controlParameterCache;

    std::vector<const AbsFunction *>   _diffEqn;
    std::set<Data >                    _fx;
    bool                               _locked;
    const RKStepper                   *_stepper;
  private:


  };

  class RKIntegrator::RKFunction : public AbsFunction {

    FUNCTION_OBJECT_DEF(RKFunction)

      public:

    // Constructor
    RKFunction(std::shared_ptr<RKData>  & data, unsigned int index);

    // Destructor
    virtual ~RKFunction();
  
    // Copy constructor
    RKFunction(const RKFunction &right);
  
    // Retreive function value
    virtual double operator ()(double argument) const;
    virtual double operator ()(const Argument & a) const {return operator() (a[0]);}

  private:

    // It is illegal to assign a RKFunction
    const RKFunction & operator=(const RKFunction &right);

    // The shared data:
    std::shared_ptr<RKData>       _data;
    const  unsigned int           _index;

};


  // An abstract base class for steppers:
  class RKIntegrator::RKStepper {
  public:

    virtual ~RKStepper();
    virtual void step (const std::shared_ptr<RKData>  & data, 
		       const RKIntegrator::RKData::Data & sdata, 
		       RKIntegrator::RKData::Data       & ddata, 
		       double timeLimit=0) const =0;
    virtual RKStepper *clone() const=0;

  };

} // namespace Genfun

#endif
