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

//--------------------------ConstTimesParameter-----------------------------//
//                                                                          //
// ConstTimesParameter, result of multiplying a parameter and a constant.   //
//                                                                          //
//--------------------------------------------------------------------------//

#ifndef ConstTimesParameter_h
#define ConstTimesParameter_h 1
#include "QatGenericFunctions/AbsParameter.h"

namespace Genfun {

  class ConstTimesParameter : public AbsParameter  {

    PARAMETER_OBJECT_DEF(ConstTimesParameter)
  
      public:
  
    // Constructor
    ConstTimesParameter(double, const AbsParameter *);
  
    // Copy constructor
    ConstTimesParameter(const ConstTimesParameter &right);

    // Destructor
    virtual ~ConstTimesParameter();
  
    // Retreive function value
    virtual double getValue() const;

  private:

    // It is illegal to assign a ConstTimesParameter
    const ConstTimesParameter & operator=(const ConstTimesParameter &right);

    double        _constant;
    AbsParameter *_parameter;
  };
} // namespace Genfun
#endif
