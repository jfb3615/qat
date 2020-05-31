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

#ifndef _PlotFunction1DDialog_h_
#define _PlotFunction1DDialog_h_
#include "QatGenericFunctions/Parameter.h"
class PlotFunction1D;
#include <QDialog>
class PlotFunction1DDialog: public QDialog {
 
  Q_OBJECT

 public:

  // Constructor:
  PlotFunction1DDialog(QWidget *parent);
  
  // Destructor:
  virtual ~PlotFunction1DDialog();

  void synchTo(PlotFunction1D *);
  void clearParameters();
  void addParameter(Genfun::Parameter * p);

  public slots:
  
  void setLineWidth(int);
  void toggleLineColor(bool);
  void toggleLineStyle(bool);
  void changeParameter(int row, int column);
  void popParameterDial(int row, int column);

  void synchToFunction();

 signals:

  void updateFunction();

 private:

  class Clockwork;
  Clockwork *c; 

}; 

#endif
