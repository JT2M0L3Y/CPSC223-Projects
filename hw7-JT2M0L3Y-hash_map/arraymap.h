//---------------------------------------------------------------------------
// NAME: Jonathan Smoley
// DATE: Spring 2022
// DESC: My class definition for the Map ADT using prior ArraySeq class.
//---------------------------------------------------------------------------

#ifndef ARRAYMAP_H
#define ARRAYMAP_H

#include "map.h"
#include "arrayseq.h"

template<typename K, typename V>
class ArrayMap : public Map<K,V>
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

  // Extends the collection by adding the given key-value pair.
  // Expects key to not exist in map prior to insertion.
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
  bool prev_key(const K& key, K& next_key) const;

  // Removes all key-value pairs from the map.
  void clear();

private:

  // implemented as a resizable array of (key-value) pairs
  ArraySeq<std::pair<K,V>> seq;

};

// ArrayMap Definitions

template<typename K, typename V>
int ArrayMap<K,V>::size() const
{
  // count number of key-value pairs in the map
  return seq.size();
}

template<typename K, typename V>
bool ArrayMap<K,V>::empty() const
{
  // check if the map is empty
  return seq.empty();
}

template<typename K, typename V>
V& ArrayMap<K,V>::operator[](const K& key)
{
  // return the value associated with the given key
  for (int i = 0; i < seq.size(); ++i)
  {
    if (seq[i].first == key)
    {
      return seq[i].second;
    }
  }
  // throw out of range error if key is not in the map
  throw std::out_of_range("update[]: key not found in map");
}

template<typename K, typename V>
const V& ArrayMap<K,V>::operator[](const K& key) const
{
  // return the value associated with the given key
  for (int i = 0; i < seq.size(); ++i)
  {
    if (seq[i].first == key)
    {
      return seq[i].second;
    }
  }
  // throw out of range error if key is not in the map
  throw std::out_of_range("access[]: key not found in map");
}

template<typename K, typename V>
void ArrayMap<K,V>::insert(const K& key, const V& value)
{
  // add key-value pair to end of map
  seq.insert({key, value}, seq.size());
}

template<typename K, typename V>
void ArrayMap<K,V>::erase(const K& key)
{
  // throw out of range if key not in map
  if (!contains(key))
  {
    throw std::out_of_range("erase(): key not in map");
  }
  // modify only if key in map
  else
  {
    // remove key-value pair with given key
    for (int i = 0; i < seq.size(); ++i)
    {
      if (seq[i].first == key)
      {
        seq.erase(i);
        return;
      }
    }
  }
}

template<typename K, typename V>
bool ArrayMap<K,V>::contains(const K& key) const
{
  // return true if key in map, false otherwise
  for (int i = 0; i < seq.size(); ++i)
  {
    if (seq[i].first == key)
    {
      return true;
    }
  }
  return false;
}

template<typename K, typename V>
ArraySeq<K> ArrayMap<K,V>::find_keys(const K& k1, const K& k2) const
{
  // return keys between k1 <= k <= k2
  ArraySeq<K> keys;
  for (int i = 0; i < seq.size(); ++i)
  {
    if (seq[i].first >= k1 && seq[i].first <= k2)
    {
      keys.insert(seq[i].first, keys.size());
    }
  }
  return keys;
}

template<typename K, typename V>
ArraySeq<K> ArrayMap<K,V>::sorted_keys() const
{
  // return current keys in ascending order
  ArraySeq<K> keys;
  for (int i = 0; i < seq.size(); ++i)
  {
    keys.insert(seq[i].first, keys.size());
  }
  keys.sort();
  return keys;
}

template<typename K, typename V>
bool ArrayMap<K,V>::next_key(const K& key, K& next_key) const
{
  // given key, look for first key greater than key and exit
  K temp_key;
  bool found = false;
  for (int i = 0; i < seq.size(); ++i)
  {
    if (seq[i].first > key)
    {
      temp_key = seq[i].first;
      break;
    }
  }
  for (int i = 0; i < seq.size(); ++i)
  {
    if (seq[i].first > key && seq[i].first <= temp_key)
    {
      temp_key = seq[i].first;
      found = true;
    }
  }
  if (found)
  {
    next_key = temp_key;
    return true;
  }
  else
  {
    return false;
  }
}

template<typename K, typename V>
bool ArrayMap<K,V>::prev_key(const K& key, K& next_key) const
{
  // given key, look for first key smaller than it and exit
  K temp_key;
  bool found = false;
  for (int i = seq.size() - 1; i >= 0; --i)
  {
    if (seq[i].first < key)
    {
      temp_key = seq[i].first;
      break;
    }
  }
  for (int i = seq.size() - 1; i >= 0; --i)
  {
    if (seq[i].first < key && seq[i].first >= temp_key)
    {
      temp_key = seq[i].first;
      found = true;
    }
  }
  if (found)
  {
    next_key = temp_key;
    return true;
  }
  else
  {
    return false;
  }
}

template<typename K, typename V>
void ArrayMap<K,V>::clear()
{
  // remove all key-value pairs from the map
  seq.clear();
}

#endif
