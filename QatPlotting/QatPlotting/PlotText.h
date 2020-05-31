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



#ifndef _PLOTTEXT_H_
#define _PLOTTEXT_H_
#include "QatPlotting/Plotable.h"
#include <QRectF>
#include <QString>
#include <QFont>

class QGraphicsTextItem;
class QTextDocument;

class PlotText: public Plotable {

public:

   // Constructors
  PlotText(double x, double y, const QString & string=QString("")); 

  // Destructor
  virtual ~PlotText();

  // Get the "natural rectangular border"
  virtual const QRectF & rectHint() const;

  // Describe to plotter, in terms of primitives:
  virtual void describeYourselfTo(AbsPlotter *plotter) const;

  // Set the document:
  void setDocument(QTextDocument *document);

  // Get the font:
  QFont font() const;

  // Set the font:
  void setFont (const QFont & font);

  // Copy constructor:
  PlotText(const PlotText &);
  
  // Assignment operator:
  PlotText & operator=(const PlotText &); 
  
private:

  class Clockwork;
  Clockwork *c;

};
#endif
