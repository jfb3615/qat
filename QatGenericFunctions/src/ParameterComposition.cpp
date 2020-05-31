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

#include "QatGenericFunctions/ParameterComposition.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/AbsFunction.h"

namespace Genfun {
PARAMETER_OBJECT_IMP(ParameterComposition)

ParameterComposition::ParameterComposition(const AbsFunction *arg1, const AbsParameter *arg2):
  AbsParameter(),
  _arg1(arg1->clone()),
  _arg2(arg2->clone())
{
  if (arg2->parameter() && _arg2->parameter()) _arg2->parameter()->connectFrom(arg2->parameter());
}

ParameterComposition::ParameterComposition(const ParameterComposition & right) :
AbsParameter(),
_arg1(right._arg1->clone()),
_arg2(right._arg2->clone())
{}


ParameterComposition::~ParameterComposition()
{
  delete _arg1;
  delete _arg2;
}


double ParameterComposition::getValue() const {
  return _arg1->operator() (_arg2->getValue());
}

} // namespace Genfun
