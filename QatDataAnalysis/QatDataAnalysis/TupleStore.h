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

#ifndef _TupleStore_h_
#define _TupleStore_h_
#include "QatDataAnalysis/RCBase.h"
#include "QatDataAnalysis/TupleConstLink.h"
#include "QatDataAnalysis/TupleLink.h"

class TupleStore : public Genfun::RCBase {


public:

  // Constructor:
  TupleStore();

  // Read
  virtual TupleConstLink operator [](size_t i) const = 0;

  // Write (if implemented)
  virtual void push_back(TupleConstLink) {
    throw std::runtime_error ("Write only tuple store");
  }

  // Get the size:
  virtual size_t size() const = 0;

  // lock
  void lock() const;

  // check lock:
  bool isLocked() const;

protected:

  // Destructor:
  virtual ~TupleStore();

  // Data:
  mutable bool _lock;

  // Silence compiler warnings about solitude
  friend class ImaginaryFriend;


};

typedef Link< TupleStore  > TupleStoreLink;
typedef ConstLink< TupleStore  > TupleStoreConstLink;

class SimpleStore: public TupleStore {

 public:

  // Constructor
  SimpleStore();
  
  // Read
  virtual TupleConstLink operator [](size_t i) const;

  // Write
  virtual void push_back(TupleConstLink);

  // Get the size:
  virtual size_t size() const;

 private:
  
  // Destructor:
  ~SimpleStore();

  // Illegal Operations:
  SimpleStore(const SimpleStore &);
  SimpleStore & operator = (const SimpleStore & );
  
  // Data:
  std::vector<TupleConstLink> _tVector;

  // Silence compiler warnings about solitude
  friend class ImaginaryFriend;

};


class CompoundStore: public TupleStore {
 public:

  // Constructor
  CompoundStore();
  
  // Read
  virtual TupleConstLink operator [](size_t i) const;

  // Extend
  virtual void push_back(TupleStoreLink);

  // Write
  virtual void push_back(TupleConstLink);

  // Get the size:
  virtual size_t size() const;

 private:
  
  // Destructor:
  ~CompoundStore();

  // Illegal Operations:
  CompoundStore(const CompoundStore &);
  CompoundStore & operator = (const CompoundStore & );
  

  // Data:
  std::vector<TupleStoreLink> _sVector;
  
  // Silence compiler warnings about solitude
  friend class ImaginaryFriend;

};


#endif

