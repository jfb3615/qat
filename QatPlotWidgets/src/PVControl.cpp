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

#include "QatPlotWidgets/PVControl.h"
#include <QDoubleValidator>
#include "ui_PVControl.h"
#include <sstream>
#include <iostream>
#include <limits>

class PVControl::Clockwork {
public:
  Ui::PVControl ui;
  QDoubleValidator *validator;
  QRectF rectI;
  QRectF rectII;
  PlotView *plotView;

};

QRectF PVControl::rect() {
  return c->rectII;
}

PVControl::~PVControl() {
  delete c;
}

Ui::PVControl & PVControl::ui() {
  return c->ui;
}

PVControl::PVControl (PlotView *parent, const QRectF & rect):QDialog(parent), c(new Clockwork()){
  c->validator=NULL;
  c->plotView=parent;
  
  c->ui.setupUi(this);
  setRect(rect);

  QObject::connect(c->ui.xZeroCheckBox, SIGNAL(clicked(bool)), parent, SLOT(setXZero(bool)));
  QObject::connect(c->ui.yZeroCheckBox, SIGNAL(clicked(bool)), parent, SLOT(setYZero(bool)));
  QObject::connect(c->ui.gridCheckBox,  SIGNAL(clicked(bool)), parent, SLOT(setGrid(bool)));
  QObject::connect(c->ui.logXCheckBox,  SIGNAL(clicked(bool)), parent, SLOT(setLogX(bool)));
  QObject::connect(c->ui.logYCheckBox,  SIGNAL(clicked(bool)), parent, SLOT(setLogY(bool)));
  QObject::connect(c->ui.labelBoxBox,   SIGNAL(clicked(bool)), parent, SLOT(setBox(bool)));

  QObject::connect(c->ui.xMaxDial,   SIGNAL(valueChanged(int)), this, SLOT(setXMax(int)));
  QObject::connect(c->ui.xMinDial,   SIGNAL(valueChanged(int)), this, SLOT(setXMin(int)));
  QObject::connect(c->ui.yMaxDial,   SIGNAL(valueChanged(int)), this, SLOT(setYMax(int)));
  QObject::connect(c->ui.yMinDial,   SIGNAL(valueChanged(int)), this, SLOT(setYMin(int)));


  QObject::connect(c->ui.xMinBox, SIGNAL(editingFinished()), this, SLOT(changeXMin()));
  QObject::connect(c->ui.xMaxBox, SIGNAL(editingFinished()), this, SLOT(changeXMax()));
  QObject::connect(c->ui.yMinBox, SIGNAL(editingFinished()), this, SLOT(changeYMin()));
  QObject::connect(c->ui.yMaxBox, SIGNAL(editingFinished()), this, SLOT(changeYMax()));

  QObject::connect(c->ui.labelHomePushButton, SIGNAL(clicked()), this, SIGNAL(labelHome()));
  QObject::connect(c->ui.xAxisFontButton, SIGNAL(clicked()), this, SIGNAL(editXAxisFont()));
  QObject::connect(c->ui.yAxisFontButton, SIGNAL(clicked()), this, SIGNAL(editYAxisFont()));

  QObject::connect(c->ui.labelWidthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setLabelXSizePercentage(int)));
  QObject::connect(c->ui.labelHeightSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setLabelYSizePercentage(int)));

  c->validator=new QDoubleValidator(this);
  c->validator->setRange(-std::numeric_limits<double>::max(),
		      +std::numeric_limits<double>::max(),
		      10);


  c->ui.xMinBox->setValidator(c->validator);
  c->ui.xMaxBox->setValidator(c->validator);
  c->ui.yMinBox->setValidator(c->validator);
  c->ui.yMaxBox->setValidator(c->validator);
}

void PVControl::setRect( const QRectF & rect) {
  
  QObject *sender = QObject::sender();
 
  std::ostringstream leftStream, rightStream, topStream, bottomStream;
  leftStream   << rect.left();
  rightStream  << rect.right();
  topStream    << rect.top();
  bottomStream << rect.bottom();

  if (sender== c->ui.xMinBox || sender==c->ui.xMaxBox || sender==c->ui.yMinBox || sender==c->ui.yMaxBox ||
      sender== c->ui.xMinDial || sender==c->ui.xMaxDial || sender==c->ui.yMinDial || sender==c->ui.yMaxDial) {
  }
  else {
    c->ui.xMinBox->setText(leftStream.str().c_str());
    c->ui.yMinBox->setText(topStream.str().c_str());
    c->ui.xMaxBox->setText(rightStream.str().c_str());
    c->ui.yMaxBox->setText(bottomStream.str().c_str());
    c->rectI =rect;

    c->ui.yMinDial->setValue(0);
    c->ui.yMaxDial->setValue(99);
    c->ui.xMinDial->setValue(0);
    c->ui.xMaxDial->setValue(99);
    c->rectII=rect;
    c->ui.logXCheckBox->setEnabled(rect.left()>0.0);
    c->ui.logYCheckBox->setEnabled(rect.top()>0.0);
  }


}

