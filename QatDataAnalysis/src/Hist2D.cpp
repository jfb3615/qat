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

#include "QatDataAnalysis/Hist2D.h"
#include <stdexcept>
Hist2D::Hist2D(const std::string & name, size_t nBinsX, double minX, double maxX,
	                                  size_t nBinsY, double minY, double maxY):
  c(new Clockwork)
{
  c->name=name;
  c->nBinsX=nBinsX;
  c->nBinsY=nBinsY;
  c->minX=minX;
  c->maxX=maxX;
  c->minY=minY;
  c->maxY=maxY;
  c->binWidthX=(maxX-minX)/nBinsX;
  c->binWidthY=(maxY-minY)/nBinsY;
  c->overUnderFlow[0][0]=c->overUnderFlow[0][1]=c->overUnderFlow[0][2]=0;
  c->overUnderFlow[1][0]=c->overUnderFlow[1][1]=c->overUnderFlow[1][2]=0;
  c->overUnderFlow[2][0]=c->overUnderFlow[2][1]=c->overUnderFlow[2][2]=0;
  c->overflow=0;
  c->xW=0;
  c->xxW=0; 
  c->yW=0;
  c->yyW=0;
  c->xyW=0;
  c->sumWeight=0;
  c->minContents=0;
  c->maxContents=0;
  c->contents.resize(nBinsX*nBinsY);
  c->errors.resize(nBinsX*nBinsY);
}
Hist2D::Hist2D(size_t nBinsX, double minX, double maxX,
	       size_t nBinsY, double minY, double maxY):
  c(new Clockwork)
{
  c->name="ANONYMOUS";
  c->nBinsX=nBinsX;
  c->nBinsY=nBinsY;
  c->minX=minX;
  c->maxX=maxX;
  c->minY=minY;
  c->maxY=maxY;
  c->binWidthX=(maxX-minX)/nBinsX;
  c->binWidthY=(maxY-minY)/nBinsY;
  c->overUnderFlow[0][0]=c->overUnderFlow[0][1]=c->overUnderFlow[0][2]=0;
  c->overUnderFlow[1][0]=c->overUnderFlow[1][1]=c->overUnderFlow[1][2]=0;
  c->overUnderFlow[2][0]=c->overUnderFlow[2][1]=c->overUnderFlow[2][2]=0;
  c->overflow=0;
  c->xW=0;
  c->xxW=0; 
  c->yW=0;
  c->yyW=0;
  c->xyW=0;
  c->sumWeight=0;
  c->minContents=0;
  c->maxContents=0;
  c->contents.resize(nBinsX*nBinsY);
  c->errors.resize(nBinsX*nBinsY);
}

void Hist2D::clear() {
  c->overUnderFlow[0][0]=c->overUnderFlow[0][1]=c->overUnderFlow[0][2]=0;
  c->overUnderFlow[1][0]=c->overUnderFlow[1][1]=c->overUnderFlow[1][2]=0;
  c->overUnderFlow[2][0]=c->overUnderFlow[2][1]=c->overUnderFlow[2][2]=0;
  c->overflow=0;
  c->xW=0;
  c->xxW=0; 
  c->yW=0;
  c->yyW=0;
  c->xyW=0;
  c->sumWeight=0;
  c->minContents=0;
  c->maxContents=0;
  std::fill(c->contents.begin(),c->contents.end(), 0);
  std::fill(c->errors.begin(),  c->errors.end(),   0);}


Hist2D::~Hist2D() {
  delete c;
}

void Hist2D::accumulate(double x, double y, double weight) {

  int    nx = c->nBinsX, ny=c->nBinsY;

  OVF a,b;
  int binX = int((x-c->minX)/c->binWidthX);
  int binY = int((y-c->minY)/c->binWidthY);
  if (x<c->minX) {
    a=Underflow;
  }
  else if (binX<nx) {
    a=InRange;
  }
  else {
    a=Overflow;
  }
  if (y<c->minY) {
    b=Underflow;
  }
  else if (binY<ny) {
    b=InRange;
  }
  else {
    b=Overflow;
  }
  c->overUnderFlow[a][b]++;

  if (a==InRange && b==InRange) {
    
    c->xW        +=  x*weight;
    c->xxW       += (x*x*weight);
    c->yW        += (y*weight);
    c->yyW       += (y*y*weight);
    c->xyW       += (x*y*weight);
    c->sumWeight += weight;
    c->minContents = 0;
    c->maxContents = 0;
    c->contents[ii(binX,binY)]+= weight;
    c->errors[ii(binX,binY)]  += weight*weight;
  }
}

