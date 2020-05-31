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

#include "QatPlotWidgets/PlotHist1DDialog.h"
#include "QatPlotting/PlotHist1D.h"
#include "ui_PlotDirDialog.h"
class PlotHist1DDialog::Clockwork {
public:
  PlotHist1D *synchedPlot;
  Ui::PlotDirDialog ui;
};

PlotHist1DDialog::PlotHist1DDialog(QWidget *parent): QDialog(parent),c(new Clockwork()) {
  c->synchedPlot=NULL;
  c->ui.setupUi(this);
  c->ui.tabWidget->removeTab(5);
  c->ui.tabWidget->removeTab(4);
  c->ui.tabWidget->removeTab(2);

  // Unveil later (note, do this also on  2D, Dir dialog);
  c->ui.moreLineColorsToolButton1D->hide();
  c->ui.moreBrushColorsToolButton1D->hide();
  c->ui.moreLineColorsToolButton2D->hide();
  c->ui.moreBrushColorsToolButton1D->hide();


  connect(c->ui.lineWidthSpinBox1D,  SIGNAL(valueChanged(int)), this, SLOT(setLineWidth(int)));
  connect(c->ui.symbolSizeSpinBox1D, SIGNAL(valueChanged(int)), this, SLOT(setSymbolSize(int)));

  connect(c->ui.blackLineToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleLineColor(bool)));
  connect(c->ui.redLineToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleLineColor(bool)));
  connect(c->ui.greenLineToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleLineColor(bool)));
  connect(c->ui.blueLineToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleLineColor(bool)));
  connect(c->ui.cyanLineToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleLineColor(bool)));
  connect(c->ui.purpleLineToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleLineColor(bool)));
  connect(c->ui.grayLineToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleLineColor(bool)));


  connect(c->ui.blackBrushToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushColor(bool)));
  connect(c->ui.redBrushToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushColor(bool)));
  connect(c->ui.greenBrushToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushColor(bool)));
  connect(c->ui.blueBrushToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushColor(bool)));
  connect(c->ui.cyanBrushToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushColor(bool)));
  connect(c->ui.purpleBrushToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushColor(bool)));
  connect(c->ui.grayBrushToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushColor(bool)));


  connect(c->ui.solidLineToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleLineStyle(bool)));
  connect(c->ui.dashLineToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleLineStyle(bool)));
  connect(c->ui.dotLineToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleLineStyle(bool)));
  connect(c->ui.dashDotLineToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleLineStyle(bool)));
  connect(c->ui.dashDotDotLineToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleLineStyle(bool)));
  connect(c->ui.noLineToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleLineStyle(bool)));

  connect(c->ui.solidBrushToolButton1D,  SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle(bool)));
  connect(c->ui.dense1BrushToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle(bool)));
  connect(c->ui.dense2BrushToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle(bool)));
  connect(c->ui.dense3BrushToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle(bool)));
  connect(c->ui.dense4BrushToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle(bool)));
  connect(c->ui.dense5BrushToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle(bool)));
  connect(c->ui.dense6BrushToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle(bool)));
  connect(c->ui.dense7BrushToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle(bool)));
  connect(c->ui.horizontalBrushToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle(bool)));
  connect(c->ui.verticalBrushToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle(bool)));
  connect(c->ui.crossPatternBrushToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle(bool)));
  connect(c->ui.bDiagBrushToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle(bool)));
  connect(c->ui.fDiagBrushToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle(bool)));
  connect(c->ui.diagCrossPatternBrushToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle(bool)));
  connect(c->ui.noBrushToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle(bool)));


  connect(c->ui.circleSymbolToolButton1D,       SIGNAL(toggled(bool)), this, SLOT(togglePlotStyle(bool)));
  connect(c->ui.squareSymbolToolButton1D,       SIGNAL(toggled(bool)), this, SLOT(togglePlotStyle(bool)));
  connect(c->ui.upTriangleSymbolToolButton1D,   SIGNAL(toggled(bool)), this, SLOT(togglePlotStyle(bool)));
  connect(c->ui.downTriangleSymbolToolButton1D, SIGNAL(toggled(bool)), this, SLOT(togglePlotStyle(bool)));
  connect(c->ui.noSymbolToolButton1D, SIGNAL(toggled(bool)), this, SLOT(togglePlotStyle(bool)));

}

