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

#include "QatDataAnalysis/IODriver.h"
#include "QatDataAnalysis/HistogramManager.h"
#include "QatDataAnalysis/Tuple.h"
#include <ios>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <unistd.h>
#include <memory>
#include <stdexcept>
#include <sqlite3.h>
#include <cstring>
#include <cstdio>
//* -----------------declarations -------------------*//

class SQLiteDriver: public IODriver {
 
public:

  // Constructor:
  SQLiteDriver();

  // Destructor:
  ~SQLiteDriver();

  // New Manager:
  virtual HistogramManager *newManager(const std::string & filename)  const;
  
  // OpenManager:
  virtual const HistogramManager *openManager(const std::string & filename) const;

  // Writes the Histograms out.
  virtual void close (const HistogramManager *mgr) const;

  // Closes the file:
  virtual void write (HistogramManager *mgr) const;
    
 private:


  void read(HistogramManager * mgr) const;
  
  mutable std::map<std::string, HistogramManager *> _mgrMap;
  mutable std::map<std::string, sqlite3             *> _ostrMap;
  mutable std::map<std::string, sqlite3             *> _istrMap;
  mutable sqlite3 *_id{nullptr};
  mutable sqlite3 *_od{nullptr};
  mutable std::vector<std::string>                     _dirPath;
};


// Creation routine:
extern "C" void *SQLiteDriverCreate();






class SQLiteIOStore: public TupleStore {

public:

  enum TCode {
    REAL=0,
    INT
  };
    
  // Constructor:
  SQLiteIOStore(sqlite3 *db, const std::string & tableName);

  // For input:
  virtual TupleConstLink operator [](size_t i) const;

  // Get the size:
  virtual size_t size() const {
    return tupleStoreSize;
  }

  AttributeListConstLink attributeList() const { 
    return internalTable->attributeList();
  }

private:

  // Destructor:
  virtual ~SQLiteIOStore();

  static int initcallback (void *This, int argc, char **argv, char **colName);

  sqlite3                        *_db;
  const std::string              _tableName;
  
  std::unique_ptr<Table>         internalTable;
  TupleLink                      internalTuple;
  std::vector<std::string>       nList;
  size_t                         nAttributes;
  size_t                         tupleStoreSize;
  friend class ImaginaryFriend;

};

TupleConstLink SQLiteIOStore::operator [] (size_t i) const {

  ValueList & vList = const_cast <ValueList &> (internalTuple->valueList());

  
  if (i>=tupleStoreSize) return NULL;
  sqlite3_stmt *stmt=nullptr;

  std::string tName=_tableName;
  tName.replace(0,1,"[T");
  tName+="]";
  std::string sql=std::string("select * from ")+tName+ std::string(" where PKID=")+std::to_string(i);
  sqlite3_prepare_v2(_db, sql.c_str(), -1, &stmt, nullptr);

  while (sqlite3_step(stmt) != SQLITE_DONE) {
    int i;
    int num_cols = sqlite3_column_count(stmt);
    for (i = 0; i < num_cols; i++)
      {
	
	std::string name=(char *) sqlite3_column_name(stmt,i);
	if (name=="PKID") continue; 
	
	// In SQLite, all real numbers are stored as doubles and all
	// integers are stored as long integer (size_type)
	switch (sqlite3_column_type(stmt, i))
	  {
	  case (SQLITE3_TEXT):
	    printf("Illegal text data: %s, ", sqlite3_column_text(stmt, i));
	    break;
	  case (SQLITE_INTEGER):
	    {
	      size_t value=sqlite3_column_int64(stmt,i);
              char * target = (char *) internalTuple->valueList()[i-1].asCharStar();
	      memcpy(target, &value, sizeof(size_t));
	    }
	    break;
	  case (SQLITE_FLOAT):
	    {
	      double value=sqlite3_column_double(stmt,i);
              char * target = (char *) internalTuple->valueList()[i-1].asCharStar();
	      memcpy(target, &value, sizeof(double));
	    }
	    break;
	  default:
	    printf("Illegal data type \n");
	    break;
	  }
      }
  }
  sqlite3_finalize(stmt);


  internalTuple->uncache();
  
  return internalTuple;

}

