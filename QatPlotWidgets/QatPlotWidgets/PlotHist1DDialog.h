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

#ifndef _PlotHist1DDialog_h_
#define _PlotHist1DDialog_h_
#include <QDialog>
class PlotHist1D;
class PlotHist1DDialog: public QDialog {
 
  Q_OBJECT

 public:

  // Constructor:
  PlotHist1DDialog(QWidget *parent);
  
  // Destructor:
  virtual ~PlotHist1DDialog();

  void synchTo(PlotHist1D *);

  public slots:
  
  void setLineWidth(int);
  void setSymbolSize(int);
  void toggleLineColor(bool);
  void toggleBrushColor(bool);
  void toggleLineStyle(bool);
  void toggleBrushStyle(bool);
  void togglePlotStyle(bool);

 signals:

  void updateHisto();

 private:

  class Clockwork;
  Clockwork *c; 

}; 

#endif