PlotHist1DDialog::~PlotHist1DDialog() {
  delete c;
}   
 

#include <iostream>
void PlotHist1DDialog::synchTo(PlotHist1D *plot) {
  
  c->synchedPlot=plot;

  if (!plot) return;

  PlotHist1D::Properties prop=plot->properties();
  QPen pen=prop.pen;
  QBrush brush = prop.brush;

  int width=pen.width();
  c->ui.lineWidthSpinBox1D->setValue(width);

  int symSize=prop.symbolSize;
  c->ui.symbolSizeSpinBox1D->setValue(symSize);

  {
    QColor color = pen.color();
    if (color==QColor("black"))  c->ui.blackLineToolButton1D->setChecked(true);
    else if (color==QColor("red"))    c->ui.redLineToolButton1D->setChecked(true);
    else if (color==QColor("green"))  c->ui.greenLineToolButton1D->setChecked(true);
    else if (color==QColor("blue"))   c->ui.blueLineToolButton1D->setChecked(true);
    else if (color==QColor("cyan"))   c->ui.cyanLineToolButton1D->setChecked(true);
    else if (color==QColor("purple")) c->ui.purpleLineToolButton1D->setChecked(true);
    else if (color==QColor("gray"))   c->ui.grayLineToolButton1D->setChecked(true);
    else {
      c->ui.moreLineColorsToolButton1D->setChecked(true);
    }
    Qt::PenStyle style = pen.style();
    if (style==Qt::SolidLine)           c->ui.solidLineToolButton1D->setChecked(true);
    else if (style==Qt::DashLine)       c->ui.dashLineToolButton1D->setChecked(true);
    else if (style==Qt::DotLine)        c->ui.dotLineToolButton1D->setChecked(true);
    else if (style==Qt::DashDotLine)    c->ui.dashDotLineToolButton1D->setChecked(true);
    else if (style==Qt::DashDotDotLine) c->ui.dashDotDotLineToolButton1D->setChecked(true);
    else if (style==Qt::NoPen)          c->ui.noLineToolButton1D->setChecked(true);
  }


  {
    QColor color = brush.color();
    if (color==QColor("black"))  c->ui.blackBrushToolButton1D->setChecked(true);
    else if (color==QColor("red"))    c->ui.redBrushToolButton1D->setChecked(true);
    else if (color==QColor("green"))  c->ui.greenBrushToolButton1D->setChecked(true);
    else if (color==QColor("blue"))   c->ui.blueBrushToolButton1D->setChecked(true);
    else if (color==QColor("cyan"))   c->ui.cyanBrushToolButton1D->setChecked(true);
    else if (color==QColor("purple")) c->ui.purpleBrushToolButton1D->setChecked(true);
    else if (color==QColor("gray"))   c->ui.grayBrushToolButton1D->setChecked(true);
    else {
      c->ui.moreBrushColorsToolButton1D->setChecked(true);
    }
    Qt::BrushStyle style = brush.style();
    if (style==Qt::SolidPattern)             c->ui.solidBrushToolButton1D->setChecked(true);
    else if (style==Qt::Dense1Pattern)       c->ui.dense1BrushToolButton1D->setChecked(true);
    else if (style==Qt::Dense2Pattern)       c->ui.dense2BrushToolButton1D->setChecked(true);
    else if (style==Qt::Dense3Pattern)       c->ui.dense3BrushToolButton1D->setChecked(true);
    else if (style==Qt::Dense4Pattern)       c->ui.dense4BrushToolButton1D->setChecked(true);
    else if (style==Qt::Dense5Pattern)       c->ui.dense5BrushToolButton1D->setChecked(true);
    else if (style==Qt::Dense6Pattern)       c->ui.dense6BrushToolButton1D->setChecked(true);
    else if (style==Qt::Dense7Pattern)       c->ui.dense7BrushToolButton1D->setChecked(true);
    else if (style==Qt::HorPattern)          c->ui.horizontalBrushToolButton1D->setChecked(true);
    else if (style==Qt::VerPattern)          c->ui.verticalBrushToolButton1D->setChecked(true);
    else if (style==Qt::CrossPattern)        c->ui.crossPatternBrushToolButton1D->setChecked(true);
    else if (style==Qt::BDiagPattern)        c->ui.bDiagBrushToolButton1D->setChecked(true);
    else if (style==Qt::FDiagPattern)        c->ui.fDiagBrushToolButton1D->setChecked(true);
    else if (style==Qt::DiagCrossPattern)    c->ui.diagCrossPatternBrushToolButton1D->setChecked(true);
    else if (style==Qt::NoBrush)             c->ui.noBrushToolButton1D->setChecked(true);
  }

  PlotHist1D::Properties::PlotStyle     plotStyle   = prop.plotStyle;
  PlotHist1D::Properties::SymbolStyle   symbolStyle = prop.symbolStyle;

  if (plotStyle==PlotHist1DProperties::LINES) {
    c->ui.noSymbolToolButton1D->setChecked(true);
  }
  else {
    if      (symbolStyle==PlotHist1D::Properties::CIRCLE)     c->ui.circleSymbolToolButton1D->setChecked(true);
    else if (symbolStyle==PlotHist1D::Properties::SQUARE)     c->ui.squareSymbolToolButton1D->setChecked(true);
    else if (symbolStyle==PlotHist1D::Properties::TRIANGLE_U) c->ui.upTriangleSymbolToolButton1D->setChecked(true);
    else if (symbolStyle==PlotHist1D::Properties::TRIANGLE_L) c->ui.downTriangleSymbolToolButton1D->setChecked(true);
  }


}

