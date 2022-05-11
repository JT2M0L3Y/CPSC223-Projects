//----------------------------------------------------------------------
// NAME: Jonathan Smoley
// FILE: linkedlist.h
// DATE: Spring 2022
// DESC: My linked list implementation from HW2.
//----------------------------------------------------------------------


#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdexcept>
#include <ostream>
#include "sequence.h"


template<typename T>
class LinkedSeq : public Sequence<T>
{
public:

  // Default constructor
  LinkedSeq();

  // Copy constructor
  LinkedSeq(const LinkedSeq& rhs);

  // Move constructor
  LinkedSeq(LinkedSeq&& rhs);

  // Copy assignment operator
  LinkedSeq& operator=(const LinkedSeq& rhs);

  // Move assignment operator
  LinkedSeq& operator=(LinkedSeq&& rhs);
  
  // Destructor
  ~LinkedSeq();
  
  // Returns the number of elements in the sequence
  int size() const override;

  // Tests if the sequence is empty
  bool empty() const override;

  // Removes all of the elements from the sequence
  void clear() override;
  
  // Returns a reference to the element at the index in the
  // sequence. Throws out_of_range if index is invalid.
  T& operator[](int index) override;

  // Returns a constant address to the element at the index in the
  // sequence. Throws out_of_range if index is invalid.
  const T& operator[](int index) const override;

  // Extends the sequence by inserting the element at the given
  // index. Throws out_of_range if the index is invalid.
  void insert(const T& elem, int index) override;

  // Shrinks the sequence by removing the element at the index in the
  // sequence. Throws out_of_range if index is invalid.
  void erase(int index) override;

  // Returns true if the element is in the sequence, and false
  // otherwise.
  bool contains(const T& elem) const override;

  // Sorts the elements in the sequence in place using less than equal
  // (<=) operator. Uses merge sort.
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

  // linked list node
  struct Node {
    T value;
    Node* next = nullptr;
  };

  // head pointer
  Node* head = nullptr;

  // tail pointer
  Node* tail = nullptr;

  // size of list
  int node_count = 0;

  // sort function helpers
  Node* merge_sort(Node* left, int len);
  Node* quick_sort(Node* start, int len);
  Node* quick_sort_random(Node* start, int len);

  // random seed for quick sort
  int seed = 22;

};


template<typename T>
std::ostream& operator<<(std::ostream& stream, const LinkedSeq<T>& seq)
{
  int n = seq.size();
  for (int i = 0; i < n - 1; ++i) 
    stream << seq[i] << ", ";
  if (n > 0)
    stream << seq[n - 1];
  return stream;
}


// Default constructor (shouldn't have anything to do)
template<typename T>
LinkedSeq<T>::LinkedSeq()
{
}


// Copy constructor
template<typename T>
LinkedSeq<T>::LinkedSeq(const LinkedSeq& rhs)
{
  // call the copy assignment operator
  *this = rhs;
}


// Move constructor
template<typename T>
LinkedSeq<T>::LinkedSeq(LinkedSeq&& rhs)
{
  // call the move assignment operator
  *this = std::move(rhs);
}


// Copy assignment operator
template<typename T>
LinkedSeq<T>& LinkedSeq<T>::operator=(const LinkedSeq<T>& rhs)
{
  if (this != &rhs)
  {
    // clear current object of nodes
    clear();
    if (!rhs.empty())
    {
      // perform a deep copy
      for (int i = 0; i < rhs.size(); ++i)
      {
        insert(rhs[i], i);
      }
    }
  }
  return *this;
}


// Move assignment operator
template<typename T>
LinkedSeq<T>& LinkedSeq<T>::operator=(LinkedSeq<T>&& rhs)
{
  if (this != &rhs)
  {
    // clear current object of nodes
    clear();
    // transfer ownership of the nodes
    head = rhs.head;
    tail = rhs.tail;
    node_count = rhs.node_count;
    // reset object parameter
    rhs.head = rhs.tail = nullptr;
    rhs.node_count = 0;
  }
  return *this;
}


// Destructor
template<typename T>
LinkedSeq<T>::~LinkedSeq()
{
  clear();
}


// Tests if the sequence is empty
template<typename T>
bool LinkedSeq<T>::empty() const
{
  return node_count == 0;
}


// Removes all of the elements from the sequence
template<typename T>
void LinkedSeq<T>::clear()
{
  while (head != nullptr)
  {
    Node* temp = head;
    head = head->next;
    delete temp;
  }
  head = tail = nullptr;
  node_count = 0;
}


// Returns a reference to the element at the index
template<typename T>
T& LinkedSeq<T>::operator[](int index)
{
  if (index >= node_count || index < 0)
  {
    throw std::out_of_range("Operator[] by reference: index out of range");
  }
  // check for access/update tail node
  if (index == node_count - 1)
  {
    return tail->value;
  }
  else
  {
    // iterate until index is reached
    Node* curr = head;
    for (int i = 0; i < index; ++i)
    {
      // final iteration points to desired node
      curr = curr->next;
    }
    return curr->value;
  }
}


