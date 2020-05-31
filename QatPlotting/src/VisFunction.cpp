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

#include "QatPlotting/VisFunction.h"
#include <vector>
#include <iostream>
class VisFunction::Clockwork {
public:
  std::string                                 name;
  std::vector<Genfun::Parameter *>            parameter; 
  std::vector<const Genfun::AbsFunction *>    function;
  std::vector<const Cut<double> *>            domainRestriction;
  PRectF                                      rectHint;
  VisFunction::Mode                           mode=VisFunction::NORMAL;
};

VisFunction::VisFunction(const std::string & name):
  c(new Clockwork()) 
{
  c->name = name;
}

VisFunction::~VisFunction() 
{
  for (unsigned int i=0;i<c->function.size();i++) {
    delete c->function[i];
    delete c->domainRestriction[i];
  }
  for (unsigned int i=0;i<c->parameter.size();i++) {
    delete c->parameter[i];
  }
  delete c;
}


void VisFunction::addParameter(Genfun::Parameter *p) {
  c->parameter.push_back(p);
}

unsigned int VisFunction::getNumParameters() const {
  return c->parameter.size();
}

Genfun::Parameter *VisFunction::getParameter(unsigned int i) const {
  return c->parameter[i];
}


void VisFunction::addFunction(Genfun::GENFUNCTION f, const Cut<double> * domainRestriction) {
  c->function.push_back(f.clone());
  c->domainRestriction.push_back(domainRestriction ? domainRestriction->clone(): NULL);
}

unsigned int VisFunction::getNumFunctions() const {
  return c->function.size();
}

const Genfun::AbsFunction *VisFunction::getFunction(unsigned int i) const {
    return c->function[i];
}

const Cut<double> *VisFunction::getDomainRestriction(unsigned int i) const {
    return c->domainRestriction[i];
}


const std::string & VisFunction::name() const {
  return c->name;
}


const PRectF & VisFunction::rectHint() const {
  return c->rectHint;
}


PRectF & VisFunction::rectHint() {
  return c->rectHint;
}

VisFunction::Mode VisFunction::getMode() const {
  return c->mode;
}

void VisFunction::setMode(Mode mode) {
  c->mode=mode;
}