void PlotHist1DDialog::setLineWidth (int i) {
  if (c->synchedPlot) {
    PlotHist1D::Properties prop = c->synchedPlot->properties();
    prop.pen.setWidth(i);
    c->synchedPlot->setProperties(prop);
    emit updateHisto();
  }
}

void PlotHist1DDialog::setSymbolSize (int i) {
  if (c->synchedPlot) {
    PlotHist1D::Properties prop = c->synchedPlot->properties();
    prop.symbolSize=i;
    c->synchedPlot->setProperties(prop);
    emit updateHisto();
  }
}


void PlotHist1DDialog::toggleLineColor (bool checked) {
  if (c->synchedPlot) {
    if (checked) {
      QObject *theSender = sender();
      PlotHist1D::Properties prop = c->synchedPlot->properties();
      if (theSender==c->ui.blackLineToolButton1D) prop.pen.setColor(QColor("black"));
      else if (theSender==c->ui.redLineToolButton1D) prop.pen.setColor(QColor("red"));
      else if (theSender==c->ui.greenLineToolButton1D) prop.pen.setColor(QColor("green"));
      else if (theSender==c->ui.blueLineToolButton1D) prop.pen.setColor(QColor("blue"));
      else if (theSender==c->ui.cyanLineToolButton1D) prop.pen.setColor(QColor("cyan"));
      else if (theSender==c->ui.purpleLineToolButton1D) prop.pen.setColor(QColor("purple"));
      else if (theSender==c->ui.grayLineToolButton1D) prop.pen.setColor(QColor("gray"));
      c->synchedPlot->setProperties(prop);
      emit updateHisto();
    }
  }
}

void PlotHist1DDialog::toggleBrushColor (bool checked) {
  if (c->synchedPlot) {
    if (checked) {
      QObject *theSender = sender();
      PlotHist1D::Properties prop = c->synchedPlot->properties();
      if (theSender==c->ui.blackBrushToolButton1D) prop.brush.setColor(QColor("black"));
      else if (theSender==c->ui.redBrushToolButton1D) prop.brush.setColor(QColor("red"));
      else if (theSender==c->ui.greenBrushToolButton1D) prop.brush.setColor(QColor("green"));
      else if (theSender==c->ui.blueBrushToolButton1D) prop.brush.setColor(QColor("blue"));
      else if (theSender==c->ui.cyanBrushToolButton1D) prop.brush.setColor(QColor("cyan"));
      else if (theSender==c->ui.purpleBrushToolButton1D) prop.brush.setColor(QColor("purple"));
      else if (theSender==c->ui.grayBrushToolButton1D) prop.brush.setColor(QColor("gray"));
      c->synchedPlot->setProperties(prop);
      emit updateHisto();
    }
  }
}

