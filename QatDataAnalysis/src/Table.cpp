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

#include "QatDataAnalysis/Table.h"
#include "QatDataAnalysis/AttributeList.h"
#include "QatDataAnalysis/AttributeListLink.h"
#include "QatDataAnalysis/Tuple.h"
#include "QatGenericFunctions/Variable.h"
#include <iomanip>
#include <stdexcept>



AttributeListConstLink Table::attributeList()  const {
  if (!c->tuple) return AttributeListConstLink();
  return c->tuple->attributeList();
}

Table::Table(const std::string & name, const TupleStoreLink s):
  c(new Clockwork)
{
  c->name    = name;

  // Make a compound table (it is the most general!)
  c->store=s;


  if (s->size()==0) {
    AttributeListConstLink attributeList=new AttributeList();
    c->tuple=new Tuple(attributeList); 
  }
  else {
    c->tuple = (TupleLink) (*s)[s->size()-1];
  }

}

Table::Table(const std::string & name):
  c(new Clockwork)
{
  c->name    = name;

  // Assign to Link, will delete automatically.
  AttributeListConstLink attributeList=new AttributeList();

  // Assign to Link, will delete automatically.
  c->tuple=new Tuple(attributeList); 

  // Make a compound table (it is the most general!)
  c->store=new CompoundStore();
}

Table::~Table() {
  delete c;
}

TupleConstLink Table::capture() {
  
  if (!c->store->size()) {                                        

    AttributeListLink  attListPtr = AttributeListLink(c->tuple->attributeList());
    ValueList         * valListPtr  = & c->tuple->valueList();
    AttributeList     & attList = *attListPtr;
    ValueList         & valList = *valListPtr;
    
    size_t maxNameLength=0;

    //
    // Bubble sort both the list of atrributes and their values:--//
    // These operations will re-organize the tuple (requiring const_cast).
    //
    for (unsigned int i = 0; i< attList.size(); i++) {
      maxNameLength = std::max(maxNameLength,attList[i].name().size());
      for (unsigned int j = 0; j< attList.size(); j++) {
	if (attList[i] < attList[j]) {
	  std::swap(attList[i], attList[j]);
	  std::swap(valList[i], valList[j]);
	}
      }
    }
    for (unsigned int i=0;i<attList.size();i++) {
      c->nameList.push_back(attList[i].name());
      attList[i].attrId()=i;
    }
  }
  c->store->push_back(c->tuple);
  c->tuple=new Tuple(c->tuple->attributeList(),c->tuple->valueList());
  
  return (*(c->store))[c->store->size()-1];
}

std::ostream &  Table::print (std::ostream & o) const {
  if (c->store->size()==0) return o;
  
  for (AttributeList::ConstIterator t=c->tuple->attributeList()->begin();t!=c->tuple->attributeList()->end();t++) {
    o << std::setw(10);
    o << (*t).name() << " ";
  } 
  o << std::endl;
  for (size_t t = 0; t< numTuples();t++) {
    (*this)[t]->print(o);
  }
  return o;
}


size_t Table::numAttributes() const { 
  if (c->store->size()==0) throw std::runtime_error ("Table::size.  Attribute list not formed");
  return c->tuple->attributeList()->size();
}


TupleConstLink Table::operator [] (size_t index) const {
  return (*(c->store))[index];
  
}



void Table::operator += (const Table & a) {

 CompoundStore *cStore = dynamic_cast<CompoundStore *> (c->store.operator->());
 if (!cStore) throw std::runtime_error("Error summing tables:  table type?");

 if (cStore->isLocked()) throw std::runtime_error("Error summing tables:  first operand is locked");

  // Check that both tables are compatible with the union:
 
 if (!attributeList()||attributeList()->size()==0)   {
   c->tuple=new Tuple(a.attributeList(),a.c->tuple->valueList());
 }

 if (a.attributeList()->size()!=attributeList()->size()) {
   throw std::runtime_error ("Error summing tables: operands are not compatible with the union");
 }
 for (unsigned int i=0;i<a.attributeList()->size(); i++) {
   if ((*attributeList())[i].name()!=(*a.attributeList())[i].name()) {
     throw std::runtime_error ("Error summing tables: operands are not compatible with the union");
   }
   if ((*attributeList())[i].typeName()!=(*a.attributeList())[i].typeName()) {
     throw std::runtime_error ("Error summing tables: operands are not compatible with the union");
   }
 }
 cStore->push_back(a.c->store);
 
 // Once this table has been used in an "operation" it is locked to further modification!
 
 a.c->store->lock();
}

Table::Table(const Table & table) :
  c(new Clockwork(*table.c))
{
}

TupleStoreConstLink Table::store() const {
  return c->store;
}

Genfun::Variable Table::symbol (const std::string & name) const {
  const Attribute & a = attribute(name);
  return Genfun::Variable(a.attrId(),numAttributes());
}



Table *Table::memorize() const{

  Table *t = new Table(c->name);
  t->c->store=new CompoundStore();
  t->c->tuple = c->tuple;

  for (unsigned int n=0;n<numTuples();n++) {
    TupleConstLink tp=(*c->store)[n];
    TupleConstLink nt=new Tuple(tp->attributeList(),tp->valueList());
    t->c->store->push_back(nt);
  }
  return t;
}
