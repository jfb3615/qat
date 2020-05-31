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

#ifndef _PVControl_h_
#define _PVControl_h_
#include "QatPlotWidgets/PlotView.h"
#include <QDialog>

namespace Ui {
  class PVControl;
}

class PVControl:public QDialog {

  Q_OBJECT

    public:
  PVControl(PlotView * parent, const QRectF &rect);
  ~PVControl();

  QRectF rect();
  PlotView::Style xAxisStyle();
  PlotView::Style yAxisStyle();

 public slots:

  // This one sets the rectangle externally:
  void setRect(const QRectF & rect);
  
  void setXMin(int);
  void setXMax(int);
  void setYMin(int);
  void setYMax(int);
  void setLabelXSizePercentage(int);
  void setLabelYSizePercentage(int);

  void changeXMin ();
  void changeXMax ();
  void changeYMin ();
  void changeYMax ();

signals: 
  
  void rectChanged(const QRectF &);
  void labelHome();
  void editXAxisFont();
  void editYAxisFont();
  void labelXSizePercentageChanged(qreal);
  void labelYSizePercentageChanged(qreal);

 private:

  Ui::PVControl & ui();

  class Clockwork;
  Clockwork *c;

  friend class PlotView;  

};
#endif
