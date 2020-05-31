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

#ifndef _GaussQuadratureRule_h_
#define _GaussQuadratureRule_h_
#include "QatGenericFunctions/AbsFunction.h"
#include "QatGenericFunctions/TchebyshevPolynomial.h"
#include <memory>
#include <string>
#include <map>
#include <vector>
#include <iostream>
//----------------------------------------------------------------------------//
//                                                                            //
// A base class for quadrature rules used in Gaussian Integration, plus       //
// several subclasses.                                                        //
//                                                                            //
//----------------------------------------------------------------------------//
namespace Genfun {

  class GaussQuadratureRule {

  public:
    
    // Constructor
    GaussQuadratureRule(const std::string & name, unsigned int npoints, double min, double max, GENFUNCTION W);
    
    // Destructor
    virtual ~GaussQuadratureRule()=0;
    
    // Clone.  
    virtual const GaussQuadratureRule *clone() const=0;

    // Access the name:
    const std::string & name() const;

    // Access limits of integration:
    double min() const;
    double max() const;


    //
    // Coefficients in the three-term recurrence relation 
    // p_{i+1} (x) =(a_i x + b_i) p_i(x) - c_i p_{i-i}(x) 
    //
    virtual double a(unsigned int i)    const=0; 
    virtual double b(unsigned int i)    const=0; 
    virtual double c(unsigned int i)    const=0;
    virtual double lognorm(unsigned int i) const=0;
    //
    // mu=\int w(x) dx 
    //
    virtual double mu()              const=0;

    // Access the weight function:
    GENFUNCTION weightFunction() const; 

    // Access the number of points:
    unsigned int nPoints() const;

    // Access abscissa
    const double & abscissa(unsigned int i) const;
    
    // Access weight:
    const double & weight(unsigned int i) const;
    

  protected:

    // Abscissas and weights:
    struct Data{
      std::vector<std::pair<double,double>> xw;
    };

    static void  calcData(const GaussQuadratureRule &rule, Data &data);
    
    // Add an abscissa/weight pair:
    void add(double abcissa, double weight);
    
    // Facilitates static storage of abscissa which is necessary
    // to avoid re-computation of abscissas and weights:

    class Shared {
    public:
      std::map<unsigned int, Data> dMap;
    };

    // Facilitates static storage of abscissa which is necessary
    // to avoid re-computation of abscissas and weights:
    class SharedV2 {
    public:
      struct Key {
      public: 
      Key(unsigned int n, double a) :n(n),a(a){}

      bool operator < (const Key & k) const {
	return  n < k.n || (n==k.n && a<k.a);
      }
      private:
	unsigned int n;
	double       a;
      };
      std::map<Key, Data> dMap;
    };
      
  private:
      
      std::string                  _name;
      unsigned int                 _npoints;
      double                       _min;
      double                       _max;
      std::shared_ptr<AbsFunction> _wFunction;
      std::vector<double>          _abscissa;
      std::vector<double>          _weight;
  };
  // =====================================================================
  // 
  // Some instances of Gauss Quadrature Rules
  //
  // =====================================================================
  class GaussLegendreRule:public GaussQuadratureRule {
    
  public:
      
    // Constructor:
    GaussLegendreRule(unsigned int npoints);

    // Clone
    const GaussLegendreRule *clone() const;
    //
    // Coefficients in the three-term recurrence relation 
    // p_{i+1} (x) =(a_i x + b_i) p_i(x) - c_i p_{i-i}(x) 
    //
    virtual double a(unsigned int i)    const; 
    virtual double b(unsigned int i)    const; 
    virtual double c(unsigned int i)    const;
    virtual double lognorm(unsigned int i) const;
    //
    // mu=\int w(x) dx 
    //
    virtual double mu()              const;

  private:
      
    static GaussQuadratureRule::Shared shared;
      
  };

  class GaussHermiteRule:public GaussQuadratureRule {
    
  public:
      
    // Constructor:
    GaussHermiteRule(unsigned int npoints);

    // Clone
    const GaussHermiteRule *clone() const;

    //
    // Coefficients in the three-term recurrence relation 
    // p_{i+1} (x) =(a_i x + b_i) p_i(x) - c_i p_{i-i}(x) 
    //
    virtual double a(unsigned int i)    const; 
    virtual double b(unsigned int i)    const; 
    virtual double c(unsigned int i)    const;
    virtual double lognorm(unsigned int i) const;
    //
    // mu=\int w(x) dx 
    //
    virtual double mu()              const;

  private:
      
    static GaussQuadratureRule::Shared shared; 
      
  };

  class GaussTchebyshevRule:public GaussQuadratureRule {
    
  public:
      
    // Constructor:
    GaussTchebyshevRule(unsigned int npoints, 
			TchebyshevPolynomial::Type=TchebyshevPolynomial::FirstKind);

    // Clone
    const GaussTchebyshevRule *clone() const;
    //
    // Coefficients in the three-term recurrence relation 
    // p_{i+1} (x) =(a_i x + b_i) p_i(x) - c_i p_{i-i}(x) 
    //
    virtual double a(unsigned int i)    const; 
    virtual double b(unsigned int i)    const; 
    virtual double c(unsigned int i)    const;
    virtual double lognorm(unsigned int i) const;
    //
    // mu=\int w(x) dx 
    //
    virtual double mu()              const;

  private:
      
    static GaussQuadratureRule::Shared sharedFirstKind; 
    static GaussQuadratureRule::Shared sharedSecondKind; 
    TchebyshevPolynomial::Type         _type;
  };

  class GaussLaguerreRule:public GaussQuadratureRule {
    
  public:
      
    // Constructor:
    GaussLaguerreRule(unsigned int npoints, double alpha=0);

    // Clone
    const GaussLaguerreRule *clone() const;

    //
    // Coefficients in the three-term recurrence relation 
    // p_{i+1} (x) =(a_i x + b_i) p_i(x) - c_i p_{i-i}(x) 
    //
    virtual double a(unsigned int i)    const; 
    virtual double b(unsigned int i)    const; 
    virtual double c(unsigned int i)    const;
    virtual double lognorm(unsigned int i) const;
    //
    // mu=\int w(x) dx 
    //
    virtual double mu()              const;

  private:
      
    static GaussQuadratureRule::SharedV2 shared; 
    const  double        alpha;
  };

}
std::ostream & operator << (std::ostream &, const Genfun::GaussQuadratureRule & );

#include "QatGenericFunctions/GaussQuadratureRule.icc"
#endif