// Returns a constant address of the element at the index
template<typename T>
const T& LinkedSeq<T>::operator[](int index) const
{
  if (index >= node_count || index < 0)
  {
    throw std::out_of_range("Operator[] by address: index out of range");
  }
  // check for access/update tail node
  if (index == node_count - 1)
  {
    return tail->value;
  }
  else
  {
    // iterate until index is reached
    Node* curr = head;
    for (int i = 0; i < index; ++i)
    {
      // final iteration points to desired node
      curr = curr->next;
    }
    return curr->value;
  }
}


// Extends the sequence by inserting the element at the index
template<typename T>
void LinkedSeq<T>::insert(const T& elem, int index)
{
  if (index > node_count || index < 0)
  {
    throw std::out_of_range("Insert: index out of range");
  }
  Node* new_node = new Node;
  new_node->value = elem;
  // check for start of list
  if (index == 0)
  {
    // insert at head
    new_node->next = head;
    head = new_node;
    // check for empty list
    if (node_count == 0)
    {
      tail = new_node;
    }
  }
  // check for end of list
  else if (index == node_count)
  {
    // insert at tail
    tail->next = new_node;
    tail = new_node;
  }
  // otherwise insert at middle of list
  else
  {
    // insert in middle
    Node* curr = head;
    for (int i = 0; i < index - 1; ++i)
    {
      curr = curr->next;
    }
    new_node->next = curr->next;
    curr->next = new_node;
  }
  ++node_count;
}


// Removes element at the index parameter
template<typename T>
void LinkedSeq<T>::erase(int index)
{
  if (index < 0 || index >= node_count)
  {
    throw std::out_of_range("Erase: index out of range");
  }
  Node* curr = nullptr;
  // if size is 1, no need to iterate through list
  if (node_count == 1)
  {
    curr = head;
    head = tail = nullptr;
  }
  // otherwise, iterate to remove at desired index
  else
  {
    Node* temp = head;
    for (int i = 1; i < index; ++i)
    {
      temp = temp->next;
    }
    // assign curr to the node to be removed
    curr = temp->next;
    // check for start of list
    if (index == 0)
    {
      curr = head;
      head = head->next;
    }
    // check for end of list
    else if (index == node_count - 1)
    {
      tail = temp;
      tail->next = nullptr;
    }
    // otherwise, assign next node
    else
    {
      temp->next = curr->next;
    }
  }
  --node_count;
}


// Contains checks if an element is in a sequence
template<typename T>
bool LinkedSeq<T>::contains(const T& elem) const
{
  Node* curr = head;
  while (curr != nullptr)
  {
    // if element is found, return true
    if (curr->value == elem)
    {
      return true;
    }
    curr = curr->next;
  }
  return false;
}

// Returns number of nodes in list
template<typename T>
int LinkedSeq<T>::size() const
{
  return node_count;
}


template<typename T>
void LinkedSeq<T>::sort()
{
  merge_sort();
}


template<typename T>
void LinkedSeq<T>::merge_sort()
{
  // if list is empty or has 1 node, no need to sort
  if (node_count <= 1)
  {
    return;
  }
  // call merge sort
  head = merge_sort(head, node_count);
  // update tail
  tail = head;
  while (tail->next != nullptr)
  {
    tail = tail->next;
  }
}


template<typename T>
void LinkedSeq<T>::quick_sort()
{
  // if list is empty or has 1 node, no need to sort
  if (node_count <= 1)
  {
    return;
  }
  // call quick sort
  head = quick_sort(head, node_count);
  // update tail
  tail = head;
  while (tail->next != nullptr)
  {
    tail = tail->next;
  }
}


template<typename T>
void LinkedSeq<T>::quick_sort_random()
{
  // seed the pseudo-random number generator
  std::srand(seed);

  // if list is empty or has 1 node, no need to sort
  if (node_count <= 1)
  {
    return;
  }
  // call quick sort
  head = quick_sort_random(head, node_count);
  // update tail
  tail = head;
  while (tail->next != nullptr)
  {
    tail = tail->next;
  }
}