int SQLiteIOStore::initcallback (void *data, int argc, char **argv, char **colName) {
  SQLiteIOStore *This= (SQLiteIOStore *) data;
  std::string name, type;
  for (int i=0;i<argc;i++) {
    if (colName[i]==std::string("name")) name=argv[i];
    else if  (colName[i]==std::string("type")) type=argv[i];
  }
  
  if (name=="PKID") return SQLITE_OK;  // primary key is not part of the ntuple proper
  
  // There are only two basic types that we handle, (int) and (double). 
  if (type==std::string("INTEGER")) {
    This->internalTable->add(name, size_t(0));
  }
  else if (type==std::string("REAL")) {
    This->internalTable->add(name, double(0));
  }
  else {
    std::cerr << "Warning, column " << name << " of unrecognized type " << type << std::endl;
    return SQLITE_ERROR;
  }
  This->nList.push_back(name);
  return SQLITE_OK;
}



SQLiteIOStore::~SQLiteIOStore() {
}

SQLiteIOStore::SQLiteIOStore(sqlite3 *db, const std::string & tableName):
  _db(db),
  _tableName(tableName),
  internalTuple(NULL)
{
  // pad for sqlite
  std::string tName=tableName;
  tName.replace(0,1,"[T");
  tName+="]";
  
  char *errmsg=0;
  if (!internalTable) { // present if initialized
    // find the schema of the table, hold it in the "internal table"
    internalTable       = std::make_unique<Table> ("Internal Use Only");
    std::string cmd = "PRAGMA table_info("+ tName+")";
    int status = sqlite3_exec(_db, cmd.c_str(), &SQLiteIOStore::initcallback, (void *) this, &errmsg);
    if (status!=SQLITE_OK) {
      std::cerr <<"SQL error: "<<sqlite3_errmsg(db) << std::endl;
      sqlite3_free(errmsg);
    }
    internalTuple       = (TupleLink) internalTable->capture();
    // find the size of the table:
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(_db, ("select COUNT(*) from "+tName).c_str(), -1, &stmt, NULL);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
      int num_cols = sqlite3_column_count(stmt);
      if (num_cols!=1) {
	std::cerr << tableName << std::endl;
	throw std::logic_error("Strange response when accessing column count for table "+ tableName); 
	tupleStoreSize=0;
      }
      else {
	tupleStoreSize=sqlite3_column_int(stmt,0);
      }
    }
    sqlite3_finalize(stmt);

  }
    
  ValueList & vList = internalTuple->valueList();


  for (size_t j=0;j<nList.size();j++) {
    try {
      int i = internalTable->attribute(nList[j]).attrId();
    }
    catch (std::runtime_error &) {
    }
  }
}

//*----------------implementation------------------*//


extern "C" void *SQLiteDriverCreate() {
  return new SQLiteDriver();
} 


SQLiteDriver::SQLiteDriver():_id(nullptr),_od(nullptr) {
  std::cerr << "[SQLiteDriver:  HELLO.]" << std::endl;
}


SQLiteDriver::~SQLiteDriver() {
}

const HistogramManager *SQLiteDriver::openManager(const std::string & filename)  const {
  // See if this file is indeed an sqlite (3) file :

  // Ceci n'est pas une pipe:
  char *cBuff=nullptr;
  size_t len=0;
  FILE *ceci=popen( ("file " + filename).c_str(),"r");
  size_t nchar=getline(&cBuff,&len,ceci);
  {
    if (nchar!=0 && std::string(cBuff).find("SQLite")!=std::string::npos)  {
    }
    else {
      std::cerr << "[SQLiteDriver: File NOT OK:" << filename << "]" << std::endl;
      return NULL;
    }
  }
  sqlite3*db=nullptr;
  if (sqlite3_open(filename.c_str(), &db) != SQLITE_OK) {
    throw std::invalid_argument("SQLite file " + filename + " cannot be opened.");
  }
  
   
  _ostrMap[filename]=db;

   
  HistogramManager *mgr = new HistogramManager(filename);
  _mgrMap[filename]=mgr;
   
  read(mgr);
   
  std::cerr << "[SQLiteDriver:  Open Histogram file " << filename << "]" << std::endl;
  
  return mgr;

}


