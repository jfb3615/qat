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

#include "QatGenericFunctions/SimpleIntegrator.h"
#include "QatGenericFunctions/AbsFunction.h"
// Constructor:

namespace Genfun {

  class SimpleIntegrator::Clockwork {

  public:

    double                 a;
    double                 b;
    const QuadratureRule * rule;
    unsigned int           nIntervals;
    bool                   isClosed;
  };

  SimpleIntegrator::SimpleIntegrator(double                  a, 
				     double                  b, 
				     const QuadratureRule   &rule,
				     unsigned int            nIntervals):
    c(new Clockwork())
  {
    c->a=a;
    c->b=b;
    c->rule=rule.clone();
    c->nIntervals=nIntervals;
    c->isClosed  =false;
    //
    // Is the quadrature rule closed? If so set flag.
    //
    if (rule.size()>1) {
      unsigned int begin=0,end=rule.size()-1;
      if (rule.abscissa(begin)==0 && rule.abscissa(end)==1.0) {
	c->isClosed=true;
      }      
    }
  }
  
  // Copy Constructor:
  SimpleIntegrator::SimpleIntegrator(const SimpleIntegrator & source):
    c(new Clockwork())
  {
    c->a=source.c->a;
    c->b=source.c->b;
    c->rule=source.c->rule->clone();
    c->nIntervals=source.c->nIntervals;
    c->isClosed=source.c->isClosed;
  }
  
  // Assignment Operator:
  SimpleIntegrator & SimpleIntegrator::operator=(const SimpleIntegrator & source) {
    if (&source !=this) {
      delete c->rule;
      c->a=source.c->a;
      c->b=source.c->b;
      c->rule=source.c->rule->clone();
      c->nIntervals=source.c->nIntervals;
      c->isClosed=source.c->isClosed;
     }
    return *this;
  }
  
  // Destructor:
  SimpleIntegrator::~SimpleIntegrator(){
    delete c->rule;
    delete c;
  }
  
  // Take the definite integral of a function between the bounds:
  double SimpleIntegrator::operator () (const AbsFunction & function) const{

    double y=0.0;

    const QuadratureRule *rule=c->rule;
    unsigned int nIntervals=c->nIntervals;
    unsigned int size=rule->size();
    double dx=(c->b-c->a)/nIntervals;
    double x0=c->a;
    bool isClosed=c->isClosed;
    double wClosed = rule->weight(0) +  rule->weight(size-1);

    for (unsigned int i=0;i<c->nIntervals;i++) {
      double x=x0;
      unsigned int beginLimit = isClosed ? 1              : 0;
      unsigned int endLimit   = isClosed ? rule->size()-1 : rule->size();
      if (isClosed) {
	if (i==0) {
	  beginLimit=0;
	}
	if (i==nIntervals-1){
	  endLimit=rule->size();
	}
	else {
	  x=x0+dx;
	  y+=function(x)*wClosed*dx;
	}
      }
      for (unsigned int s=beginLimit;s<endLimit;s++) {
	x=x0+rule->abscissa(s)*dx;
	y+=function(x)*rule->weight(s)*dx;
      }
      x0+=dx;
    }
    return y;
  }
}
