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

#ifndef ConstLink_h
#define ConstLink_h 1
#ifndef NULL
#define NULL            0
#endif
//     Smart links to reference-counted pointers.

//	Template Smart Pointer Class.  This reference counted
//	link allocates on demand. It audits the total pointer
//	count and collects the garbage when nobody's looking.
//
template <class T>
class ConstLink 
{
  public:
  

  // Constructor
  ConstLink();
  
  // Copy Constructor
  ConstLink(const ConstLink< T > &right);
  
  // Constructor
  ConstLink (const T *target);
  
  // Destructor 
  virtual ~ConstLink();
  
  // Assignment
  ConstLink< T > & operator=(const ConstLink< T > &right);
  
  // Equality
  bool operator==(const ConstLink< T > &right) const;
  
  // Inequality
  bool operator!=(const ConstLink< T > &right) const;
  
  
  // Relational operator
  bool operator<(const ConstLink< T > &right) const;
  
  // Relational operator
  bool operator>(const ConstLink< T > &right) const;
  
  // Relational operator
  bool operator<=(const ConstLink< T > &right) const;
  
  // Relational operator
  bool operator>=(const ConstLink< T > &right) const;
  
  
  
  // Dereference:  (*t).method();
  virtual const T & operator * () const;
  
  // Dereference: t->method()
  virtual const T * operator -> () const;
  
  // Check pointer validity:  if (t) {...}
  operator bool () const;
  


  private: 

  // Raw pointer to reference counted object.
  mutable T *ptr;
};



template <class T>
inline ConstLink<T>::ConstLink()
  :ptr(NULL)
{
}

template <class T>
inline ConstLink<T>::ConstLink(const ConstLink<T> &right)
  :ptr(right.ptr)
{
  if (ptr) ptr->ref();
}

template <class T>
inline ConstLink<T>::ConstLink (const T *target)
  :ptr(const_cast<T *> (target))
{
  if (target) target->ref();
}


template <class T>
inline ConstLink<T>::~ConstLink()
{
  if (ptr) ptr->unref();
}


template <class T>
inline ConstLink<T> & ConstLink<T>::operator=(const ConstLink<T> &right)
{
  if (this!=&right) {
    if (ptr) ptr->unref();
    ptr = right.ptr;
    if (ptr) ptr->ref();
  }
  return *this;
}


template <class T>
inline bool ConstLink<T>::operator==(const ConstLink<T> &right) const
{
  return ptr==right.ptr;
}

template <class T>
inline bool ConstLink<T>::operator!=(const ConstLink<T> &right) const
{
  return ptr!=right.ptr;
}


template <class T>
inline bool ConstLink<T>::operator<(const ConstLink<T> &right) const
{
  return ptr<right.ptr;
}

template <class T>
inline bool ConstLink<T>::operator>(const ConstLink<T> &right) const
{
  return ptr>right.ptr;
}

template <class T>
inline bool ConstLink<T>::operator<=(const ConstLink<T> &right) const
{
  return ptr<=right.ptr;
}

template <class T>
inline bool ConstLink<T>::operator>=(const ConstLink<T> &right) const
{
  return ptr>=right.ptr;
}



template <class T>
inline const T & ConstLink<T>::operator * () const
{
  return *ptr;
}

template <class T>
inline const T * ConstLink<T>::operator -> () const
{
  return ptr;
}

template <class T>
inline ConstLink<T>::operator bool () const
{
  return ptr;
}

#endif
