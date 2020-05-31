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

#include "QatProjectForm.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QCheckBox>
#include <QRadioButton>
#include <iostream>
#include <string>
#include <stdexcept>
#include <unistd.h>
#include <sys/stat.h> 
#include <sys/types.h>
#include <errno.h>
#define STR_VALUE(arg) #arg
#define STR_NAME(name) STR_VALUE(name)

QatProjectForm::QatProjectForm(QWidget *parent) :
  QWidget(parent) 
{
  ui.setupUi(this);
  connect(ui.quitButton,                  SIGNAL(clicked()),         qApp, SLOT(closeAllWindows()));
  connect(ui.createProjectButton,         SIGNAL(clicked()),         this, SLOT(createProject()));
  connect(ui.programNameLineEdit,         SIGNAL(editingFinished()), this, SLOT(editingFinished()));
  connect(ui.directoryNamePushButton,     SIGNAL(clicked()),         this, SLOT(changeDir()));
}

QatProjectForm::~QatProjectForm() {
}

void QatProjectForm::createProject() {
  
  std::string programName = ui.programNameLineEdit->text().toStdString();
  if (programName=="") {

    QMessageBox::warning(this, tr("qat-project"),
			 tr("You must select a name for your program"),
			 QMessageBox::Ok);

    return;
  }

  std::string directoryName = ui.directoryLabel->text().toStdString();
  if (directoryName=="") {

    QMessageBox::warning(this, tr("qat-project"),
			 tr("You must select a directory for your program"),
			 QMessageBox::Ok);

    return;
  }
  

  bool plotView=ui.plotViewCheckBox->isChecked();
  enum Type {ZeroToZero, OneToZero, NToZero, ZeroToOne, OneToOne, NToOne};

  Type type = ZeroToZero;
  if (ui.zeroToOneRadioButton->isChecked()) type=ZeroToOne;
  if (ui.oneToOneRadioButton->isChecked())  type=OneToOne;
  if (ui.nToOneRadioButton->isChecked())    type=NToOne;
  if (ui.nToZeroRadioButton->isChecked())   type=NToZero;
  if (ui.oneToZeroRadioButton->isChecked()) type=OneToZero;
  

  // Make the directory:
  if (mkdir(directoryName.c_str(), 0755)) {
    QMessageBox::warning(this, tr("qat-project"),
			 QString("Error creating directory ") + QString(directoryName.c_str())+ QString(": ") + QString(strerror(errno)),
			 QMessageBox::Ok);
    
    return;
  }
  mkdir((directoryName+"/src").c_str(),0755);
  //
  int iret=0;  // return code from system.
  //
  std::string shareDirName= STR_NAME(QATSHAREDIR);
  {
    iret=system ((std::string("cp ")+shareDirName+std::string("/qat/templates/qt.pro ") + directoryName+"/src").c_str()); 
    if (iret!=0) throw std::runtime_error("Error installing qt.pro into local directory");
  }
#ifdef __APPLE__
  {
    iret=system (("sed -i .orig s/\"<app>\"/" + programName + "/g " + directoryName + "/src/qt.pro").c_str());
    if (iret!=0) throw std::runtime_error("Error patching qt.pro in local directory");
  }
#else
  {
    iret=system (("sed -i  s/\\<app\\>/" + programName + "/g " + directoryName + "/src/qt.pro").c_str());
    if (iret!=0) throw std::runtime_error("Error patching qt.pro in local directory.");
  }
#endif
  // There are eight templates.  Choose one of them:
  if (plotView) {
    if (type==ZeroToZero) iret=system (("cp "+shareDirName+ "/qat/templates/templateZeroToZeroView.cpp " + directoryName + "/src/" + programName + ".cpp").c_str()); 
    if (type==OneToZero)  iret=system (("cp "+shareDirName+ "/qat/templates/templateOneToZeroView.cpp "  + directoryName + "/src/" + programName + ".cpp").c_str()); 
    if (type==NToZero)    iret=system (("cp "+shareDirName+ "/qat/templates/templateNToZeroView.cpp "    + directoryName + "/src/" + programName + ".cpp").c_str()); 
    if (type==ZeroToOne)  iret=system (("cp "+shareDirName+ "/qat/templates/templateZeroToOneView.cpp "  + directoryName + "/src/" + programName + ".cpp").c_str()); 
    if (type==OneToOne)   iret=system (("cp "+shareDirName+ "/qat/templates/templateOneToOneView.cpp "   + directoryName + "/src/" + programName + ".cpp").c_str()); 
    if (type==NToOne)     iret=system (("cp "+shareDirName+ "/qat/templates/templateNToOneView.cpp "     + directoryName + "/src/" + programName + ".cpp").c_str()); 
  }
  else {
    if (type==ZeroToZero) iret=system (("cp "+shareDirName+ "/qat/templates/templateZeroToZero.cpp "     + directoryName + "/src/" + programName + ".cpp").c_str()); 
    if (type==OneToZero)  iret=system (("cp "+shareDirName+ "/qat/templates/templateOneToZero.cpp "      + directoryName + "/src/" + programName + ".cpp").c_str()); 
    if (type==NToZero)    iret=system (("cp "+shareDirName+ "/qat/templates/templateNToZero.cpp "        + directoryName + "/src/" + programName + ".cpp").c_str()); 
    if (type==ZeroToOne)  iret=system (("cp "+shareDirName+ "/qat/templates/templateZeroToOne.cpp "      + directoryName + "/src/" + programName + ".cpp").c_str()); 
    if (type==OneToOne)   iret=system (("cp "+shareDirName+ "/qat/templates/templateOneToOne.cpp "       + directoryName + "/src/" + programName + ".cpp").c_str()); 
    if (type==NToOne)     iret=system (("cp "+shareDirName+ "/qat/templates/templateNToOne.cpp "         + directoryName + "/src/" + programName + ".cpp").c_str()); 
  }
  if (iret!=0) throw std::runtime_error("Error installing source code template into local directory");

  qApp->closeAllWindows();


}

void QatProjectForm::editingFinished() {
  char BUFF[1024];
  if (!ui.programNameLineEdit->text().isEmpty() && ui.directoryLabel->text().isEmpty()) 
    ui.directoryLabel->setText(QString(getcwd(BUFF,1024))+QString("/")+ui.programNameLineEdit->text().toUpper()); 
}

void QatProjectForm::changeDir() {
  char BUFF[1024];
  QString fileName = QFileDialog::getSaveFileName(this, tr("Set Project Directory"),
						  getcwd(BUFF,1024),
						  QString(""),
						  NULL,
						  QFileDialog::ShowDirsOnly);
  ui.directoryLabel->setText(fileName);
}
