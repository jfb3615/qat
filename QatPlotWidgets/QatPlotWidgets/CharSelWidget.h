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

#ifndef _CharSelWidget_h_
#define _CharSelWidget_h_
#include <QWidget>
#include <string>
class CharSelWidget: public QWidget {

  Q_OBJECT

 public:
  
  // Constructor:
  CharSelWidget(QWidget *parent=0);

  // Destructor:
  ~CharSelWidget();

  public slots:
    
    void charSel(int row, int col);
    void setFamily (const QString & family);

    
 signals:

    void selChar (const QString & c);

    
 private:

  // Clockwork:
  class Clockwork;
  Clockwork *c;

  // Illegal operations:
  CharSelWidget & operator = (const CharSelWidget &)=delete;
  CharSelWidget(const CharSelWidget &)=delete;

};


#endif
