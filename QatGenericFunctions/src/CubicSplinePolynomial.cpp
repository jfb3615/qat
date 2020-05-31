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

#include  "QatGenericFunctions/CubicSplinePolynomial.h"
#include  "Eigen/Dense"
#include <cmath>
#include <cfloat>
namespace Genfun {
  FUNCTION_OBJECT_IMP(CubicSplinePolynomial)
  
  class CubicSplinePolynomial::Clockwork {
  public:

    bool                                   stale;
    mutable Eigen::MatrixXd                A;
    mutable Eigen::VectorXd                V;
    mutable Eigen::VectorXd                Q;
    std::vector<std::pair<double,double> > xPoints;
    void computeSlopes() const {

      unsigned int N=xPoints.size()-1;
      A=Eigen::MatrixXd::Zero(N+1,N+1);
      V=Eigen::VectorXd::Zero(N+1);
 
      // First take care of the "normal elements, i=1,N-1;
      for (unsigned int i=1;i<N;i++) {

	double dxPlus = xPoints[i+1].first -xPoints[i].first;
	double dyPlus = xPoints[i+1].second-xPoints[i].second;
	double mPlus  = dyPlus/dxPlus;

	double dx = xPoints[i].first -xPoints[i-1].first;
	double dy = xPoints[i].second-xPoints[i-1].second;
	double m  = dy/dx;

	A(i,i-1) = 1/dx;
	A(i,i)   = 2*(1/dxPlus + 1/dx);
	A(i,i+1) = 1/dxPlus;

	V[i] = 3*(m/dx+mPlus/dxPlus); 

      }
      // Special treatment for i=0;
      {
	double dx = xPoints[1].first -xPoints[0].first;
	double dy = xPoints[1].second-xPoints[0].second;
	double m  = dy/dx;
	A(0,0) = 2.0;
	A(0,1) = 1;
	V(0)    = 3*m;
      }
      // Special treatment for i=N-1;
      {
	double dx = xPoints[N].first -xPoints[N-1].first;
	double dy = xPoints[N].second-xPoints[N-1].second;
	double m  = dy/dx;
	A(N,N-1) = 1.0;
	A(N,N)   = 2.0;
	V(N)      = 3*m;
      }
      
      Q=A.lu().solve(V);
    }
  };

  CubicSplinePolynomial::CubicSplinePolynomial()
    :Genfun::AbsFunction(),c(new Clockwork())
  {
    c->stale=true;
  }
  
  CubicSplinePolynomial::CubicSplinePolynomial(const CubicSplinePolynomial & right)
    :Genfun::AbsFunction(),c(new Clockwork) 
  {
    (*c) = (*right.c);
  }
  
  CubicSplinePolynomial::~CubicSplinePolynomial() {
    delete c;
  }
  
  double CubicSplinePolynomial::operator() (double x) const {
    unsigned int N=c->xPoints.size()-1;

    if (c->xPoints.size()==0) return 0;
    if (c->xPoints.size()==1) return c->xPoints[0].second;
    if (c->stale) {
      c->computeSlopes();
      c->stale=false;
    }
    std::pair<double,double> fk(x,0);
    std::vector<std::pair<double,double> >::const_iterator 
      n=std::lower_bound(c->xPoints.begin(),c->xPoints.end(),fk);
    unsigned int i = n-c->xPoints.begin();
    if (i==0) {      
      double x0=c->xPoints[0].first;
      double y0=c->xPoints[0].second;
      double m = c->Q[0];
      return y0 + m*(x-x0);
    }
    else if (i==c->xPoints.size()) {
      double x0=c->xPoints[N].first;
      double y0=c->xPoints[N].second;
      double m = c->Q[N];
      return y0 + m*(x-x0);
    }
    else {
      double x0=c->xPoints[i-1].first;
      double x1=c->xPoints[i].first;
      double y0=c->xPoints[i-1].second;
      double y1=c->xPoints[i].second;
      double dx = x1-x0;
      double dy = y1-y0;
      double m = dy/dx;
      double Q0 = c->Q[i-1];
      double Q1 = c->Q[i];
      double a  = (Q0-m)*dx;
      double b  = (m-Q1)*dx;
      double t = (x-x0)/dx;
      double u = (1-t);
      return u*y0+t*y1 + t*u*(u*a + t*b);
    }
  }
  
  void CubicSplinePolynomial::addPoint( double x, double y) {
    c->xPoints.push_back(std::make_pair(x,y));
    std::sort(c->xPoints.begin(),c->xPoints.end());
    c->stale=true;
  }

  void CubicSplinePolynomial::getRange( double & min, double & max) const {
    min=DBL_MAX, max=-DBL_MAX;
    for (unsigned int i=0;i<c->xPoints.size();i++) {
      min = std::min(min,c->xPoints[i].first);
      max = std::max(max,c->xPoints[i].first);
    }
  }
} // namespace Genfun
