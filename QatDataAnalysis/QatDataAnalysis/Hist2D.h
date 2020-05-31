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

#ifndef _HIST2D_H_
#define _HIST2D_H_
#include <string>
#include <vector>
#include <cmath>
class Hist2D {

 public:


  // Define a type describing the kind of overflow:
  enum OVF { Overflow, InRange, Underflow};

  // Constructor:
  Hist2D(size_t nBinsX, double minX, double maxX,
	 size_t nBinsY, double minY, double maxY);

  // Constructor:
  Hist2D(const std::string & name, 
	 size_t nBinsX, double minX, double maxX,
	 size_t nBinsY, double minY, double maxY);

  // Copy constructor:
  Hist2D (const Hist2D & source);

  // Destructor
  ~Hist2D();

  // Accumulate Data:
  void accumulate(double x, double y, double weight=1);

  // Properties of the container:
  inline std::string & name();
  inline const std::string & name()  const;
  inline size_t nBinsX() const;
  inline size_t nBinsY() const;
  inline double minX() const;
  inline double maxX() const;
  inline double minY() const;
  inline double maxY() const;
  inline double binWidthX() const;
  inline double binWidthY() const;

  // Properties of the bins:
  inline double binUpperEdgeX(unsigned int i, unsigned int j) const;
  inline double binLowerEdgeX(unsigned int i, unsigned int j) const;
  inline double binUpperEdgeY(unsigned int i, unsigned int j) const;
  inline double binLowerEdgeY(unsigned int i, unsigned int j) const;
  inline double binCenterX(unsigned int i,    unsigned int j) const;
  inline double binCenterY(unsigned int i,    unsigned int j) const;

  // Stored data:
  inline double bin(unsigned int i, unsigned int j) const;
  inline double binError(unsigned int i, unsigned int j) const;
  inline size_t overflow(OVF a, OVF b) const;
  inline size_t overflow() const;  
  inline size_t entries() const;

  // Statistical properties of the data:
  inline double varianceX() const;
  inline double varianceY() const;
  inline double varianceXY() const;
  inline double meanX() const;
  inline double meanY() const;
  inline double minContents() const;
  inline double maxContents() const;
  inline double sum() const;

  // Operations
  Hist2D & operator  = (const Hist2D & source);
  Hist2D & operator += (const Hist2D & source);
  Hist2D & operator -= (const Hist2D & source);
  Hist2D & operator *= (double scale);
  
  // Clear
  void clear();

  // For accessing bins:
  inline size_t       ii(size_t i, size_t j) const;

  // The internals are described like this:
  class Clockwork; 
  
  // Get the internals:
  const Clockwork *clockwork() const;
  
  // Remake this from the internals:
  Hist2D(const Clockwork * c);

 private:

  Clockwork           *c;
 

};

#include "QatDataAnalysis/Hist2D.icc"

#endif

