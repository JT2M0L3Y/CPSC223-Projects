//---------------------------------------------------------------------------
// NAME: S. Bowers
// FILE: hw3_perf.cpp
// DATE: Spring 2022
// DESC: Simple performance test driver for HW-3. To run from the
//       command line use:
//          ./hw3_perf 
//       which will print out the corresponding HW-3 timing data.To
//       save this data to a file, run the command:
//          ./hw3_perf > output.dat
//       This file can then be used by the plotting script to generate
//       the corresponding performance graphs.
//---------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <chrono>
#include <cmath>
#include <functional>
#include <vector>
#include "util.h"
#include "arrayseq.h"

using namespace std;
using namespace std::chrono;

// insert calls
double timed_linked_list_insert_front(ArraySeq<int>& list);
double timed_cpp_vector_insert_front(vector<int>& list);
double timed_linked_list_insert_middle(ArraySeq<int>& list);
double timed_cpp_vector_insert_middle(vector<int>& list);
double timed_linked_list_insert_end(ArraySeq<int>& list);
double timed_cpp_vector_insert_end(vector<int>& list);
// update calls
double timed_linked_list_update_front(ArraySeq<int>& list);
double timed_cpp_vector_update_front(vector<int>& list);
double timed_linked_list_update_middle(ArraySeq<int>& list);
double timed_cpp_vector_update_middle(vector<int>& list);
double timed_linked_list_update_end(ArraySeq<int>& list);
double timed_cpp_vector_update_end(vector<int>& list);
// erase calls
double timed_linked_list_erase_front(ArraySeq<int>& list);
double timed_cpp_vector_erase_front(vector<int>& list);
double timed_linked_list_erase_middle(ArraySeq<int>& list);
double timed_cpp_vector_erase_middle(vector<int>& list);
double timed_linked_list_erase_end(ArraySeq<int>& list);
double timed_cpp_vector_erase_end(vector<int>& list);


// test parameters
const int start = 0;
const int step = 1000000; // 1,000,000
const int stop = 12000000; // 12,000,000
const int runs = 5;


int main(int argc, char* argv[])
{
  // configure output
  cout << fixed << showpoint;
  cout << setprecision(2);

  // output data header
  cout << "# All times in milliseconds (msec)" << endl;
  cout << "# Column 1 = input data size" << endl;
  cout << "# Column 2 = avg time arrayseq insert front" << endl;
  cout << "# Column 3 = avg time cpp-vector insert front" << endl;
  cout << "# Column 4 = avg time arrayseq insert middle" << endl;
  cout << "# Column 5 = avg time cpp-vector insert middle" << endl;
  cout << "# Column 6 = avg time arrayseq insert end" << endl;
  cout << "# Column 7 = avg time cpp-vector insert end" << endl;
  cout << "# Column 8 = avg time arrayseq update front" << endl;
  cout << "# Column 9 = avg time cpp-vector update front" << endl;
  cout << "# Column 10 = avg time arrayseq update middle" << endl;
  cout << "# Column 11 = avg time cpp-vector update middle" << endl;
  cout << "# Column 12 = avg time arrayseq update end" << endl;
  cout << "# Column 13 = avg time cpp-vector update end" << endl;
  cout << "# Column 14 = avg time arrayseq erase front" << endl;
  cout << "# Column 15 = avg time cpp-vector erase front" << endl;
  cout << "# Column 16 = avg time arrayseq erase middle" << endl;
  cout << "# Column 17 = avg time cpp-vector erase middle" << endl;
  cout << "# Column 18 = avg time arrayseq erase end" << endl;
  cout << "# Column 19 = avg time cpp-vector erase end" << endl;

  // build up the test case sizes including resize events
  vector<int> test_cases;
  int last_seen_p2 = 0;
  for (int n = start; n <= stop; n += step) {
    // get largest power of 2 less than size
    int p2_size = pow(2, (int)floor(log2(n)));
    if (p2_size > last_seen_p2) {
      test_cases.push_back(p2_size);
      last_seen_p2 = p2_size;
    }
    test_cases.push_back(n);
  }

  
  // run tests and print test results
  for (int size : test_cases) {
    ArraySeq<int>* list1 = new ArraySeq<int>;
    vector<int>* list2 = new vector<int>(0);
    // build up linked list
    for (int i = 0; i < size; ++i) {
      list1->insert(i+1, i);
      list2->push_back(i+1);
    }
    
    double c2 = timed_linked_list_insert_front(*list1);
    double c3 = timed_cpp_vector_insert_front(*list2);
    double c4 = timed_linked_list_insert_middle(*list1);
    double c5 = timed_cpp_vector_insert_middle(*list2);
    double c6 = timed_linked_list_insert_end(*list1);
    double c7 = timed_cpp_vector_insert_end(*list2);
    double c8 = timed_linked_list_update_front(*list1);
    double c9 = timed_cpp_vector_update_front(*list2);
    double c10 = timed_linked_list_update_middle(*list1);
    double c11 = timed_cpp_vector_update_middle(*list2);
    double c12 = timed_linked_list_update_end(*list1);
    double c13 = timed_cpp_vector_update_end(*list2);
    double c14 = timed_linked_list_erase_front(*list1);
    double c15 = timed_cpp_vector_erase_front(*list2);
    double c16 = timed_linked_list_erase_middle(*list1);
    double c17 = timed_cpp_vector_erase_middle(*list2);
    double c18 = timed_linked_list_erase_end(*list1);
    double c19 = timed_cpp_vector_erase_end(*list2);
    
    cout << size << " " << c2 << " " << c3 << " " << c4 << " "
         << c5 << " " << c6 << " " << c7  << " " << c8 << " "
         << c9 << " " << c10 << " " << c11 << " " << c12 << " "
         << c13 << " " << c14 << " " << c15 << " " << c16 << " "
         << c17 << " " << c18 << " " << c19 << endl;

    delete list1;
    delete list2;
  }
}

