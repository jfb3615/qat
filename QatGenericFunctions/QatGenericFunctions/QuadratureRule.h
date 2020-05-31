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

#ifndef _QuadratureRule_h_
#define _QuadratureRule_h_
#include <vector>
//----------------------------------------------------------------------------//
//                                                                            //
// This is a base class for quadrature rules.  It stores the abscissas and    //
// weights for the rule.  The class is abstract.  Subclasses are supposed to  //
// initialize the classes by adding abscissa/weight pairs in the constructor. //
//                                                                            //
//----------------------------------------------------------------------------//
namespace Genfun {
  class QuadratureRule {
  public:
    
    // Constructor
    QuadratureRule();
    
    // Destructor
    virtual ~QuadratureRule()=0;
    
    // Access abscissa
    const double & abscissa(unsigned int i) const;
    
    // Access weight:
    const double & weight(unsigned int i) const;
    
    // Access the size of the quadrature rule.
    unsigned int size() const;

    // Clone.  
    virtual const QuadratureRule *clone() const=0;

  protected:
    
    // Add an abscissa/weight pair:
    void add(double abcissa, double weight);
    
  private:
    
    std::vector<double> _abscissa;
    std::vector<double> _weight;
    
  };
  
  struct TrapezoidRule:public QuadratureRule {
    
    // Constructor:
    TrapezoidRule();

    // Clone
    const TrapezoidRule *clone() const;

  };
  
  struct OpenTrapezoidRule:public QuadratureRule {
    
    // Constructor:
    OpenTrapezoidRule();

    // Clone
    const OpenTrapezoidRule *clone() const;

  };
  


  struct SimpsonsRule:public QuadratureRule {
    
    // Constructor:
    SimpsonsRule();
    
    // Clone
    const SimpsonsRule *clone() const;
  };
  
  struct MidpointRule:public QuadratureRule {
    
    // Constructor:
    MidpointRule();

    // Clone
    const MidpointRule *clone() const;

    
  };

  struct MilnesRule:public QuadratureRule {
    
    // Constructor:
    MilnesRule();

    // Clone
    const MilnesRule *clone() const;

    
  };
}
#include "QatGenericFunctions/QuadratureRule.icc"
#endif
