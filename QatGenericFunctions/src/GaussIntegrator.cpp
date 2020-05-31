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

#include "QatGenericFunctions/GaussIntegrator.h"
namespace Genfun {
  class GaussIntegrator::Clockwork {

  public:
    std::shared_ptr<const GaussQuadratureRule> rule;
    Type type;		
  };

  GaussIntegrator::GaussIntegrator(const GaussQuadratureRule &rule, Type type)
  :AbsFunctional(),c(new Clockwork())	 
  {
    c->rule=std::shared_ptr< const GaussQuadratureRule> (rule.clone());
    c->type=type;
  }

  double GaussIntegrator::operator()(GENFUNCTION F) const {
    double integral=0.0;
    for (unsigned int i=0;i<c->rule->nPoints();i++) {
      if (c->type==INTEGRATE_WX_DX) {	
	integral += c->rule->weight(i)*F(c->rule->abscissa(i));
      }
      else {
	integral += c->rule->weight(i)*F(c->rule->abscissa(i))/c->rule->weightFunction()(c->rule->abscissa(i));
      }
    }
    return integral;
  }  
}
