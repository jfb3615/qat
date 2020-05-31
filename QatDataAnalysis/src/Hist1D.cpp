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

#include "QatDataAnalysis/Hist1D.h"
#include <stdexcept>
Hist1D::Hist1D(const std::string & name, size_t nBins, double min, double max):
  c(new Clockwork()) 
{
  c->name=name;
  c->nBins=nBins;
  c->min=min;
  c->max=max;
  c->binWidth=(max-min)/nBins;
  c->overflow=0;
  c->underflow=0;
  c->entries=0;
  c->xW=0;
  c->xxW=0;
  c->sumWeight=0;
  c->minContents=0;
  c->maxContents=0;
  c->contents.resize(nBins);
  c->errors.resize(nBins);
}

Hist1D::Hist1D(size_t nBins, double min, double max):
  c(new Clockwork()) 
{
  c->name="ANONYMOUS";
  c->nBins=nBins;
  c->min=min;
  c->max=max;
  c->binWidth=(max-min)/nBins;
  c->overflow=0;
  c->underflow=0;
  c->entries=0;
  c->xW=0;
  c->xxW=0;
  c->sumWeight=0;
  c->minContents=0;
  c->maxContents=0;
  c->contents.resize(nBins);
  c->errors.resize(nBins);
}

Hist1D::~Hist1D() {
  delete c;
}

void Hist1D::accumulate(double x, double weight) {
  int bin = int((x-c->min)/c->binWidth);
  if (x<c->min) {
    c->underflow++;
  }
  else if (x>=c->max) {
    c->overflow++;
  }
  else {
    c->entries++;
    c->xW        +=  x*weight;
    c->xxW       += (x*x*weight);
    c->sumWeight += weight;
    c->minContents = 0;
    c->maxContents = 0;
    c->contents[bin]+= weight;
    c->errors[bin]  += weight*weight;
  }
}

Hist1D & Hist1D::operator += (const Hist1D & source) {
  if ((c->nBins!=source.c->nBins) ||
      (c->min  !=source.c->min)   ||
      (c->max  !=source.c->max)   ) {
    throw (std::runtime_error("Incompatible histograms are added"));
  }
  else {
    c->overflow  += source.c->overflow;
    c->underflow += source.c->underflow;
    c->entries   += source.c->entries;
    c->xW        += source.c->xW;
    c->xxW       += source.c->xxW;
    c->sumWeight += source.c->sumWeight;
    c->minContents = 0;
    c->maxContents = 0;
    for (unsigned int i=0;i<c->nBins;i++) {
      c->contents[i] += source.c->contents[i];
      c->errors[i]   += source.c->errors[i];
    }
  }

  return *this;
}

Hist1D & Hist1D::operator -= (const Hist1D & source) {
  if ((c->nBins!=source.c->nBins) ||
      (c->min  !=source.c->min)   ||
      (c->max  !=source.c->max)   ) {
    throw (std::runtime_error("Incompatible histograms are added"));
  }
  else {
    c->overflow  += source.c->overflow;
    c->underflow += source.c->underflow;
    c->entries   += source.c->entries;
    c->xW        -= source.c->xW;
    c->xxW       -= source.c->xxW;
    c->sumWeight -= source.c->sumWeight;
    c->minContents = 0;
    c->maxContents = 0;
    for (unsigned int i=0;i<c->nBins;i++) {
      c->contents[i] -= source.c->contents[i];
      c->errors[i]   += source.c->errors[i];
    }
  }
  return *this;
}

Hist1D & Hist1D::operator *= (double scale) {
  c->xW        *= scale;
  c->xxW       *= scale;
  c->sumWeight *= scale;
  c->minContents = 0;
  c->maxContents = 0;
  for (unsigned int i=0;i<c->nBins;i++) {
    c->contents[i] *= scale;
    c->errors[i]   *= (scale*scale);
  }
  return *this;
}

void Hist1D::clear() {

  c->overflow    = 0;
  c->underflow   = 0;
  c->entries     = 0;
  c->xW          = 0;
  c->xxW         = 0;
  c->sumWeight   = 0;
  c->minContents = 0;
  c->maxContents = 0;
  std::fill(c->contents.begin(),c->contents.end(), 0);
  std::fill(c->errors.begin(),  c->errors.end(),   0);
  
}

// Get the internals:
const Hist1D::Clockwork *Hist1D::clockwork() const {
  return c;
}
  
// Remake this from the interals:
Hist1D::Hist1D(const Clockwork * cw) {
  c  = new Clockwork(*cw);
}

Hist1D::Hist1D( const Hist1D & source) {
  c = new Clockwork(*source.c);
}

Hist1D & Hist1D::operator = (const Hist1D & source ) {
  if (this!=&source) {
    delete c;
    c = new Clockwork(*source.c);
  }
  return *this;
}
