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

#include "QatDataAnalysis/TupleStore.h"
#include "QatDataAnalysis/Link.h"
#include <stdexcept>

template class Link<TupleStore>;

TupleStore::TupleStore(): _lock(false) {
}
TupleStore::~TupleStore() {
}
void TupleStore::lock() const {
  _lock=true;
}
bool TupleStore::isLocked() const {
  return _lock;
}

SimpleStore::SimpleStore(){
}

SimpleStore::~SimpleStore() {
}


TupleConstLink SimpleStore::operator [](size_t i) const {
  return _tVector[i];
}

void SimpleStore::push_back(TupleConstLink t) {

  if (isLocked()) {
    throw std::runtime_error("Attempt to write to a locked tuple store!");
  }
  else {
    _tVector.push_back(t);
  }
}

size_t SimpleStore::size() const {
  return _tVector.size();
}


CompoundStore::CompoundStore(){
}

CompoundStore::~CompoundStore() {
}


TupleConstLink CompoundStore::operator [](size_t i) const {
  unsigned int V=0;
  for (unsigned int v=0;v<_sVector.size();v++) {
    if ((i-V)<(*_sVector[v]).size()) {
      return (*_sVector[v])[i-V];
    }
    else {
      V+=(*_sVector[v]).size();
    }
  }
  return TupleConstLink();
}

void CompoundStore::push_back(TupleConstLink t) {

  if (isLocked()) {
    throw std::runtime_error("Attempt to write to a locked tuple store!");
  }
  else {
    SimpleStore *s = _sVector.empty() ? 0 : dynamic_cast<SimpleStore *> (_sVector.back().operator ->()) ;
    if (!s) {
      if (!_sVector.empty()) (*_sVector.back()).lock();
      s = new SimpleStore();
      TupleStoreLink sLink(s);
      _sVector.push_back(sLink);      
    }
    s->push_back(t);
  }
}
void CompoundStore::push_back(TupleStoreLink t) {
  if (isLocked()) {
    throw std::runtime_error("Attempt to write to a locked tuple store!");
  }
  else if (t.operator->()==this) {
    throw std::runtime_error("Attempt to add a compound store to itself");
  }
  else {
    // Lock the store being added.  No more modification possible.
    t->lock();
    
    // Lock also the last tupleStore, if it exists.
    if (!_sVector.empty()) (*_sVector.back()).lock();
    
     _sVector.push_back(t);
  }
}

size_t CompoundStore::size() const {
  unsigned int total(0);

  for (unsigned int i=0;i<_sVector.size();i++) {
    total+=(*_sVector[i]).size();
  }
  return total;
}
