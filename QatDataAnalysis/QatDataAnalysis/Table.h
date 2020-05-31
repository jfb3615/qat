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

#ifndef _TABLE_H_
#define _TABLE_H_
#include "QatGenericFunctions/Variable.h"
#include "QatDataAnalysis/TupleStore.h"
#include <vector>
#include <list>
#include <set>
#include <iostream>
class Tuple;
class Attribute;
class AttributeList;
class Table {

 public:


  // Constructor:
  Table(const std::string & name);

  // Copy Constructor:
  Table(const Table & t);

  // Construct from a store (not for casual users)
  Table(const std::string & name, const TupleStoreLink s);

  // Destructor:
  virtual ~Table();

  // Get the title:
  std::string & name();
  const std::string & name() const;

  // Getting the data into the Table.  Sets values and builds header at same time.
  template <typename T> void add (const std::string & name, const T & t);
  template <typename T> void add (const Attribute & a,      const T & t); 
 
  // Capture:
  virtual TupleConstLink capture();

  // Get the size of the table (number of tuples):
  virtual size_t numTuples() const;

  // Getting the data out of the Table:
  virtual TupleConstLink operator [] (size_t index) const;

  // Alternate.  Convenient but slow.
  template <typename T> void read (size_t index, const std::string & aname, T & t) const;

  // Print:
  std::ostream &  print (std::ostream & o= std::cout) const;

  // Get the size of the table (number of attributes):
  size_t numAttributes() const;

  // Get Attributes:
  const Attribute     & attribute(unsigned int i) const;
  const Attribute     & attribute(const std::string & name) const;
  AttributeListConstLink attributeList() const;

  //  Get the attribute symbol.  For symbolic expressions:
  Genfun::Variable symbol (const std::string & name) const;

  // Operations:
  void operator += (const Table &);

  // Obtain the store (not for casual users).
  TupleStoreConstLink store() const;

  // Copy the Table (which may be disk-resident) to a simple memory
  // resident version:

  Table *memorize() const;

 private:
  
  // These operations are illegal:
  Table & operator=(const Table & t);

  // Keep these details out of the interface:
  class Clockwork;
  Clockwork *c;

};



#include "QatDataAnalysis/Table.icc"
#endif
