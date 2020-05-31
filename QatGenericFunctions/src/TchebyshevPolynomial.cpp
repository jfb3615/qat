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

#include "QatGenericFunctions/TchebyshevPolynomial.h"
#include "QatGenericFunctions/Variable.h"
#include "QatGenericFunctions/Power.h"
#include <stdexcept>
#include <cmath>
namespace Genfun {
  FUNCTION_OBJECT_IMP(TchebyshevPolynomial)
  
  
  TchebyshevPolynomial::TchebyshevPolynomial(unsigned int n, Type type, NormalizationType normType):
  AbsFunction(),
    _n(n),
    _type(type),
    _normType(normType)
  {
  }
  
  TchebyshevPolynomial::~TchebyshevPolynomial() {
  }
  
  TchebyshevPolynomial::TchebyshevPolynomial(const TchebyshevPolynomial & right):
    AbsFunction(),
    _n(right._n),
    _type(right._type),
    _normType(right._normType)
  {
  }
  
  double TchebyshevPolynomial::operator() (double x) const {
    static const double normFactor=sqrt(M_PI);
    static const double normFactor0=sqrt(M_PI/2.0);
    double p0=1;
    if (_n==0) return _normType==STD ? p0 : normFactor0*p0;
    double p1=_type==FirstKind ? x : 2*x;
    if (_n==1) return _normType==STD ? p1 : normFactor*p1;
    unsigned int k=2;
    while (1) {
      double p2=2*x*p1-p0;
      p0=p1;
      p1=p2;
      if (k==_n) return _normType==STD ? p2: normFactor*p2;
      k++;
    }
    return 0; // never occurs
  }
  
  unsigned int TchebyshevPolynomial::n() const {
    return _n;
  }
  
  TchebyshevPolynomial::Type TchebyshevPolynomial::type() const {
    return _type;
  }
  

  Derivative TchebyshevPolynomial::partial(unsigned int index) const {
    if (index!=0) throw std::range_error("TchebyshevPolynomial: partial derivative index out of range");
    if (_n==0) return 0;
    
    const double norm=_normType==STD ? 1.0:sqrt(M_PI);
    
    Variable X;
    
    const AbsFunction & fPrime =  _type==FirstKind ? 
      norm*_n*TchebyshevPolynomial(_n-1, SecondKind, STD)
      :
      norm*(((_n+1)*TchebyshevPolynomial(_n+1,FirstKind, STD)-X*TchebyshevPolynomial(_n,SecondKind, STD))/(X+1)/(X-1));
    return Derivative(& fPrime);
  }


} // namespace Genfun
