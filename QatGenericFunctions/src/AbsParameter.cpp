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

#include "QatGenericFunctions/AbsParameter.h"
#include "QatGenericFunctions/Parameter.h"
namespace Genfun {

// Two do-nothing functions:
AbsParameter::AbsParameter() {
}
  
AbsParameter::~AbsParameter(){
}

AbsParameter::AbsParameter(const AbsParameter &)
{}

AbsParameter *AbsParameter::clone() const {
  return _clone();
}

  
ParameterSum operator + (const AbsParameter & a, const AbsParameter & b) {
  return ParameterSum(&a,&b);
}

ParameterDifference operator - (const AbsParameter & a, const AbsParameter & b) {
  return ParameterDifference(&a,&b);
}

ParameterProduct operator * (const AbsParameter & a, const AbsParameter & b) {
  return ParameterProduct(&a,&b);
}

ParameterQuotient operator / (const AbsParameter & a, const AbsParameter & b) {
  return ParameterQuotient(&a,&b);
}


ParameterNegation operator - (const AbsParameter & a) {
  return ParameterNegation(&a);
}


ConstTimesParameter           operator * (double c, const AbsParameter &op2){
  return ConstTimesParameter (c, &op2);
}

ConstPlusParameter               operator + (double c, const AbsParameter &op2){
  return ConstPlusParameter (c, &op2);
}

ConstMinusParameter        operator - (double c, const AbsParameter &op2){
  return ConstMinusParameter(c, &op2);
}

ConstOverParameter          operator / (double c, const AbsParameter &op2){
  return ConstOverParameter(c, &op2);
}

ConstTimesParameter           operator * (const AbsParameter &op2, double c){
  return ConstTimesParameter (c, &op2);
}

ConstPlusParameter               operator + (const AbsParameter &op2, double c){
  return ConstPlusParameter (c, &op2);
}

ConstPlusParameter        operator - (const AbsParameter &op2, double c){
  return ConstPlusParameter(-c, &op2);
}

ConstTimesParameter          operator / (const AbsParameter &op2, double c){
  return ConstTimesParameter(1/c, &op2);
}


} // namespace Genfun
