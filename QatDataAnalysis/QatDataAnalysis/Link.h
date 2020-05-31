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

#ifndef Link_h_
#define Link_h_ 1

#include "QatDataAnalysis/ConstLink.h"
template<class T>
class Link : public ConstLink<T>
{


  public:

  // Constructor:
  Link();

  // Copy Constructor:
  Link(const Link<T> &right);

  // Promotion:
  explicit Link(const ConstLink<T> &right);

  // Construct from a pointer:
  Link (const T  *addr);

  // Destructor:
  virtual ~Link();

  // Assignment:
  Link<T> & operator = (const Link<T> & right);

  // 
  virtual T & operator * ();

  virtual T * operator -> ();
  
  virtual const T & operator * () const;
  
  virtual const T * operator -> () const;


};



template <class T>
inline Link<T>::Link()
  :ConstLink<T>(NULL)
{
}



template <class T>
inline Link<T>::Link(const Link<T> &right)
  :ConstLink<T>(right)
{
}

template <class T>
inline Link<T>::Link(const ConstLink<T> &right)
  :ConstLink<T>(right)
{
}

template <class T>
inline Link<T>::Link (const T *target)
  :ConstLink<T>(target)
{
}


template <class T>
inline Link<T>::~Link()
{
}


template <class T>
inline Link<T> & Link<T>::operator=(const Link<T> &right)
{
  ConstLink<T>::operator=(right);
  return *this;
}


template <class T>
inline T & Link<T>::operator * () 
{
  return const_cast<T &> (ConstLink<T>::operator*());
}

template <class T>
inline T * Link<T>::operator -> () 
{
  return const_cast<T *> (ConstLink<T>::operator -> ());
}


template <class T>
inline const T & Link<T>::operator * () const
{
  return ConstLink<T>::operator*();
}

template <class T>
inline const T * Link<T>::operator -> () const
{
  return ConstLink<T>::operator -> ();
}

#endif