template<typename T>
typename LinkedSeq<T>::Node* LinkedSeq<T>::merge_sort(Node* left, int len)
{
  // base case
  if (len <= 1)
  {
    return left;
  }
  // compute mid
  int mid = (len + 1) / 2;
  // set up right pointer
  Node* temp = left;
  for (int i = 1; i < mid; ++i)
  {
    temp = temp->next;
  }
  Node* right = temp->next;
  temp->next = nullptr;
  // merge sort on left and right
  left = merge_sort(left, mid);
  right = merge_sort(right, len - mid);
  // check if left or right is empty
  if (left == nullptr)
  {
    return right;
  }
  if (right == nullptr)
  {
    return left;
  }
  // merge left and right
  Node* curr_left = left;
  Node* curr_right = right;
  // check if left val is smaller than right val
  Node* result = nullptr;
  if (curr_left->value <= curr_right->value)
  {
    result = left;
    curr_left = curr_left->next;
  }
  else
  {
    result = right;
    curr_right = curr_right->next;
  }
  // merge sorted lists
  Node* temp_left = result;
  // while there are still nodes to be merged
  while (curr_left != nullptr && curr_right != nullptr)
  {
    if (curr_left->value <= curr_right->value)
    {
      temp_left->next = curr_left;
      curr_left = curr_left->next;
      temp_left = temp_left->next;
    }
    else
    {
      temp_left->next = curr_right;
      curr_right = curr_right->next;
      temp_left = temp_left->next;
    }
  }
  // add remaining nodes
  if (curr_left == nullptr)
  {
    temp_left->next = curr_right;
  }
  else
  {
    temp_left->next = curr_left;
  }
  // return pointer to first node of merged list
  return result;
}


template<typename T>
typename LinkedSeq<T>::Node* LinkedSeq<T>::quick_sort(Node* start, int len)
{
  // base case
  if (len <= 1)
  {
    return start;
  }
  // set up pointers
  int sml_len = 0, lrg_len = 0;
  Node* sml_tail = nullptr;
  Node* smaller = sml_tail;
  Node* lrg_tail = nullptr;
  Node* larger = lrg_tail;
  // partition into smaller and larger lists
  Node* curr = start->next;
  while (curr != nullptr)
  {
    // if element is larger than pivot, add to larger list
    if (curr->value > start->value)
    {
      if (lrg_len == 0)
      {
        larger = curr;
      }
      else
      {
        lrg_tail->next = curr;
      }
      lrg_tail = curr;
      ++lrg_len;
      curr = curr->next;
      lrg_tail->next = nullptr;
    }
    // otherwise, add to smaller list
    else
    {
      if (sml_len == 0)
      {
        smaller = curr;
      }
      else
      {
        sml_tail->next = curr;
      }
      sml_tail = curr;
      ++sml_len;
      curr = curr->next;
      sml_tail->next = nullptr;
    }
  }
  // sort smaller list
  smaller = quick_sort(smaller, sml_len);
  if (smaller == nullptr)
  {
    smaller = start;
  }
  else
  {
    sml_tail = smaller;
    while (sml_tail->next != nullptr)
    {
      sml_tail = sml_tail->next;
    }
    sml_tail->next = start;
  }
  // sort larger list
  larger = quick_sort(larger, lrg_len);
  if (larger != nullptr)
  {
    lrg_tail = larger;
    while (lrg_tail->next != nullptr)
    {
      lrg_tail = lrg_tail->next;
    }
    lrg_tail->next = nullptr;
  }
  start->next = larger;
  // return pointer to first node of sorted list
  return smaller;
}


template<typename T>
typename LinkedSeq<T>::Node* LinkedSeq<T>::quick_sort_random(Node* start, int len)
{
  // base case
  if (len <= 1)
  {
    return start;
  }
  // find random pivot
  int rand_ndx = rand() % len;
  Node* pivot = start;
  for (int i = 0; i < rand_ndx; ++i)
  {
    pivot = pivot->next;
  }
  // swap pivot value with start value
  int temp = start->value;
  start->value = pivot->value;
  pivot->value = temp;
  // set up pointers
  int sml_len = 0, lrg_len = 0;
  Node* sml_tail = nullptr;
  Node* smaller = sml_tail;
  Node* lrg_tail = nullptr;
  Node* larger = lrg_tail;
  // partition into smaller and larger lists
  Node* curr = start->next;
  while (curr != nullptr)
  {
    // if element is larger than pivot, add to larger list
    if (curr->value > start->value)
    {
      if (lrg_len == 0)
      {
        larger = curr;
      }
      else
      {
        lrg_tail->next = curr;
      }
      lrg_tail = curr;
      ++lrg_len;
      curr = curr->next;
      lrg_tail->next = nullptr;
    }
    // otherwise, add to smaller list
    else
    {
      if (sml_len == 0)
      {
        smaller = curr;
      }
      else
      {
        sml_tail->next = curr;
      }
      sml_tail = curr;
      ++sml_len;
      curr = curr->next;
      sml_tail->next = nullptr;
    }
  }
  // sort smaller list
  smaller = quick_sort_random(smaller, sml_len);
  if (smaller == nullptr)
  {
    smaller = start;
  }
  else
  {
    sml_tail = smaller;
    while (sml_tail->next != nullptr)
    {
      sml_tail = sml_tail->next;
    }
    sml_tail->next = start;
  }
  // sort larger list
  larger = quick_sort_random(larger, lrg_len);
  if (larger != nullptr)
  {
    lrg_tail = larger;
    while (lrg_tail->next != nullptr)
    {
      lrg_tail = lrg_tail->next;
    }
    lrg_tail->next = nullptr;
  }
  start->next = larger;
  // return pointer to first node of sorted list
  return smaller;
}


#endif
