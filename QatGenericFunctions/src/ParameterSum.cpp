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

#include "QatGenericFunctions/ParameterSum.h"
#include "QatGenericFunctions/Parameter.h"

namespace Genfun {
PARAMETER_OBJECT_IMP(ParameterSum)

ParameterSum::ParameterSum(std::shared_ptr<const AbsParameter> arg1, std::shared_ptr<const AbsParameter> arg2):
  _arg1(arg1),
  _arg2(arg2)
{
}

ParameterSum::ParameterSum(const ParameterSum & right) :
AbsParameter(),
_arg1(right._arg1),
_arg2(right._arg2)
{}


ParameterSum::~ParameterSum()
{
}


double ParameterSum::getValue() const {
  return _arg1->getValue()+_arg2->getValue();
}

} // namespace Genfun
