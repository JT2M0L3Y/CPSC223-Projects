//---------------------------------------------------------------------------
// NAME: Jonathan Smoley
// FILE: binsearchmap.h
// DATE: Spring 2022
// DESC: Header file for a binary search version of the ArrayMap class.
//---------------------------------------------------------------------------

#ifndef BINSEARCHMAP_H
#define BINSEARCHMAP_H

#include "map.h"
#include "arrayseq.h"

template<typename K, typename V>
class BinSearchMap : public Map<K,V>
{
public:

  // Returns the number of key-value pairs in the map
  int size() const;

  // Tests if the map is empty
  bool empty() const;

  // Allows values associated with a key to be updated. Throws
  // out_of_range if the given key is not in the collection.
  V& operator[](const K& key);

  // Returns the value for a given key. Throws out_of_range if the
  // given key is not in the collection. 
  const V& operator[](const K& key) const;

  // Extends the collection by adding the given key-value
  // pair. Assumes the key being added is not present in the
  // collection. Insert does not check if the key is present.
  void insert(const K& key, const V& value);

  // Shrinks the collection by removing the key-value pair with the
  // given key. Does not modify the collection if the collection does
  // not contain the key. Throws out_of_range if the given key is not
  // in the collection.
  void erase(const K& key);

  // Returns true if the key is in the collection, and false
  // otherwise.
  bool contains(const K& key) const;

  // Returns the keys k in the collection such that k1 <= k <= k2
  ArraySeq<K> find_keys(const K& k1, const K& k2) const;

  // Returns the keys in the collection in ascending sorted order.
  ArraySeq<K> sorted_keys() const;

  // Gives the key (as an ouptput parameter) immediately after the
  // given key according to ascending sort order. Returns true if a
  // successor key exists, and false otherwise.
  bool next_key(const K& key, K& next_key) const;

  // Gives the key (as an ouptput parameter) immediately before the
  // given key according to ascending sort order. Returns true if a
  // predecessor key exists, and false otherwise.
  bool prev_key(const K& key, K& prev_key) const; 

  // Removes all key-value pairs from the map.
  void clear();
  

private:

  // If the key is in the collection, bin_search returns true and
  // provides the key's index within the array sequence (via the index
  // output parameter). If the key is not in the collection,
  // bin_search returns false and provides the last index checked by
  // the binary search algorithm. 
  bool bin_search(const K& key, int& index) const;
  
  // implemented as a resizable array of (key-value) pairs
  ArraySeq<std::pair<K,V>> seq;

};

// BinSearchMap Definitions

template<typename K, typename V>
int BinSearchMap<K,V>::size() const
{
  // grab number of pairs in the sequence
  return seq.size();
}

template<typename K, typename V>
bool BinSearchMap<K,V>::empty() const
{
  // check if no pairs in sequence
  return seq.empty();
}

template<typename K, typename V>
V& BinSearchMap<K,V>::operator[](const K& key)
{
  int ndx = -1;
  // use bin_search directly
  bin_search(key, ndx);
  if (seq[ndx].first == key)
  {
    // return value at index
    return seq[ndx].second;
  }
  // otherwise, throw out_of_range
  else
  {
    throw std::out_of_range("update[]: key not found");
  }
}

template<typename K, typename V>
const V& BinSearchMap<K,V>::operator[](const K& key) const
{
  int ndx = -1;
  // use bin_search directly
  bin_search(key, ndx);
  if (seq[ndx].first == key)
  {
    // return value at index
    return seq[ndx].second;
  }
  // otherwise, throw out_of_range
  else
  {
    throw std::out_of_range("access[]: key not found");
  }
}

template<typename K, typename V>
void BinSearchMap<K,V>::insert(const K& key, const V& value)
{
  int ndx = 0;
  std::pair<K,V> p {key, value};
  if (seq.empty())
  {
    seq.insert(p, ndx);
  }
  else
  {
    bin_search(key, ndx);
    if (key < seq[ndx].first)
    {
      seq.insert(p, ndx);
    }
    else
    {
      seq.insert(p, ndx + 1);
    }
  }
}

template<typename K, typename V>
void BinSearchMap<K,V>::erase(const K& key)
{
  int ndx = 0;
  // bin_search to find correct ndx
  if (bin_search(key, ndx))
  {
    // erase pair at ndx
    seq.erase(ndx);
  }
  // otherwise, throw out_of_range
  else
  {
    throw std::out_of_range("erase(): key not found");
  }
}

template<typename K, typename V>
bool BinSearchMap<K,V>::contains(const K& key) const
{
  // return true if key in collection, false otherwise
  int ndx = 0;
  return bin_search(key, ndx);
}

template<typename K, typename V>
ArraySeq<K> BinSearchMap<K,V>::find_keys(const K& k1, const K& k2) const
{
  ArraySeq<K> keys;
  int ndx = 0;
  bin_search(k1, ndx);
  if (!seq.empty() && seq[ndx].first < k1)
  {
    ++ndx;
  }
  for (int i = ndx; i < seq.size(); ++i)
  {
    if (seq[i].first > k2)
    {
      return keys;
    }
    keys.insert(seq[i].first, 0);
  }
  return keys;
}

template<typename K, typename V>
ArraySeq<K> BinSearchMap<K,V>::sorted_keys() const
{
  // return current keys in ascending order
  ArraySeq<K> keys;
  for (int i = 0; i < seq.size(); ++i)
  {
    // add key to sequence
    keys.insert(seq[i].first, keys.size());
  }
  return keys;
}

template<typename K, typename V>
bool BinSearchMap<K,V>::next_key(const K& key, K& next_key) const
{
  // given key, look for next key after key
  int ndx;
  bin_search(key, ndx);
  if (!seq.empty() && seq[ndx].first > key)
  {
    next_key = seq[ndx].first;
    return true;
  }
  else
  {
    if (ndx + 1 < seq.size())
    {
      next_key = seq[ndx + 1].first;
      return true;
    }
  }
  return false;
}

template<typename K, typename V>
bool BinSearchMap<K,V>::prev_key(const K& key, K& prev_key) const
{
  // given key, look for next key after key
  int ndx;
  bin_search(key, ndx);
  if (seq[ndx].first < key)
  {
    prev_key = seq[ndx].first;
    return true;
  }
  else
  {
    if (ndx - 1 >= 0)
    {
      prev_key = seq[ndx - 1].first;
      return true;
    }
  }
  return false;
}

template<typename K, typename V>
void BinSearchMap<K,V>::clear()
{
  // remove all pairs from sequence
  seq.clear();
}

template<typename K, typename V>
bool BinSearchMap<K,V>::bin_search(const K& key, int& index) const
{
  // check base case
  if (seq.empty())
  {
    return false;
  }
  else
  {
    // set indices to iterate toward mid
    int first = 0;
    int last = seq.size() - 1;
    int mid = 0;
    bool found = false;
    // iterate until indices meet or key is found
    while (first <= last)
    {
      mid = (first + last) / 2;
      index = mid;
      if (seq[mid].first == key)
      {
        return true;
      }
      else if (seq[mid].first < key)
      {
        first = mid + 1;
      }
      else
      {
        last = mid - 1;
      }
    }
    return false;
  }
}

#endif