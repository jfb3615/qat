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

#include "QatGenericFunctions/Parameter.h"

namespace Genfun {
PARAMETER_OBJECT_IMP(Parameter)

Parameter::Parameter(std::string name, double value, double lowerLimit, double upperLimit):
_name(name),_value(value),_lowerLimit(lowerLimit),_upperLimit(upperLimit),_sourceParameter(std::make_shared<const AbsParameter *>(nullptr))
{
} 

Parameter::~Parameter() {
}

std::ostream & operator << ( std::ostream & o, const Parameter & p) {
  return o << p.getName()   << "\t" << " value = "
	   << p.getValue()  << "\t" <<" limits: ["
	   << p.getLowerLimit() << "," 
	   << p.getUpperLimit() << "]" << std::endl; 
}

const std::string & Parameter::getName() const {
  return _name;
}

double Parameter::getValue() const
{
  if (_origin) return _origin->getValue();
  if (*_sourceParameter) {
    return (*_sourceParameter)->getValue();
  }
  else {
    return _value;
  }
}

double Parameter::getLowerLimit() const
{
  if (_origin) return _origin->getLowerLimit();
  if (*_sourceParameter) {
    return -1E-100;
  }
  else {
    return _lowerLimit;
  }
}

double Parameter::getUpperLimit() const
{
  if (_origin) return _origin->getUpperLimit();
  if (*_sourceParameter) {
    return 1E100;
  }
  else {
    return _upperLimit;
  }
}

void Parameter::setValue(double value)
{
  if (*_sourceParameter) {
    std::cerr
      << "Warning:  Parameter is connected.  setValue has no effect."
      << std::endl;
  }
  else if (_origin) {
    std::cerr
      << "Warning:  Parameter is a copy.  setValue has no effect."
      << std::endl;
  }
  else {
    _value=value;
  }
}

void Parameter::setLowerLimit(double lowerLimit)
{


  if (*_sourceParameter) {
    std::cerr
      << "Warning:  Parameter is connected.  setLowerLimit has no effect."
      << std::endl;
  }
  else if (_origin) {
    std::cerr
      << "Warning:  Parameter is a copy.  setLowerLimit has no effect."
      << std::endl;
  }
  else {
    _lowerLimit=lowerLimit;
  }
}

void Parameter::setUpperLimit(double upperLimit)
{
  if (*_sourceParameter) {
    std::cerr
      << "Warning:  Parameter is connected.  setUpperLimit has no effect."
      << std::endl;
  }
  else if (_origin) {
    std::cerr
      << "Warning:  Parameter is a copy. setUpperLimit has no effect."
      << std::endl;
  }
  else {
    _upperLimit=upperLimit;
  }
}

void Parameter::connectFrom(const AbsParameter *  source)
{
  const Parameter *sp = source->parameter();
  if (sp && *sp->_sourceParameter) {
    *_sourceParameter=sp;
  }
  else {
    *_sourceParameter = source;
  }
}

 
Parameter::Parameter(const Parameter & right):
  AbsParameter(right),
  _name(right._name),
  _value(right._value),
  _lowerLimit(right._lowerLimit),
  _upperLimit(right._upperLimit),
  _origin(&right),
  _sourceParameter(right._sourceParameter)
{
  //while (_origin) {
  //  if (_origin->_origin) _origin=_origin->_origin; else break;
  //}
}
  
const Parameter & Parameter::operator=(const Parameter &right) {
  if (this != &right) {	
    _name=right._name;
    _value=right._value;
    _lowerLimit=right._lowerLimit;
    _upperLimit=right._upperLimit;
    _origin=&right;
    _sourceParameter=right._sourceParameter;
  }
  //  while (_origin) {
  //  if (_origin->_origin) _origin=_origin->_origin; else break;
  // }

  return *this;
}

} // namespace Genfun
