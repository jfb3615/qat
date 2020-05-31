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

// This is a class the creates an N-Dimensional Phase Space     //

// It is for use in computing the time development of classical //
// Hamiltonian Systems.                                         //

//--------------------------------------------------------------//

#ifndef _ClassicalSolver_h__
#define _ClassicalSolver_h__

#include "QatGenericFunctions/PhaseSpace.h"
#include "QatGenericFunctions/Variable.h"
#include "QatGenericFunctions/Parameter.h"


namespace Genfun {
  class EnergyFunction;
}


namespace Classical {


  class Solver {
        
  public:
    //
    // Constructor--takes a hamiltonian and a point in p-space: 
    //
    Solver(){};
    //
    // Destructor:
    //
    virtual ~Solver(){};
    //
    // Returns the time evolution for a variable (q_i or p_i)
    //
    virtual Genfun::GENFUNCTION equationOf(const Genfun::Variable & v) const=0;
    //
    // Returns the phase space 
    //
    virtual const PhaseSpace & phaseSpace() const=0;
    //
    // Returns the Hamiltonian (function of the 2N phase space variables). 
    //
    virtual Genfun::GENFUNCTION hamiltonian() const=0;
    //
    // Returns the energy (function of time).
    //
    virtual Genfun::GENFUNCTION energy() const=0;
    //
    // This is in the rare case that the user needs to edit starting values.
    // or parameterize the Hamiltonian.  Most users:  can ignore.  
    virtual Genfun::Parameter *takeQ0(unsigned int index)=0;
    virtual Genfun::Parameter *takeP0(unsigned int index)=0;
    virtual Genfun::Parameter *createControlParameter(const std::string & variableName="anon",
						      double defStartingValue=0.0,
						      double startingValueMin=0.0,
						      double startingValueMax=0.0) const = 0;
    


  private:
    
    // Illegal Operations:
    Solver (const Solver &);
    Solver & operator=(const Solver &);
    
  };
}

namespace Genfun {

  class EnergyFunction: public Genfun::AbsFunction {
  
    FUNCTION_OBJECT_DEF(EnergyFunction)

      public:

    // Constructor
    EnergyFunction(const Classical::Solver &);

    // Destructor
    virtual ~EnergyFunction();
  
    // Copy constructor
    EnergyFunction(const EnergyFunction &right);
  
    // Retreive function value
    virtual double operator ()(double argument) const;
    virtual double operator ()(const Argument & a) const {return operator() (a[0]);}

  private:

    // It is illegal to assign a EnergyFunction
    const EnergyFunction & operator=(const EnergyFunction &right);

    const Classical::Solver & solver;

  };

}



#endif
