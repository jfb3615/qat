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

#include "QatGenericFunctions/ClassicalSolver.h"
#include "QatGenericFunctions/PhaseSpace.h"
#include <cmath>

namespace Genfun {
  
  FUNCTION_OBJECT_IMP(EnergyFunction)

  EnergyFunction::EnergyFunction(const Classical::Solver & xsolver):
  solver(xsolver)
  {}
  
  EnergyFunction::~EnergyFunction() {
  }
  
  EnergyFunction::EnergyFunction(const EnergyFunction & right) 
    :AbsFunction(right), solver(right.solver)
  {  }
  
  
  double EnergyFunction::operator() (double t) const {
    const Classical::PhaseSpace & phaseSpace=solver.phaseSpace();
    const unsigned int DIM=phaseSpace.dim();
    Genfun::Argument arg(2*DIM);
    for (unsigned int i=0;i<phaseSpace.dim();i++) {
      arg[i]    =solver.equationOf(phaseSpace.coordinates()[i])(t);
      arg[i+DIM]=solver.equationOf(phaseSpace.momenta()[i])(t);
    }
    Genfun::GENFUNCTION H = solver.hamiltonian(); 
    return H(arg);
  }


} // namespace Genfun
