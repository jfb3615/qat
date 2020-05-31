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

#include "QatPlotWidgets/CharSelWidget.h"
#include "ui_CharSelWidget.h"
#include <QHBoxLayout>
#include <QTableWidgetItem>
#include <QMouseEvent>
#include <iostream>
#include <iomanip>
#include <sstream>
class CharSelWidget::Clockwork {
public:

  // User Interface stuff:
  Ui::CharSelWidget        ui;
};


CharSelWidget::CharSelWidget(QWidget *parent):QWidget(parent),c(new Clockwork()){
  c->ui.setupUi(this);
  unsigned int ncols=c->ui.tableWidget->columnCount();
  c->ui.tableWidget->setRowCount(1);
  unsigned int row=0,col=0;
  for (int i=0;i<(1<<16);i++) {
    QChar q(i);
    if (q.isPrint() && q.script()<=4) {
      
      QTableWidgetItem *item=new QTableWidgetItem;
      item->setText(QString(q));

      std::ostringstream stream;
      stream << std::hex;
      stream << i;
      item->setToolTip(QString::fromStdString(stream.str()));

      
      c->ui.tableWidget->setItem(row,col,item);
      if (col==ncols-1) {
	col=0;
	row=c->ui.tableWidget->rowCount();
	c->ui.tableWidget->setRowCount(row + 1);
      }
      else {
	col++;
      }
    }
  }
  QObject::connect(c->ui.tableWidget, SIGNAL(cellDoubleClicked(int, int)),
		   this, SLOT(charSel(int, int)));
}

// Destructor:
CharSelWidget::~CharSelWidget(){
  delete c;
}

void CharSelWidget::charSel(int row, int col)
{
  QTableWidgetItem *item=c->ui.tableWidget->item(row,col);
  emit selChar(item->text());
}
void CharSelWidget::setFamily (const QString & family) {
  QFont font=c->ui.tableWidget->font();
  font.setFamily(family);
  c->ui.tableWidget->setFont(font);
  c->ui.tableWidget->update();
}
