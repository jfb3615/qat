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

// *-----------------------------------------------------------------*/
// * A base class for functions to be visualized in the presenter    */
// * ----------------------------------------------------------------*/
#ifndef VisFunction_h_
#define VisFunction_h_
class Plotable;
#include <map>
#include <string>
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/AbsFunction.h"
#include "QatGenericFunctions/CutBase.h"
#include "QatPlotting/PRectF.h"
class PRectF;

class VisFunction {
  
public:

  enum Mode {NORMAL, ORBIT};
  
  // Constructor:
  VisFunction(const std::string & name);

  // Destructor:
  virtual ~VisFunction();

  // Get the number of parameters:
  unsigned int getNumParameters() const;

  // Get the Parameter:
  Genfun::Parameter *getParameter(unsigned int i) const;

  // Add a parameter
  void addParameter(Genfun::Parameter *parameter);

  // Get the number of functions:
  unsigned int getNumFunctions() const;

  // Get the Function
  const Genfun::AbsFunction *getFunction(unsigned int i) const;

  // Get the Domain Resctriction (NULL if unrestricted)
  const Cut<double> *getDomainRestriction(unsigned int i) const;

  // Add a function.  (Clones the function in, also the cut if any).
  void addFunction(Genfun::GENFUNCTION f, const Cut<double> *domainRestriction=NULL);

  // Name
  const std::string & name() const;

  // Rect Hint
  const PRectF & rectHint() const;

  // Rect Hint
  PRectF & rectHint();

  // Get the mode:
  Mode getMode() const;

 protected:
  
  void setMode(Mode mode);
  
 private:
  
  // Illegal operations:
  VisFunction (const VisFunction & );
  VisFunction & operator= (const VisFunction &);

  // Internals:
  class Clockwork;
  Clockwork *c;

};
#endif
