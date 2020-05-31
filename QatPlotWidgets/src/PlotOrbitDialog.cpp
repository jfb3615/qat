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

#include "QatPlotWidgets/PlotOrbitDialog.h"
#include "QatPlotWidgets/ParameterDialog.h"
#include "QatPlotting/PlotOrbit.h"
#include "ui_PlotDirDialog.h"
#include <sstream>
  
class PlotOrbitDialog::Clockwork {
public:
  Ui::PlotDirDialog ui;
  PlotOrbit *synchedPlot;
  std::vector<Genfun::Parameter *> parameters;
  std::map<Genfun::Parameter *, ParameterDialog *> paramDialogMap;
};

PlotOrbitDialog::PlotOrbitDialog(QWidget *parent): QDialog(parent),c(new Clockwork()) {
  c->synchedPlot=NULL;
  c->ui.setupUi(this);
  c->ui.tabWidget->removeTab(4);
  c->ui.tabWidget->removeTab(3);
  c->ui.tabWidget->removeTab(2);
  c->ui.tabWidget->removeTab(0);

  // Unveil later (note, do this also on  2D, Dir dialog);
  c->ui.moreLineColorsToolButton1D->hide();
  c->ui.moreBrushColorsToolButton1D->hide();
  c->ui.moreLineColorsToolButton2D->hide();
  c->ui.moreBrushColorsToolButton1D->hide();


  connect(c->ui.lineWidthSpinBox1D,  SIGNAL(valueChanged(int)), this, SLOT(setLineWidth(int)));

  connect(c->ui.blackLineToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleLineColor(bool)));
  connect(c->ui.redLineToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleLineColor(bool)));
  connect(c->ui.greenLineToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleLineColor(bool)));
  connect(c->ui.blueLineToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleLineColor(bool)));
  connect(c->ui.cyanLineToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleLineColor(bool)));
  connect(c->ui.purpleLineToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleLineColor(bool)));
  connect(c->ui.grayLineToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleLineColor(bool)));

  connect(c->ui.solidLineToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleLineStyle(bool)));
  connect(c->ui.dashLineToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleLineStyle(bool)));
  connect(c->ui.dotLineToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleLineStyle(bool)));
  connect(c->ui.dashDotLineToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleLineStyle(bool)));
  connect(c->ui.dashDotDotLineToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleLineStyle(bool)));
  connect(c->ui.noLineToolButton1D, SIGNAL(toggled(bool)), this, SLOT(toggleLineStyle(bool)));


  connect(c->ui.parameterTableWidget, SIGNAL(cellChanged(int, int)), this, SLOT(changeParameter(int, int)));
  connect(c->ui.parameterTableWidget, SIGNAL(cellDoubleClicked(int,int)), this, SLOT (popParameterDial(int,int)));

  c->ui.buttonBox->button(QDialogButtonBox::Ok)->setDefault(false);
  c->ui.buttonBox->button(QDialogButtonBox::Cancel)->setDefault(false);
  c->ui.buttonBox->button(QDialogButtonBox::Ok)->setAutoDefault(false);
  c->ui.buttonBox->button(QDialogButtonBox::Cancel)->setAutoDefault(false);

}

PlotOrbitDialog::~PlotOrbitDialog() {
  delete c;
}   
 

#include <iostream>
void PlotOrbitDialog::synchTo(PlotOrbit *plot) {
  
  c->synchedPlot=plot;

  if (!plot) return;

  PlotOrbit::Properties prop=plot->properties();
  QPen pen=prop.pen;

  int width=pen.width();
  c->ui.lineWidthSpinBox1D->setValue(width);

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


}

void PlotOrbitDialog::setLineWidth (int i) {
  if (c->synchedPlot) {
    PlotOrbit::Properties prop = c->synchedPlot->properties();
    prop.pen.setWidth(i);
    c->synchedPlot->setProperties(prop);
    emit updateFunction();
  }
}