// timing functions

double timed_linked_list_insert_front(ArraySeq<int>& list)
{
  double total = 0;
  int n = list.size();
  for (int r = 0; r < runs; ++r) {
    auto t0 = high_resolution_clock::now();
    list.insert(n+1, 0);
    auto t1 = high_resolution_clock::now();
    total += duration_cast<milliseconds>(t1 - t0).count();
  }
  return total / runs;
}

double timed_cpp_vector_insert_front(vector<int>& list)
{
  double total = 0;
  int n = list.size();
  for (int r = 0; r < runs; ++r) {
    auto t0 = high_resolution_clock::now();
    list.insert(list.begin() + 0, n+1);
    auto t1 = high_resolution_clock::now();
    total += duration_cast<milliseconds>(t1 - t0).count();
  }
  return total / runs;
}

double timed_linked_list_insert_middle(ArraySeq<int>& list)
{
  double total = 0;
  int n = list.size();
  for (int r = 0; r < runs; ++r) {
    auto t0 = high_resolution_clock::now();
    list.insert(n+1, n/2);
    auto t1 = high_resolution_clock::now();
    total += duration_cast<milliseconds>(t1 - t0).count();
  }
  return total / runs;
}

double timed_cpp_vector_insert_middle(vector<int>& list)
{
  double total = 0;
  int n = list.size();
  for (int r = 0; r < runs; ++r) {
    auto t0 = high_resolution_clock::now();
    auto it = list.begin();
    advance(it, n/2);
    list.insert(it, n+1);
    auto t1 = high_resolution_clock::now();
    total += duration_cast<milliseconds>(t1 - t0).count();
  }
  return total / runs;
}

double timed_linked_list_insert_end(ArraySeq<int>& list)
{
  double total = 0;
  int n = list.size();
  for (int r = 0; r < runs; ++r) {
    auto t0 = high_resolution_clock::now();
    list.insert(n+r+1, n+r);
    auto t1 = high_resolution_clock::now();
    total += duration_cast<milliseconds>(t1 - t0).count();
  }
  return total / runs;
}

double timed_cpp_vector_insert_end(vector<int>& list)
{
  double total = 0;
  int n = list.size();
  for (int r = 0; r < runs; ++r) {
    auto t0 = high_resolution_clock::now();
    list.push_back(n+r+1);
    auto t1 = high_resolution_clock::now();
    total += duration_cast<milliseconds>(t1 - t0).count();
  }
  return total / runs;
}

double timed_linked_list_update_front(ArraySeq<int>& list)
{
  double total = 0;
  int n = list.size();
  for (int r = 0; r < runs; ++r) {
    auto t0 = high_resolution_clock::now();
    list[0] = n+1;
    auto t1 = high_resolution_clock::now();
    total += duration_cast<milliseconds>(t1 - t0).count();
  }
  return total / runs;
}

