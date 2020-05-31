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

//--------------------------ParameterComposition----------------------------//
//                                                                          //
// ParameterComposition, result of composition of two parameters            //
//                                                                          //
//--------------------------------------------------------------------------//

#ifndef ParameterComposition_hh
#define ParameterComposition_hh 
#include "QatGenericFunctions/AbsFunction.h"
#include "QatGenericFunctions/AbsParameter.h"

namespace Genfun {

  class ParameterComposition : public AbsParameter {

    PARAMETER_OBJECT_DEF(ParameterComposition)

      public:

    // Constructor
    ParameterComposition(const AbsFunction *arg1, const AbsParameter *arg2);

    // Copy constructor
    ParameterComposition(const ParameterComposition &right);
  
    // Destructor
    virtual ~ParameterComposition();

    // Retreive parameter value
    virtual double getValue() const;


  private:

    // It is illegal to assign a ParameterComposition
    const ParameterComposition & operator=(const ParameterComposition &right);

    const AbsFunction  *_arg1;
    AbsParameter *_arg2;  
  };
} // namespace Genfun
#endif
