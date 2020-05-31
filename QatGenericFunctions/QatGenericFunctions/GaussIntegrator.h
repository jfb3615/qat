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

//---------------------------------------------------------------------------//
//                                                                           //
//    Class GaussIntegrator                                                 //
//                                                                           //
//    This class uses a quadrature rule to compute the integral of a         //
//    function over a specified range, by applying the quadrature rule       //
//    within a specified number of intervals.                                //
//                                                                           //
//    If no quadrature rule is provided, then by default this integrator     //
//    will use the trapezoid rule.                                           //
//                                                                           //
//    If the number of intervals is not specified, the integrator will       //
//    use a default of 2^6=64.                                               //
//                                                                           //
//---------------------------------------------------------------------------//
#ifndef _GaussIntegrator_h_
#define _GaussIntegrator_h_
#include "QatGenericFunctions/GaussQuadratureRule.h"
#include "QatGenericFunctions/AbsFunctional.h"
#include <memory>
namespace Genfun {
  class GaussIntegrator : public AbsFunctional {
    
  public:

    // Type definition:
    enum Type {INTEGRATE_DX, INTEGRATE_WX_DX};
    
    // Constructor:
    GaussIntegrator(const GaussQuadratureRule   &rule, Type type=INTEGRATE_DX);

    //     
    // Take the definite integral of a function 
    virtual double operator () (GENFUNCTION function) const;

  private:
    
    class Clockwork;
    std::shared_ptr<Clockwork> c;

  };
}
#endif
