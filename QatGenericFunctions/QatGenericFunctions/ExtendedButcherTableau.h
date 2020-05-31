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

#ifndef _ExtendedButcherTableau_h_
#define _ExtendedButcherTableau_h_
// This class defines a ExtendedButcher Tableau, which completely specifies
// an *embedded* Runge-Kutte integration scheme. ExtendedButcher Tableau 
// are described in Numerical Methods for Ordinary Differential Equations, 
// John Wiley & sons, West Sussex England.
//
// General form is :
// 
//      c|A
//      ---
//       |b^T
//       |bHat^T
//
// where A is a matrix and b, bHat, and c are column vectors. 
// 
// The ExtendedButcher Tableau Class presents itself as an empty structure
// that the user has to fill up.  One can blithely fill write into
// any element of A, b, bHat or c. Space is automatically allocated.

#include <vector>
#include <string>
namespace Genfun {
  class ExtendedButcherTableau {
    
  public:
    
    // Constructor:
    inline ExtendedButcherTableau(const std::string &name, unsigned int order, unsigned int orderHat);
    
    // Returns the name:
    inline const std::string & name() const;
    
    // Returns the order of the main formula
    inline unsigned int order() const;

    // Returns the order of the controlling formula
    inline unsigned int orderHat() const;
    
    // Returns the number of steps:
    inline unsigned int nSteps() const;
    
    // Write access to elements:
    inline double & A(unsigned int i, unsigned int j);
    inline double & b(unsigned int i);
    inline double & bHat(unsigned int i);
    inline double & c(unsigned int i);
    
    // Read access to elements (inline for speed)
    inline const double & A(unsigned int i, unsigned int j) const;
    inline const double & b(unsigned int i) const;
    inline const double & bHat(unsigned int i) const;
    inline const double & c(unsigned int i) const;
    
    
  private:
    
    std::vector< std::vector<double> > _A;
    std::vector<double>                _b;
    std::vector<double>                _bHat;
    std::vector<double>                _c;
    std::string                        _name;
    unsigned int                       _order;
    unsigned int                       _orderHat;

  };


  class HeunEulerXtTableau: public ExtendedButcherTableau {
    // Constructor:
  public:
    inline HeunEulerXtTableau();
  };

  class BogackiShampineXtTableau: public ExtendedButcherTableau {
    // Constructor:
  public:
    inline BogackiShampineXtTableau();
  };

  class FehlbergRK45F2XtTableau: public ExtendedButcherTableau {
    // Constructor:
  public:
    inline FehlbergRK45F2XtTableau();
  };

  class CashKarpXtTableau: public ExtendedButcherTableau {
    // Constructor:
  public:
    inline CashKarpXtTableau();
  };

}

inline std::ostream & operator << (std::ostream & o, const Genfun::ExtendedButcherTableau & b);


#include "QatGenericFunctions/ExtendedButcherTableau.icc"

#endif
