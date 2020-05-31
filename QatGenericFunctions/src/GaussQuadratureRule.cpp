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

#define _GAUSSQUADRATURERULE_CPP_
#include "QatGenericFunctions/GaussQuadratureRule.h"
#include "QatGenericFunctions/TchebyshevPolynomial.h"
#include "QatGenericFunctions/FixedConstant.h"
#include "QatGenericFunctions/Variable.h"
#include "QatGenericFunctions/Exp.h"
#include "QatGenericFunctions/Sqrt.h"
#include "QatGenericFunctions/Power.h"
#include "Eigen/Dense"
namespace Genfun {



  GaussQuadratureRule::Shared   GaussLegendreRule::shared;
  GaussQuadratureRule::Shared   GaussHermiteRule::shared;
  GaussQuadratureRule::Shared   GaussTchebyshevRule::sharedFirstKind;
  GaussQuadratureRule::Shared   GaussTchebyshevRule::sharedSecondKind;
  GaussQuadratureRule::SharedV2 GaussLaguerreRule::shared;
  
  GaussQuadratureRule::GaussQuadratureRule(const std::string & name, 
					   unsigned int npoints, 
					   double min, 
					   double max, 
					   GENFUNCTION W) :
    _name(name),
    _npoints(npoints),
    _min(min),
    _max(max),
    _wFunction(W.clone())
  {
    
  }
  
  // Add an abscissa/weight pair:
  void GaussQuadratureRule::add(double abcissa, double weight){
    _abscissa.push_back(abcissa);
    _weight.push_back(weight);
  }

  
  void GaussQuadratureRule::calcData(const GaussQuadratureRule & rule, Data & d) {
    unsigned int n=rule.nPoints();
    Eigen::MatrixXd T=Eigen::MatrixXd::Zero(n,n);
#ifdef GAUSSQUAD_ALT_VERSION
    for (unsigned int i=0;i<n;i++) {
    for (unsigned int j=0;j<n;j++) {
        double an=rule.a(i);
	double bn=rule.b(i);
	double cn=rule.c(i);
	double anpOne=rule.a(i+1);
	double cnpOne=rule.c(i+1);
	if (j==i-1) {
	  double anmOne=rule.a(i-1);
	  T(i,j)=sqrt(cn/an/anmOne);
	}
	else if (j==i) {
	  T(i,j)=-bn/an;
	}
	else if (j==i+1) {
	  T(i,j)=sqrt(cnpOne/an/anpOne);
	}
      }
  }
#endif
    for (unsigned int i=0;i<n;i++) {
      for (unsigned int j=0;j<n;j++) {
	double an=rule.a(i);
	double bn=rule.b(i);
	if (j==i-1) {
	  T(i,j)=exp(rule.lognorm(i)-rule.lognorm(i-1))/rule.a(i-1);
	}
	else if (j==i) {
	  T(i,j)=-bn/an;
	}
	else if (j==i+1) {
	  T(i,j)=exp(rule.lognorm(i+1)-rule.lognorm(i))/an;
	}
      }
    } 
    Eigen::EigenSolver<Eigen::MatrixXd> solver(T);
    Eigen::VectorXcd eigenvalues=solver.eigenvalues();
    Eigen::MatrixXcd eigenvectors=solver.eigenvectors();
    for (int i=0;i<eigenvalues.rows();i++) {
      double x=eigenvalues(i).real();
      double w=pow(real(eigenvectors(0,i)),2)*rule.mu(); 
      d.xw.push_back(std::make_pair(x,w));
    }
    std::sort(d.xw.begin(),d.xw.end());
  }

 GaussLaguerreRule::GaussLaguerreRule(unsigned int npoint,double alpha):
   GaussQuadratureRule("Gauss-Laguerre", npoint, 0.0, HUGE_VAL, Exp()(-Variable())*Power(alpha)(Variable())),alpha(alpha){
   
   // This is done once per class per npoint value
   if (shared.dMap.find(GaussQuadratureRule::SharedV2::Key(nPoints(),alpha))==shared.dMap.end()) {
     GaussQuadratureRule::Data d;
     calcData(*this,d);
     shared.dMap[GaussQuadratureRule::SharedV2::Key(nPoints(),alpha)]=d;
   }
   const GaussQuadratureRule::Data & d=shared.dMap[GaussQuadratureRule::SharedV2::Key(nPoints(),alpha)];
   for (unsigned int i=0;i<d.xw.size();i++) {
     add(d.xw[i].first,d.xw[i].second);
   }
 }

  double GaussLaguerreRule::a(unsigned int i) const{
    return -1/double(i+1);
  }

  double GaussLaguerreRule::b(unsigned int i) const{
    return (2*i+alpha+1)/double(i+1);
  } 
  
  double GaussLaguerreRule::c(unsigned int i) const{
    return(i+alpha)/double(i+1);
  }

  double GaussLaguerreRule::lognorm(unsigned int i) const{
    return 0.5*(lgamma(i+alpha+1)-lgamma(i+1));
  }

  double GaussLaguerreRule::mu()              const{
    return tgamma(alpha+1);
  }
  

  const GaussLaguerreRule *GaussLaguerreRule::clone() const {
    return new GaussLaguerreRule (nPoints(),alpha);	       
  }    


  double GaussLegendreRule::a(unsigned int i) const{
    return (2*i+1)/double(i+1);
  }

  double GaussLegendreRule::b(unsigned int ) const{
    return 0;
  } 
  
  double GaussLegendreRule::c(unsigned int i) const{
    return i/double(i+1);
  }

