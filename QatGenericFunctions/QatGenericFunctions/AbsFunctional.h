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

// A functional is a mapping from functions to real numbers.
// Here is their base class:
#ifndef _AbsFunctional_h_
#define _AbsFunctional_h_

namespace Genfun {

  class AbsFunction;
  
  class AbsFunctional {

  public:

    // Constructor
    AbsFunctional(); 

    // Destructor:
    virtual ~AbsFunctional();

    // A functional is a mapping from functions to 
    // real numbers.  This defines that mapping:
    virtual double operator() (const AbsFunction & function) const = 0;

  };
} // namespace Genfun
#endif
