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

// -------------------------Tuple-------------------------------//
//                                                              //
// This class represents a single row of a table. It can hold   //
// a variety of different data types and provides access to     //
// them.  Tables build tuples and monger tables.  The           //
// way in which you get data out of a data is to call one of    //
// the overloaded read methods.  You can then check the status  //
// to see whether the read succeeded:                           //
//                                                              //
//     int value;                                               //
//     if (tuple->read(value,i)) {                              //
//       ..                                                     //
//     }                                                        //
//                                                              //
// If the data type of datum i is wrong, the read will fail.    //
// This provides a way of detecting data type, which is not     //
// really recommended for repeated accesses if speed is an      //
// If speed is an issue, the datatype should be determined      //
// (say, from the table) and the fastread method should be      //
// used.                                                        //
//                                                              //
//     int value                                                //
//     switch (attribute(i).type()) {                           //             
//       case Attribute::DOUBLE:                                //
//         {                                                    //
//           tuple->fastread(value,i);                          //
//           break;                                             //
//         }                                                    //
//                                                              //
// And then there is a third way to access the information for  //
// very fast operations on double precision representations of  //
// data (when they exist):                                      //
//                                                              //
//     const Genfun::Argument & a = tuple->asDoublePrec();      //
//     double x = a[i];                                         //
//                                                              //
//--------------------------------------------------------------//

#ifndef _Tuple_h_
#define _Tuple_h_
#include "QatDataAnalysis/RCSBase.h"
#include "QatGenericFunctions/AbsFunction.h"
#include "QatDataAnalysis/AttributeListConstLink.h"
#include "QatDataAnalysis/ValueList.h"
#include <iostream>
#include <vector>
#include <set>


class Tuple:public RCSBase {

 public:
  

  // Constructors
  Tuple(AttributeListConstLink);

  // Constructors
  Tuple(AttributeListConstLink, const ValueList &);

  // Access to the attributeList (where header info lives).:
  AttributeListConstLink attributeList() const {return _attributeList;}

  // Print method:
  std::ostream & print (std::ostream & o= std::cout) const;

  // Check status of last operation:
  operator bool () const { return _status;}

  // Read in an integer:
  inline const Tuple & read   (int    & i, unsigned int pos) const;
  inline void fastread        (int    & i, unsigned int pos) const;

  // Read an unsigned int:
  inline const Tuple & read   (unsigned int & i, unsigned int pos) const;
  inline void fastread        (unsigned int & i, unsigned int pos) const;

  // Read a double:
  inline const Tuple & read   (double & d, unsigned int pos) const;
  inline void fastread        (double & d, unsigned int pos) const;

  // Read a float:
  inline const Tuple & read   (float  & f, unsigned int pos) const;
  inline void fastread        (float  & f, unsigned int pos) const;

  // Access to values:
  inline ValueList & valueList();
  inline const ValueList & valueList() const;

  // Access to a double precision rep of all quantities:
  const Genfun::Argument & asDoublePrec() const;

  // Uncache.
  virtual void uncache() const;

 private:
  
  // Destructor
  virtual ~Tuple();

  // Copy Constructor:
  Tuple (const Tuple & tuple);

  // Assignment Operator:
  Tuple & operator=(const Tuple & tuple);


  AttributeListConstLink     _attributeList;
  mutable ValueList         _valList;
  //Maybe:
  //IOAddress                 *_io;

  mutable bool              _status;
  mutable Genfun::Argument *_doubleRep;
  mutable bool              _doubleRepStale;

};

#include "QatDataAnalysis/Tuple.icc"
#endif
