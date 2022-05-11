//---------------------------------------------------------------------------
// NAME: Jonathan Smoley
// FILE: arrayseq.h
// DATE: Spring 2022
// DESC: This file defines the ArraySeq class including fields and methods.
//---------------------------------------------------------------------------


#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <stdexcept>
#include <ostream>
#include "sequence.h"


template<typename T>
class ArraySeq : public Sequence<T>
{
public:

  // Default constructor
  ArraySeq();

  // Copy constructor
  ArraySeq(const ArraySeq& rhs);

  // Move constructor
  ArraySeq(ArraySeq&& rhs);

  // Copy assignment operator
  ArraySeq& operator=(const ArraySeq& rhs);

  // Move assignment operator
  ArraySeq& operator=(ArraySeq&& rhs);
  
  // Destructor
  ~ArraySeq();
  
  // Returns the number of elements in the sequence
  int size() const;

  // Tests if the sequence is empty
  bool empty() const;

  // Removes all of the elements from the sequence
  void clear();
  
  // Returns a reference to the element at the index in the
  // sequence. Throws out_of_range if index is invalid (less than 0 or
  // greater than or equal to size()).
  T& operator[](int index);

  // Returns a constant address to the element at the index in the
  // sequence. Throws out_of_range if index is invalid (less than 0 or
  // greater than or equal to size()).
  const T& operator[](int index) const;

  // Extends the sequence by inserting the element at the given index.
  // Throws out_of_range if the index is invalid (less than 0 or
  // greater than size()).
  void insert(const T& elem, int index);

  // Shrinks the sequence by removing the element at the index in the
  // sequence. Throws out_of_range if index is invalid.
  void erase(int index);

  // Returns true if the element is in the sequence, and false
  // otherwise.
  bool contains(const T& elem) const;

  // Sorts the elements in the sequence using less than equal (<=)
  // operator. (Not implemented in HW-3)
  void sort(); 
  
private:

  // resizable array
  T* array = nullptr;

  // size of list
  int count = 0;

  // max capacity of the array
  int capacity = 0;

  // helper to double the capacity of the array
  void resize();
  
};


template<typename T>
std::ostream& operator<<(std::ostream& stream, const ArraySeq<T>& seq)
{
  int n = seq.size();
  for (int i = 0; i < n - 1; ++i) 
    stream << seq[i] << ", ";
  if (n > 0)
    stream << seq[n - 1];
  return stream;
}


template<typename T>
ArraySeq<T>::ArraySeq()
{
}


// Copy constructor
template<typename T>
ArraySeq<T>::ArraySeq(const ArraySeq<T>& rhs)
{
  for (int i = 0; i < rhs.count; ++i)
  {
    insert(rhs[i], i);
  }
}


// Move constructor
template<typename T>
ArraySeq<T>::ArraySeq(ArraySeq<T>&& rhs)
{
  *this = std::move(rhs);
}


// Copy assignment operator
template<typename T>
ArraySeq<T>& ArraySeq<T>::operator=(const ArraySeq<T>& rhs)
{
  if (this != &rhs)
  {
    if (!rhs.empty())
    {
      // create new array to size of rhs
      T* new_array = new T[rhs.capacity];
      // fill with values present in rhs
      for (int i = 0; i < rhs.count; ++i)
      {
        new_array[i] = rhs[i];
      }
      // delete old lhs array
      delete[] array;
      // update current size and capacity
      count = rhs.count;
      capacity = rhs.capacity;
      // assign new array to lhs
      array = new_array;
    }
  }
    return *this;
}


// Move assignment operator
template<typename T>
ArraySeq<T>& ArraySeq<T>::operator=(ArraySeq<T>&& rhs)
{
  if (this != &rhs)
  {
    // shift rhs contents to current array
    array = rhs.array;
    count = rhs.count;
    capacity = rhs.capacity;
    // remove link to rhs array
    rhs.clear();
    rhs.array = nullptr;
  }
  return *this;
}
  

// Destructor
template<typename T>
ArraySeq<T>::~ArraySeq()
{
  clear();
  delete[] array;
}
  

// Returns the number of elements in the sequence
template<typename T>
int ArraySeq<T>::size() const
{
  return count;
}


// Tests if the sequence is empty
template<typename T>
bool ArraySeq<T>::empty() const
{
  return count == 0;
}


// Removes all of the elements from the sequence
template<typename T>
void ArraySeq<T>::clear()
{
  count = 0;
  capacity = 0;
}


// Access/Update operator called in left side cases
template<typename T>
T& ArraySeq<T>::operator[](int index)
{
  if (index >= count || index < 0)
  {
    throw std::out_of_range("lhs[]: index out of range");
  }
  return array[index];
}


// Access/Update operator called in right side cases
template<typename T>
const T& ArraySeq<T>::operator[](int index) const
{
  if (index >= count || index < 0)
  {
    throw std::out_of_range("rhs[]: index out of range");
  }
  return array[index];
}


// Insert elem at index in sequence, extending it if necessary
template<typename T>
void ArraySeq<T>::insert(const T& elem, int index)
{
  if (index > count || index < 0)
  {
    throw std::out_of_range("Insert(): index out of range");
  }
  // only resize if capacity is already full
  if (count + 1 > capacity)
  {
    resize();
  }
  // shift all elements right of index to the right
  for (int i = count; i > index; --i)
  {
    array[i] = array[i - 1];
  }
  // update count and insert new element
  array[index] = elem;
  ++count;
}


// Removes elem at index in sequence, shrinking it
template<typename T>
void ArraySeq<T>::erase(int index)
{
  if (index >= count || index < 0)
  {
    throw std::out_of_range("Erase(): index out of range");
  }
  else
  {
    // shift all elements left of index to left
    for (int i = index; i < count - 1; ++i)
    {
      array[i] = array[i + 1];
    }
    --count;
  }
}


// Searches for elem in the current sequence
template<typename T>
bool ArraySeq<T>::contains(const T& elem) const
{
  for (int i = 0; i < count; ++i)
  {
    if (array[i] == elem)
    {
      return true;
    }
  }
  return false;
}


// helper to increase the capacity of the array
template<typename T>
void ArraySeq<T>::resize()
{
  // at capacity 0, increment capacity to 1
  if (capacity == 0)
  {
    capacity = 1;
  }
  // otherwise, double the capacity
  else
  {
    capacity *= 2;
  }
  // create larger array and replace the old one with it
  T* temp = new T[this->capacity];
  for (int i = 0; i < count; ++i)
  {
    temp[i] = array[i];
  }
  delete[] array;
  array = temp;
}


template<typename T>
void ArraySeq<T>::sort()
{
  // Ignore for HW-3: Will be implemented in a future assignment
}


#endif
