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

#include "QatGenericFunctions/EmbeddedRKStepper.h"
#include "QatGenericFunctions/ExtendedButcherTableau.h"
#include <stdexcept>
namespace Genfun {

  
  EmbeddedRKStepper::EmbeddedRKStepper(const ExtendedButcherTableau & mtableau):
    tableau(mtableau){
  }

  EmbeddedRKStepper::~EmbeddedRKStepper() {
  }

  void EmbeddedRKStepper::step   (const std::shared_ptr<RKIntegrator::RKData>       & data, 
				  const RKIntegrator::RKData::Data & s, 
				  RKIntegrator::RKData::Data       & d, 
				  std::vector<double>              & errors) const {

    // First step:
    double h = d.time - s.time;
    if (h<=0) throw std::runtime_error ("Runtime error in RKIntegrator (zero or negative stepsize)");
    unsigned int nvar = s.variable.size();

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
    // 
    // 
    //
    errors.resize(nvar);
    for (unsigned int v=0;v<nvar;v++) errors[v] = 0;
    for (unsigned int i=0;i<tableau.nSteps();i++) {
      for (unsigned int v=0;v<nvar;v++) errors[v] += (h*(tableau.bHat(i)-tableau.b(i))*k[i][v]);
    }
    return;
  }

  EmbeddedRKStepper *EmbeddedRKStepper::clone() const {
    return new EmbeddedRKStepper(*this);
  }

  unsigned int EmbeddedRKStepper::order() const {
    return tableau.order();
  }
}
