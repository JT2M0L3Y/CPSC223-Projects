
//----------------------------------------------------------------------
// FILE: linkedlist.h
// NAME: Jonathan Smoley
// DATE: Spring 2022
// DESC: Linked-list implementation of the sequence interface
//----------------------------------------------------------------------


#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdexcept>
#include <ostream>
#include "sequence.h"

/* TODO: 

   (1) Read through all of the HW-2 instructions. The instructions
       include implementation requirements as well as basic
       information for using valgrind and gdb.
   
   (2) Implement each of the LinkedSeq functions below the class
       definition in this file. IMPORTANT: DO NOT IMPLEMENT THE
       FUNCTIONS INLINE! ALL OF THE FUNCTIONS MUST BE IMPLEMENTED
       OUTSIDE OF THE CLASS DEFINITION.

   (3) It is best to implement your code incrementally (for example,
       one or two functions at a time). As you implement your program,
       you should frequently run make (to ensure your program
       compiles) as well as the unit tests (./hw2_test) to ensure your
       implementation is correct. Being familiar with the unit tests
       will help as you work on your implementation. You can think of
       the unit tests as part of the specification for LinkedSeq.

   (4) Once your program passes all unit tests, you need to run
       valgrind to ensure you don't have any memory issues or memory
       leaks. See the hw instructions for details.

   (5) After step 4, you can run the performance tests. This works the
       same way as in HW-1 (e.g., run: ./hw2_perf > output.dat). It is
       a good idea to run the preformance tests first, without saving
       the output to a file to ensure there aren't any issues.

   (6) Once you have created the output.dat file, run gnuplot with
       plot_script.gp (again, the same as in hw1: gnuplot -c
       plot_script.gp). This will generate three graphs (stored in
       corresponding png files). Use these files in your writeup.

   (7) Once your writeup is complete (see instructions for
       expectations), you should make sure all of your code and
       writeups are pushed to your GitHub repository. To finalize your
       submissions, fill out the submission form (see Piazza for
       details).

 */


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

  // Sorts the elements in the sequence using less than equal (<=)
  // operator. (Not implemented in HW-2)
  void sort() override; 
  
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

};


// The following can be used to print a LinkedSeq object. Given a
// LinkedSeq variable s, e.g., you can print s using: cout << s << endl;
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


/* 
   Place the remaining function implementations below (between
   this command and the size() function implementation)
*/


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
    this->clear();
    // perform a deep copy
    for (int i = 0; i < rhs.size(); ++i)
    {
      this->insert(rhs[i], i);
    }
    // update the node count
    this->node_count = rhs.node_count;
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
    this->clear();
    // transfer ownership of the nodes
    this->head = rhs.head;
    this->tail = rhs.tail;
    this->node_count = rhs.node_count;
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
  this->clear();
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
  Node* curr = head;
  while (curr != nullptr)
  {
    Node* temp = curr->next;
    delete curr;
    curr = temp;
  }
  head = tail = nullptr;
  node_count = 0;
}
  

// Returns a reference to the element at the index
template<typename T>
T& LinkedSeq<T>::operator[](int index)
{
  if (index < 0 || index >= node_count)
  {
    throw std::out_of_range("Operator[] by reference: index out of range");
  }
  // iterate until index is reached
  Node* curr = head;
  for (int i = 0; i < index; ++i)
  {
    // final iteration points to desired node
    curr = curr->next;
  }

  return curr->value;
}


// Returns a constant address of the element at the index
template<typename T>
const T& LinkedSeq<T>::operator[](int index) const
{
  if (index < 0 || index >= node_count)
  {
    throw std::out_of_range("Operator[] by address: index out of range");
  }
  // iterate until index is reached
  Node* curr = head;
  for (int i = 0; i < index; ++i)
  {
    // final iteration points to desired node
    curr = curr->next;
  }

  return curr->value;
}


// Extends the sequence by inserting the element at the index
template<typename T>
void LinkedSeq<T>::insert(const T& elem, int index)
{
  if (index < 0 || index > node_count)
  {
    throw std::out_of_range("Insert: index out of range");
  }
  Node* new_node = new Node;
  new_node->value = elem;
  // check for start of list
  if (index == 0)
  {
    new_node->next = head;
    head = new_node;
  }
  // check for end of list
  else if (index == this->size())
  {
    tail->next = new_node;
    tail = new_node;
  }
  // otherwise, insert in middle
  else
  {
    Node* curr = head;
    for (int i = 1; i < index; ++i)
    {
      curr = curr->next;
    }
    // insert node into list
    new_node->next = curr->next;
    curr->next = new_node;
  }
  // if end of list, update tail
  if (index == this->size())
  {
    tail = new_node;
  }
  ++node_count;
}


// Removes element at the index parameter
template<typename T>
void LinkedSeq<T>::erase(int index)
{
  if (index < 0 || index >= this->size())
  {
    throw std::out_of_range("Erase: index out of range");
  }
  Node* curr;
  // if size is 1, no need to iterate through list
  if(this->size() == 1)
  {
    curr = head;
    head = tail = nullptr;
  }
  // otherwise, iterate to remove at desired index
  else
  {
    Node* temp = head;
    for(int i = 1; i < index; ++i)
    {
      temp = temp->next;
    }
    // assign curr to the node to be removed
    curr = temp->next;
    // check for start of list
    if(index == 0)
    {
      curr = head;
      head = head -> next;
    }
    // check for end of list
    else if(index == this->size() - 1)
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
  delete curr;
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
  // TODO: future assignment
}


#endif
