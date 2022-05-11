//---------------------------------------------------------------------------
// FILE: hw1_test.cpp
// NAME: Jonathan Smoley
// DATE: Spring 2022
// DESC: This .cpp file implements 5 tests for 3 test 
//       cases (bubble, insertion, and selection sort).
//---------------------------------------------------------------------------

#include <iostream>
#include <string>
#include <gtest/gtest.h>
#include "simple_sorts.h"


/* Your job for this file is to:

    (1) Fill in the file header comment above

    (2) Implement 2 tests as described below for each sorting
        algorithm (so, 6 tests in total)

    (3) Get all of the tests to pass (including the 6 you write)

    (4) Make sure you remove all TODO comments

    (5) Make sure you comment the tests you write 

    (6) Optionally write additional tests to cover more cases for your
        algorithms

    NOTE: To disable a test, prefix the test name with DISABLED_ E.g.,
          DISABLED_EmptyArray. You should not disable any tests for
          your final submission. However, it can be helfpul when
          working on your code (especially if the test is causing
          hitting a segmentation fault in your code.
*/

//---------------------------------------------------------------------------
// Bubble Sort Tests
//---------------------------------------------------------------------------

TEST(BasicBubbleSortTest, EmptyArray)
{
  int array[] {};
  bubble_sort(array, 0);
}

TEST(BasicBubbleSortTest, ReverseOrder)
{
  int array[] {4, 3, 2, 1};
  bubble_sort(array, 4);
  for (int i = 0; i < 3; ++i)
    ASSERT_LE(array[i], array[i+1]);
}

TEST(BasicBubbleSortTest, InOrder)
{
  int array[] {1, 2, 3, 4};
  bubble_sort(array, 4);
  for (int i = 0; i < 3; ++i)
    ASSERT_LE(array[i], array[i+1]);
}

TEST(BasicBubbleSortTest, PartiallyUnorderedList)
{
  int array[] {1, 3, 2, 4}; // only the middle two values are out of order
  bubble_sort(array, 4);
  for (int i = 0; i < 3; ++i)
    ASSERT_LE(array[i], array[i+1]);
}

TEST(BasicBubbleSortTest, UnorderedList)
{
  int array[] {3, 1, 4, 2}; // every value is out of order
  bubble_sort(array, 4);
  for (int i = 0; i < 3; ++i)
    ASSERT_LE(array[i], array[i+1]);
}


//---------------------------------------------------------------------------
// Insertion Sort Tests
//---------------------------------------------------------------------------

TEST(BasicInsertionSortTest, EmptyArray)
{
  int array[] {};
  insertion_sort(array, 0);
}

TEST(BasicInsertionSortTest, ReverseOrder)
{
  int array[] {4, 3, 2, 1};
  insertion_sort(array, 4);
  for (int i = 0; i < 3; ++i)
    ASSERT_LE(array[i], array[i+1]);
}

TEST(BasicInsertionSortTest, InOrder)
{
  int array[] {1, 2, 3, 4};
  insertion_sort(array, 4);
  for (int i = 0; i < 3; ++i)
    ASSERT_LE(array[i], array[i+1]);
}

TEST(BasicInsertionSortTest, PartiallyUnorderedList)
{
  int array[] {1, 3, 2, 4}; // only the middle two values are out of order
  insertion_sort(array, 4);
  for (int i = 0; i < 3; ++i)
    ASSERT_LE(array[i], array[i+1]);
}

TEST(BasicInsertionSortTest, UnorderedList)
{
  int array[] {3, 1, 4, 2}; // every value is out of order
  insertion_sort(array, 4);
  for (int i = 0; i < 3; ++i)
    ASSERT_LE(array[i], array[i+1]);
}


//---------------------------------------------------------------------------
// Selection Sort Tests
//---------------------------------------------------------------------------

TEST(BasicSelectionSortTest, EmptyArray)
{
  int array[] {};
  selection_sort(array, 0);
}

TEST(BasicSelectionSortTest, ReverseOrder)
{
  int array[] {4, 3, 2, 1};
  selection_sort(array, 4);
  for (int i = 0; i < 3; ++i)
    ASSERT_LE(array[i], array[i+1]);
}

TEST(BasicSelectionSortTest, InOrder)
{
  int array[] {1, 2, 3, 4};
  selection_sort(array, 4);
  for (int i = 0; i < 3; ++i)
    ASSERT_LE(array[i], array[i+1]);
}

TEST(BasicSelectionSortTest, PartiallyUnorderedList)
{
  int array[] {1, 3, 2, 4}; // only the middle two values are out of order
  selection_sort(array, 4);
  for (int i = 0; i < 3; ++i)
    ASSERT_LE(array[i], array[i+1]);
}

TEST(BasicSelectionSortTest, UnorderedList)
{
  int array[] {3, 1, 4, 2}; // every value is out of order
  selection_sort(array, 4);
  for (int i = 0; i < 3; ++i)
    ASSERT_LE(array[i], array[i+1]);
}


//---------------------------------------------------------------------------
// Main
//----------------------------------------------------------------------

int main(int argc, char* argv[])
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
