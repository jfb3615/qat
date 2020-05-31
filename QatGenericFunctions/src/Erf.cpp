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

#include "QatGenericFunctions/Erf.h"
#include "QatGenericFunctions/Variable.h"
#include "QatGenericFunctions/NormalDistribution.h"
#include <stdexcept>
#include <cmath>

namespace Genfun {
FUNCTION_OBJECT_IMP(Erf)

Erf::Erf()
{}

Erf::~Erf() {
}

Erf::Erf(const Erf & right) : AbsFunction(right)
{  }


double Erf::operator() (double x) const {
  return erf(x);
}



  Derivative Erf::partial(unsigned int index) const {
    if (index!=0) throw std::range_error("Erf: partial derivative index out of range");
    Genfun::NormalDistribution gauss;
    gauss.sigma().setValue(1/sqrt(2));
    GENFUNCTION fPrime=2.0*gauss;
    return Derivative(&fPrime);
  }

} // namespace Genfun