HistogramManager *SQLiteDriver::newManager(const std::string & filename)  const {

  remove(filename.c_str());

  sqlite3 *db=nullptr;
  int status = sqlite3_open(filename.c_str(), &db);
  if (status != SQLITE_OK) {
    std::cerr << sqlite3_errstr(status) << std::endl;
    throw std::invalid_argument("SQLite file " + filename + " cannot be opened!");
  }
  sqlite3_busy_timeout(db, 500); // total sleep time of 500ms before returning SQLITE_BUSY

  
  _istrMap[filename]=db;
                                                                                                           
  HistogramManager *mgr = new HistogramManager();
  _mgrMap[filename]=mgr;
                                                                                                           
  std::cerr << "[SQLiteDriver:  New Histogram file " << filename << "]" << std::endl;

  return mgr;

}

// Writes the Histograms out.
void SQLiteDriver::close (const HistogramManager *mgr) const {
  // Find the file name:
  std::map<std::string, HistogramManager *>::iterator 
    begin=_mgrMap.begin(),
    end=_mgrMap.end(),m=begin;
  std::string filename;
  while (m!=end) {
    m++;
    if ((*m).second==mgr) {
      filename = (*m).first;
      break;
    }
  }
  
  if (filename!="") {
    {
     sqlite3 *db = _ostrMap[filename];
     if (db) sqlite3_close(db);
    }
    {
      sqlite3 *db = _istrMap[filename];
      if (db) sqlite3_close(db);
    }
    {
      std::map<std::string, HistogramManager *>::iterator d=_mgrMap.find(filename);
      if (d!=_mgrMap.end()) _mgrMap.erase(d);
    }
    {
      std::map<std::string, sqlite3 *>::iterator d=_istrMap.find(filename);
      if (d!=_istrMap.end()) _istrMap.erase(d);
    }
    {
      std::map<std::string, sqlite3 *>::iterator d=_ostrMap.find(filename);
      if (d!=_ostrMap.end()) _ostrMap.erase(d);
    }
  }
  
  
}


