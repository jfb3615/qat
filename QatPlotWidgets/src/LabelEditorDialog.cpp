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

#include "QatPlotWidgets/LabelEditorDialog.h"
#include <QColorDialog>
#include <iostream>
#include "ui_LabelEditorDialog.h"

class LabelEditorDialog::Clockwork {
public:
  Ui::LabelEditorDialog ui;
  CharSelWidget *cw;
};

LabelEditorDialog::~LabelEditorDialog() {
  delete c;
}
LabelEditorDialog::LabelEditorDialog(QWidget *parent): QDialog(parent) ,c(new Clockwork()){
  c->ui.setupUi(this);
  connect (c->ui.boldButton,         SIGNAL(clicked()), this, SLOT(textBold()));
  connect (c->ui.italicButton,       SIGNAL(clicked()), this, SLOT(textItalic()));
  connect (c->ui.scriptButton,       SIGNAL(clicked()), this, SLOT(textScript()));
  connect (c->ui.fontComboBox,       SIGNAL(activated( const QString & ) ), this, SLOT(textFamily(const QString &)));
  connect (c->ui.fontSizeSpinBox,    SIGNAL(valueChanged(int)),             this, SLOT(textSize(int)));
  connect (c->ui.colorButton,        SIGNAL(clicked()),                     this, SLOT(textColor()));        
  connect (c->ui.specialCButton,     SIGNAL(clicked()),                     this, SLOT(toggleChars()));
  QColor color("black");

  QPalette palette = c->ui.colorButton->palette();
  palette.setColor(QPalette::Button,color);
  c->ui.colorButton->setAutoFillBackground(true);
  c->ui.colorButton->setPalette(palette);

  c->cw=new CharSelWidget(0);
  c->ui.frame->setWidget(c->cw);
  connect (c->ui.fontComboBox,       SIGNAL(activated( const QString &)),   c->cw,   SLOT(setFamily(const QString & )));
  connect (c->cw, SIGNAL(selChar(const QString &)), c->ui.textEdit, SLOT(insertPlainText(const QString & )));
  
  //c->cw->updateSize("12");

  connect(c->ui.textEdit, SIGNAL(currentCharFormatChanged(const QTextCharFormat &)), this, SLOT(updateCharFormat(const QTextCharFormat &)));
  c->ui.frame->hide();
  setMinimumSize(600,200);
  resize(minimumSize());

  QTextCharFormat format;
  QFont font;
  font.setFamily("Arial");
  font.setPointSize(24);
  format.setFont(font);
  c->ui.textEdit->setCurrentCharFormat(format);
  c->ui.textEdit->setFocus();
  c->ui.textEdit->setText(QString(""));
}

const QTextEdit *LabelEditorDialog::textEdit() const {
  return c->ui.textEdit;
}


QTextEdit *LabelEditorDialog::textEdit() {
  return c->ui.textEdit;
}




void LabelEditorDialog::textFamily( const QString &f )
{
  QTextEdit * tE = c->ui.textEdit;
  QTextCharFormat format; 
  format.setFontFamily(f);
  tE->mergeCurrentCharFormat(format);
}


void LabelEditorDialog::textSize( int i )
{
  QTextEdit * tE = c->ui.textEdit;
  QTextCharFormat format; 
  format.setFontPointSize(i);
  tE->mergeCurrentCharFormat(format);
}


void LabelEditorDialog::textBold()
{
 
  QFont font=c->ui.boldButton->font();
  bool flag=font.bold();
  flag = !flag;
  font.setBold(flag);
  c->ui.boldButton->setFont(font);


  QTextEdit * tE = c->ui.textEdit;
  QTextCharFormat format; 
  format.setFontWeight(flag ?  QFont::Bold : QFont::Normal );
  tE->mergeCurrentCharFormat(format);

}

void LabelEditorDialog::textItalic()
{
  QFont font=c->ui.italicButton->font();
  bool flag=font.italic();
  flag = !flag;
  font.setItalic(flag);
  c->ui.italicButton->setFont(font);


  QTextEdit * tE = c->ui.textEdit;
  QTextCharFormat format; 
  format.setFontItalic(flag);
  tE->mergeCurrentCharFormat(format);
}


void LabelEditorDialog::textScript()
{
  QTextEdit * tE = c->ui.textEdit;
  QTextCharFormat format;

  QString  text = c->ui.scriptButton->text();
  if (text==QString("-")) {
    c->ui.scriptButton->setText(QString("^"));
    format.setVerticalAlignment(QTextCharFormat::AlignSuperScript);
  }
  if (text==QString("^")){
    c->ui.scriptButton->setText(QString("_"));  
    format.setVerticalAlignment(QTextCharFormat::AlignSubScript);
  } 
  if (text==QString("_")) {
    c->ui.scriptButton->setText(QString("-"));
    format.setVerticalAlignment(QTextCharFormat::AlignNormal);
  }
  
  tE->mergeCurrentCharFormat(format);
}


void LabelEditorDialog::textColor()
{
  QColor textColorVal;
  QColor col = QColorDialog::getColor(textColorVal, this );
  if ( !col.isValid() ) return;

  QPalette palette = c->ui.colorButton->palette();
  palette.setColor(QPalette::Button,col);
  c->ui.colorButton->setAutoFillBackground(true);
  c->ui.colorButton->setPalette(palette);

  QTextEdit * tE = c->ui.textEdit;
  tE->setTextColor(col);
}

void LabelEditorDialog::updateCharFormat(const QTextCharFormat &f) {
  
  // Don't update if cursor moves to select text, it is damn annoying.
  if (c->ui.textEdit->textCursor().selectionStart()!=c->ui.textEdit->textCursor().selectionEnd()) return;

  static QTextCharFormat df;
  df.setVerticalAlignment(QTextCharFormat::AlignNormal);
  df.setFontItalic(false);
  df.setFontWeight(QFont::Normal);
  df.setFontPointSize(24);
  df.setFontFamily("Arial");

  QTextCharFormat F = f.fontPointSize()!=0.0 ? f: df;
  if (F.verticalAlignment()==QTextCharFormat::AlignNormal)     c->ui.scriptButton->setText(QString("-"));
  if (F.verticalAlignment()==QTextCharFormat::AlignSubScript)  c->ui.scriptButton->setText(QString("_"));
  if (F.verticalAlignment()==QTextCharFormat::AlignSuperScript)  c->ui.scriptButton->setText(QString("^"));
  {
    QFont font=c->ui.italicButton->font(); 
    font.setItalic (F.fontItalic()); 
    c->ui.italicButton->setFont(font); 
  }
  {
    QFont font=c->ui.boldButton->font(); 
    font.setBold(F.fontWeight()==QFont::Bold);
    c->ui.boldButton->setFont(font); 
  }

  c->ui.fontSizeSpinBox->setValue(int(F.fontPointSize()+0.5));

  QString fam=F.fontFamily();
  c->ui.fontComboBox->setEditText(fam);

  
  QColor color=F.foreground().color();
  QPalette palette = c->ui.colorButton->palette();
  palette.setColor(QPalette::Button,color);
  c->ui.colorButton->setAutoFillBackground(true);
  c->ui.colorButton->setPalette(palette);
}



void LabelEditorDialog::toggleChars() {
  if (c->ui.frame->isHidden()) {
    c->ui.frame->show();
    setMinimumSize(600,600);
    resize(minimumSize());
  }
  else {
    
    c->ui.frame->hide();
    setMinimumSize(600,200);
    resize(minimumSize());
  }

}

