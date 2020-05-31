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

#include "QatPlotWidgets/PlotHist2DDialog.h"
#include "QatPlotting/PlotHist2D.h"
#include "ui_PlotDirDialog.h"
class PlotHist2DDialog::Clockwork {
public:
  PlotHist2D *synchedPlot;
  Ui::PlotDirDialog ui;
};

PlotHist2DDialog::PlotHist2DDialog(QWidget *parent): QDialog(parent),c(new Clockwork()) {
  c->synchedPlot=NULL;
  c->ui.setupUi(this);
  c->ui.tabWidget->removeTab(5);
  c->ui.tabWidget->removeTab(3);
  c->ui.tabWidget->removeTab(1);
  c->ui.tabWidget->removeTab(0);

  // Unveil later (note, do this also on 1D, Dir dialog);
  c->ui.moreLineColorsToolButton1D->hide();
  c->ui.moreBrushColorsToolButton1D->hide();
  c->ui.moreLineColorsToolButton2D->hide();
  c->ui.moreBrushColorsToolButton1D->hide();

  connect(c->ui.lineWidthSpinBox2D,  SIGNAL(valueChanged(int)), this, SLOT(setLineWidth(int)));


  connect(c->ui.blackLineToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleLineColor(bool)));
  connect(c->ui.redLineToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleLineColor(bool)));
  connect(c->ui.greenLineToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleLineColor(bool)));
  connect(c->ui.blueLineToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleLineColor(bool)));
  connect(c->ui.cyanLineToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleLineColor(bool)));
  connect(c->ui.purpleLineToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleLineColor(bool)));
  connect(c->ui.grayLineToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleLineColor(bool)));


  connect(c->ui.blackBrushToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushColor(bool)));
  connect(c->ui.redBrushToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushColor(bool)));
  connect(c->ui.greenBrushToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushColor(bool)));
  connect(c->ui.blueBrushToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushColor(bool)));
  connect(c->ui.cyanBrushToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushColor(bool)));
  connect(c->ui.purpleBrushToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushColor(bool)));
  connect(c->ui.grayBrushToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushColor(bool)));


  connect(c->ui.solidLineToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleLineStyle(bool)));
  connect(c->ui.noLineToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleLineStyle(bool)));

  connect(c->ui.solidBrushToolButton2D,  SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle(bool)));
  connect(c->ui.dense1BrushToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle(bool)));
  connect(c->ui.dense2BrushToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle(bool)));
  connect(c->ui.dense3BrushToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle(bool)));
  connect(c->ui.dense4BrushToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle(bool)));
  connect(c->ui.dense5BrushToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle(bool)));
  connect(c->ui.dense6BrushToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle(bool)));
  connect(c->ui.dense7BrushToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle(bool)));
  connect(c->ui.noBrushToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle(bool)));


}

PlotHist2DDialog::~PlotHist2DDialog() {
  delete c;
}   
 

#include <iostream>
void PlotHist2DDialog::synchTo(PlotHist2D *plot) {
  
  c->synchedPlot=plot;

  if (!plot) return;

  PlotHist2D::Properties prop=plot->properties();
  QPen pen=prop.pen;
  QBrush brush = prop.brush;

  int width=pen.width();
  c->ui.lineWidthSpinBox2D->setValue(width);


  {
    QColor color = pen.color();
    if (color==QColor("black"))  c->ui.blackLineToolButton2D->setChecked(true);
    else if (color==QColor("red"))    c->ui.redLineToolButton2D->setChecked(true);
    else if (color==QColor("green"))  c->ui.greenLineToolButton2D->setChecked(true);
    else if (color==QColor("blue"))   c->ui.blueLineToolButton2D->setChecked(true);
    else if (color==QColor("cyan"))   c->ui.cyanLineToolButton2D->setChecked(true);
    else if (color==QColor("purple")) c->ui.purpleLineToolButton2D->setChecked(true);
    else if (color==QColor("gray"))   c->ui.grayLineToolButton2D->setChecked(true);
    else {
      c->ui.moreLineColorsToolButton2D->setChecked(true);
    }
    Qt::PenStyle style = pen.style();
    if (style==Qt::SolidLine)           c->ui.solidLineToolButton2D->setChecked(true);
    else if (style==Qt::NoPen)          c->ui.noLineToolButton2D->setChecked(true);
  }


  {
    QColor color = brush.color();
    if (color==QColor("black"))  c->ui.blackBrushToolButton2D->setChecked(true);
    else if (color==QColor("red"))    c->ui.redBrushToolButton2D->setChecked(true);
    else if (color==QColor("green"))  c->ui.greenBrushToolButton2D->setChecked(true);
    else if (color==QColor("blue"))   c->ui.blueBrushToolButton2D->setChecked(true);
    else if (color==QColor("cyan"))   c->ui.cyanBrushToolButton2D->setChecked(true);
    else if (color==QColor("purple")) c->ui.purpleBrushToolButton2D->setChecked(true);
    else if (color==QColor("gray"))   c->ui.grayBrushToolButton2D->setChecked(true);
    else {
      c->ui.moreBrushColorsToolButton2D->setChecked(true);
    }
    Qt::BrushStyle style = brush.style();
    if (style==Qt::SolidPattern)             c->ui.solidBrushToolButton2D->setChecked(true);
    else if (style==Qt::Dense1Pattern)       c->ui.dense1BrushToolButton2D->setChecked(true);
    else if (style==Qt::Dense2Pattern)       c->ui.dense2BrushToolButton2D->setChecked(true);
    else if (style==Qt::Dense3Pattern)       c->ui.dense3BrushToolButton2D->setChecked(true);
    else if (style==Qt::Dense4Pattern)       c->ui.dense4BrushToolButton2D->setChecked(true);
    else if (style==Qt::Dense5Pattern)       c->ui.dense5BrushToolButton2D->setChecked(true);
    else if (style==Qt::Dense6Pattern)       c->ui.dense6BrushToolButton2D->setChecked(true);
    else if (style==Qt::Dense7Pattern)       c->ui.dense7BrushToolButton2D->setChecked(true);
    else if (style==Qt::NoBrush)             c->ui.noBrushToolButton2D->setChecked(true);
  }


}

