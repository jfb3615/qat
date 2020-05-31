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

#include "QatGenericFunctions/ParameterNegation.h"
#include "QatGenericFunctions/Parameter.h"

namespace Genfun {
PARAMETER_OBJECT_IMP(ParameterNegation)

ParameterNegation::ParameterNegation(const AbsParameter *arg1):
  _arg1(arg1->clone())
{
  if (arg1->parameter() && _arg1->parameter()) _arg1->parameter()->connectFrom(arg1->parameter());
}

ParameterNegation::ParameterNegation(const ParameterNegation & right) :
AbsParameter(),
_arg1(right._arg1->clone())
{}


ParameterNegation::~ParameterNegation()
{
  delete _arg1;
}


double ParameterNegation::getValue() const {
  return - _arg1->getValue();
}

} // namespace Genfun
