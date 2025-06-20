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

#include "QatGenericFunctions/AbsFunction.h"
#include "QatGenericFunctions/ConstTimesFunction.h"
#include "QatGenericFunctions/ConstOverFunction.h"
#include "QatGenericFunctions/ConstPlusFunction.h"
#include "QatGenericFunctions/ConstMinusFunction.h"
#include "QatGenericFunctions/FunctionSum.h"
#include "QatGenericFunctions/FunctionDifference.h"
#include "QatGenericFunctions/FunctionProduct.h"
#include "QatGenericFunctions/FunctionQuotient.h"
#include "QatGenericFunctions/FunctionNegation.h"
#include "QatGenericFunctions/FunctionDirectProduct.h"
#include "QatGenericFunctions/FixedConstant.h"
#include "QatGenericFunctions/FunctionNumDeriv.h"
#include "QatGenericFunctions/Variable.h"
#include "QatGenericFunctions/FunctionNoop.h"
#include "QatGenericFunctions/ParameterComposition.h"
#include <stdexcept>
namespace Genfun {
AbsFunction::AbsFunction() {
}

AbsFunction::~AbsFunction() {
}

AbsFunction::AbsFunction(const AbsFunction &) {
}

FunctionComposition AbsFunction::operator () (const AbsFunction &function) const {
  return FunctionComposition(this, &function);
}

ParameterComposition AbsFunction::operator() (const AbsParameter &p) const {
  return ParameterComposition(this, &p);
}

Derivative AbsFunction::partial(unsigned int index) const {
  FunctionNumDeriv fPrime(this,index);
  return Derivative(&fPrime);
}


Derivative AbsFunction::partial(const Variable &v) const {
  return partial(v.index());
}

Derivative AbsFunction::prime() const {
  if (dimensionality()!=1) throw std::runtime_error("AbsFunction::prime() called for a function of more than one variable");
  return partial(0);
}

FunctionSum operator + (const AbsFunction & a, const AbsFunction & b) {
  return FunctionSum(&a,&b);
}

FunctionDifference operator - (const AbsFunction & a, const AbsFunction & b) {
  return FunctionDifference(&a,&b);
}

FunctionProduct operator * (const AbsFunction & a, const AbsFunction & b) {
  return FunctionProduct(&a,&b);
}

FunctionQuotient operator / (const AbsFunction & a, const AbsFunction & b) {
  return FunctionQuotient(&a,&b);
}


FunctionNegation operator - (const AbsFunction & a) {
  return FunctionNegation(&a);
}

unsigned int AbsFunction::dimensionality() const {
  return 1;
}

FunctionDirectProduct operator % (const AbsFunction & a, const AbsFunction & b) {
  return FunctionDirectProduct(&a,&b);
}

ConstTimesFunction operator * (const AbsFunction &op2, double c) {
  std::shared_ptr<const AbsFunction> o{op2.clone()};
  return ConstTimesFunction(c, o);
}

ConstPlusFunction  operator + (const AbsFunction &op2, double c) {
  std::shared_ptr<const AbsFunction> o{op2.clone()};
  return ConstPlusFunction(c,o);
}

ConstPlusFunction  operator - (const AbsFunction &op2, double c) {
  std::shared_ptr<const AbsFunction> o{op2.clone()};
  return ConstPlusFunction(-c, o);
}

ConstTimesFunction operator / (const AbsFunction &op2, double c) {
  std::shared_ptr<const AbsFunction> o{op2.clone()};
  return ConstTimesFunction(1/c, o);
}


ConstTimesFunction           operator * (double c, const AbsFunction &op2) {
  std::shared_ptr<const AbsFunction> o{op2.clone()};
  return ConstTimesFunction(c,o);
}

ConstPlusFunction               operator + (double c, const AbsFunction &op2) {
  std::shared_ptr<const AbsFunction> o{op2.clone()};
  return ConstPlusFunction(c,o);
}

ConstMinusFunction        operator - (double c, const AbsFunction &op2) {
  std::shared_ptr<const AbsFunction> o{op2.clone()};
  return ConstMinusFunction(c,o);
}

ConstOverFunction          operator / (double c, const AbsFunction &op2) {
  std::shared_ptr<const AbsFunction> o{op2.clone()};
  return ConstOverFunction(c,o);
}


FunctionTimesParameter operator * (const AbsFunction &f, const AbsParameter & p) {
  std::shared_ptr<const AbsParameter> op{p.clone()};
  std::shared_ptr<const AbsFunction>  of{f.clone()};
  return FunctionTimesParameter(op, of);
}

FunctionPlusParameter operator + (const AbsFunction &f, const AbsParameter & p) {
  std::shared_ptr<const AbsParameter> o{p.clone()}; 
  return FunctionPlusParameter(o, &f);
}

FunctionPlusParameter operator - (const AbsFunction &f, const AbsParameter & p) {
  std::shared_ptr<const AbsParameter> o{(-p).clone()}; 
  return FunctionPlusParameter(o, &f);
  
}

FunctionTimesParameter operator / (const AbsFunction &f, const AbsParameter & p) {
  //  GENPARAMETER oneOverP = 1.0/p;
  std::shared_ptr<const AbsParameter> op{(1.0/p).clone()};
  std::shared_ptr<const AbsFunction>  of{f.clone()};
  return FunctionTimesParameter(op, of);
}

FunctionTimesParameter operator * (const AbsParameter & p, const AbsFunction &f) {
  std::shared_ptr<const AbsParameter> op{p.clone()};
  std::shared_ptr<const AbsFunction>  of{f.clone()};
  return FunctionTimesParameter(op, of);
}

FunctionPlusParameter operator + (const AbsParameter & p, const AbsFunction &f) {
  std::shared_ptr<const AbsParameter> o{p.clone()};
  return FunctionPlusParameter(o, &f);
}

FunctionPlusParameter operator - (const AbsParameter & p, const AbsFunction &f) {
  GENFUNCTION MinusF = -f;
  std::shared_ptr<const AbsParameter> o{p.clone()};
  return FunctionPlusParameter(o, &MinusF);
}

FunctionTimesParameter operator / (const AbsParameter & p, const AbsFunction &f) {
  GENFUNCTION oneOverF = 1.0/f;
  std::shared_ptr<const AbsParameter> op{p.clone()};
  std::shared_ptr<const AbsFunction>  of{oneOverF.clone()};
  return FunctionTimesParameter(op,of);
}
} // namespace Genfun