void PlotHist1DDialog::toggleLineStyle (bool checked) {
  if (c->synchedPlot) {
    if (checked) {
      QObject *theSender = sender();
      PlotHist1D::Properties prop = c->synchedPlot->properties();
      if (theSender==c->ui.solidLineToolButton1D)           prop.pen.setStyle(Qt::SolidLine);
      else if (theSender==c->ui.dashLineToolButton1D)       prop.pen.setStyle(Qt::DashLine);
      else if (theSender==c->ui.dotLineToolButton1D)        prop.pen.setStyle(Qt::DotLine);
      else if (theSender==c->ui.dashDotLineToolButton1D)    prop.pen.setStyle(Qt::DashDotLine);
      else if (theSender==c->ui.dashDotDotLineToolButton1D) prop.pen.setStyle(Qt::DashDotDotLine);
      else if (theSender==c->ui.noLineToolButton1D)         prop.pen.setStyle(Qt::NoPen);
      c->synchedPlot->setProperties(prop);
      emit updateHisto();
    }
  }
}

void PlotHist1DDialog::toggleBrushStyle (bool checked) {
  if (c->synchedPlot) {
    if (checked) {
      QObject *theSender = sender(); 
      PlotHist1D::Properties prop = c->synchedPlot->properties();
      if      (theSender==c->ui.solidBrushToolButton1D)            prop.brush.setStyle(Qt::SolidPattern);
      else if (theSender==c->ui.dense1BrushToolButton1D)           prop.brush.setStyle(Qt::Dense1Pattern);
      else if (theSender==c->ui.dense2BrushToolButton1D)           prop.brush.setStyle(Qt::Dense2Pattern);
      else if (theSender==c->ui.dense3BrushToolButton1D)           prop.brush.setStyle(Qt::Dense3Pattern);
      else if (theSender==c->ui.dense4BrushToolButton1D)           prop.brush.setStyle(Qt::Dense4Pattern);
      else if (theSender==c->ui.dense5BrushToolButton1D)           prop.brush.setStyle(Qt::Dense5Pattern);
      else if (theSender==c->ui.dense6BrushToolButton1D)           prop.brush.setStyle(Qt::Dense6Pattern);
      else if (theSender==c->ui.dense7BrushToolButton1D)           prop.brush.setStyle(Qt::Dense7Pattern);
      else if (theSender==c->ui.horizontalBrushToolButton1D)       prop.brush.setStyle(Qt::HorPattern);
      else if (theSender==c->ui.verticalBrushToolButton1D)         prop.brush.setStyle(Qt::VerPattern);
      else if (theSender==c->ui.crossPatternBrushToolButton1D)     prop.brush.setStyle(Qt::CrossPattern);
      else if (theSender==c->ui.bDiagBrushToolButton1D)            prop.brush.setStyle(Qt::BDiagPattern);
      else if (theSender==c->ui.fDiagBrushToolButton1D)            prop.brush.setStyle(Qt::FDiagPattern);
      else if (theSender==c->ui.diagCrossPatternBrushToolButton1D) prop.brush.setStyle(Qt::DiagCrossPattern);
      else if (theSender==c->ui.noBrushToolButton1D)               prop.brush.setStyle(Qt::NoBrush);
      c->synchedPlot->setProperties(prop);
      emit updateHisto();
    }
  }
}


void PlotHist1DDialog::togglePlotStyle (bool checked) {
  if (c->synchedPlot) {
    if (checked) {
      QObject *theSender = sender();
      PlotHist1D::Properties prop = c->synchedPlot->properties();

      if (theSender==c->ui.noSymbolToolButton1D)            {
	prop.plotStyle=PlotHist1D::Properties::LINES;
      }
      else {
	prop.plotStyle=PlotHist1D::Properties::SYMBOLS;
      }

      
      if       (theSender==c->ui.circleSymbolToolButton1D)         prop.symbolStyle=PlotHist1D::Properties::CIRCLE;
      else if  (theSender==c->ui.squareSymbolToolButton1D)         prop.symbolStyle=PlotHist1D::Properties::SQUARE;
      else if  (theSender==c->ui.upTriangleSymbolToolButton1D)     prop.symbolStyle=PlotHist1D::Properties::TRIANGLE_U;
      else if  (theSender==c->ui.downTriangleSymbolToolButton1D)   prop.symbolStyle=PlotHist1D::Properties::TRIANGLE_L;

      c->synchedPlot->setProperties(prop);
      emit updateHisto();
    }
  }
}

