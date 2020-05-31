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

#include "QatGenericFunctions/VoigtDistribution.h"
#include <cmath>
#include <complex>


using namespace std;


inline std::complex<double> nwwerf(std::complex<double> z) {
  std::complex<double>  zh,r[38],s,t,v;
  //
  // Constants:
  //
  static const double z1 = 1;
  static const double hf = z1/2;
  static const double z10 = 10;
  static const double c1 = 74/z10;
  static const double c2 = 83/z10;
  static const double c3 = z10/32;
  static const double c4 = 16/z10;
  static const double c = 1.12837916709551257;
  static const double p = pow(2.0*c4,33);

  double x=z.real();
  double y=z.imag();
  double xa=(x >= 0) ? x : -x;
  double ya=(y >= 0) ? y : -y;
  if(ya < c1 && xa < c2){
    zh = std::complex<double>(ya+c4,xa);
    r[37]= std::complex<double>(0,0);
    //       do 1 n = 36,1,-1
    for(int n = 36; n>0; n--){   
      t=zh+double(n)*std::conj(r[n+1]);
      r[n]=hf*t/std::norm(t);
    }
    double xl=p;
    s=std::complex<double>(0,0);
    //    do 2 n = 33,1,-1
    for(int k=33; k>0; k--){
      xl=c3*xl;
      s=r[k]*(s+xl);
    }
    v=c*s;
  }
  else{
      zh=std::complex<double>(ya,xa);
      r[1]=std::complex<double>(0,0);
       //       do 3 n = 9,1,-1
       for(int n=9;n>0;n--){
         t=zh+double(n)*std::conj(r[1]);
         r[1]=hf*t/std::norm(t);
       }
       v=c*r[1];
  }
  if(ya == 0) v= std::complex<double>(exp(-xa*xa),v.imag());
  if(y < 0) {
    v=2.0*std::exp(std::complex<double>(-xa,-ya)*std::complex<double>(xa,ya))-v;
    if(x > 0) v=std::conj(v);
  }
  else{
    if(x < 0) v=std::conj(v);
  }
  return v;
}



namespace Genfun {
FUNCTION_OBJECT_IMP(VoigtDistribution)


VoigtDistribution::VoigtDistribution():
  _delta ("delta", 5, 0,   100),
  _sigma("sigma",  5, 0,   100)
{}

  VoigtDistribution::VoigtDistribution(const VoigtDistribution & right):
    AbsFunction(),
    _delta (right._delta),
    _sigma(right._sigma)
{
}

VoigtDistribution::~VoigtDistribution() {
}

double VoigtDistribution::operator() (double x) const {
  double G=_delta.getValue();
  double s=_sigma.getValue();
  static const double sqrt2=sqrt(2.0);
  static const double sqrt2PI=sqrt(2.0*M_PI);
  static const std::complex<double> I(0,1);
  std::complex<double> z = (x + I*G)/sqrt2/s;
  
  double f=nwwerf(z).real()/s/sqrt2PI;

  return f;

}



Parameter & VoigtDistribution::delta() {
  return _delta;
}

Parameter & VoigtDistribution::sigma() {
  return _sigma;
}


} // namespace Genfun
