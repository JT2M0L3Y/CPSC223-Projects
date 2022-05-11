//---------------------------------------------------------------------------
// FILE: simple_sorts.cpp
// NAME: Jonathan Smoley
// DATE: Spring 2022
// DESC: This .cpp file implements a swap function and three 
//       sorting functions (bubble, insertion, and selection).
//---------------------------------------------------------------------------

#include <iostream>
#include "simple_sorts.h"

/* Your job for this file is to:

    (1) Fill in the file header comment above

    (2) Implement the three sorting algorithms below. Your
        implementation MUST implement the algorithms presented in
        class and given in the lecture notes. Note that pseudocode is
        also provided in the lecture notes, so it should largely
        require you to translate the pseudocode to C++.

    (3) As you implement the algorithms you should frequently compile
        (via make) and run the unit tests (using the command
        hw1_test).

    (4) Once you feel that your code is correct and you have finished
        writing the new tests for hw1_test.cpp (and they pass), you
        should run the performance tests (using the command hw1_perf). 

    (5) Note that the performance tests run the algorithms are much
        larger test cases, and so they may also identify bugs in your
        code. Note that if the performance tests report an error, it
        suggests an issue in your implementation, not the performance
        tests themselves. Once you have the performance tests working
        following the instructions in the homework assignment to
        generate the performance graphs. 
*/

void swap(int& x, int& y)
{
  int tmp = x;
  x = y;
  y = tmp;
}


void bubble_sort(int array[], int n)
{
  bool sorted = false;
  for (int i = 1; i < n && !sorted; ++i)
  {
    // short circuit loop if no swaps
    sorted = true;
    for (int j = 1; j < n - i + 1; ++j)
    {
      if (array[j - 1] > array[j])
      {
        // call swap function
        swap(array[j - 1], array[j]);
        sorted = false;
      }
    }
  }
}


void insertion_sort(int array[], int n)
{
  for (int i = 1; i < n; ++i)
  {
    int val = array[i];
    int j = i;
    // continue shifting until val is in correct place
    while (j > 0 && array[j - 1] > val)
    {
      array[j] = array[j - 1];
      j = j - 1;
    }
    // perform only part of operations in swap function
    array[j] = val;
  }
}


void selection_sort(int array[], int n)
{
  for (int i = 0; i < n - 1; ++i) 
  {
    // set current iteration of i as lower bound of unsorted group
    int min = i;
    for (int j = i + 1; j < n; ++j)
    {
      if (array[j] < array[min])
      {
        min = j;
      }
    }
    // call swap function
    swap(array[i], array[min]);
  }
}

