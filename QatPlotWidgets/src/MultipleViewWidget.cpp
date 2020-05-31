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

#include "QatPlotWidgets/MultipleViewWidget.h"
#include "ui_MultipleViewWidget.h"
#include <QHBoxLayout>
#include <iostream>
class MultipleViewWidget::Clockwork {
public:

  // User Interface stuff:
  Ui::MultipleViewWidget        ui;
};


MultipleViewWidget::MultipleViewWidget(QWidget *parent):QWidget(parent),c(new Clockwork()){
  c->ui.setupUi(this);
}

// Destructor:
MultipleViewWidget::~MultipleViewWidget(){
  delete c;
}

void MultipleViewWidget::add(QWidget *w, const std::string & tabTitle) {
  QWidget *widget= new QWidget;
  QHBoxLayout *layout = new QHBoxLayout;
  widget->setLayout(layout);
  layout->addWidget(w);
  
  c->ui.tabWidget->addTab(widget, tabTitle.c_str());
 

    
  //c->ui.tabWidget->addTab(w,tabTitle.c_str());
}
