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

#ifndef _MinuitMinimizer_h_
#define _MinuitMinimizer_h_
#include <vector>
#include "Eigen/Dense"
//
// The class that will do all the work for us:
//
//
namespace Genfun {
  class AbsFunctional;
  class Parameter;
  class AbsFunction;
}

class ObjectiveFunction;

namespace ROOT {
  namespace Minuit2 {
    class Minuit2Minimizer;
  }
}

class MinuitMinimizer {
  
public:

  class ObjectiveInfo;
  
  // Constructor:
  MinuitMinimizer                 (bool verbose=false);

  // Destructor:
  virtual ~MinuitMinimizer                ();

  // Add a parameter.  Minuit minimizes the statistic w.r.t the parameters.
  virtual void addParameter       (Genfun::Parameter * par);

  // Add a statistic:  a function, and a functional (e.g. Chisqaured, Likelihood)  
  void addStatistic               (const Genfun::AbsFunctional * functional, const Genfun::AbsFunction * function);

  // Add a statistic:  a (Gaussian) constraint or similar function of parameters.
  void addStatistic               (const ObjectiveFunction *);

  // Tell minuit to minimize this:
  virtual void minimize           ();

  // Get the parameter value:
  double getValue(const Genfun::Parameter *) const;

  // Get the parameter error:
  double getError(const Genfun::Parameter *ipar, const Genfun::Parameter *jpar=NULL) const;

  // Get the functionValue
  double getFunctionValue() const;

  // Get the status
  int getStatus() const;

  // Get all of the values, as a vector:
  Eigen::VectorXd  getValues() const;
  
  // Get all of the errors, as a matrix:
  Eigen::MatrixXd getErrorMatrix() const;

  // Get information on parameters:
  unsigned int getNumParameters() const;

  // Get Parameter:
  const Genfun::Parameter *getParameter( unsigned int i) const;

  // Get Parameter
  Genfun::Parameter * getParameter(unsigned int i);

  
private:

  // Illegal operations:
  MinuitMinimizer(const MinuitMinimizer & ) = delete;
  MinuitMinimizer & operator=(const MinuitMinimizer & ) = delete;

  // Private member data:
  ROOT::Minuit2::Minuit2Minimizer *m2minimizer;
  ObjectiveInfo                   *objInfo;
  bool                            verbose;


};

class HistogramManager;

struct MinuitMeasurement {
  double value;
  double error;
};

// This records the state of the minimizer in an output directory:
void record(HistogramManager *output, const MinuitMinimizer & minimizer);

// This retrieves a single measurement:
MinuitMeasurement getMeasurement(const HistogramManager *manager, const std::string & name);

#endif
