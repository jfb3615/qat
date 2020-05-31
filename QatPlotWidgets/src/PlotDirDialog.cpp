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

#include "QatPlotWidgets/PlotDirDialog.h"
#include "QatPlotting/PlotHist1D.h"
#include "QatPlotting/PlotHist2D.h"
#include "ui_PlotDirDialog.h"
class PlotDirDialog::Clockwork {
public:
  std::vector<PlotHist1D *> synchedPlot1D;
  std::vector<PlotHist2D *> synchedPlot2D;
  Ui::PlotDirDialog ui;
};

PlotDirDialog::PlotDirDialog(QWidget *parent): QDialog(parent),c(new Clockwork()) {
  c->ui.setupUi(this);
  c->ui.tabWidget->removeTab(5);
  // Unveil later (note, do this also on 1D, 2D dialog);
  c->ui.moreLineColorsToolButton1D->hide();
  c->ui.moreBrushColorsToolButton1D->hide();
  c->ui.moreLineColorsToolButton2D->hide();
  c->ui.moreBrushColorsToolButton1D->hide();

  connect(c->ui.circleSymbolToolButton1D,       SIGNAL(toggled(bool)), this, SLOT(togglePlotStyle1D(bool)));
  connect(c->ui.squareSymbolToolButton1D,       SIGNAL(toggled(bool)), this, SLOT(togglePlotStyle1D(bool)));
  connect(c->ui.upTriangleSymbolToolButton1D,   SIGNAL(toggled(bool)), this, SLOT(togglePlotStyle1D(bool)));
  connect(c->ui.downTriangleSymbolToolButton1D, SIGNAL(toggled(bool)), this, SLOT(togglePlotStyle1D(bool)));
  connect(c->ui.noSymbolToolButton1D, SIGNAL(toggled(bool)), this, SLOT(togglePlotStyle1D(bool)));

  connect(c->ui.symbolSizeSpinBox1D, SIGNAL(valueChanged(int)), this, SLOT(setSymbolSize1D(int)));


  connect(c->ui.lineWidthSpinBox1D,  SIGNAL(valueChanged(int)), this, SLOT(setLineWidth1D(int)));

  connect(c->ui.blackLineToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleLineColor1D(bool)));
  connect(c->ui.redLineToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleLineColor1D(bool)));
  connect(c->ui.greenLineToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleLineColor1D(bool)));
  connect(c->ui.blueLineToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleLineColor1D(bool)));
  connect(c->ui.cyanLineToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleLineColor1D(bool)));
  connect(c->ui.purpleLineToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleLineColor1D(bool)));
  connect(c->ui.grayLineToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleLineColor1D(bool)));


  connect(c->ui.blackBrushToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushColor1D(bool)));
  connect(c->ui.redBrushToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushColor1D(bool)));
  connect(c->ui.greenBrushToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushColor1D(bool)));
  connect(c->ui.blueBrushToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushColor1D(bool)));
  connect(c->ui.cyanBrushToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushColor1D(bool)));
  connect(c->ui.purpleBrushToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushColor1D(bool)));
  connect(c->ui.grayBrushToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushColor1D(bool)));


  connect(c->ui.solidLineToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleLineStyle1D(bool)));
  connect(c->ui.dashLineToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleLineStyle1D(bool)));
  connect(c->ui.dotLineToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleLineStyle1D(bool)));
  connect(c->ui.dashDotLineToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleLineStyle1D(bool)));
  connect(c->ui.dashDotDotLineToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleLineStyle1D(bool)));
  connect(c->ui.noLineToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleLineStyle1D(bool)));

  connect(c->ui.solidBrushToolButton1D,  SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle1D(bool)));
  connect(c->ui.dense1BrushToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle1D(bool)));
  connect(c->ui.dense2BrushToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle1D(bool)));
  connect(c->ui.dense3BrushToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle1D(bool)));
  connect(c->ui.dense4BrushToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle1D(bool)));
  connect(c->ui.dense5BrushToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle1D(bool)));
  connect(c->ui.dense6BrushToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle1D(bool)));
  connect(c->ui.dense7BrushToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle1D(bool)));
  connect(c->ui.horizontalBrushToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle1D(bool)));
  connect(c->ui.verticalBrushToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle1D(bool)));
  connect(c->ui.crossPatternBrushToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle1D(bool)));
  connect(c->ui.bDiagBrushToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle1D(bool)));
  connect(c->ui.fDiagBrushToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle1D(bool)));
  connect(c->ui.diagCrossPatternBrushToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle1D(bool)));
  connect(c->ui.noBrushToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle1D(bool)));






  connect(c->ui.lineWidthSpinBox2D,  SIGNAL(valueChanged(int)), this, SLOT(setLineWidth2D(int)));

  connect(c->ui.blackLineToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleLineColor2D(bool)));
  connect(c->ui.redLineToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleLineColor2D(bool)));
  connect(c->ui.greenLineToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleLineColor2D(bool)));
  connect(c->ui.blueLineToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleLineColor2D(bool)));
  connect(c->ui.cyanLineToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleLineColor2D(bool)));
  connect(c->ui.purpleLineToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleLineColor2D(bool)));
  connect(c->ui.grayLineToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleLineColor2D(bool)));


  connect(c->ui.blackBrushToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushColor2D(bool)));
  connect(c->ui.redBrushToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushColor2D(bool)));
  connect(c->ui.greenBrushToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushColor2D(bool)));
  connect(c->ui.blueBrushToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushColor2D(bool)));
  connect(c->ui.cyanBrushToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushColor2D(bool)));
  connect(c->ui.purpleBrushToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushColor2D(bool)));
  connect(c->ui.grayBrushToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushColor2D(bool)));


  connect(c->ui.solidLineToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleLineStyle2D(bool)));
  connect(c->ui.noLineToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleLineStyle2D(bool)));

  connect(c->ui.solidBrushToolButton2D,  SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle2D(bool)));
  connect(c->ui.dense1BrushToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle2D(bool)));
  connect(c->ui.dense2BrushToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle2D(bool)));
  connect(c->ui.dense3BrushToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle2D(bool)));
  connect(c->ui.dense4BrushToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle2D(bool)));
  connect(c->ui.dense5BrushToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle2D(bool)));
  connect(c->ui.dense6BrushToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle2D(bool)));
  connect(c->ui.dense7BrushToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle2D(bool)));
  connect(c->ui.noBrushToolButton2D, SIGNAL(toggled(bool)), this, SLOT(toggleBrushStyle2D(bool)));



}