void PlotHist2DDialog::setLineWidth (int i) {
  if (c->synchedPlot) {
    PlotHist2D::Properties prop = c->synchedPlot->properties();
    prop.pen.setWidth(i);
    c->synchedPlot->setProperties(prop);
    emit updateHisto();
  }
}



void PlotHist2DDialog::toggleLineColor (bool checked) {
  if (c->synchedPlot) {
    if (checked) {
      QObject *theSender = sender();
      PlotHist2D::Properties prop = c->synchedPlot->properties();
      if (theSender==c->ui.blackLineToolButton2D) prop.pen.setColor(QColor("black"));
      else if (theSender==c->ui.redLineToolButton2D) prop.pen.setColor(QColor("red"));
      else if (theSender==c->ui.greenLineToolButton2D) prop.pen.setColor(QColor("green"));
      else if (theSender==c->ui.blueLineToolButton2D) prop.pen.setColor(QColor("blue"));
      else if (theSender==c->ui.cyanLineToolButton2D) prop.pen.setColor(QColor("cyan"));
      else if (theSender==c->ui.purpleLineToolButton2D) prop.pen.setColor(QColor("purple"));
      else if (theSender==c->ui.grayLineToolButton2D) prop.pen.setColor(QColor("gray"));
      c->synchedPlot->setProperties(prop);
      emit updateHisto();
    }
  }
}

void PlotHist2DDialog::toggleBrushColor (bool checked) {
  if (c->synchedPlot) {
    if (checked) {
      QObject *theSender = sender();
      PlotHist2D::Properties prop = c->synchedPlot->properties();
      if (theSender==c->ui.blackBrushToolButton2D) prop.brush.setColor(QColor("black"));
      else if (theSender==c->ui.redBrushToolButton2D) prop.brush.setColor(QColor("red"));
      else if (theSender==c->ui.greenBrushToolButton2D) prop.brush.setColor(QColor("green"));
      else if (theSender==c->ui.blueBrushToolButton2D) prop.brush.setColor(QColor("blue"));
      else if (theSender==c->ui.cyanBrushToolButton2D) prop.brush.setColor(QColor("cyan"));
      else if (theSender==c->ui.purpleBrushToolButton2D) prop.brush.setColor(QColor("purple"));
      else if (theSender==c->ui.grayBrushToolButton2D) prop.brush.setColor(QColor("gray"));
      c->synchedPlot->setProperties(prop);
      emit updateHisto();
    }
  }
}

void PlotHist2DDialog::toggleLineStyle (bool checked) {
  if (c->synchedPlot) {
    if (checked) {
      QObject *theSender = sender();
      PlotHist2D::Properties prop = c->synchedPlot->properties();
      if (theSender==c->ui.solidLineToolButton2D)           prop.pen.setStyle(Qt::SolidLine);
      else if (theSender==c->ui.noLineToolButton2D)         prop.pen.setStyle(Qt::NoPen);
      c->synchedPlot->setProperties(prop);
      emit updateHisto();
    }
  }
}

void PlotHist2DDialog::toggleBrushStyle (bool checked) {
  if (c->synchedPlot) {
    if (checked) {
      QObject *theSender = sender(); 
      PlotHist2D::Properties prop = c->synchedPlot->properties();
      if      (theSender==c->ui.solidBrushToolButton2D)            prop.brush.setStyle(Qt::SolidPattern);
      else if (theSender==c->ui.dense1BrushToolButton2D)           prop.brush.setStyle(Qt::Dense1Pattern);
      else if (theSender==c->ui.dense2BrushToolButton2D)           prop.brush.setStyle(Qt::Dense2Pattern);
      else if (theSender==c->ui.dense3BrushToolButton2D)           prop.brush.setStyle(Qt::Dense3Pattern);
      else if (theSender==c->ui.dense4BrushToolButton2D)           prop.brush.setStyle(Qt::Dense4Pattern);
      else if (theSender==c->ui.dense5BrushToolButton2D)           prop.brush.setStyle(Qt::Dense5Pattern);
      else if (theSender==c->ui.dense6BrushToolButton2D)           prop.brush.setStyle(Qt::Dense6Pattern);
      else if (theSender==c->ui.dense7BrushToolButton2D)           prop.brush.setStyle(Qt::Dense7Pattern);
      else if (theSender==c->ui.noBrushToolButton2D)               prop.brush.setStyle(Qt::NoBrush);
      c->synchedPlot->setProperties(prop);
      emit updateHisto();
    }
  }
}