void PlotOrbitDialog::toggleLineColor (bool checked) {
  if (c->synchedPlot) {
    if (checked) {
      QObject *theSender = sender();
      PlotOrbit::Properties prop = c->synchedPlot->properties();
      if (theSender==c->ui.blackLineToolButton1D) prop.pen.setColor(QColor("black"));
      else if (theSender==c->ui.redLineToolButton1D) prop.pen.setColor(QColor("red"));
      else if (theSender==c->ui.greenLineToolButton1D) prop.pen.setColor(QColor("green"));
      else if (theSender==c->ui.blueLineToolButton1D) prop.pen.setColor(QColor("blue"));
      else if (theSender==c->ui.cyanLineToolButton1D) prop.pen.setColor(QColor("cyan"));
      else if (theSender==c->ui.purpleLineToolButton1D) prop.pen.setColor(QColor("purple"));
      else if (theSender==c->ui.grayLineToolButton1D) prop.pen.setColor(QColor("gray"));
      c->synchedPlot->setProperties(prop);
      emit updateFunction();
    }
  }
}

void PlotOrbitDialog::toggleLineStyle (bool checked) {
  if (c->synchedPlot) {
    if (checked) {
      QObject *theSender = sender();
      PlotOrbit::Properties prop = c->synchedPlot->properties();
      if (theSender==c->ui.solidLineToolButton1D)           prop.pen.setStyle(Qt::SolidLine);
      else if (theSender==c->ui.dashLineToolButton1D)       prop.pen.setStyle(Qt::DashLine);
      else if (theSender==c->ui.dotLineToolButton1D)        prop.pen.setStyle(Qt::DotLine);
      else if (theSender==c->ui.dashDotLineToolButton1D)    prop.pen.setStyle(Qt::DashDotLine);
      else if (theSender==c->ui.dashDotDotLineToolButton1D) prop.pen.setStyle(Qt::DashDotDotLine);
      else if (theSender==c->ui.noLineToolButton1D)         prop.pen.setStyle(Qt::NoPen);
      c->synchedPlot->setProperties(prop);
      emit updateFunction();
    }
  }
}

void PlotOrbitDialog::clearParameters() {
  c->parameters.erase(c->parameters.begin(),c->parameters.end());
  c->ui.parameterTableWidget->clear();
  c->ui.parameterTableWidget->hide();
}

void PlotOrbitDialog::addParameter(Genfun::Parameter *p) {
  c->parameters.push_back(p);
  c->ui.parameterTableWidget->setRowCount(c->parameters.size());


  std::ostringstream stream;
  stream << p->getValue();
  
  {
    QTableWidgetItem *item = new QTableWidgetItem(p->getName().c_str());
    item->setFlags(0);
    c->ui.parameterTableWidget->setItem(c->parameters.size()-1, 0, item);
  }

  {
    QTableWidgetItem *item = new QTableWidgetItem(stream.str().c_str());
    c->ui.parameterTableWidget->setItem(c->parameters.size()-1, 1, item);
    item->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled);
  }
  c->ui.parameterTableWidget->show();
    
}

void PlotOrbitDialog::changeParameter(int row, int column) {

  std::istringstream inStream(c->ui.parameterTableWidget->item(row, column)->text().toStdString().c_str());
  double x; 
  if (inStream >> x) {
    if (x > c->parameters[row]->getLowerLimit() && x < c->parameters[row]->getUpperLimit()) {
      c->parameters[row]->setValue(x);
      emit updateFunction();
      return;
    }
  }
  std::ostringstream outStream;
  outStream << c->parameters[row]->getValue();
  QTableWidgetItem *valueItem=c->ui.parameterTableWidget->item(row,1);
  if (valueItem) valueItem->setText(outStream.str().c_str());
}


void PlotOrbitDialog::popParameterDial(int row, int column) {
  if (column==1) {
    
    ParameterDialog *d=NULL;
    std::map<Genfun::Parameter *, ParameterDialog *>::iterator pd=c->paramDialogMap.find(c->parameters[row]);
    if (pd==c->paramDialogMap.end()) {
      
      
      d = new ParameterDialog(0, c->parameters[row]);
      d->setWindowTitle(c->parameters[row]->getName().c_str());
      connect(d, SIGNAL(updateFunction()), this, SIGNAL(updateFunction()));
      connect(d, SIGNAL(updateFunction()), this, SLOT(synchToFunction()));
      c->paramDialogMap[c->parameters[row]]=d;
    }
    else {
      d = (*pd).second;
    }
    d->show();
    d->raise();
  }

}

void PlotOrbitDialog::synchToFunction() {
  blockSignals(true);

  for (unsigned int row=0;row<c->parameters.size();row++) {
    std::ostringstream outStream;
    outStream << c->parameters[row]->getValue();
    QTableWidgetItem *valueItem=c->ui.parameterTableWidget->item(row,1);
    if (valueItem) valueItem->setText(outStream.str().c_str());
  }
  blockSignals(false);
}