Hist2D & Hist2D::operator += (const Hist2D & source) {
  if ((c->nBinsX!=source.c->nBinsX) ||
      (c->minX  !=source.c->minX)   ||
      (c->maxX  !=source.c->maxX)   ||
      (c->nBinsY!=source.c->nBinsY) ||
      (c->minY  !=source.c->minY)   ||
      (c->maxY  !=source.c->maxY)) 
    {
      throw (std::runtime_error("Incompatible histograms are added"));
    }
  else 
    {
    c->overflow  += source.c->overflow;
    for (int i=0;i<3;i++) {
      for (int j=0;j<3;j++) {
	c->overUnderFlow[i][j]+=source.c->overUnderFlow[i][j];
      }
    }

    c->xW        += source.c->xW;
    c->yW        += source.c->yW;
    c->xxW       += source.c->xxW;
    c->yyW       += source.c->yyW;
    c->xyW       += source.c->xyW;
    c->sumWeight += source.c->sumWeight;
    c->minContents = 0;
    c->maxContents = 0;
    for (size_t i=0;i<c->nBinsX;i++) {
      for (size_t j=0; j<c->nBinsY;j++) {
	size_t k = ii(i,j);
	c->contents[k] += source.c->contents[k];
	c->errors[k]   += source.c->errors[k];
      }
    }
  }
  return *this;
}

Hist2D & Hist2D::operator -= (const Hist2D & source) {
  if ((c->nBinsX!=source.c->nBinsX) ||
      (c->minX  !=source.c->minX)   ||
      (c->maxX  !=source.c->maxX)   ||
      (c->nBinsY!=source.c->nBinsY) ||
      (c->minY  !=source.c->minY)   ||
      (c->maxY  !=source.c->maxY)) 
    {
      throw (std::runtime_error("Incompatible histograms are subtracted"));
    }
  else 
    {
    c->overflow  += source.c->overflow;
    for (int i=0;i<3;i++) {
      for (int j=0;j<3;j++) {
	c->overUnderFlow[i][j]+=source.c->overUnderFlow[i][j];
      }
    }

    c->xW        -= source.c->xW;
    c->yW        -= source.c->yW;
    c->xxW       -= source.c->xxW;
    c->yyW       -= source.c->yyW;
    c->xyW       -= source.c->xyW;
    c->sumWeight -= source.c->sumWeight;
    c->minContents = 0;
    c->maxContents = 0;
    for (size_t i=0;i<c->nBinsX;i++) {
      for (size_t j=0; j<c->nBinsY;j++) {
	size_t k = ii(i,j);
	c->contents[k] -= source.c->contents[k];
	c->errors[k]   += source.c->errors[k];
      }
    }
  }
  return *this;
}

Hist2D & Hist2D::operator *= (double scale) {

  c->xW        *= scale;
  c->yW        *= scale;
  c->xxW       *= scale;
  c->yyW       *= scale;
  c->xyW       *= scale;
  c->sumWeight *= scale;
  c->minContents = 0;
  c->maxContents = 0;
  for (size_t i=0;i<c->nBinsX;i++) {
    for (size_t j=0; j<c->nBinsY;j++) {
      size_t k = ii(i,j);
      c->contents[k] *= scale;
      c->errors[k]   *= scale;
    }
  }
return *this;
}


// Get the internals:
const Hist2D::Clockwork *Hist2D::clockwork() const {
  return c;
}
  
// Remake this from the interals:
Hist2D::Hist2D(const Clockwork * cw) {
  c  = new Clockwork(*cw);
}

Hist2D::Hist2D( const Hist2D & source) {
  c = new Clockwork(*source.c);
}

Hist2D & Hist2D::operator = (const Hist2D & source ) {
  if (this!=&source) {
    delete c;
    c = new Clockwork(*source.c);
  }
  return *this;
}