// Closes the file:
void SQLiteDriver::write (HistogramManager *mgr) const {
  // Find the output file
  // Find the output stream for this:
  std::string name;
  std::map<std::string , HistogramManager *>::iterator m;
  if (!_od) {
    for (m=_mgrMap.begin();m!=_mgrMap.end();m++) {
      if (mgr==(*m).second) {
	name = (*m).first;
	_od= _istrMap[name];
	break;
      }
    }
    // Create a table for the directory
    {  
      char *errmsg(nullptr);
      std::string sql = "CREATE TABLE DIRECTORIES( PKID INTEGER PRIMARY KEY, NAME TEXT); ";	
      int status = sqlite3_exec(_od, sql.c_str(),0,0, &errmsg);
      if (status != SQLITE_OK) {
	std::cerr << "Error creating directory table" << std::endl;
      }
    }

  }

  if (!_od) return;

  std::string path;
  {
    
    char *errmsg(nullptr);
    _dirPath.push_back(mgr->name());
    for (const std::string & s : _dirPath) {
      path+="/";
      path+=s;
    }
    std::string sql = "INSERT INTO DIRECTORIES   VALUES (NULL, '"+path+"');";
    int status = sqlite3_exec(_od, sql.c_str(),0,0, &errmsg);
    if (status != SQLITE_OK) {
      std::cerr << "Error modifying directory table" << std::endl;
    }
    // Now get the primary key for this directory:
  }


  
  // Write out all of the ntuples:

  HistogramManager::DConstIterator d;
  for (d=mgr->beginDir();d!=mgr->endDir();d++) {
    write (*d);
  }
  
  // Write out all ntuples:
  HistogramManager::TConstIterator t;
  
  for (t=mgr->beginTable();t!=mgr->endTable();t++) {


    
    // Create table: 
    {  
      char *errmsg(nullptr);
      std::string sql = "CREATE TABLE [T:"  + path+"/"+(*t)->name() + "]( PKID INTEGER PRIMARY KEY";	
      /* Execute SQL statement */
      for (size_t a=0;a<(**t).numAttributes();a++) {
	Attribute::Type type=(**t).attribute(a).type();
	std::string     name=(**t).attribute(a).name();
	sql += ", ";
	if (type==Attribute::DOUBLE || type==Attribute::FLOAT)  {
	  sql += name +  " REAL ";
	}
	else if (type==Attribute::INT  || type==Attribute::UINT ||  type==Attribute::UINT64) {
	  sql += name +  " INTEGER";
	}
      }
      sql += ");";
      int status = sqlite3_exec(_od, sql.c_str(),0,0, &errmsg);
      if (status != SQLITE_OK) {
	std::cerr << "Error creating ntuple table" << std::endl;
      }
    }

    for (int i=0;i<(*t)->numTuples();i++)  {  
      TupleConstLink tup=(**t)[i];
      std::string sql = "INSERT INTO  [T:" +  path+"/"+(*t)->name()+"] VALUES (" +std::to_string(i);

      for (int j=0;j<(**t).numAttributes();j++) {
	sql+=",";
	Attribute::Type type=(**t).attribute(j).type();
	std::string     name=(**t).attribute(j).name();
	if (type==Attribute::DOUBLE) {
	  double val;
	  tup->fastread(val,j);
	  sql += std::to_string(val);
	}
	else if (type==Attribute::FLOAT) {
	  float val;
	  tup->fastread(val,j);
	  sql += std::to_string(val);
	}
	else if (type==Attribute::INT) {
	  int val;
	  tup->fastread(val,j);
	  sql += std::to_string(val);
	}
	else if (type==Attribute::UINT) {
	  unsigned int val;
	  tup->fastread(val,j);
	  sql += std::to_string(val);
	}
	else if (type==Attribute::UINT64) {
	  size_t val;
	  tup->fastread(val,j);
	  sql += std::to_string(val);
	}
      }
      sql += ");";
      
      /* Execute SQL statement */
      char *errmsg(nullptr);
      int status = sqlite3_exec(_od, sql.c_str(),0,0, &errmsg);
      if (status != SQLITE_OK) {
	std::cerr << "Error filling ntuple table" << std::endl;
      }
    }
  }
    

  for (HistogramManager::H1ConstIterator hh=mgr->beginH1();hh!=mgr->endH1();hh++) {
    const Hist1D::Clockwork *h=(*hh)->clockwork();
    
    {
      //
      // create the table table
      //
      std::string sql = "CREATE TABLE [1:" +path+"/"+ h->name + "] (ID INTEGER PRIMARY KEY AUTOINCREMENT, TITLE VARCHAR(255), ";
      sql += "NBINS INTEGER, ";
      sql += "MIN REAL, ";
      sql += "MAX REAL, ";
      sql += "BINWIDTH REAL, ";
      sql += "OVERFLOW INTEGER, ";
      sql += "UNDERFLOW INTEGER, ";
      sql += "ENTRIES INTEGER, ";
      sql += "XW REAL, ";
      sql += "XXW REAL, ";
      sql += "SUMWEIGHT REAL, ";
      sql += "MINCONTENTS REAL, ";
      sql += "MAXCONTENTS REAL, ";
      sql += "CONTENTS BLOB, ";
      sql += "ERRORS BLOB ";
      sql += ");";
      char* error=nullptr; 
      int status = sqlite3_exec(_od, sql.c_str(), NULL, 0, &error);
      if (status != SQLITE_OK) {
	std::cerr << "Error creating Hist1D table" << std::endl;
	std::cerr << sql << std::endl;
      }
    }
    {
      
      sqlite3_stmt* stmtInsert = nullptr;
      std::string cmd="INSERT INTO [1:" + path + "/"+h->name + "] VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
      sqlite3_prepare_v2(_od, cmd.c_str(), -1, &stmtInsert, nullptr);
      
      //    sqlite3_bind_int (stmtInsert, 3, matrix.cols( ));
      sqlite3_bind_text (stmtInsert, 2, h->name.c_str(),h->name.size(),NULL);
      sqlite3_bind_int (stmtInsert, 3, h->nBins);
      sqlite3_bind_double (stmtInsert, 4, h->min);
      sqlite3_bind_double (stmtInsert, 5, h->max);
      sqlite3_bind_double (stmtInsert, 6, h->binWidth);
      sqlite3_bind_int (stmtInsert, 7, h->overflow);
      sqlite3_bind_int (stmtInsert, 8, h->underflow);
      sqlite3_bind_int (stmtInsert, 9, h->entries);
      sqlite3_bind_double (stmtInsert, 10, h->xW);
      sqlite3_bind_double (stmtInsert, 11, h->xxW);
      sqlite3_bind_double (stmtInsert, 12, h->sumWeight);
      sqlite3_bind_double (stmtInsert, 13, h->minContents);
      sqlite3_bind_double (stmtInsert, 14, h->maxContents);
      sqlite3_bind_blob(stmtInsert, 15, h->contents.data(), static_cast<int>(h->contents.size() * sizeof(double)), SQLITE_STATIC);
      sqlite3_bind_blob(stmtInsert, 16, h->errors.data(),   static_cast<int>(h->errors.size() * sizeof(double)), SQLITE_STATIC);
      
      if (sqlite3_step(stmtInsert) != SQLITE_DONE) {
	std::cerr << "Error saving histogram data, SQL= : " << cmd << " failed"  << std::endl;
      }

      sqlite3_finalize(stmtInsert);
    } 
    
  }


  for (HistogramManager::H2ConstIterator hh=mgr->beginH2();hh!=mgr->endH2();hh++) {
    const Hist2D::Clockwork *h=(*hh)->clockwork();
    
    {
      //
      // create the table table
      //
      std::string sql = "CREATE TABLE [2:" +path+"/"+ h->name + "] (ID INTEGER PRIMARY KEY AUTOINCREMENT, TITLE VARCHAR(255), ";
      sql += "NBINSX INTEGER, ";
      sql += "NBINSY INTEGER, ";
      sql += "MINX REAL, ";
      sql += "MINY REAL, ";
      sql += "MAXX REAL, ";
      sql += "MAXY REAL, ";
      sql += "BINWIDTHX REAL, ";
      sql += "BINWIDTHY REAL, ";
      sql += "OVERUNDER00 INTEGER, ";
      sql += "OVERUNDER01 INTEGER, ";
      sql += "OVERUNDER02 INTEGER, ";
      sql += "OVERUNDER10 INTEGER, ";
      sql += "OVERUNDER11 INTEGER, ";
      sql += "OVERUNDER12 INTEGER, ";
      sql += "OVERUNDER20 INTEGER, ";
      sql += "OVERUNDER21 INTEGER, ";
      sql += "OVERUNDER22 INTEGER, ";
      sql += "OVERFLOW INTEGER, ";
      sql += "XW REAL, ";
      sql += "XXW REAL, ";
      sql += "YW REAL, ";
      sql += "YYW REAL, ";
      sql += "SUMWEIGHT REAL, ";
      sql += "MINCONTENTS REAL, ";
      sql += "MAXCONTENTS REAL, ";
      sql += "CONTENTS BLOB, ";
      sql += "ERRORS BLOB ";
      sql += ");";
      char* error=nullptr; 
      int status = sqlite3_exec(_od, sql.c_str(), NULL, 0, &error);
      if (status != SQLITE_OK) {
	std::cerr << "Error creating Hist1D table" << std::endl;
	std::cerr << sql << std::endl;
      }
    }
    {
      
      sqlite3_stmt* stmtInsert = nullptr;
      std::string cmd="INSERT INTO [2:" + path + "/"+h->name + "] VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?,  ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
      sqlite3_prepare_v2(_od, cmd.c_str(), -1, &stmtInsert, nullptr);
      
      //    sqlite3_bind_int (stmtInsert, 3, matrix.cols( ));
      sqlite3_bind_text (stmtInsert, 2, h->name.c_str(),h->name.size(),NULL);
      sqlite3_bind_int (stmtInsert, 3, h->nBinsX);
      sqlite3_bind_int (stmtInsert, 4, h->nBinsY);
      sqlite3_bind_double (stmtInsert, 5, h->minX);
      sqlite3_bind_double (stmtInsert, 6, h->minY);
      sqlite3_bind_double (stmtInsert, 7, h->maxX);
      sqlite3_bind_double (stmtInsert, 8, h->maxY);
      sqlite3_bind_double (stmtInsert, 9, h->binWidthX);
      sqlite3_bind_double (stmtInsert, 10, h->binWidthY);
      sqlite3_bind_int (stmtInsert, 11, h->overUnderFlow[0][0]);
      sqlite3_bind_int (stmtInsert, 12, h->overUnderFlow[0][1]);
      sqlite3_bind_int (stmtInsert, 13, h->overUnderFlow[0][2]);
      sqlite3_bind_int (stmtInsert, 14, h->overUnderFlow[1][0]);
      sqlite3_bind_int (stmtInsert, 15, h->overUnderFlow[1][1]);
      sqlite3_bind_int (stmtInsert, 16, h->overUnderFlow[1][2]);
      sqlite3_bind_int (stmtInsert, 17, h->overUnderFlow[2][0]);
      sqlite3_bind_int (stmtInsert, 18, h->overUnderFlow[2][1]);
      sqlite3_bind_int (stmtInsert, 19, h->overUnderFlow[2][2]);
      sqlite3_bind_int (stmtInsert, 20, h->overflow);
      sqlite3_bind_double (stmtInsert, 21, h->xW);
      sqlite3_bind_double (stmtInsert, 22, h->xxW);
      sqlite3_bind_double (stmtInsert, 23, h->yW);
      sqlite3_bind_double (stmtInsert, 24, h->yyW);
      sqlite3_bind_double (stmtInsert, 25, h->sumWeight);
      sqlite3_bind_double (stmtInsert, 26, h->minContents);
      sqlite3_bind_double (stmtInsert, 27, h->maxContents);
      sqlite3_bind_blob(stmtInsert, 28, h->contents.data(), static_cast<int>(h->contents.size() * sizeof(double)), SQLITE_STATIC);
      sqlite3_bind_blob(stmtInsert, 29, h->errors.data(),   static_cast<int>(h->errors.size() * sizeof(double)), SQLITE_STATIC);
      
      if (sqlite3_step(stmtInsert) != SQLITE_DONE) {
	std::cerr << "Error saving histogram data, SQL= : " << cmd << " failed"  << std::endl;
      }

      sqlite3_finalize(stmtInsert);
    } 
    
  }

  _dirPath.pop_back();
  return;
}