double timed_cpp_vector_update_front(vector<int>& list)
{
  double total = 0;
  int n = list.size();
  for (int r = 0; r < runs; ++r) {
    auto t0 = high_resolution_clock::now();
    list.front() = n+1;
    auto t1 = high_resolution_clock::now();
    total += duration_cast<milliseconds>(t1 - t0).count();
  }
  return total / runs;
}

double timed_linked_list_update_middle(ArraySeq<int>& list)
{
  double total = 0;
  int n = list.size();
  for (int r = 0; r < runs; ++r) {
    auto t0 = high_resolution_clock::now();
    list[n/2] = n+1;
    auto t1 = high_resolution_clock::now();
    total += duration_cast<milliseconds>(t1 - t0).count();
  }
  return total / runs;
}

double timed_cpp_vector_update_middle(vector<int>& list)
{
  double total = 0;
  int n = list.size();
  for (int r = 0; r < runs; ++r) {
    auto t0 = high_resolution_clock::now();
    if (list.size() > 0)
      list[n/2] = n + 1;
    auto t1 = high_resolution_clock::now();
    total += duration_cast<milliseconds>(t1 - t0).count();
  }
  return total / runs;
}

double timed_linked_list_update_end(ArraySeq<int>& list)
{
  double total = 0;
  int n = list.size();
  for (int r = 0; r < runs; ++r) {
    auto t0 = high_resolution_clock::now();
    list[n-1] = n+1;
    auto t1 = high_resolution_clock::now();
    total += duration_cast<milliseconds>(t1 - t0).count();
  }
  return total / runs;
}

double timed_cpp_vector_update_end(vector<int>& list)
{
  double total = 0;
  int n = list.size();
  for (int r = 0; r < runs; ++r) {
    auto t0 = high_resolution_clock::now();
    list.back() = n+1;
    auto t1 = high_resolution_clock::now();
    total += duration_cast<milliseconds>(t1 - t0).count();
  }
  return total / runs;
}

double timed_linked_list_erase_front(ArraySeq<int>& list)
{
  double total = 0;
  int n = list.size();
  for (int r = 0; r < runs; ++r) {
    auto t0 = high_resolution_clock::now();
    list.erase(0);
    auto t1 = high_resolution_clock::now();
    total += duration_cast<milliseconds>(t1 - t0).count();
  }
  return total / runs;
}

double timed_cpp_vector_erase_front(vector<int>& list)
{
  double total = 0;
  int n = list.size();
  for (int r = 0; r < runs; ++r) {
    auto t0 = high_resolution_clock::now();
    list.erase(list.begin() + 0);
    auto t1 = high_resolution_clock::now();
    total += duration_cast<milliseconds>(t1 - t0).count();
  }
  return total / runs;
}

double timed_linked_list_erase_middle(ArraySeq<int>& list)
{
  double total = 0;
  int n = list.size();
  for (int r = 0; r < runs; ++r) {
    auto t0 = high_resolution_clock::now();
    list.erase(n/2);
    auto t1 = high_resolution_clock::now();
    total += duration_cast<milliseconds>(t1 - t0).count();
  }
  return total / runs;
}

double timed_cpp_vector_erase_middle(vector<int>& list)
{
  double total = 0;
  int n = list.size();
  for (int r = 0; r < runs; ++r) {
    auto t0 = high_resolution_clock::now();
    auto it = list.begin();
    advance(it, n/2);
    list.erase(it);
    auto t1 = high_resolution_clock::now();
    total += duration_cast<milliseconds>(t1 - t0).count();
  }
  return total / runs;
}

double timed_linked_list_erase_end(ArraySeq<int>& list)
{
  double total = 0;
  int n = list.size();
  for (int r = 0; r < runs; ++r) {
    auto t0 = high_resolution_clock::now();
    list.erase((n-1)-r);
    auto t1 = high_resolution_clock::now();
    total += duration_cast<milliseconds>(t1 - t0).count();
  }
  return total / runs;
}

double timed_cpp_vector_erase_end(vector<int>& list)
{
  double total = 0;
  int n = list.size();
  for (int r = 0; r < runs; ++r) {
    auto t0 = high_resolution_clock::now();
    list.pop_back();
    auto t1 = high_resolution_clock::now();
    total += duration_cast<milliseconds>(t1 - t0).count();
  }
  return total / runs;
}
