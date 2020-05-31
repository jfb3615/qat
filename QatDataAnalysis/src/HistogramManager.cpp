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

#include "QatDataAnalysis/HistogramManager.h"
#include <algorithm>

HistogramManager::HistogramManager(const std::string & name): _name(name) {
}


const std::string & HistogramManager::name() const { 
  return _name;
}


std::string & HistogramManager::name() { 
  return _name;
}


HistogramManager::H1Iterator HistogramManager::beginH1() {
  return _histograms.begin();
}

HistogramManager::H1Iterator HistogramManager::endH1() {
  return _histograms.end();
}

HistogramManager::H1ConstIterator HistogramManager::beginH1() const{
  return _histograms.begin();
}

HistogramManager::H1ConstIterator HistogramManager::endH1() const {
  return _histograms.end();
}

HistogramManager::H2Iterator HistogramManager::beginH2() {
  return _scatterPlots.begin();
}

HistogramManager::H2Iterator HistogramManager::endH2() {
  return _scatterPlots.end();
}

HistogramManager::H2ConstIterator HistogramManager::beginH2() const{
  return _scatterPlots.begin();
}

HistogramManager::H2ConstIterator HistogramManager::endH2() const {
  return _scatterPlots.end();
}

HistogramManager::DIterator HistogramManager::beginDir() {
  return _managers.begin();
}

HistogramManager::DIterator HistogramManager::endDir() {
  return _managers.end();
}

HistogramManager::DConstIterator HistogramManager::beginDir() const{
  return _managers.begin();
}

HistogramManager::DConstIterator HistogramManager::endDir() const {
  return _managers.end();
}

HistogramManager::TIterator HistogramManager::beginTable() {
  return _tables.begin();
}

HistogramManager::TIterator HistogramManager::endTable() {
  return _tables.end();
}

HistogramManager::TConstIterator HistogramManager::beginTable() const{
  return _tables.begin();
}

HistogramManager::TConstIterator HistogramManager::endTable() const {
  return _tables.end();
}

const Hist1D           *HistogramManager::findHist1D(const std::string & name) const{
  H1ConstIterator i = std::find_if(_histograms.begin(),_histograms.end(), NameEq(name));
  if (i!=_histograms.end()) return *i;
  else return NULL;
}

const Hist2D           *HistogramManager::findHist2D(const std::string & name) const{
  H2ConstIterator i = std::find_if(_scatterPlots.begin(),_scatterPlots.end(), NameEq(name));
  if (i!=_scatterPlots.end()) return *i;
  else return NULL;
}

const HistogramManager           *HistogramManager::findDir(const std::string & name) const {
  DConstIterator i = std::find_if(_managers.begin(),_managers.end(), NameEq(name));
  if (i!=_managers.end()) return *i;
  else return NULL;
}

const Table           *HistogramManager::findTable(const std::string & name) const {
  TConstIterator i = std::find_if(_tables.begin(),_tables.end(), NameEq(name));
  if (i!=_tables.end()) return *i;
  else return NULL;
}




Hist1D *HistogramManager::newHist1D(const std::string & name, unsigned int nBins, double min, double max) {
  purge(name);
  Hist1D * newHist = new Hist1D(name,nBins,min, max);
  _histograms.push_back(newHist);
  _histograms.sort(SortByName());
  return newHist;
}

Hist1D *HistogramManager::newHist1D(const Hist1D & source) {
  purge(source.name());
  Hist1D * newHist = new Hist1D(source);
  _histograms.push_back(newHist);
  _histograms.sort(SortByName());
  return newHist;
}

Table *HistogramManager::newTable(const Table & source) {


  purge(source.name());
  Table * newTable = new Table(source);
  _tables.push_back(newTable);
  _tables.sort(SortByName());
  return newTable;
}

Hist2D *HistogramManager::newHist2D(const std::string & name, unsigned int nBinsX, double minX, double maxX,
				                            unsigned int nBinsY, double minY, double maxY) {

  purge(name);
  Hist2D *newHist = new Hist2D(name,nBinsX,minX, maxX, nBinsY, minY, maxY);
  _scatterPlots.push_back(newHist);
  _scatterPlots.sort(SortByName());
  return newHist;
}

Hist2D *HistogramManager::newHist2D(const Hist2D & source) {
  purge(source.name());
  Hist2D * newHist = new Hist2D(source);
  _scatterPlots.push_back(newHist);
  _scatterPlots.sort(SortByName());
  return newHist;
}




Table           *HistogramManager::newTable(const std::string & name, TupleStoreLink store) {
  purge (name);
  Table *t = store ? new Table(name,store): new Table(name);
  _tables.push_back(t);
  _tables.sort(SortByName());
  return t;
}


HistogramManager *HistogramManager::newDir(const std::string & name) {
  purge (name);
  HistogramManager *t = new HistogramManager(name);
  _managers.push_back(t);
  _managers.sort(SortByName());
  return t;
}


void HistogramManager::nameOrder() {
  // Sort tables: Other objects already ordered.
  _tables.sort(SortByName());
}

void HistogramManager::purge(const std::string & name) {

  H1Iterator doomedHistogram = std::find_if(_histograms.begin(),_histograms.end(), NameEq(name));
  if (doomedHistogram!=_histograms.end()) {
    delete *doomedHistogram;
    _histograms.erase(doomedHistogram);
  }
  
  H2Iterator doomedScatterPlot = std::find_if(_scatterPlots.begin(),_scatterPlots.end(), NameEq(name));
  if (doomedScatterPlot!=_scatterPlots.end()) {
    delete *doomedScatterPlot;
    _scatterPlots.erase(doomedScatterPlot);
  }
  
  TIterator doomedTable = std::find_if(_tables.begin(),_tables.end(), NameEq(name));
  if (doomedTable!=_tables.end()) {
    delete *doomedTable;
    _tables.erase(doomedTable);
  }
  
  DIterator doomedDir = std::find_if(_managers.begin(),_managers.end(), NameEq(name));
  if (doomedDir!=_managers.end()) {
    delete *doomedDir;
    _managers.erase(doomedDir);
  }
}



HistogramManager::~HistogramManager() {
  for (H1Iterator i=_histograms.begin();   i!=_histograms.end();  i++)   delete *i;
  for (DIterator  i=_managers.begin();     i!=_managers.end();    i++)   delete *i;
  for (H2Iterator i=_scatterPlots.begin(); i!=_scatterPlots.end();i++)   delete *i;
  for (TIterator  i=_tables.begin();       i!=_tables.end();      i++)   delete *i;
}