PlotDirDialog::~PlotDirDialog() {
  delete c;
}   
 

#include <iostream>
void PlotDirDialog::synchTo(std::vector<PlotHist1D *> p1D, std::vector<PlotHist2D *> p2D) {
  
  c->synchedPlot1D=p1D;
  c->synchedPlot2D=p2D;

}


void PlotDirDialog::togglePlotStyle1D (bool checked) {
  QObject *theSender = sender();
  for (unsigned int i=0;i<c->synchedPlot1D.size();i++) {
    PlotHist1D *plot=c->synchedPlot1D[i];
    if (checked) {
      PlotHist1D::Properties prop = plot->properties();

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

      plot->setProperties(prop);
    }
  }
  emit updateManager();
}




void PlotDirDialog::setSymbolSize1D (int sz) {
  for (unsigned int i=0;i<c->synchedPlot1D.size();i++) {
    PlotHist1D *plot=c->synchedPlot1D[i];
    PlotHist1D::Properties prop = plot->properties();
    prop.symbolSize=sz;
    plot->setProperties(prop);
  }
  emit updateManager();
}




void PlotDirDialog::setLineWidth1D (int width) {

  for (unsigned int i=0;i<c->synchedPlot1D.size();i++) {
    PlotHist1D *plot=c->synchedPlot1D[i];
    PlotHist1D::Properties prop = plot->properties();
    prop.pen.setWidth(width);
    plot->setProperties(prop);
  }
  emit updateManager();

}



