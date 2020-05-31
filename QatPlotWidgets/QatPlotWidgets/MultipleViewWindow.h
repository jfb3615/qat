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

#ifndef _MultipleViewWindow_h_
#define _MultipleViewWindow_h_
#include <QMainWindow>
#include <string>
class MultipleViewWindow: public QMainWindow {

  Q_OBJECT

 public:
  
  // Constructor:
  MultipleViewWindow(QWidget *parent=0);

  // Destructor:
  ~MultipleViewWindow();

  // Add a widget.  It will automatically create a tab and
  // a layout for this widget.
  void add (QWidget *w, const std::string & tabTitle);
  
  
 private:

  // Clockwork:
  class Clockwork;
  Clockwork *c;

  // Illegal operations:
  MultipleViewWindow & operator = (const MultipleViewWindow &)=delete;
  MultipleViewWindow(const MultipleViewWindow &)=delete;

};


#endif
