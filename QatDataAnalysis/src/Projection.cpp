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

#include "QatDataAnalysis/Projection.h"
#include "QatDataAnalysis/AttributeList.h"
#include "QatDataAnalysis/AttributeListLink.h"
#include "QatDataAnalysis/Table.h"
#include <sstream>
#include <typeinfo>


class ProjectionStore: public TupleStore {

public:
  
  // Constructor:
  ProjectionStore(TupleStoreConstLink StoreA, AttributeListConstLink BList, const std::vector<int> & Dist):
    opA(StoreA),bList(BList),ssize(-1),dist(Dist){

  }

  // For input:
  virtual TupleConstLink operator [](size_t i) const {

    TupleConstLink t = ((*opA)[i]);
    if (!t) return NULL;
    
    TupleLink xt = new Tuple(bList);
    for (unsigned int i=0;i<dist.size();i++) {
      xt->valueList().add(t->valueList()[dist[i]]);
    }
    return xt;
  }

  // Get the size:
  virtual size_t size() const {
    
    if (ssize<0) {
      ssize=opA->size();
    }
    return ssize;
  }

private:

  // Destructor:
  virtual ~ProjectionStore() {
  }

  TupleStoreConstLink           opA;
  AttributeListConstLink        bList;
  mutable int                   ssize;
  std::vector<int>              dist;


 // Silence compiler warnings about solitude
  friend class ImaginaryFriend;
};


Table Projection::operator * (const Table & table) const
{

  AttributeListLink bList = new AttributeList();

  // Add an entry

  std::vector<int> dist;

  for (AttributeList::ConstIterator a = table.attributeList()->begin();a!= table.attributeList()->end();a++) {
    std::string name = (*a).name();
    if (std::find(_nameList.begin(),_nameList.end(),name)!=_nameList.end()) {
      
      //int distance = a-table.attributeList()->begin();
      dist.push_back((*a).attrId());

      if ((*a).type()==Attribute::DOUBLE)  {
	bList->add(name,typeid(double(0)));
      }
      else if ((*a).type()==Attribute::FLOAT)  {
	bList->add(name,typeid(float(0)));
      }
      else if ((*a).type()==Attribute::INT)  {
	bList->add(name,typeid(int(0)));
      }
      else if ((*a).type()==Attribute::UINT)  {
	bList->add(name,typeid(size_t(0)));
      }
      else {
	throw std::runtime_error("Error unknown type in projection operation");
      }
    }
  }

  const ProjectionStore *newStore = new ProjectionStore(table.store(),bList,dist);

  Table t(table.name(),newStore);
  return t;
}



Projection::Projection ()
{
  _nameList.insert(std::string("Entry"));
}

Projection::~Projection () {

}

Projection::Projection(const Projection & projection):
  _nameList(projection._nameList) 
{
}

Projection & Projection::operator = (const Projection & projection) {
  if (&projection != this) {
    _nameList=projection._nameList;
  }
  return *this;
}

void Projection::add (const std::string & name) {
  _nameList.insert (name);
}
