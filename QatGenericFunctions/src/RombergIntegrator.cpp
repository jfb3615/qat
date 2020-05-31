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

#include <cmath>
#include <vector>
#include <stdexcept>
#include "QatGenericFunctions/RombergIntegrator.h"
#include "QatGenericFunctions/AbsFunction.h"
#include "QatGenericFunctions/InterpolatingPolynomial.h"


namespace Genfun {


  class RombergIntegrator::Clockwork {
    
    //
    // This class has limited visibility its functions, data,
    // and nested classes are all public:
    // 
    
  public:
    
    class QuadratureRule {
      
    public:
      
      // Constructorctor:
      QuadratureRule() {};
      
      // Destructor:
      virtual ~QuadratureRule() {};
      
      // Integrate at the j^th level of refinement:
      virtual double integrate(const AbsFunction & function, 
			       double a,
			       double b,
			       unsigned int j) const=0;
      
      // Return the step multiplier:
      virtual unsigned int stepMultiplier () const=0;

      // Return the number of function calls:
      virtual unsigned int numFunctionCalls() const =0;

    };
    
    class TrapezoidQuadratureRule:public QuadratureRule {
      
    public:
      
      // Constructor:
      TrapezoidQuadratureRule():retVal(0),nFunctionCalls(0) {};
      
      // Destructor:
      ~TrapezoidQuadratureRule() {};
      
      // Integrate at the j^th level of refinement:
      virtual double integrate(const AbsFunction & function, 
			       double a,
			       double b,
			       unsigned int j) const;
      
      // The step is doubled at each level of refinement:
      virtual unsigned int stepMultiplier () const {return 2;}

      // Returns number of function calls:
      virtual unsigned int numFunctionCalls() const {return nFunctionCalls;};
      
    private:

      mutable double retVal;
      mutable unsigned int nFunctionCalls;

    };
    
    class XtMidpointQuadratureRule:public QuadratureRule {
      
    public:
      
      // Constructor:
      XtMidpointQuadratureRule():retVal(0),nFunctionCalls(0) {};
      
      // Destructorctor:
      ~XtMidpointQuadratureRule() {};
      
      // Integrate at the j^th level of refinement:
      virtual double integrate(const AbsFunction & function, 
			       double a,
			       double b,
			       unsigned int j) const;
      
      // The step is tripled at each level of refinement:
      virtual unsigned int stepMultiplier () const {return 3;}

      // Returns number of function calls:
      virtual unsigned int numFunctionCalls() const {return nFunctionCalls;};
      
    private:

      mutable double retVal;
      mutable unsigned int nFunctionCalls;
    };
    
    double                              a;              // lower limit of integration
    double                              b;              // upper limit of integration
    RombergIntegrator::Type              type;           // open or closed
    mutable unsigned int                nFunctionCalls; // bookkeeping
    unsigned int                        MAXITER;        // Max no of step doubling, tripling, etc.
    double                              EPS;            // Target precision
    unsigned int                        K;              // Minimum order =2*5=10
    ConvergenceCriterion                convCrit;       // Convergence criterion
    
  };

  
  RombergIntegrator::RombergIntegrator(double a, double b, Type type, ConvergenceCriterion crit):
    c(new Clockwork()) {
    c->a              = a;
    c->b              = b;
    c->type           = type;
    c->nFunctionCalls = 0;
    c->MAXITER        = type==OPEN ? 20 : 14; 
    c->EPS            = 1.0E-6;
    c->K              = 5;
    c->convCrit       = crit;
  }
  
  RombergIntegrator::~RombergIntegrator() {
    delete c;
  }

  RombergIntegrator::RombergIntegrator(const RombergIntegrator & right) 
  :AbsFunctional(right), c(new Clockwork(*right.c)) {
  }

  RombergIntegrator & RombergIntegrator::operator = (const RombergIntegrator & right) {
    if (this!=&right) {
      delete c;
      c = new Clockwork(*right.c);
    }
    return *this;
  }
  
  void RombergIntegrator::setEpsilon(double eps) {
    c->EPS=eps;
  }

  void RombergIntegrator::setMaxIter(unsigned int maxIter) {
    c->MAXITER=maxIter;
  }

  void RombergIntegrator::setMinOrder(unsigned int minOrder) {
    c->K=(minOrder+1)/2;
  }

  double RombergIntegrator::operator () (const AbsFunction & function) const {

    const Clockwork::QuadratureRule * rule = c->type==OPEN ? 
      (Clockwork::QuadratureRule *) new Clockwork::XtMidpointQuadratureRule() : 
      (Clockwork::QuadratureRule *) new Clockwork::TrapezoidQuadratureRule();
    double xMult=rule->stepMultiplier();

    c->nFunctionCalls=0;

    double h=1.0;
    InterpolatingPolynomial Interpolator;
    for (unsigned int j=0;j<c->MAXITER;j++) {
      double s=rule->integrate(function, c->a, c->b, j+1);
      c->nFunctionCalls=rule->numFunctionCalls();
      if (j+1>=c->K) {
	Interpolator.addPoint(h,s);
	double sInterp=Interpolator(0.0); // interpolate to zero.
	double delta=Interpolator.getDelta();
	double cutoff = c->convCrit==RELATIVE ? c->EPS*fabs(sInterp) : c->EPS;	
	if (fabs(delta) <= cutoff) {
	  delete rule;
	  return sInterp;
	}
      }
      h=h/xMult/xMult;
    }
    delete rule;
    throw std::runtime_error("RombergIntegrator:  too many steps.  No convergence");
    return 0.0;                   // Never get here.
  }
  
  unsigned int RombergIntegrator::numFunctionCalls() const {
    return c->nFunctionCalls;
  }

  // Quadrature rules:
  double RombergIntegrator::Clockwork::TrapezoidQuadratureRule::integrate(const AbsFunction & function, double a, double b, unsigned int n) const {
    if (n==1) {
      retVal = 0.5*(b-a)*(function(a)+function(b));
      nFunctionCalls+=2;
    }
    else { 
      unsigned int nMeshpoints = 1<<(n-2);
      double dx = (b-a)/nMeshpoints;
      double x=a+0.5*dx;
      double sum=0.0;
      for (unsigned int j=1;j<=nMeshpoints;j++) {
	sum += function(x);
	nFunctionCalls++;
	x+=dx;
      }
      retVal = 0.5*(retVal+(b-a)*sum/nMeshpoints);
    }
    return retVal;
  }

  // Quadrature rules:
  double RombergIntegrator::Clockwork::XtMidpointQuadratureRule::integrate(const AbsFunction & function, double a, double b, unsigned int n) const {
    unsigned int it, j;
    if (n==1) {
      retVal = (b-a)*(function((a+b)/2.0));
      nFunctionCalls+=1;
    }
    else { 
      for (it=1,j=1;j<n-1;j++)  it *=3;
      double tnm=it;
      double del  = (b-a)/(3.0*tnm);
      double ddel = del+del;
      double x=a+0.5*del;
      double sum=0;
      for (j=1;j<=it;j++) {
	sum +=function(x);
	x+=ddel;
	sum +=function(x);
	x+=del;
	nFunctionCalls+=2;
      }
      retVal = (retVal+(b-a)*sum/tnm)/3.0;
    }
    return retVal;
  }
  
  

} // namespace Genfun
