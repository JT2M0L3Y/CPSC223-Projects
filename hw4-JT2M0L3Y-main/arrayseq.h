//---------------------------------------------------------------------------
// NAME: Jonathan Smoley
// FILE: arrayseq.h
// DATE: Spring 2022
// DESC: My resizable array implementation from HW3.
//----------------------------------------------------------------------


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

  // Sorts the elements in the sequence in place using less than equal
  // (<=) operator. Uses quick sort with a randomly selected pivot
  // index.
  void sort(); 

  // Sorts the sequence in place using the merge sort algorithm.
  void merge_sort();

  // Sorts the sequence in place using the quick sort algorithm. Uses
  // first element for pivot values.
  void quick_sort();

  // Sorts the sequence in place using the quick sort algorithm. Uses
  // randomly selected indexes for pivot values.
  void quick_sort_random();

  
private:

  // resizable array
  T* array = nullptr;

  // size of list
  int count = 0;

  // max capacity of the array
  int capacity = 0;

  // helper to double the capacity of the array
  void resize();

  // sort function helpers
  void merge_sort(int start, int end);
  void quick_sort(int start, int end);
  void quick_sort_random(int start, int end);  

  // random seed for quick sort
  int seed = 22;
  
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
  *this = rhs;
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
    clear();
    if (!rhs.empty())
    {
      // create new array to size of rhs
      T* new_array = new T[rhs.capacity];
      // fill with values present in rhs
      for (int i = 0; i < rhs.count; ++i)
      {
        new_array[i] = rhs[i];
      }
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
    clear();
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
  delete[] array;
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
  T temp1 = array[index];
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
  // shift all elements to the right of index to the left
  for (int i = index; i < count - 1; ++i)
  {
    array[i] = array[i + 1];
  }
  --count;
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
  T* temp = new T[capacity];
  for (int i = 0; i < capacity / 2; ++i)
  {
    temp[i] = array[i];
  }
  delete[] array;
  array = temp;
}


// standard for sorting in arrayseq (quick random sort)
template<typename T>
void ArraySeq<T>::sort()
{
  quick_sort_random();
}


// call to merge sort
template<typename T>
void ArraySeq<T>::merge_sort()
{
  merge_sort(0, count - 1);
}


// call to quick sort with first element
template<typename T>
void ArraySeq<T>::quick_sort()
{
  quick_sort(0, count - 1);
}


// call to quick sort with random pivot
template<typename T>
void ArraySeq<T>::quick_sort_random()
{
  // seed the pseudo-random number generator
  std::srand(seed);

  quick_sort_random(0, count - 1);
}


// complete merge sort algorithm
template<typename T>
void ArraySeq<T>::merge_sort(int start, int end)
{
  // base case
  if (start < end)
  {
    // set mid
  int mid = (start + end) / 2;
    // recursive calls
    merge_sort(start, mid);
    merge_sort(mid + 1, end);
    // merge
    T temp[(end - start) + 1];
    int first1 = start;
    int first2 = mid + 1;
    int i = 0;
    // merge into temp
    while (first1 <= mid && first2 <= end)
    {
      if (array[first1] < array[first2])
      {
        temp[i++] = array[first1++];
      }
      else
      {
        temp[i++] = array[first2++];
      }
    }
    // copy rest
    while (first1 <= mid)
    {
      temp[i++] = array[first1++];
    }
    // copy rest
    while (first2 <= end)
    {
      temp[i++] = array[first2++];
    }
    // copy to array
    for (int i = 0; i <= (end - start); ++i)
    {
      array[start + i] = temp[i];
    }
  }
}


// quick sort using first element as pivot
template<typename T>
void ArraySeq<T>::quick_sort(int start, int end)
{
  // if start is less than end, sort the array
  if (start < end)
  {
    // set pivot and pivot value
    int pivot = start;
    int pivot_val = array[start];
    // partition
    for (int i = start + 1; i <= end; ++i)
    {
      if (array[i] < pivot_val)
      {
        ++pivot;
        // swap lesser element with end index
        int temp = array[i];
        array[i] = array[pivot];
        array[pivot] = temp;
      }
    }
    // swap start with pivot
    int temp = array[start];
    array[start] = array[pivot];
    array[pivot] = temp;
    // sort left and right sides
    quick_sort(start, pivot - 1);
    quick_sort(pivot + 1, end);
  }
}


// quick sort using random element as pivot
template<typename T>
void ArraySeq<T>::quick_sort_random(int start, int end)
{
  // if start is less than end, sort the array
  if (start < end)
  {
    // set random pivot, swap with first value in array
    int random_pivot = std::rand() % (end - start) + start;
    int initial = array[start];
    array[start] = array[random_pivot];
    array[random_pivot] = initial;
    int pivot_val = array[start];
    int pivot = start;
    for (int i = start + 1; i <= end; ++i)
    {
      // if element is smaller than pivot, move it to the left
      if (array[i] < pivot_val)
      {
        ++pivot;
        // perform swap
        int temp = array[i];
        array[i] = array[pivot];
        array[pivot] = temp;
      }
    }
    // perform another swap
    int temp = array[start];
    array[start] = array[pivot];
    array[pivot] = temp;
    // sort left and right sides
    quick_sort_random(start, pivot - 1);
    quick_sort_random(pivot + 1, end);
  }
}


#endif
