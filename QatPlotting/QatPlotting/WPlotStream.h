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

#ifndef _WPlotStream_h_
#define _WPlotStream_h_
#include <sstream>
#include <iomanip>
#include <QTextEdit>
#include <QColor>
#include <string>
//  This is a simple class to be used with editable label widgets
class WPlotStream {

 public:

  class Clear {
  public:
    Clear() {}
  };

  class EndP {
  public:
    EndP() {}
  };

  class Family {
  public:
    Family(const std::string & name):name(name) {}
    std::string name;
  };

  class Size {
  public:
    Size(unsigned int pointsize):pointsize(pointsize){}
    unsigned int pointsize;
  };
  class SetPrecision {
  public:
    SetPrecision (unsigned int val):val(val){}
    unsigned int val;
  };

  class SetWidth {
  public:
    SetWidth (unsigned int val):val(val){}
    unsigned int val;
  };

  class Super {
  public:
    Super(){}
  };

  class Sub {
  public:
    Sub(){}
  };

  class Normal {
  public:
    Normal(){}
  };

  class Bold {
  public:
    Bold(){}
  };

  class Regular {
  public:
    Regular(){}
  };

  class Italic {
  public:
    Italic(){}
  };

  class Oblique {
  public:
    Oblique(){}
  };

  class Left {
  public:
    Left(){}
  };

  class Right {
  public:
    Right(){}
  };

  class Center {
  public:
    Center(){}
  };

  class Color {
  public:
    Color(const QColor &color):color(color){}
    QColor color;
  };

  WPlotStream (QTextEdit * textEdit):textEdit(textEdit){}

  WPlotStream & operator << (const Clear &){ 
    textEdit->clear();
    stream.str(L"");
    return *this;
  }
  WPlotStream & operator << (const EndP &){ 
    textEdit->insertPlainText(QString::fromStdWString(stream.str()));
    stream.str(L"");
    return *this;
  }
  WPlotStream & operator << (const Family & family){
    textEdit->insertPlainText(QString::fromStdWString(stream.str()));
    stream.str(L"");
    textEdit->setFontFamily(QString(family.name.c_str()));
    return *this;
  }
  WPlotStream & operator << (const Size & sz){
    textEdit->insertPlainText(QString::fromStdWString(stream.str()));
    stream.str(L"");
    textEdit->setFontPointSize(sz.pointsize);
    return *this;
  }

  WPlotStream & operator << (const SetPrecision & sz){
    stream<<std::setprecision(sz.val);
    return *this;
  }
  WPlotStream & operator << (const SetWidth & sz){
    stream<<std::setw(sz.val);
    return *this;
  }
  WPlotStream & operator << (const Super &){
    textEdit->insertPlainText(QString::fromStdWString(stream.str()));
    stream.str(L"");
    QTextCharFormat format=textEdit->currentCharFormat();
    format.setVerticalAlignment(QTextCharFormat::AlignSuperScript);
    textEdit->setCurrentCharFormat(format);
    return *this;
  }
  WPlotStream & operator << (const Sub &){
    textEdit->insertPlainText(QString::fromStdWString(stream.str()));
    stream.str(L"");
    QTextCharFormat format=textEdit->currentCharFormat();
    format.setVerticalAlignment(QTextCharFormat::AlignSubScript);
    textEdit->setCurrentCharFormat(format);
    return *this;
  }

  WPlotStream & operator << (const Normal &){
    textEdit->insertPlainText(QString::fromStdWString(stream.str()));
    stream.str(L"");
    QTextCharFormat format=textEdit->currentCharFormat();
    format.setVerticalAlignment(QTextCharFormat::AlignNormal);
    textEdit->setCurrentCharFormat(format);
    return *this;
  }

  WPlotStream & operator << (const Left &){
    textEdit->insertPlainText(QString::fromStdWString(stream.str()));
    stream.str(L"");
    textEdit->setAlignment(Qt::AlignLeft);
    return *this;
  }

  WPlotStream & operator << (const Right &){
    textEdit->insertPlainText(QString::fromStdWString(stream.str()));
    stream.str(L"");
    textEdit->setAlignment(Qt::AlignRight);
    return *this;
  }

  WPlotStream & operator << (const Center &){
    textEdit->insertPlainText(QString::fromStdWString(stream.str()));
    stream.str(L"");
    textEdit->setAlignment(Qt::AlignCenter);
    return *this;
  }

  WPlotStream & operator << (const Bold &){
    textEdit->insertPlainText(QString::fromStdWString(stream.str()));
    stream.str(L"");
    textEdit->setFontWeight(QFont::Bold);
    return *this;
  }

  WPlotStream & operator << (const Regular &){
    textEdit->insertPlainText(QString::fromStdWString(stream.str()));
    stream.str(L"");
    textEdit->setFontWeight(QFont::Normal);
    return *this;
  }

  WPlotStream & operator << (const Italic &){
    textEdit->insertPlainText(QString::fromStdWString(stream.str()));
    stream.str(L"");
    textEdit->setFontItalic(true);
    return *this;
  }

  WPlotStream & operator << (const Oblique &){
    textEdit->insertPlainText(QString::fromStdWString(stream.str()));
    stream.str(L"");
    textEdit->setFontItalic(false);
    return *this;
  }

  WPlotStream & operator << (const Color & color) {
    textEdit->insertPlainText(QString::fromStdWString(stream.str()));
    stream.str(L"");
    textEdit->setTextColor(color.color);
    return *this;
  }

  WPlotStream & operator << (const std::wstring & text) {
    stream << text;
    return *this;
  }

  WPlotStream & operator << (const wchar_t c) {
    stream << c;
    return *this;
  }

  WPlotStream & operator << (double x) {
    stream << x;
    return *this;
  }

  WPlotStream & operator << (int i) {
    stream << i;
    return *this;
  }
 
  QTextEdit *textEdit;
  std::wostringstream stream;
};

#endif
