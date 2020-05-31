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

#include "QatGenericFunctions/RootFinder.h"
#include "QatGenericFunctions/Variable.h"
#include <limits>
#include <cmath>
#include <stdexcept>

namespace Genfun {

  RootFinder::RootFinder(){
  }

  RootFinder::~RootFinder(){
  }
  


  Bisection::Bisection(GENFUNCTION P, double EPS, unsigned int maxcalls):p(P),eps(EPS),maxcalls(maxcalls),ncalls(0) {}
  
  unsigned int Bisection::nCalls() const {
    return ncalls;
  }
  
  
  double  Bisection::root(double estimatedRoot) const {
    double a=lowerBound(), b=upperBound();
    double pa=p(a), pb=p(b);
    if (pa*pb>=0) {
      throw std::runtime_error("Bisection methods fails because p(a)*p(b)>=0");
    }
    double c=estimatedRoot, pc=p(c);

    if (pc==0) return c;

    // We start at i==4.  Three calls have already been made;
    for (unsigned int i=3;i<maxcalls;i++) {
      a= pc*pa>0 ? c:a;
      b= pc*pb>0 ? c:b;
      c=(a+b)/2;
      
      double tVariable = fabs(a-b);
      if (tVariable<eps)  {    // We are finished:
	ncalls=i;
	return c;
      }                         // We must continue
      else {
	pa=p(a);
	pb=p(b);
	pc=p(c);
	if (pc==0) {
	  ncalls=i;
	  return c;
	}
      }
    }
    throw std::runtime_error("Bisection:: Max iterations exceeded.");
    return 0;
  }

  NewtonRaphson::NewtonRaphson(GENFUNCTION P, double EPS, unsigned int maxcalls):p(P),eps(EPS),maxcalls(maxcalls),ncalls(0) {}
  
  unsigned int NewtonRaphson::nCalls() const {
    return ncalls;
  }
  
  
  double  NewtonRaphson::root(double estimatedRoot) const {
    double x=estimatedRoot;
    if (p(x)==0) return x;

    for (unsigned int i=0;i<maxcalls;i++) {
      double x1=x;
      double deltaX=-p(x)/p.prime()(x);
      if (!std::isfinite(deltaX)) {
	throw std::runtime_error("NewtonRaphson: non finite value");
      }
      double pX=x+deltaX;
      while (pX<lowerBound() || pX>upperBound()) {
	deltaX/=2.0;
	if (deltaX==0.0) {
	  ncalls=i;
	  return x;
	}
	pX=x+deltaX;
      }
      x=pX;
      double tVariable = fabs(x1-x);
      if (tVariable<eps)  {    // We are finished:
	ncalls=i;
	return x;
      }                         // We must continue
      else {
	x1=x;
      }
    }
    throw std::runtime_error("NewtonRaphson:: Max iterations exceeded.");
    return 0;
  }


  DeflatedNewtonRaphson::DeflatedNewtonRaphson(GENFUNCTION P, double EPS, unsigned int maxcalls):p(P),eps(EPS),maxcalls(maxcalls),ncalls(0) {}
  
  unsigned int DeflatedNewtonRaphson::nCalls() const {
    return ncalls;
  }
  
  
  double  DeflatedNewtonRaphson::root(double estimatedRoot) const {
    Variable X;
    double x=estimatedRoot;
    if (p(x)==0) {
      sum.accumulate(1/(X-x));
      return x;
    }
    for (unsigned int i=0;i<maxcalls;i++) {
      double x1=x;
      double deltaX=-p(x)/(p.prime()(x)-p(x)*sum(x));
      if (!std::isfinite(deltaX)) {
	throw std::runtime_error("DeflatedNewtonRaphson: non finite value");
      }
      double pX=x+deltaX;
      while (pX<lowerBound() || pX>upperBound()) {
	deltaX/=2.0;
	if (deltaX==0.0) {
	  ncalls=i;
	  sum.accumulate(1/(X-x));
	  return x;
	}
	pX=x+deltaX;
      }
      x=pX;
      double tVariable = fabs(x1-x);
      if (tVariable<eps)  {    // We are finished:
	ncalls=i;
	sum.accumulate(1/(X-x));
	return x;
      }                         // We must continue
      else {
	x1=x;
      }
    }
    throw std::runtime_error("DeflatedNewtonRaphson:: Max iterations exceeded.");
    return 0;
  }



  
}
