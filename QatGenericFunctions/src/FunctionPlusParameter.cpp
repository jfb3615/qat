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

#include "QatGenericFunctions/FunctionPlusParameter.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/KVector.h"
namespace Genfun {
FUNCTION_OBJECT_IMP(FunctionPlusParameter)

FunctionPlusParameter::FunctionPlusParameter(const AbsParameter *parameter, const AbsFunction *function):
  _function(function->clone()),
  _parameter(parameter->clone())
{
  if (parameter->parameter() && _parameter->parameter()) {
    _parameter->parameter()->connectFrom(parameter->parameter());
  }
}

FunctionPlusParameter::FunctionPlusParameter(const FunctionPlusParameter & right) :
  AbsFunction(right),
  _function(right._function->clone()),
  _parameter(right._parameter->clone())
{}

unsigned int FunctionPlusParameter::dimensionality() const {
  return _function->dimensionality();
}

FunctionPlusParameter::~FunctionPlusParameter()
{
  delete _function;
  delete _parameter;
}


double FunctionPlusParameter::operator ()(double x) const
{
  return _parameter->getValue() + (*_function)(x);
}


double FunctionPlusParameter::operator ()(const Argument & x) const
{
  return _parameter->getValue() + (*_function)(x);
}

Derivative FunctionPlusParameter::partial(unsigned int index) const {
  const Derivative & d=_function->partial(index);
  return d;
}

} // namespace Genfun
