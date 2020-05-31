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

#ifndef _LabelEditorDialog_h_
#define _LabelEditorDialog_h_
#include "QatPlotWidgets/CharSelWidget.h"
#include <QScrollArea>
#include <QDialog>
#include <QTextEdit>

class LabelEditorDialog: public QDialog
 {
     Q_OBJECT

 public:

     // Constructor:
     LabelEditorDialog(QWidget *parent = 0);

     // Destructor:
     ~LabelEditorDialog();

     // Get the text editor:
     const QTextEdit *textEdit() const;
     QTextEdit *textEdit(); 

 private slots:

     void textFamily( const QString &f );
     void textSize(int i);
     void textBold();
     void textItalic();
     void textScript();
     void textColor();
     void toggleChars();
     void updateCharFormat(const QTextCharFormat &f);


 private:

     class Clockwork;
     Clockwork *c;


 };
#endif

