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

#ifndef _RootFinder_h_
#define _RootFinder_h_
//============================================================================//
//                                                                            //
// This header defines a RootFinder base class as well as two subclasses,     //
// NewtonRaphson and DeflatedNetwonRaphson.  The former is generally          //
// applicable, the latter is applicable to polynomials with simple roots,     //
// particularly good for orthogonal polynomials.                              //
//                                                                            //
// Deflated Newton-Raphson has memory and should be used to find roots one    //
// by one until all n roots of the polynomial are discovered.  Going beyond   //
// that will cause the root finder to fail.                                   //
//                                                                            //
// RootFinders throw std::runtime_error if the maximum number of calls is     //
// exceeded.                                                                  //
//                                                                            //
//============================================================================//
#include "QatGenericFunctions/Sigma.h"
#include <cmath>
namespace Genfun {

  class RootFinder {
    
  public:

    // Constructor:
    RootFinder();

    // Destructor:
    virtual ~RootFinder();

    // Find a root:
    virtual double root(double estimatedRoot) const=0;

    // Lower and upper bounds:
    const double & lowerBound() const;
    double & lowerBound();
    const double & upperBound() const;
    double & upperBound();
    
  private:

    double _lowerBound=-HUGE_VAL;
    double _upperBound= HUGE_VAL;
    

  };

  class Bisection: public RootFinder {
    
  public:
    
    // Constructor
    Bisection(GENFUNCTION P, 
	      double EPS=1.0E-12,
	      unsigned int MAXCALLS=100);
    
    // Find the next root
    virtual double root(double estimatedRoot) const;

    // Retrieve number of calls (previous operation)
    unsigned int nCalls() const;
    
  private:

    // Illegal operations
    Bisection (const Bisection &)=delete;
    Bisection & operator = (const Bisection &)=delete;
    
    GENFUNCTION          p;
    double               eps;
    unsigned int         maxcalls;
    mutable unsigned int ncalls;
  };

  class NewtonRaphson: public RootFinder {
    
  public:
    
    // Constructor
    NewtonRaphson(GENFUNCTION P, 
			  double EPS=1.0E-12,
			  unsigned int MAXCALLS=100);
    
    // Find the next root
    virtual double root(double estimatedRoot) const;

    // Retrieve number of calls (previous operation)
    unsigned int nCalls() const;
    
  private:

    // Illegal operations
    NewtonRaphson (const NewtonRaphson &)=delete;
    NewtonRaphson & operator = (const NewtonRaphson &)=delete;
    
    GENFUNCTION          p;
    double               eps;
    unsigned int         maxcalls;
    mutable unsigned int ncalls;
  };


  class DeflatedNewtonRaphson: public RootFinder {
    
  public:
    
    // Constructor
    DeflatedNewtonRaphson(GENFUNCTION P, 
			  double EPS=1.0E-12,
			  unsigned int MAXCALLS=100);
    
    // Find the next root
    virtual double root(double estimatedRoot) const;

    // Retrieve number of calls (previous operation)
    unsigned int nCalls() const;
    
  private:

    // Illegal operations
    DeflatedNewtonRaphson(const DeflatedNewtonRaphson &)=delete;
    DeflatedNewtonRaphson & operator = (const DeflatedNewtonRaphson &)=delete;
    
    GENFUNCTION          p;
    double               eps;
    unsigned int         maxcalls;
    mutable Sigma        sum;
    mutable unsigned int ncalls;
  };
}
#include "QatGenericFunctions/RootFinder.icc"
#endif