void SQLiteDriver::read(HistogramManager *mgr) const {
  std::string name;
  std::map<std::string , HistogramManager *>::iterator m;
  if (!_id) {
    for (m=_mgrMap.begin();m!=_mgrMap.end();m++) {
      if (mgr==(*m).second) {
	name = (*m).first;
	_id = _ostrMap[name];
	break;
      }
    }
  }
  if (!_id) return;


  // Read in the directories and create the structure:

  std::vector<std::string>        dirNameVec;
  std::vector<HistogramManager *> histManVec;
  {
    sqlite3_stmt *stmt=nullptr;
    std::string sql="SELECT pkid, name from DIRECTORIES;";
    sqlite3_prepare_v2(_id, sql.c_str(), -1, &stmt, nullptr);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
      int         pkid    = sqlite3_column_int(stmt,0);
      std::string fullname= (char *) sqlite3_column_text(stmt, 1);
      std::replace (fullname.begin(),fullname.end(), '/',' ');
     
      std::vector<std::string> vDir;
      std::istringstream stream(fullname);
      std::string        pathElement;
      while (stream >> pathElement) {
	vDir.push_back(pathElement);
      }

      if (vDir.size()==1) {
	dirNameVec.push_back(fullname);
	histManVec.push_back(mgr);
      }
      else {
	
	std::string newDirName = vDir.back();
	vDir.pop_back();
	size_t pos=fullname.find_last_of(' ');
	std::string updir=fullname.substr(0,pos);
       
	auto it=std::find(dirNameVec.begin(),dirNameVec.end(),updir);

	
	size_t i=it-dirNameVec.begin();
       
	HistogramManager *d2=histManVec[i];
	dirNameVec.push_back(fullname);
	histManVec.push_back(d2->newDir(newDirName));
      }
    }
    sqlite3_finalize(stmt);

  }


  
  
  {
    sqlite3_stmt *stmt=nullptr;
    std::string sql="SELECT name from sqlite_master WHERE type=\'table\'";
    sqlite3_prepare_v2(_id, sql.c_str(), -1, &stmt, nullptr);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
      std::string fullname= (char *) sqlite3_column_text(stmt, 0);

      std::string prefix(fullname.substr(0,2));
      std::string oname=fullname.substr(2,fullname.npos);
      if (prefix=="T:" || prefix=="1:" || prefix=="2:") {
	std::replace (oname.begin(),oname.end(), '/',' ');
	int pos=oname.find_last_of(' ');
	std::string dirName=oname.substr(0,pos);
	std::string tableName=oname.substr(pos,oname.npos);;
	int ptr=0;
	while (1) {
	  if (dirName==dirNameVec[ptr]) break;
	  ptr++;
	}
      
	HistogramManager *thisMgr=histManVec[ptr];
      
	// Handle the ntuples:
	if (prefix=="T:") {
	  SQLiteIOStore *store = new SQLiteIOStore(_id,fullname);
	  thisMgr->newTable(tableName,store);
	}


	else if (prefix=="1:") {
	
	  sqlite3_stmt *stmt=nullptr;
	  std::string sql = "SELECT * FROM ["  + fullname +"];";
	  sqlite3_prepare_v2(_id, sql.c_str(), -1, &stmt, nullptr);
	  if (sqlite3_step(stmt) != SQLITE_DONE) {
	    int i;
	    int num_cols = sqlite3_column_count(stmt);
	    Hist1D::Clockwork c;
	    for (i = 0; i < num_cols; i++) {
	      if (sqlite3_column_name(stmt,i)==std::string("TITLE")) {
		c.name=(char *) sqlite3_column_text(stmt,i);
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("NBINS")) {
		c.nBins=sqlite3_column_int(stmt,i);
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("MIN")) {
		c.min=sqlite3_column_double(stmt,i);
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("MAX")) {
		c.max=sqlite3_column_double(stmt,i);
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("BINWIDTH")) {
		c.binWidth=sqlite3_column_double(stmt,i);
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("OVERFLOW")) {
		c.overflow=sqlite3_column_int(stmt,i);
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("UNDERFLOW")) {
		c.underflow=sqlite3_column_int(stmt,i);
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("ENTRIES")) {
		c.entries=sqlite3_column_int(stmt,i);
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("XW")) {
		c.xW=sqlite3_column_double(stmt,i);
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("XXW")) {
		c.xxW=sqlite3_column_double(stmt,i);
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("SUMWEIGHT")) {
		c.sumWeight=sqlite3_column_double(stmt,i);
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("MINCONTENTS")) {
		c.minContents=sqlite3_column_double(stmt,i);
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("MAXCONTENTS")) {
		c.maxContents=sqlite3_column_double(stmt,i);
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("CONTENTS")) {
		int len=sqlite3_column_bytes(stmt,i)/sizeof(double);
		c.contents.resize(len);
		double * source=(double *) sqlite3_column_blob(stmt,i);
		std::copy(source, source+len, c.contents.begin());
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("ERRORS")) {
		int len=sqlite3_column_bytes(stmt,i)/sizeof(double);
		c.errors.resize(len);
		double * source=(double *) sqlite3_column_blob(stmt,i);
		std::copy(source, source+len, c.errors.begin());
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("ID")) {
		// Do nothing
	      }
	      else {
		throw std::runtime_error("Unknown field in Hist1D record ");
	      }
	    }
	    thisMgr->newHist1D(&c);
	    sqlite3_finalize(stmt);
	  }
	}

	else if (prefix=="2:") {
	  sqlite3_stmt *stmt=nullptr;
	  std::string sql = "SELECT * FROM ["  + fullname +"];";
	  sqlite3_prepare_v2(_id, sql.c_str(), -1, &stmt, nullptr);
	  if (sqlite3_step(stmt) != SQLITE_DONE) {
	    int i;
	    int num_cols = sqlite3_column_count(stmt);
	    Hist2D::Clockwork c;
	    for (i = 0; i < num_cols; i++) {
	      if (sqlite3_column_name(stmt,i)==std::string("TITLE")) {
		c.name=(char *) sqlite3_column_text(stmt,i);
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("NBINSX")) {
		c.nBinsX=sqlite3_column_int(stmt,i);
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("NBINSY")) {
		c.nBinsY=sqlite3_column_int(stmt,i);
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("MINX")) {
		c.minX=sqlite3_column_double(stmt,i);
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("MINY")) {
		c.minY=sqlite3_column_double(stmt,i);
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("MAXX")) {
		c.maxX=sqlite3_column_double(stmt,i);
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("MAXY")) {
		c.maxY=sqlite3_column_double(stmt,i);
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("BINWIDTHX")) {
		c.binWidthX=sqlite3_column_double(stmt,i);
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("BINWIDTHY")) {
		c.binWidthY=sqlite3_column_double(stmt,i);
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("OVERUNDER00")) {
		c.overUnderFlow[0][0]=sqlite3_column_int(stmt,i);
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("OVERUNDER01")) {
		c.overUnderFlow[0][1]=sqlite3_column_int(stmt,i);
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("OVERUNDER02")) {
		c.overUnderFlow[0][2]=sqlite3_column_int(stmt,i);
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("OVERUNDER10")) {
		c.overUnderFlow[1][0]=sqlite3_column_int(stmt,i);
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("OVERUNDER11")) {
		c.overUnderFlow[1][1]=sqlite3_column_int(stmt,i);
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("OVERUNDER12")) {
		c.overUnderFlow[1][2]=sqlite3_column_int(stmt,i);
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("OVERUNDER20")) {
		c.overUnderFlow[2][0]=sqlite3_column_int(stmt,i);
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("OVERUNDER21")) {
		c.overUnderFlow[2][1]=sqlite3_column_int(stmt,i);
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("OVERUNDER22")) {
		c.overUnderFlow[2][2]=sqlite3_column_int(stmt,i);
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("OVERFLOW")) {
		c.overflow=sqlite3_column_int(stmt,i);
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("XW")) {
		c.xW=sqlite3_column_double(stmt,i);
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("XXW")) {
		c.xxW=sqlite3_column_double(stmt,i);
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("YW")) {
		c.yW=sqlite3_column_double(stmt,i);
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("YYW")) {
		c.yyW=sqlite3_column_double(stmt,i);
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("SUMWEIGHT")) {
		c.sumWeight=sqlite3_column_double(stmt,i);
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("MINCONTENTS")) {
		c.minContents=sqlite3_column_double(stmt,i);
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("MAXCONTENTS")) {
		c.maxContents=sqlite3_column_double(stmt,i);
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("CONTENTS")) {
		int len=sqlite3_column_bytes(stmt,i)/sizeof(double);
		c.contents.resize(len);
		double * source=(double *) sqlite3_column_blob(stmt,i);
		std::copy(source, source+len, c.contents.begin());
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("ERRORS")) {
		int len=sqlite3_column_bytes(stmt,i)/sizeof(double);
		c.errors.resize(len);
		double * source=(double *) sqlite3_column_blob(stmt,i);
		std::copy(source, source+len, c.errors.begin());
	      }
	      else if (sqlite3_column_name(stmt,i)==std::string("ID")) {
		// Do nothing
	      }
	      else {
		throw std::runtime_error("Unknown field in Hist2D record ");
	      }
	    }
	    thisMgr->newHist2D(&c);
	    sqlite3_finalize(stmt);
	  }
	  else {
	    // We only handle tables, 1d hist, 2d hist
	  }
	}
      }
    }
   
  }
  
}
