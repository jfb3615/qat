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

#include "QatGenericFunctions/RungeKuttaClassicalSolver.h"
#include "QatGenericFunctions/RKIntegrator.h"
namespace Classical {
  //
  // This is the private innards of RungeKuttaSolver
  //
  class RungeKuttaSolver::Clockwork {
  public:

    Clockwork(Genfun::GENFUNCTION gH, const PhaseSpace & mphaseSpace):H(gH),phaseSpace(mphaseSpace), integrator(NULL){
    }
    Genfun::GENFUNCTION H;
    const Classical::PhaseSpace & phaseSpace;
    Genfun::RKIntegrator          *integrator;
    std::vector<Genfun::Parameter*> startingQ;
    std::vector<Genfun::Parameter*> startingP;
    Genfun::EnergyFunction          *energy;
  };

  RungeKuttaSolver::RungeKuttaSolver(Genfun::GENFUNCTION gH, const PhaseSpace & mphaseSpace, const Genfun::RKIntegrator::RKStepper *stepper):c(new Clockwork(gH,mphaseSpace)){
    c->integrator=new Genfun::RKIntegrator(stepper);
    //
    // Dimension (of coords, or phase space)
    //
    const unsigned int DIM=c->phaseSpace.dim();
    //
    // Equations of motion via hamilton's equations: 
    //
    const Classical::PhaseSpace::Component & X=c->phaseSpace.coordinates();
    const Classical::PhaseSpace::Component & P=c->phaseSpace.momenta();
    
    for (unsigned int i=0;i<DIM;i++) {
      Genfun::GENFUNCTION DXDT  =  c->H.partial(P[i].index());
      c->startingQ.push_back(c->integrator->addDiffEquation(&DXDT,"X",c->phaseSpace.startValue(X[i])));
    }
    for (unsigned int i=0;i<DIM;i++) {
      Genfun::GENFUNCTION DPDT  = -c->H.partial(X[i].index());
      c->startingP.push_back(c->integrator->addDiffEquation(&DPDT,"P",c->phaseSpace.startValue(P[i])));
    }
    c->energy=NULL;
    
  }
  RungeKuttaSolver::~RungeKuttaSolver(){
    delete c->integrator;
    delete c->energy;
    delete c;
  }

  Genfun::GENFUNCTION RungeKuttaSolver::equationOf(const Genfun::Variable & v) const {
    return *c->integrator->getFunction(v.index());
  }
  Genfun::GENFUNCTION RungeKuttaSolver::hamiltonian() const {
    return c->H;
  }
  const Classical::PhaseSpace & RungeKuttaSolver::phaseSpace() const {
    return c->phaseSpace;
  }
  Genfun::GENFUNCTION RungeKuttaSolver::energy() const {
    if (!c->energy) c->energy=new Genfun::EnergyFunction(*this);
    return *c->energy;
  }

  Genfun::Parameter *RungeKuttaSolver::createControlParameter(const std::string & variableName,
								      double defStartingValue,
								      double startingValueMin,
								      double startingValueMax) const {
    return c->integrator->createControlParameter(variableName, defStartingValue, startingValueMin, startingValueMax) ;
  }
      
  Genfun::Parameter *RungeKuttaSolver::takeQ0(unsigned int index) {
    return c->startingQ[index];
  }
  Genfun::Parameter *RungeKuttaSolver::takeP0(unsigned int index) {
    return c->startingP[index];
  }

}
