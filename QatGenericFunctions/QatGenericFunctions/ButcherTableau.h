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

#ifndef _ButcherTableau_h_
#define _ButcherTableau_h_
// This class defines a Butcher Tableau, which completely specifies
// a Runge-Kutte integration scheme. Butcher Tableau are described
// in Numerical Methods for Ordinary Differential Equations, John
// Wiley & sons, West Sussex England.
//
// General form is :
// 
//      c|A
//      ---
//       |b^T
//
// where A is a matrix and b, c are column vectors. 
// 
// The Butcher Tableau Class presents itself as an empty structure
// that the user has to fill up.  One can blithely fill write into
// any element of A, b, or c. Space is automatically allocated.

#include <vector>
#include <string>
namespace Genfun {
  class ButcherTableau {
    
  public:
    
    // Constructor:
    inline ButcherTableau(const std::string &name, unsigned int order);
    
    // Returns the name:
    inline const std::string & name() const;
    
    // Returns the order:
    inline unsigned int order() const;
    
    // Returns the number of steps:
    inline unsigned int nSteps() const;
    
    // Write access to elements:
    inline double & A(unsigned int i, unsigned int j);
    inline double & b(unsigned int i);
    inline double & c(unsigned int i);
    
    // Read access to elements (inline for speed)
    inline const double & A(unsigned int i, unsigned int j) const;
    inline const double & b(unsigned int i) const;
    inline const double & c(unsigned int i) const;
    
    
  private:
    
    std::vector< std::vector<double> > _A;
    std::vector<double>                _b;
    std::vector<double>                _c;
    std::string                        _name;
    unsigned int                       _order;
  };


  class EulerTableau: public ButcherTableau {
    // Constructor:
  public:
    inline EulerTableau();
  };

  class MidpointTableau: public ButcherTableau {
    // Constructor:
  public:
    inline MidpointTableau();
  };

  class TrapezoidTableau: public ButcherTableau {
    // Constructor:
  public:
    inline TrapezoidTableau();
  };

  class RK31Tableau: public ButcherTableau {
    // Constructor:
  public:
    inline RK31Tableau();
  };

  class RK32Tableau: public ButcherTableau {
    // Constructor:
  public:
    inline RK32Tableau();
  };

  class ClassicalRungeKuttaTableau: public ButcherTableau {
    // Constructor:
  public:
    inline ClassicalRungeKuttaTableau();
  };

  class ThreeEighthsRuleTableau: public ButcherTableau {
    // Constructor:
  public:
    inline ThreeEighthsRuleTableau();
  };

}

inline std::ostream & operator << (std::ostream & o, const Genfun::ButcherTableau & b);


#include "QatGenericFunctions/ButcherTableau.icc"

#endif
