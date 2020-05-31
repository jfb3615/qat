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

// --------------------------------------------------------------------//
//                                                                     //
// A function of more than one variable uses this argument class to    //
// agglomerate the variables. It is similar to a vector.               // 
//                                                                     //
//---------------------------------------------------------------------//
#ifndef __ARGUMENT_H_
#define __ARGUMENT_H_
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <initializer_list>

namespace Genfun {

  class Argument {

  public:

    // Constructor
    Argument(int ndim=0);

    // Copy Constructor
    Argument( const Argument &);

    // Construct from initializer list:
    Argument(std::initializer_list<double> );

    // Assignment operator
    const Argument & operator=(const Argument &);

    // Destructor:
    ~Argument();

    // Set/Get Value
    double & operator[] (int I);
    const double & operator[] (int i) const; 

    // Get the dimensions
    unsigned int dimension() const;

  private:

    std::vector<double> *_data;

    friend std::ostream & operator << (std::ostream & o, const Argument & a);

  };

  inline Argument::Argument(const Argument & right):
    _data(new std::vector<double>(*(right._data))){
  }

  inline const Argument & Argument::operator=( const Argument & right) {
    if (this != &right) {
      delete _data;
      _data=NULL;
      _data = new std::vector<double>(*(right._data));
    }
    return *this;
  }

  inline unsigned int Argument::dimension() const {
    return _data->size();
  }

  inline double & Argument::operator[] (int i) {
    return (*_data)[i];
  } 

  inline const double & Argument::operator[] (int i) const {
    return (*_data)[i];
  } 

  inline Argument::Argument(int ndim): _data(new std::vector<double>(ndim)) {
  }

  inline Argument::~Argument() {
    delete _data;
  }

  // Construct from initializer list:
  inline Argument::Argument(std::initializer_list<double> lst):_data(new std::vector<double>(lst)) {
  }


  inline std::ostream & operator << (std::ostream & os, const Argument & a) {
    std::ostream_iterator<double> oi(os,",");
    std::copy (a._data->begin(),a._data->end(),oi);
    return os;
  }


} // namespace Genfun
#endif
