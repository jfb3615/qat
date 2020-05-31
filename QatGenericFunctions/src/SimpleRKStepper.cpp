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

#include "QatGenericFunctions/SimpleRKStepper.h"
#include <cmath>
#include <stdexcept>
namespace Genfun {
  SimpleRKStepper::SimpleRKStepper(const ButcherTableau & mtableau,double xstepsize):
    tableau(mtableau),
    stepsize(xstepsize) 
  {
  }

  void SimpleRKStepper::step(const std::shared_ptr<RKIntegrator::RKData>       & data, 
			     const RKIntegrator::RKData::Data & s, 
			     RKIntegrator::RKData::Data       & d, 
			     double                             timeLimit ) const {
    const double h = timeLimit==0 ? stepsize : timeLimit - s.time;
    if (h<=0) throw std::runtime_error ("SimpleRKStepper:  negative stepsize");
    const unsigned int nvar = s.variable.size();
    // Compute all of the k's..:
    //
    std::vector<std::vector<double> >k(tableau.nSteps());
    for (unsigned int i=0;i<tableau.nSteps();i++) {
      k[i].resize(nvar,0);
      Argument arg(nvar);
      for (unsigned int v=0;v<nvar;v++) arg[v]=s.variable[v];
      for (unsigned int j=0;j<i;j++) {
	for (unsigned int v=0;v<nvar;v++) arg[v] += h*tableau.A(i,j)*k[j][v];
      }
      for (unsigned int v=0;v<nvar;v++) k[i][v]=(*data->_diffEqn[v])(arg);
    }
    //
    // Final result.
    //
    for (unsigned int v=0;v<nvar;v++) d.firstDerivative[v] = 0;
    for (unsigned int i=0;i<tableau.nSteps();i++) {
      for (unsigned int v=0;v<nvar;v++) d.firstDerivative[v] += tableau.b(i)*k[i][v];
    }
    for (unsigned int v=0;v<nvar;v++)   d.variable[v] =s.variable[v]+h*d.firstDerivative[v];
    d.time = timeLimit==0 ? s.time + h : timeLimit;
    
  }
  
  SimpleRKStepper::~SimpleRKStepper(){}
  
  SimpleRKStepper *SimpleRKStepper::clone() const {
    return new SimpleRKStepper(*this);
  }
}
