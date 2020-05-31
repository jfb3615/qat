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

#include "QatGenericFunctions/QuadratureRule.h"
namespace Genfun {
  // Constructor
  QuadratureRule::QuadratureRule(){
  }

  // Destructor
  QuadratureRule::~QuadratureRule(){
  }
  
  // Add an abscissa/weight pair:
  void QuadratureRule::add(double abcissa, double weight){
    _abscissa.push_back(abcissa);
    _weight.push_back(weight);
  }
  
  // Constructor:
  TrapezoidRule::TrapezoidRule():QuadratureRule() {
    add(0,0.5);
    add(1,0.5);
  }


  // Constructor:
  OpenTrapezoidRule::OpenTrapezoidRule():QuadratureRule() {
    add(1/3.0,0.5);
    add(2/3.0,0.5);
  }
  

  // Constructor:
  MilnesRule::MilnesRule():QuadratureRule() {
    add(1/4.0, 2/3.0);
    add(1/2.0,-1/3.0);
    add(3/4.0, 2/3.0);
  }
  
  
  // Constructor:
  MidpointRule::MidpointRule():QuadratureRule() {
    add(0.5,1.0);
  }
  
  // Constructor:
  SimpsonsRule::SimpsonsRule():QuadratureRule() {
    add(0.0,   1.0/6.0);
    add(0.5,   4.0/6.0);
    add(1.0,   1.0/6.0);
  }

  
  const SimpsonsRule *SimpsonsRule::clone () const   { return new SimpsonsRule(*this);}
  const MidpointRule *MidpointRule::clone () const   { return new MidpointRule(*this);}
  const TrapezoidRule *TrapezoidRule::clone () const { return new TrapezoidRule(*this);}
  const OpenTrapezoidRule *OpenTrapezoidRule::clone () const { return new OpenTrapezoidRule(*this);}
  const MilnesRule *MilnesRule::clone () const { return new MilnesRule(*this);}
}
