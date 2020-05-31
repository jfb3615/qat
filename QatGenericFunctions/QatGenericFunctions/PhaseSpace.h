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

#ifndef _PHASE_SPACE_
#define _PHASE_SPACE_
#include "QatGenericFunctions/Variable.h"
#include <vector>

namespace Classical {

  class PhaseSpace {


  public:

    // A component is like a vector: of coordinates or momenta:
    class Component;

    // constructor
    PhaseSpace(unsigned int NDIM);

    // Destructor
    ~PhaseSpace();

    // Get the dimensionality:
    unsigned int dim() const;

    // Get the coordinates:
    const Component & coordinates() const;

    // Get the momenta:
    const Component & momenta()     const; 
    
    // Set starting values for the coordinates or momenta:
    void   start (const Genfun::Variable & variable, double value);

    // Get starting values for the coordinates or momenta:
    double startValue(const Genfun::Variable & component) const ;


    // Each component has N-dimensions:
    class Component {
      
    public:
      
      // Access to the ith element;
      Genfun::Variable operator [] (unsigned int i) const; 
     
    private:

      // Constructor:
      Component(unsigned int NDIM, bool isMomentum);

      // Destructor:
      ~Component();
      
      // Illegal operations:
      Component (const Component &);
      Component & operator=(const Component &);

      // Internal clockwork;
      class Clockwork;
      Clockwork *c;
      friend class PhaseSpace; 

    };
      
  private:

    Component _coordinates; 
    Component _momenta;
    std::vector<double> _q0;
    std::vector<double> _p0;
    unsigned int DIM;
  };

}
#endif

