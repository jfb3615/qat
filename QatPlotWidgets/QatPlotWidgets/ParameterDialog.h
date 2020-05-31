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

#ifndef _ParameterDialog_h_
#define _ParameterDialog_h_
#include "QatGenericFunctions/Parameter.h"
#include <QDialog>
class ParameterDialog:public QDialog {

Q_OBJECT

  public:

  // Constructor:
  ParameterDialog(QWidget *parent, Genfun::Parameter *par);
  
  // Destructor:
  virtual ~ParameterDialog();


 public slots:
  
  void updateParameter(int val); // val from 0 to 99

 signals:
  
  void updateFunction();

 private:

  // Illegal operations:
  ParameterDialog(const ParameterDialog &);
  ParameterDialog & operator=(const ParameterDialog &);

  // Internals:
  class Clockwork;
  Clockwork *c;

};

#endif