void PlotDirDialog::toggleLineColor1D (bool checked) {
  QObject *theSender = sender();
  for (unsigned int i=0;i<c->synchedPlot1D.size();i++) {
    PlotHist1D *plot=c->synchedPlot1D[i];
    if (checked) {
      PlotHist1D::Properties prop = plot->properties();
      if (theSender==c->ui.blackLineToolButton1D) prop.pen.setColor(QColor("black"));
      else if (theSender==c->ui.redLineToolButton1D) prop.pen.setColor(QColor("red"));
      else if (theSender==c->ui.greenLineToolButton1D) prop.pen.setColor(QColor("green"));
      else if (theSender==c->ui.blueLineToolButton1D) prop.pen.setColor(QColor("blue"));
      else if (theSender==c->ui.cyanLineToolButton1D) prop.pen.setColor(QColor("cyan"));
      else if (theSender==c->ui.purpleLineToolButton1D) prop.pen.setColor(QColor("purple"));
      else if (theSender==c->ui.grayLineToolButton1D) prop.pen.setColor(QColor("gray"));
      plot->setProperties(prop);
    }
  }
  emit updateManager();
}

void PlotDirDialog::toggleBrushColor1D (bool checked) {
  QObject *theSender = sender();
  for (unsigned int i=0;i<c->synchedPlot1D.size();i++) {
    PlotHist1D *plot=c->synchedPlot1D[i];
    if (checked) {
      PlotHist1D::Properties prop = plot->properties();
      if (theSender==c->ui.blackBrushToolButton1D) prop.brush.setColor(QColor("black"));
      else if (theSender==c->ui.redBrushToolButton1D) prop.brush.setColor(QColor("red"));
      else if (theSender==c->ui.greenBrushToolButton1D) prop.brush.setColor(QColor("green"));
      else if (theSender==c->ui.blueBrushToolButton1D) prop.brush.setColor(QColor("blue"));
      else if (theSender==c->ui.cyanBrushToolButton1D) prop.brush.setColor(QColor("cyan"));
      else if (theSender==c->ui.purpleBrushToolButton1D) prop.brush.setColor(QColor("purple"));
      else if (theSender==c->ui.grayBrushToolButton1D) prop.brush.setColor(QColor("gray"));
      plot->setProperties(prop);
    }
  }
  emit updateManager();
}

void PlotDirDialog::toggleLineStyle1D (bool checked) {
  QObject *theSender = sender();
  for (unsigned int i=0;i<c->synchedPlot1D.size();i++) {
    PlotHist1D *plot=c->synchedPlot1D[i];
    if (checked) {
      PlotHist1D::Properties prop = plot->properties();
      if (theSender==c->ui.solidLineToolButton1D)           prop.pen.setStyle(Qt::SolidLine);
      else if (theSender==c->ui.dashLineToolButton1D)       prop.pen.setStyle(Qt::DashLine);
      else if (theSender==c->ui.dotLineToolButton1D)        prop.pen.setStyle(Qt::DotLine);
      else if (theSender==c->ui.dashDotLineToolButton1D)    prop.pen.setStyle(Qt::DashDotLine);
      else if (theSender==c->ui.dashDotDotLineToolButton1D) prop.pen.setStyle(Qt::DashDotDotLine);
      else if (theSender==c->ui.noLineToolButton1D)         prop.pen.setStyle(Qt::NoPen);
      plot->setProperties(prop);
    }
  }
  emit updateManager();
}

void PlotDirDialog::toggleBrushStyle1D (bool checked) {
  QObject *theSender = sender(); 
  for (unsigned int i=0;i<c->synchedPlot1D.size();i++) {
    PlotHist1D *plot=c->synchedPlot1D[i];
    if (checked) {
      PlotHist1D::Properties prop = plot->properties();
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
      plot->setProperties(prop);
    }
  }
  emit updateManager();

}


void PlotDirDialog::setLineWidth2D (int width) {

  for (unsigned int i=0;i<c->synchedPlot2D.size();i++) {
    PlotHist2D *plot=c->synchedPlot2D[i];
    PlotHist2D::Properties prop = plot->properties();
    prop.pen.setWidth(width);
    plot->setProperties(prop);
  }
  emit updateManager();

}



