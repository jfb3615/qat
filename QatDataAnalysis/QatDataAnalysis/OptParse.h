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

#ifndef _OPTPARSE_H_
#define _OPTPARSE_H_
class HistogramManager;
#include <string>
#include <vector>


// 
struct HIOZeroToOne {

  //---------------Simple Struct, few public variables--------------//
  HistogramManager *output;                                         //
  bool              verbose;                                        //
  void optParse(int & argc, char ** & argv);                        //
  //---------------End importatant part-----------------------------//

  HIOZeroToOne(const std::string & driver="");
  ~HIOZeroToOne();


  private:

  HIOZeroToOne & operator =( const HIOZeroToOne &);
  HIOZeroToOne(const HIOZeroToOne &);
  class Clockwork;
  Clockwork *c;

};

// 
struct HIOOneToOne {

  //---------------Simple Struct, few public variables--------------//
  const HistogramManager *input;                                    //
  HistogramManager *output;                                         //
  bool              verbose;                                        //
  void optParse(int & argc, char ** & argv);                        //
  //---------------End importatant part-----------------------------//

  HIOOneToOne(const std::string & driver="");
  ~HIOOneToOne();


  private:

  HIOOneToOne & operator =( const HIOOneToOne &);
  HIOOneToOne(const HIOOneToOne &);
  class Clockwork;
  Clockwork *c;

};

// 
struct HIOOneToZero {

  //---------------Simple Struct, few public variables--------------//
  const HistogramManager *input;                                    //
  bool              verbose;                                        //
  void optParse(int & argc, char ** & argv);                        //
  //---------------End importatant part-----------------------------//

  HIOOneToZero(const std::string & driver="");
  ~HIOOneToZero();


  private:

  HIOOneToZero & operator =( const HIOOneToZero &);
  HIOOneToZero(const HIOOneToZero &);
  class Clockwork;
  Clockwork *c;

};

// 
struct HIONToOne {

  //---------------Simple Struct, few public variables--------------//
  std::vector<const HistogramManager *> input;                      //
  HistogramManager *output;                                         //
  bool              verbose;                                        //
  void optParse(int & argc, char ** & argv);                        //
  //---------------End importatant part-----------------------------//

  HIONToOne(const std::string & driver="");
  ~HIONToOne();


  private:

  HIONToOne & operator =( const HIONToOne &);
  HIONToOne(const HIONToOne &);

  class Clockwork;
  Clockwork *c;

};
// 
struct HIONToZero {

  //---------------Simple Struct, few public variables--------------//
  std::vector<const HistogramManager *> input;                      //
  bool              verbose;                                        //
  void optParse(int & argc, char ** & argv);                        //
  //---------------End importatant part-----------------------------//

  HIONToZero(const std::string & driver="");
  ~HIONToZero();


  private:

  HIONToZero & operator =( const HIONToZero &);
  HIONToZero(const HIONToZero &);
  class Clockwork;
  Clockwork *c;

};


class NumericInput {
  
 public:

  NumericInput();
  ~NumericInput();

  // Declare a parameter (name, doc, value)
  template <typename T>
  void declare(const std::string & name, const std::string & doc, T val);

  
  // Then override with user input:
  void optParse(int & argc, char ** & argv);

  // Print the list of parameters:
  std::string usage() const;

  // Get the value of a parameter, by name:
  template <typename T=double>
  T getByName  (const std::string & name) const;

 private:
 
  NumericInput & operator =( const NumericInput &);
  NumericInput(const NumericInput &);

  class Clockwork;
  Clockwork *c;

};


#include "QatDataAnalysis/OptParse.icc"

#endif
