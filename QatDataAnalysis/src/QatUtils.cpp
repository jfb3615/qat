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

#include "QatDataAnalysis/QatUtils.h"
#include "QatDataAnalysis/HistogramManager.h"
#include "QatDataAnalysis/Hist1D.h"
#include "QatDataAnalysis/Hist2D.h"
#include "QatDataAnalysis/Table.h"
void deepCopy(const HistogramManager *input, HistogramManager *output) {
  for (HistogramManager::DConstIterator d=input->beginDir();d!=input->endDir();d++) {
    HistogramManager *newDir=output->newDir((*d)->name());
    deepCopy(*d,newDir);
  }

  for (HistogramManager::H1ConstIterator d=input->beginH1();d!=input->endH1();d++) {
    output->newHist1D(**d);
  }

  for (HistogramManager::H2ConstIterator d=input->beginH2();d!=input->endH2();d++) {
    output->newHist2D(**d);
  }

  for (HistogramManager::TConstIterator d=input->beginTable();d!=input->endTable();d++) {
    output->newTable(**d);
  }

}
