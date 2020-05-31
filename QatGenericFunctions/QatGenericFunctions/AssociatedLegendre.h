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

//---------------------AssociatedLegendre-----------------------------------//
//                                                                          //
// Class AssociatedLegendre.  An associated legendre polynomial. Relies on, //
// must be linked with, the gnu scientific library.                         //
//                                                                          //
//--------------------------------------------------------------------------//
#ifndef AssociatedLegendre_h
#define AssociatedLegendre_h 1
#include "QatGenericFunctions/AbsFunction.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/Defs.h"
namespace Genfun {

  class AssociatedLegendre : public AbsFunction  {
  
    FUNCTION_OBJECT_DEF(AssociatedLegendre)

      public:

    // Constructor

    // STD means a normal Associated Legendre.  TWIDDLE
    // means that the factor sqrt(2l+1)/4Pi*(l-m)!/(l+m)!) is included
    // already, as appropriate for Spherical Harmonics.   

    AssociatedLegendre(unsigned int l, int m, NormalizationType type=STD);

    // Copy constructor
    AssociatedLegendre(const AssociatedLegendre &right);
  
    // Destructor
    virtual ~AssociatedLegendre();
  
    // Retreive function value
    virtual double operator ()(double argument) const;
    virtual double operator ()(const Argument & a) const {return operator() (a[0]);}
  
    // Get the integer variable l
    unsigned int l() const;

    // Get the integer variable mj
    int m() const;

  private:

    // It is illegal to assign an adjustable constant
    const AssociatedLegendre & operator=(const AssociatedLegendre &right);

    // Here is the decay constant
    unsigned int _l;

    // Here is the sigma
    int _m;

    // Normalization type:
    NormalizationType _normType;

  };

} // namespace Local

#include "QatGenericFunctions/AssociatedLegendre.icc"


#endif