PlotView::Style PVControl::yAxisStyle() {
  return c->ui.logYCheckBox->isChecked() ? PlotView::LOG : PlotView::LINEAR;
}


PlotView::Style PVControl::xAxisStyle() {
  return c->ui.logXCheckBox->isChecked() ? PlotView::LOG : PlotView::LINEAR;
}

// called when XMax is changed:
void PVControl::setXMax(int value) {
  if (c->ui.xMinDial->value()>= value) c->ui.xMinDial->setValue(value-1);
  c->rectII.setRight(c->rectI.left()+value/99.0*c->rectI.width());
  emit rectChanged(c->rectII);
}

// called when XMin is changed:
void PVControl::setXMin(int value) {
  if (c->ui.xMaxDial->value()<= value) c->ui.xMaxDial->setValue(value+1);
  c->rectII.setLeft(c->rectI.left()+value/99.0*c->rectI.width());
  emit rectChanged(c->rectII);
}

// called when YMax is changed:
void PVControl::setYMax(int value) {
  if (c->ui.yMinDial->value()>= value) c->ui.yMinDial->setValue(value-1);
  c->rectII.setBottom(c->rectI.top()+value/99.0*c->rectI.height());
  emit rectChanged(c->rectII);
}

// called when YMin is changed:
void PVControl::setYMin(int value) {
  if (c->ui.yMaxDial->value()<= value) c->ui.yMaxDial->setValue(value+1);
  c->rectII.setTop(c->rectI.top()+value/99.0*c->rectI.height());
  emit rectChanged(c->rectII);
}

void PVControl::changeXMin() {

  std::string txt=c->ui.xMinBox->text().toStdString();

  double value;
  std::istringstream valueStream(txt.c_str());
  if ((valueStream >> value)) {
    if (value < c->rectI.right() && (!c->plotView->isLogX() || value > 0)) {
      c->rectI.setLeft(value);
      c->ui.xMinDial->setValue(0);
      setXMin(0);
      c->ui.logXCheckBox->setEnabled(c->rectI.left()>0.0);
    }
    else {
      QApplication::beep();
      std::ostringstream oldValueStream;
      oldValueStream << c->rectI.left();
      c->ui.xMinBox->setText(oldValueStream.str().c_str());
    }
  }
}
void PVControl::changeYMin() {

  std::string txt=c->ui.yMinBox->text().toStdString();

  double value;
  std::istringstream valueStream(txt.c_str());
  if ((valueStream >> value)) {
    if (value < c->rectI.bottom() && (!c->plotView->isLogY() || value>0)) {
      c->rectI.setTop(value);
      c->ui.yMinDial->setValue(0);
      setYMin(0);
      c->ui.logYCheckBox->setEnabled(c->rectI.top()>0.0);
    }
    else {
      QApplication::beep();
      std::ostringstream oldValueStream;
      oldValueStream << c->rectI.top();
      c->ui.yMinBox->setText(oldValueStream.str().c_str());
    }
  }
}



void PVControl::changeXMax() {

  std::string txt=c->ui.xMaxBox->text().toStdString();

  double value;
  std::istringstream valueStream(txt.c_str());
  if ((valueStream >> value)) {
    if (value > c->rectI.left()) {
      c->rectI.setRight(value);
      c->ui.xMaxDial->setValue(99);
      setXMax(99);
    }
    else {
      QApplication::beep();
      std::ostringstream oldValueStream;
      oldValueStream << c->rectI.right();
      c->ui.xMaxBox->setText(oldValueStream.str().c_str());
    }
  }


}


void PVControl::changeYMax() {

  std::string txt=c->ui.yMaxBox->text().toStdString();

  double value;
  std::istringstream valueStream(txt.c_str());
  if ((valueStream >> value)) {
    if (value > c->rectI.top()) {
      c->rectI.setBottom(value);
      c->ui.yMaxDial->setValue(99);
      setYMax(99);
    }
    else {
      QApplication::beep();
      std::ostringstream oldValueStream;
      oldValueStream << c->rectI.bottom();
      c->ui.yMaxBox->setText(oldValueStream.str().c_str());
    }
  }

  

}


void PVControl::setLabelXSizePercentage(int pc) {
  QObject *sender = QObject::sender();

  if (sender!=c->ui.labelWidthSpinBox) c->ui.labelWidthSpinBox->setValue(pc);
  qreal f = pc;
  emit labelXSizePercentageChanged(f);
}

void PVControl::setLabelYSizePercentage(int pc) {
  QObject *sender = QObject::sender();
  if (sender != c->ui.labelHeightSpinBox) c->ui.labelHeightSpinBox->setValue(pc);
  qreal f = pc;
  emit labelYSizePercentageChanged(f);
}
