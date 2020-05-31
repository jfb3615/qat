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

#include "QatGenericFunctions/AbsFunctional.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatDataModeling/MinuitMinimizer.h"
#include "QatDataModeling/ObjectiveFunction.h"
#include "Minuit2/Minuit2Minimizer.h"
#include <stdexcept>
#include <iostream>
#include <string>

class MinuitMinimizer::ObjectiveInfo: public ROOT::Math::IMultiGenFunction {

public:

  //Constructor:
  ObjectiveInfo():ROOT::Math::IMultiGenFunction(){}

  // Destructor:
  ~ObjectiveInfo(){}
  
  // Required Clone function:
  MinuitMinimizer::ObjectiveInfo * Clone() const {
    return new ObjectiveInfo(*this);
  }

  // Return number of parameters:
  virtual unsigned int NDim() const {
    return _parList.size();
  }

  // Evaluate!
  virtual double DoEval(const double * x) const {
    // Transfer inputs to parametes:
    for (size_t i=0;i<_parList.size();i++) _parList[i]->setValue(x[i]);

    double functionValue=0;
  
    for (size_t j=0;j<_functionalList.size();j++) {
      try {
	functionValue += (*_functionalList[j])(*_functionList[j]);
      }
      catch (const std::exception & e) {
	std::cout << e.what() <<std::endl;
	for (unsigned int p=0;p<_parList.size();p++) std::cerr << *_parList[p] << std::endl;
      }
    }
    for (unsigned int j=0;j<_objectiveList.size();j++) {
      functionValue += (*_objectiveList[j])();
    }
    return functionValue;
  }

  
  std::vector<const Genfun::AbsFunctional *> _functionalList;
  std::vector<const Genfun::AbsFunction   *> _functionList;
  std::vector<const ObjectiveFunction     *> _objectiveList;
  std::vector<Genfun::Parameter *>           _parList;

};


MinuitMinimizer::MinuitMinimizer(bool verbose)
  :
  m2minimizer(new ROOT::Minuit2::Minuit2Minimizer),
  objInfo(new ObjectiveInfo()),
  verbose(verbose)
{
}
 
void MinuitMinimizer::addParameter(Genfun::Parameter * par) {

  // add the parameter to the list:
  objInfo->_parList.push_back(par);
}


MinuitMinimizer::~MinuitMinimizer() {
  delete m2minimizer;
  delete objInfo;
}
void MinuitMinimizer::minimize() {
  m2minimizer->Clear();
  m2minimizer->SetFunction(*objInfo);
  for (size_t i=0;i<objInfo->_parList.size();i++) {
    const Genfun::Parameter *p=objInfo->_parList[i];
    m2minimizer->SetLimitedVariable(i, p->getName(), p->getValue(), (p->getUpperLimit()-p->getLowerLimit())/100.0, p->getLowerLimit(), p->getUpperLimit());
  }
  m2minimizer->Minimize();
  if (verbose) {
    m2minimizer->PrintResults();
  }
}

void MinuitMinimizer::addStatistic(const Genfun::AbsFunctional * functional, const Genfun::AbsFunction * function) {
  objInfo->_functionalList.push_back(functional);
  objInfo->_functionList.push_back(function);
}

void MinuitMinimizer::addStatistic (const ObjectiveFunction *objective) {
  objInfo->_objectiveList.push_back(objective);
}

// Get the parameter value:
double MinuitMinimizer::getValue(const Genfun::Parameter *par) const {
  for (unsigned int i=0;i<objInfo->_parList.size();i++) {
    if (objInfo->_parList[i]==par) {
      return m2minimizer->X()[i];
    }
  }
  throw std::runtime_error("Error in MinuitMinimizer::getValue.  Parameter not found");
  return 0;
}

// Get the parameter error:
double MinuitMinimizer::getError(const Genfun::Parameter *ipar, const Genfun::Parameter *jpar) const {
  
  int iIndex=-1, jIndex=-1;
  
  for (unsigned int i=0;i<objInfo->_parList.size();i++) {
    if (objInfo->_parList[i]==ipar) {
      iIndex=i;
    }
  }
  
  if (iIndex!=-1) {
    if (jpar) {
      for (unsigned int j=0;j<objInfo->_parList.size();j++) {
	if (objInfo->_parList[j]==jpar) {
	  jIndex=j;
	}
      }
    }
    else {
      jIndex=iIndex;
    }
  }
  
  if (iIndex !=-1 && jIndex!=-1) return m2minimizer->CovMatrix(iIndex,jIndex);
  throw std::runtime_error("Error in MinuitMinimizer::getError.  Parameter not found");
  return 0;
}


double MinuitMinimizer::getFunctionValue() const {
  return m2minimizer->MinValue();
}

int MinuitMinimizer::getStatus() const {
  return m2minimizer->CovMatrixStatus();
}

Eigen::VectorXd MinuitMinimizer::getValues() const {
  Eigen::VectorXd valList(objInfo->_parList.size());
  for (int i=0;i<valList.rows();i++) valList(i)=m2minimizer->X()[i];
  return valList;
}

Eigen::MatrixXd MinuitMinimizer::getErrorMatrix()  const {
  Eigen::MatrixXd errorList(objInfo->_parList.size(), objInfo->_parList.size());
  for (int i=0;i<errorList.rows();i++) {
    for (int j=0;j<errorList.cols();j++) {
      errorList(i,j)=m2minimizer->CovMatrix(i,j);
    }
  }
  return errorList;
}


unsigned int MinuitMinimizer::getNumParameters() const {
  return objInfo->_parList.size();
}

Genfun::Parameter * MinuitMinimizer::getParameter(unsigned int i)  {
  return objInfo->_parList[i];
}

const Genfun::Parameter * MinuitMinimizer::getParameter(unsigned int i) const {
  return objInfo->_parList[i];
}
  
