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

#include "QatPlotWidgets/ParameterDialog.h"
#include "ui_ParameterDialog.h"
class ParameterDialog::Clockwork {
public:
  Genfun::Parameter *p;
  Ui::ParameterDialog ui;
};

ParameterDialog::ParameterDialog(QWidget *parent, Genfun::Parameter * p): QDialog(parent), c(new Clockwork()){
  c->p=p;
  c->ui.setupUi(this);

  double xmin=c->p->getLowerLimit();
  double xmax=c->p->getUpperLimit();
  double val =c->p->getValue();
  double frac=double(val-xmin)/double(xmax-xmin);

  int min=c->ui.dial->minimum();
  int max=c->ui.dial->maximum();
  int dialVal = min + int(frac*(max-min));
  c->ui.dial->setValue(dialVal);
  connect(c->ui.dial, SIGNAL(valueChanged(int)), this, SLOT(updateParameter(int)));
}


ParameterDialog::~ParameterDialog() {
  delete c;
}

void ParameterDialog::updateParameter(int val) {

  int min=c->ui.dial->minimum();
  int max=c->ui.dial->maximum();
  
  double frac=double(val-min)/double(max-min);
  
  double xmin=c->p->getLowerLimit();
  double xmax=c->p->getUpperLimit();

  c->p->setValue(frac*(xmax-xmin)+xmin);

  emit updateFunction();
}