  double GaussLegendreRule::lognorm(unsigned int i) const{
    return 0.5*log(2/double(2*i+1));
  }

  double GaussLegendreRule::mu()              const{
    return 2;
  }

  double GaussHermiteRule::a(unsigned int ) const{
    return 2;
  }

  double GaussHermiteRule::b(unsigned int ) const{
    return 0;
  } 
  
  double GaussHermiteRule::c(unsigned int i) const{
    return 2*i;
  }

  double GaussHermiteRule::lognorm(unsigned int i) const{
    return 0.5*(0.5*log(M_PI)+i*log(2.0)+lgamma(i+1));
  }

  double GaussHermiteRule::mu()              const{
    return sqrt(M_PI);
  }

  double GaussTchebyshevRule::a(unsigned int i) const{
    if (_type==TchebyshevPolynomial::FirstKind) {
      return i==0 ? 1:2;
    }
    else {
      return 2;
    }
  }

  double GaussTchebyshevRule::b(unsigned int ) const{
    return 0;
  } 
  
  double GaussTchebyshevRule::c(unsigned int ) const{
    return 1;
  }

  double GaussTchebyshevRule::lognorm(unsigned int i) const{
    return _type==TchebyshevPolynomial::FirstKind ? 
      (i==0 ? 0.5*log(M_PI) : 0.5*log(M_PI/2)) : 
      0.5*log(M_PI/2);

  }

  double GaussTchebyshevRule::mu()              const{
    return _type==TchebyshevPolynomial::FirstKind ? M_PI: M_PI/2.0;
  }

  
   GaussTchebyshevRule::GaussTchebyshevRule(unsigned int npoint, TchebyshevPolynomial::Type type):
    GaussQuadratureRule("Gauss-Tchebyshev", npoint, -1, 1, 
			type==TchebyshevPolynomial::FirstKind ? 
			Sqrt()(1/(1-Variable()*Variable())):
			Sqrt()(1*(1-Variable()*Variable()))),_type(type){

    GaussQuadratureRule::Shared & shared = type == TchebyshevPolynomial::FirstKind ? 
      sharedFirstKind: sharedSecondKind;
    //
    // For the tchebyshev cases we can use a simple analytical formula.
    //
    if (shared.dMap.find(nPoints())==shared.dMap.end()) {
      GaussQuadratureRule::Data d;
      for (unsigned int j=0;j<npoint;j++) {
	if (type== TchebyshevPolynomial::FirstKind) {
	  double x=cos((2*j+1)/double(2*npoint)*M_PI);
	  double w=M_PI/npoint; 
	  d.xw.push_back(std::make_pair(x,w));
	}
	else {
	  double x=cos((j+1)/double(npoint+1)*M_PI);
	  double w=M_PI/(npoint+1)*pow(sin((j+1)/double(npoint+1)*M_PI),2); 
	  d.xw.push_back(std::make_pair(x,w));
	}
      }
      std::sort(d.xw.begin(),d.xw.end());
      shared.dMap[nPoints()]=d;
    }
    const GaussQuadratureRule::Data & d=shared.dMap[nPoints()];
    for (unsigned int i=0;i<d.xw.size();i++) {
      add(d.xw[i].first,d.xw[i].second);
    }
  }

  const GaussTchebyshevRule *GaussTchebyshevRule::clone() const {
    return new GaussTchebyshevRule (nPoints(),_type);	       
  }    


  GaussHermiteRule::GaussHermiteRule(unsigned int npoint):
    GaussQuadratureRule("Gauss-Hermite", npoint, -HUGE_VAL, HUGE_VAL, Exp()(-Variable()*Variable())) {
   // This is done once per class per npoint value
    if (shared.dMap.find(nPoints())==shared.dMap.end()) {
      GaussQuadratureRule::Data d;
      calcData(*this,d);
      shared.dMap[nPoints()]=d;
    }
    const GaussQuadratureRule::Data & d=shared.dMap[nPoints()];
    for (unsigned int i=0;i<d.xw.size();i++) {
      add(d.xw[i].first,d.xw[i].second);
    }
  }

  const GaussHermiteRule *GaussHermiteRule::clone() const {
    return new GaussHermiteRule (nPoints());	       
  }    

  GaussLegendreRule::GaussLegendreRule(unsigned int npoint):
    GaussQuadratureRule("Gauss-Legendre", npoint, -1.0, 1.0, FixedConstant(1.0)){

    // This is done once per class per npoint value
    if (shared.dMap.find(nPoints())==shared.dMap.end()) {
      GaussQuadratureRule::Data d;
      calcData(*this,d);
      shared.dMap[nPoints()]=d;
    }
    // 
    // Take the abscissas and weights from cache:
    //
    const GaussQuadratureRule::Data & d=shared.dMap[nPoints()];
    for (unsigned int i=0;i<d.xw.size();i++) {
      add(d.xw[i].first,d.xw[i].second);
    }
  }

  const GaussLegendreRule *GaussLegendreRule::clone() const {
    return new GaussLegendreRule (nPoints());	       
  }    

}


std::ostream & operator << (std::ostream & o, const Genfun::GaussQuadratureRule & r) {
  o << "Quadrature rule : " << r.name() << std::endl;
  o << "Npoints         : " << r.nPoints() << std::endl;
  for (unsigned int i=0;i<r.nPoints();i++) {
    o << "i= " << i << " x= " << r.abscissa(i) << " w=" << r.weight(i) << std::endl;
  }
  return o;
}


