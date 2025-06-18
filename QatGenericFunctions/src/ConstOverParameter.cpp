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

#include "QatGenericFunctions/ConstOverParameter.h"
#include "QatGenericFunctions/Parameter.h"

namespace Genfun {
PARAMETER_OBJECT_IMP(ConstOverParameter)

ConstOverParameter::ConstOverParameter(double xconstant, const std::shared_ptr<const AbsParameter> & aparm):
  _constant(xconstant),
  _parameter(aparm)
{
}

ConstOverParameter::ConstOverParameter(const ConstOverParameter & right) :
AbsParameter(right),
_constant(right._constant),
_parameter(right._parameter)
{}


ConstOverParameter::~ConstOverParameter()
{
}


double ConstOverParameter::getValue() const {
  return _constant / _parameter->getValue();
}

} // namespace Genfun
