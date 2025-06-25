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
  std::shared_ptr<const AbsParameter> o1{a.clone()},o2{b.clone()};
  return ParameterSum(o1,o2);

}

ParameterDifference operator - (const AbsParameter & a, const AbsParameter & b) {
  std::shared_ptr<const AbsParameter> o1{a.clone()},o2{b.clone()};
  return ParameterDifference(o1,o2);
}

ParameterProduct operator * (const AbsParameter & a, const AbsParameter & b) {
  std::shared_ptr<const AbsParameter> o1{a.clone()},o2{b.clone()};
  return ParameterProduct(o1,o2);
}

ParameterQuotient operator / (const AbsParameter & a, const AbsParameter & b) {
  std::shared_ptr<const AbsParameter> o1{a.clone()},o2{b.clone()};
  return ParameterQuotient(o1,o2);
}


ParameterNegation operator - (const AbsParameter & a) {
  std::shared_ptr<const AbsParameter> o{a.clone()};
  return ParameterNegation(o);
}


ConstTimesParameter           operator * (double c, const AbsParameter &op2){
  std::shared_ptr<const AbsParameter> o{op2.clone()};
  return ConstTimesParameter (c, o);
}

ConstPlusParameter               operator + (double c, const AbsParameter &op2){
 std::shared_ptr<const AbsParameter> o{op2.clone()};
 return ConstPlusParameter (c, o);
}

ConstMinusParameter        operator - (double c, const AbsParameter &op2){
  std::shared_ptr<const AbsParameter> o{op2.clone()};
  return ConstMinusParameter(c, o);
}

ConstOverParameter          operator / (double c, const AbsParameter &op2){
 std::shared_ptr<const AbsParameter> o{op2.clone()};
 return ConstOverParameter(c, o);
}

ConstTimesParameter           operator * (const AbsParameter &op2, double c){
 std::shared_ptr<const AbsParameter> o{op2.clone()};
 return ConstTimesParameter (c, o);
}

ConstPlusParameter               operator + (const AbsParameter &op2, double c){
 std::shared_ptr<const AbsParameter> o{op2.clone()};
 return ConstPlusParameter (c, o);
}

ConstPlusParameter        operator - (const AbsParameter &op2, double c){
  std::shared_ptr<const AbsParameter> o{op2.clone()};
  return ConstPlusParameter(-c, o);
}

ConstTimesParameter          operator / (const AbsParameter &op2, double c){
  std::shared_ptr<const AbsParameter> o{op2.clone()};
  return ConstTimesParameter(1/c, o);
}


} // namespace Genfun
