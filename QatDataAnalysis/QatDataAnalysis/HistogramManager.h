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

#ifndef _HISTOGRAM_MANAGER_H
#define _HISTOGRAM_MANAGER_H
// ---------------------------------------------------//
//                                                    //
// HistogramManager                                   //
//                                                    //
// This class collects and accesses Histograms,       //
// ScatterPlots, and Tables.                          //
//                                                    //
//                                                    //
//----------------------------------------------------//

#include "QatDataAnalysis/Hist1D.h"
#include "QatDataAnalysis/Hist2D.h"
#include "QatDataAnalysis/Table.h"
#include "QatDataAnalysis/TupleStore.h"
#include <list>
#include <string>

class HistogramManager {

public:

  //------------------------Constructor and Destructor-----------------------------------//
  // Constructor:                                                                        //
  HistogramManager(const std::string & name="ANONYMOUS");                                //
  //                                                                                     //
  // Destructor:                                                                         //
  ~HistogramManager();                                                                   //
  //                                                                                     //
  // Get name:                                                                           //
  std::string & name();                                                                  //
  const std::string & name() const;                                                      //
  //                                                                                     //
  //-------------------------------------------------------------------------------------//

  //------------------------Definitions:-------------------------------------------------//
  //                                                                                     //
  typedef std::list<Hist1D *>::iterator                  H1Iterator;                     //
  typedef std::list<Hist1D *>::const_iterator            H1ConstIterator;                //
  //                                                                                     //
  typedef std::list<Hist2D *>::iterator                  H2Iterator;                     //
  typedef std::list<Hist2D *>::const_iterator            H2ConstIterator;                //
  //                                                                                     //
  typedef std::list<Table *>::iterator                   TIterator;                      //
  typedef std::list<Table *>::const_iterator             TConstIterator;                 //
  //                                                                                     //
  typedef std::list<HistogramManager *>::iterator        DIterator;                      //
  typedef std::list<HistogramManager *>::const_iterator  DConstIterator;                 //
  //                                                                                     //
  //-------------------------------------------------------------------------------------//

  //-----------------------Predicates, Unary and Binary----------------------------------//
  class NameEq {                                                                         //
    //                                                                                   // 
  public:                                                                                //
    //                                                                                   //
    // Constructor:                                                                      //
    NameEq(const std::string & name):_name(name){};                                      //
    //                                                                                   //
    // Truth operators:                                                                  //
    inline bool operator () ( Hist1D * hist ) {return  hist->name() ==_name;}            //
    inline bool operator () ( Hist2D * hist ) {return  hist->name() ==_name;}            //
    inline bool operator () ( Table  * table) {return  table->name() ==_name;}           //
    inline bool operator () ( HistogramManager *m) {return  m->name()==_name;}           //
  private:                                                                               //
    const std::string _name;                                                             //
  };                                                                                     //
  //                                                                                     //
  class SortByName {                                                                     //
    //                                                                                   //
  public:                                                                                //
    //                                                                                   //
    bool operator () (Hist1D *a, Hist1D *b) const {return a->name() > b->name(); }       //
    bool operator () (Hist2D *a, Hist2D *b) const {return a->name() > b->name(); }       //
    bool operator () (Table  *a, Table *b)  const {return a->name() > b->name(); }       //
    bool operator () (HistogramManager  *a, HistogramManager *b)  const {                //
      return a->name() > b->name();                                                      //
    }                                                                                    //
    //                                                                                   //
  };                                                                                     //
  //-------------------------------------------------------------------------------------//

  
  //-----------------------Iteration:----------------------------------------------------//
  //                                                                                     //
  TIterator beginTable();                                                                //
  TConstIterator beginTable() const;                                                     //
  //                                                                                     //
  TIterator endTable();                                                                  //
  TConstIterator endTable() const;                                                       //
  //                                                                                     //
  H1Iterator beginH1();                                                                  //
  H1ConstIterator beginH1() const;                                                       //
  //                                                                                     //
  H1Iterator endH1();                                                                    //
  H1ConstIterator endH1() const;                                                         //
  //                                                                                     //
  H2Iterator beginH2();                                                                  //
  H2ConstIterator beginH2() const;                                                       //
  //                                                                                     //
  H2Iterator endH2();                                                                    //
  H2ConstIterator endH2() const;                                                         //
  //                                                                                     //
  DIterator beginDir();                                                                  //
  DConstIterator beginDir() const;                                                       //
  //                                                                                     //
  DIterator endDir();                                                                    //
  DConstIterator endDir() const;                                                         //
  //                                                                                     //
  //-------------------------------------------------------------------------------------//

  //----------------------Find existing objects.-----------------------------------------//
  //                                                                                     //
  const Hist1D           *findHist1D(const std::string &) const;                         //
  const Hist2D           *findHist2D(const std::string &) const;                         // 
  const Table            *findTable (const std::string &) const;                         //
  const HistogramManager *findDir   (const std::string &) const;                         //
  //                                                                                     //
  //-------------------------------------------------------------------------------------//

  //-----------------------Create new objects.  Overwrite any existing:------------------//
  //                                                                                     //
  Hist1D *newHist1D(const std::string &, unsigned int nBins,  double min, double max);   //
  Hist1D *newHist1D(const Hist1D & source);                                              //
  //                                                                                     //
  Hist2D *newHist2D(const std::string &,                                                 //
		    unsigned int nBinsX, double minX, double maxX,                       //
		    unsigned int nBinsY, double minY, double maxY);                      //
  Hist2D *newHist2D(const Hist2D & source);                                              //
  //                                                                                     //
  Table  *newTable (const std::string &,                                                 //
		    TupleStoreLink store = NULL);                                        // 
  Table  *newTable (const Table & source);                                               //
  //                                                                                     //
  HistogramManager *newDir(const std::string & name);                                    //
  //-------------------------------------------------------------------------------------//

  //---------------------Remove objects--------------------------------------------------//
  //                                                                                     //
  void purge (const std::string & name);                                                 //
  void rm (Hist1D *o)           { purge (o->name());}                                    //
  void rm (Hist2D *o)           { purge (o->name());}                                    //
  void rm (Table  *o)           { purge (o->name());}                                    //               
  void rm (HistogramManager *o) { purge (o->name());}                                    //
  //                                                                                     //
  //-------------------------------------------------------------------------------------//

  //---------------------Sorting operations----------------------------------------------//
  //                                                                                     //
  void nameOrder();                                                                      //
  //                                                                                     //
  //-------------------------------------------------------------------------------------//
private:

  std::string                          _name;
  std::list<Hist1D *>                  _histograms;
  std::list<Hist2D *>                  _scatterPlots;
  std::list<Table  *>                  _tables;
  std::list<HistogramManager *>        _managers;

  // Do not you dare copy or assign a HistogramManager:
  
  HistogramManager(const HistogramManager &);
  HistogramManager & operator = (const HistogramManager &);

}; 

#endif

