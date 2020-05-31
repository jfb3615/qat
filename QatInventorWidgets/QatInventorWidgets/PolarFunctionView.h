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

#ifndef _PolarFunctionView_h_
#define _PolarFunctionView_h_
#include <QGraphicsView>
#include "QatGenericFunctions/AbsFunction.h"
class SoQtExaminerViewer;
class PolarFunctionView: public QWidget {

  Q_OBJECT

 public:
  
  // Constructor:
  PolarFunctionView(QWidget *parent=0);

  // Destructor:
  ~PolarFunctionView();

  // Resize:
  virtual void resizeEvent (QResizeEvent *event);

  // Add the function which must :
  // 1.  be a function of two variables (cosTheta, phi), and
  // 2.  be maintained in the client code:
  //
  void setFunction (const Genfun::AbsFunction * F);

  // Get the examiner viewer:
  SoQtExaminerViewer *getViewer() const;
  
  public slots:


  void save (const QString & filename);
  void save (const std::string & filename);
  void save();
  void print();
  void setSquish();
  void refreshSphere();
  void toggleLatitudeLongitude(bool flag);
  void toggleAxes(bool flag);
  void toggleUnroll(bool flag);
  


  
 private:

  void setupGraphics();
  
  // Clockwork:
  class Clockwork;
  Clockwork *c;

  // Illegal operations:
  PolarFunctionView & operator = (const PolarFunctionView &)=delete;
  PolarFunctionView(const PolarFunctionView &)=delete;

};


#endif
