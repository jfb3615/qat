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



//----------------------------------------------------------------------------------//
//                                                                                  //
// Plottable adaptor for Function1D's                                               //
//                                                                                  //
//----------------------------------------------------------------------------------//
#ifndef _PLOTWAVE1D_H_
#define _PLOTWAVE1D_H_
class AbsFunction;
#include "QatPlotting/Plotable.h"
#include "QatPlotting/PlotFunction1D.h"
#include "QatPlotting/PlotWave1DProperties.h"
#include "QatGenericFunctions/CutBase.h"
#include <QRectF>
#include <vector>
#include <string>
#include <limits.h>

namespace Genfun {
  class AbsFunction;
  class Parameter;
}

class PlotWave1D: public PlotFunction1D{

public:

  // Define Properties:
  typedef PlotWave1DProperties Properties;

   // Constructors
  PlotWave1D(const Genfun::AbsFunction & function,
		 const Cut<double> & domainRestriction,
		 const QRectF   & rectHint = QRectF(QPointF(-10, -10), QSizeF(20, 20)));

   // Constructors
  PlotWave1D(const Genfun::AbsFunction & function,
		 const QRectF   & rectHint = QRectF(QPointF(-10, -10), QSizeF(20, 20)));

  // Copy constructor:
  PlotWave1D(const PlotWave1D &);

  // Assignment operator:
  PlotWave1D & operator=(const PlotWave1D &); 

  // Destructor
  virtual ~PlotWave1D();

  // Describe to plotter, in terms of primitives:
  virtual void describeYourselfTo(AbsPlotter *plotter) const;


};
#endif