void PlotDirDialog::toggleLineColor2D (bool checked) {
  QObject *theSender = sender();
  for (unsigned int i=0;i<c->synchedPlot2D.size();i++) {
    PlotHist2D *plot=c->synchedPlot2D[i];
    if (checked) {
      PlotHist2D::Properties prop = plot->properties();
      if (theSender==c->ui.blackLineToolButton2D) prop.pen.setColor(QColor("black"));
      else if (theSender==c->ui.redLineToolButton2D) prop.pen.setColor(QColor("red"));
      else if (theSender==c->ui.greenLineToolButton2D) prop.pen.setColor(QColor("green"));
      else if (theSender==c->ui.blueLineToolButton2D) prop.pen.setColor(QColor("blue"));
      else if (theSender==c->ui.cyanLineToolButton2D) prop.pen.setColor(QColor("cyan"));
      else if (theSender==c->ui.purpleLineToolButton2D) prop.pen.setColor(QColor("purple"));
      else if (theSender==c->ui.grayLineToolButton2D) prop.pen.setColor(QColor("gray"));
      plot->setProperties(prop);
    }
  }
  emit updateManager();
}

void PlotDirDialog::toggleBrushColor2D (bool checked) {
  QObject *theSender = sender();
  for (unsigned int i=0;i<c->synchedPlot2D.size();i++) {
    PlotHist2D *plot=c->synchedPlot2D[i];
    if (checked) {
      PlotHist2D::Properties prop = plot->properties();
      if (theSender==c->ui.blackBrushToolButton2D) prop.brush.setColor(QColor("black"));
      else if (theSender==c->ui.redBrushToolButton2D) prop.brush.setColor(QColor("red"));
      else if (theSender==c->ui.greenBrushToolButton2D) prop.brush.setColor(QColor("green"));
      else if (theSender==c->ui.blueBrushToolButton2D) prop.brush.setColor(QColor("blue"));
      else if (theSender==c->ui.cyanBrushToolButton2D) prop.brush.setColor(QColor("cyan"));
      else if (theSender==c->ui.purpleBrushToolButton2D) prop.brush.setColor(QColor("purple"));
      else if (theSender==c->ui.grayBrushToolButton2D) prop.brush.setColor(QColor("gray"));
      plot->setProperties(prop);
    }
  }
  emit updateManager();
}

void PlotDirDialog::toggleLineStyle2D (bool checked) {
  QObject *theSender = sender();
  for (unsigned int i=0;i<c->synchedPlot2D.size();i++) {
    PlotHist2D *plot=c->synchedPlot2D[i];
    if (checked) {
      PlotHist2D::Properties prop = plot->properties();
      if (theSender==c->ui.solidLineToolButton2D)           prop.pen.setStyle(Qt::SolidLine);
      else if (theSender==c->ui.noLineToolButton2D)         prop.pen.setStyle(Qt::NoPen);
      plot->setProperties(prop);
    }
  }
  emit updateManager();
}

void PlotDirDialog::toggleBrushStyle2D (bool checked) {
  QObject *theSender = sender(); 
  for (unsigned int i=0;i<c->synchedPlot2D.size();i++) {
    PlotHist2D *plot=c->synchedPlot2D[i];
    if (checked) {
      PlotHist2D::Properties prop = plot->properties();
      if      (theSender==c->ui.solidBrushToolButton2D)            prop.brush.setStyle(Qt::SolidPattern);
      else if (theSender==c->ui.dense1BrushToolButton2D)           prop.brush.setStyle(Qt::Dense1Pattern);
      else if (theSender==c->ui.dense2BrushToolButton2D)           prop.brush.setStyle(Qt::Dense2Pattern);
      else if (theSender==c->ui.dense3BrushToolButton2D)           prop.brush.setStyle(Qt::Dense3Pattern);
      else if (theSender==c->ui.dense4BrushToolButton2D)           prop.brush.setStyle(Qt::Dense4Pattern);
      else if (theSender==c->ui.dense5BrushToolButton2D)           prop.brush.setStyle(Qt::Dense5Pattern);
      else if (theSender==c->ui.dense6BrushToolButton2D)           prop.brush.setStyle(Qt::Dense6Pattern);
      else if (theSender==c->ui.dense7BrushToolButton2D)           prop.brush.setStyle(Qt::Dense7Pattern);
      else if (theSender==c->ui.noBrushToolButton2D)               prop.brush.setStyle(Qt::NoBrush);
      plot->setProperties(prop);
    }
  }
  emit updateManager();

}


