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

// --------------------CutBase--------------------------------//
//                                                            //
//  CutBase                                                   //
//                                                            //
//                                                            //
//  CutBase:  set of classes for doing boolean operations on  //
//  cuts.  These classes function a little like STL           //
//  predicates, but they extend functionality by permitting   //
//  Boolean operations.  They are sitting here in Generic     //
//  Functions package because they are quite similar to the   //
//  generic functions, except that instead of obeying a       //
//  Function algebra, these objects obey a Boolean algebra.   //
//                                                            //
//  IF YOU INHERIT YOUR PREDICATE FROM Cut<Type>, you will    //
//  for free get all the boolean operations on the predicate. //
//  Here is an example where the type is an integer:          //
//                                                            //
//                                                            //
//  class IsPrime:public Cut<int> {                           //
//     // Implies you will implement operator () (int) const, //
//     // And clone() const                                   //
//  }                                                         //
//                                                            //
//  class IsInRange:public Cut<int> {                         //
//     // Implies you will implement operator () (int) const, //
//     // And clone() const                                   //
//  }                                                         //
//                                                            //
//                                                            //
//  Then the following example should work, note the use of   //
//  Boolean operations:                                       //
//                                                            //
//  const int N=100;                                          //
//  int array[N];                                             //
//  for (int i=0;i<N;i++) array[i]=i;                         //
//  std::ostream_iterator<int> dest(std::cout,"\n");          //
//                                                            //
//  const Cut<int>::Predicate cut = IsPrime() && IsInRange(3,9);
//  std::remove_copy_if(array, array+N, dest, !cut);          //
//                                                            //
//                                                            //
//                                                            //
//                                                            //
// -----------------------------------------------------------//

#ifndef _CutBase_h_
#define _CutBase_h_

#include <functional>

template<class Type> 
class Cut {
public:

  //-----------Boolean operations-----------------------------//
  //                                                          //
  //...For OR'ing the cuts.                                   //
  //                                                          //
  class OR;                                                   //
  OR operator ||( const Cut<Type> & A ) const;                //
  //                                                          //
  //...For AND'ing the cuts.                                  //
  //                                                          //
  class AND;                                                  //
  AND operator &&( const Cut<Type> & A ) const;               //
  //                                                          //
  //...For negating the cuts:                                 //
  //                                                          //
  class NOT;                                                  //
  NOT operator ! ( void ) const;                              //
  //                                                          //
  //----------------------------------------------------------//

  //-----------Constructors & cetera--------------------------//
  Cut();                                                      //
  Cut(const Cut & right);                                     //
  virtual ~Cut();                                             //
  virtual Cut * clone() const = 0;                            //
  //----------------------------------------------------------//

  //-----------Concrete class holding any cut:----------------//
  //                                                          //
  class Predicate;                                            //
  //                                                          //
  //----------------------------------------------------------//

  //----------------------------------------------------------//
  // Evaluate predicate                                       //
  //                                                          //
  virtual bool operator ()( const Type & t ) const = 0;       //
  //                                                          //
  //----------------------------------------------------------//

};

//--------------------------------------------------------------------------
// Common standard Cut classes
//--------------------------------------------------------------------------
template<class Type>
class Cut<Type>::AND : public Cut<Type>  {

public:
  AND( const AND & right );
  AND( const Cut & A, const Cut & B );
  virtual ~AND();
  virtual AND * clone( void ) const;
  virtual bool operator ()( const  Type & t ) const;
private:
  const AND & operator=( const AND & right );
  Cut * _pA;
  Cut * _pB;
};

template<class Type>
class Cut<Type>::OR : public Cut<Type>
{
public:
  OR( const OR & right );
  OR( const Cut & A, const Cut & B );
  virtual ~OR();
  virtual OR * clone( void ) const;
  virtual bool operator ()( const  Type & t ) const;
private:
  const OR & operator=( const OR & right );
  Cut * _pA;
  Cut * _pB;
};

template<class Type>
class Cut<Type>::NOT : public Cut<Type>  
{
public:
  NOT( const NOT & right );
  NOT( const Cut & A );
  virtual ~NOT();
  virtual NOT * clone( void ) const;
  virtual bool operator ()( const  Type & t ) const;
private:
  const NOT & operator=( const NOT & right );
  Cut * _pA   ;
};


template<class Type>
class Cut<Type>::Predicate : public Cut<Type>  
{
public:
  Predicate( const Predicate & right );
  Predicate( const Cut & A );
  virtual ~Predicate();
  virtual Predicate * clone( void ) const;
  virtual bool operator ()( const  Type & t ) const;
private:
  const Predicate & operator=( const Predicate & right );
  Cut * _pA   ;
};


#include "QatGenericFunctions/CutBase.icc"

#endif
