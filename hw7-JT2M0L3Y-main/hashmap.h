//---------------------------------------------------------------------------
// NAME: Jonathan Smoley
// FILE: hashmap.h
// DATE: Spring 2022
// DESC: This file contains the class definition for a map-based hash table.
//---------------------------------------------------------------------------

#ifndef HASHMAP_H
#define HASHMAP_H

#include "map.h"
#include "arrayseq.h"

template<typename K, typename V>
class HashMap : public Map<K,V>
{
public:

  // default constructor
  HashMap();

  // copy constructor
  HashMap(const HashMap& rhs);

  // move constructor
  HashMap(HashMap&& rhs);

  // copy assignment
  HashMap& operator=(const HashMap& rhs);

  // move assignment
  HashMap& operator=(HashMap&& rhs);  

  // destructor
  ~HashMap();
  
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

  // Returns true if the key is in the collection, and false otherwise.
  bool contains(const K& key) const;

  // Returns the keys k in the collection such that k1 <= k <= k2
  ArraySeq<K> find_keys(const K& k1, const K& k2) const;

  // Returns the keys in the collection in ascending sorted order
  ArraySeq<K> sorted_keys() const;  

  // Gives the key (as an ouptput parameter) immediately after the
  // given key according to ascending sort order. Returns true if a
  // successor key exists, and false otherwise.
  bool next_key(const K& key, K& next_key) const; 

  // Gives the key (as an ouptput parameter) immediately before the
  // given key according to ascending sort order. Returns true if a
  // predecessor key exists, and false otherwise.
  bool prev_key(const K& key, K& next_key) const;

  // Removes all key-value pairs from the map. Does not change the
  // current capacity of the table.
  void clear();
  
  // statistics functions for the hash table implementation
  int min_chain_length() const;
  int max_chain_length() const;
  double avg_chain_length() const;
  
private:

  // node for linked-list separate chaining
  struct Node {
    K key;
    V value;
    Node* next;
  };

  // number of key-value pairs in map
  int count = 0;

  // max size of the (array) table
  int capacity = 16;

  // threshold for resize and rehash
  const double load_factor_threshold = 0.75;
  
  // array of linked lists
  Node** table = new Node*[capacity];

  // the hash function
  int hash(const K& key) const;

  // resize and rehash the table
  void resize_and_rehash();

  // initialize the table to all nullptr
  void init_table();
  
};

// HashMap Definitions

// default constructor
template<typename K, typename V>
HashMap<K,V>::HashMap()
{
  init_table();
}

// copy constructor
template<typename K, typename V>
HashMap<K,V>::HashMap(const HashMap& rhs)
{
  init_table();
  *this = rhs;
}

// move constructor
template<typename K, typename V>
HashMap<K,V>::HashMap(HashMap&& rhs)
{
  init_table();
  *this = std::move(rhs);
}

// copy assignment
template<typename K, typename V>
HashMap<K,V>& HashMap<K,V>::operator=(const HashMap<K,V>& rhs)
{
  if (this != &rhs)
  {
    Node** new_table = new Node* [rhs.capacity];
    for (int i = 0; i < rhs.capacity; ++i)
    {
      new_table[i] = nullptr;
    }
    for (int i = 0; i < rhs.capacity; ++i)
    {
      Node* temp = rhs.table[i];
      while(temp != nullptr)
      {
        Node* new_node = new Node();
        new_node->key = temp->key;
        new_node->value = temp->value;
        new_node->next = temp->next;
        if (new_table[i] == nullptr)
        {
          new_table[i] = new_node;
        }
        else
        {
          new_node->next = new_table[i];
          new_table[i] = new_node;
        }
        temp = temp->next;
      }
    }
    capacity = rhs.capacity;
    count = rhs.count;
    table = new_table;
  }
  return *this;
}

// move assignment
template<typename K, typename V>
HashMap<K,V>& HashMap<K,V>::operator=(HashMap<K,V>&& rhs)
{
  if (this != &rhs)
  {
    clear();
    count = rhs.count;
    capacity = rhs.capacity;
    for (int i = 0; i < rhs.capacity; ++i)
    {
      table[i] = std::move(rhs.table[i]);
    }
    rhs.clear();
  }
  return *this;
}

// destructor
template<typename K, typename V>
HashMap<K,V>::~HashMap()
{
  clear();
  delete[] table;
}

template<typename K, typename V>
int HashMap<K,V>::size() const
{
  return count;
}

template<typename K, typename V>
bool HashMap<K,V>::empty() const
{
  return count == 0;
}

template<typename K, typename V>
V& HashMap<K,V>::operator[](const K& key)
{
  // check out of range
  if (!contains(key))
  {
    throw std::out_of_range("Access[]: Out of range");
  }
  // hash to bucket
  int temp_val = hash(key);
  int ndx = temp_val % capacity;
  Node* temp = table[ndx];
  // iterate to proper node
  while (temp->key != key)
  {
    temp = temp->next;
  }
  // return value at node in bucket
  return temp->value;
}

template<typename K, typename V>
const V& HashMap<K,V>::operator[](const K& key) const
{
  // check out of range
  if (!contains(key))
  {
    throw std::out_of_range("Access[]: Out of range");
  }
  // hash to bucket
  int temp_val = hash(key);
  int ndx = temp_val % capacity;
  Node* temp = table[ndx];
  // iterate to proper node
  while (temp->key != key)
  {
    temp = temp->next;
  }
  // return value at node in bucket
  return temp->value;
}

template<typename K, typename V>
void HashMap<K,V>::insert(const K& key, const V& value)
{
  // check if load factor has been met/exceeded
  if ((double)count / capacity > load_factor_threshold)
  {
    resize_and_rehash();
  }
  // either way, hash to bucket
  ++count;
  int temp_val = hash(key);
  int ndx = temp_val % capacity;
  // create new node to add
  Node* new_key = new Node();
  new_key->key = key;
  new_key->value = value;
  // add to front of linked list
  if (table[ndx] == nullptr)
  {
    table[ndx] = new_key;
    new_key->next = nullptr;
  }
  else
  {
    Node* temp = table[ndx];
    new_key->next = temp;
    table[ndx] = new_key;
  }
}

template<typename K, typename V>
void HashMap<K,V>::erase(const K& key)
{
  // check out of range
  if (!contains(key))
  {
    throw std::out_of_range("Erase(): Out of range");
  }
  // hash to find proper index
  int temp_val = hash(key);
  int ndx = temp_val % capacity;
  if (table[ndx] != nullptr)
  {
    // find node in linked list
    if (table[ndx]->key == key)
    {
      // remove
      Node* temp = table[ndx];
      table[ndx] = table[ndx]->next;
      delete temp;
      --count;
    }
  }
}

template<typename K, typename V>
bool HashMap<K,V>::contains(const K& key) const
{
  // hash to bucket where key is located
  int temp_val = hash(key);
  int ndx = temp_val % capacity;
  Node* temp = table[ndx];
  // iterate over linked list
  while (temp != nullptr)
  {
    if (temp->key == key)
    {
      // return true when key is found
      return true;
    }
    temp = temp->next;
  }
  // return false when key is not found
  return false;
}

template<typename K, typename V>
ArraySeq<K> HashMap<K,V>::find_keys(const K& k1, const K& k2) const
{
  ArraySeq<K> keys;
  for (int i = 0; i < capacity; ++i)
  {
    Node* temp = table[i];

    while (temp != nullptr)
    {
      if (temp->key >= k1 && temp->key <= k2)
      {
        keys.insert(temp->key, 0);
      }
      temp = temp->next;
    }
  }
  return keys;
}

template<typename K, typename V>
ArraySeq<K> HashMap<K,V>::sorted_keys() const
{
  ArraySeq<K> keys;
  for (int i = 0; i < capacity; ++i)
  {
    Node* temp = table[i];
    while (temp != nullptr)
    {
      keys.insert(temp->key, keys.size());
      temp = temp->next;
    }
  }
  keys.sort();
  return keys;
}

template<typename K, typename V>
bool HashMap<K,V>::next_key(const K& key, K& next_key) const
{
  Node* temp = nullptr;
  bool first = false;

  for (int i = 0; i < capacity; ++i)
  {
    temp = table[i];
    while (temp != nullptr)
    {
      if (temp->key > key && !first)
      {
        next_key = temp->key;
        first = true;
      }
      else if (temp->key > key && temp->key < next_key)
      {
        next_key = temp->key;
      }
      temp = temp->next;
    }
  }
  return first;
}

template<typename K, typename V>
bool HashMap<K,V>::prev_key(const K& key, K& next_key) const
{
  Node* temp = nullptr;
  bool first = false;

  for (int i = 0; i < capacity; ++i)
  {
    temp = table[i];
    while (temp != nullptr)
    {
      if (temp->key < key && !first)
      {
        next_key = temp->key;
        first = true;
      }
      else if (temp->key < key && temp->key > next_key)
      {
        next_key = temp->key;
      }
      temp = temp->next;
    }
  }
  return first;
}

template<typename K, typename V>
void HashMap<K,V>::clear()
{
  if (*table != nullptr)
  {
    for (int i = 0; i < capacity; ++i)
    {
      if (table[i] != nullptr)
      {
        Node* curr = table[i];
        Node* next = nullptr;
        while (curr != nullptr)
        {
          next = curr->next;
          delete curr;
          curr = next;
        }
      }
    }
  }
  count = 0;
  capacity = 16;
}

template<typename K, typename V>
int HashMap<K,V>::min_chain_length() const
{
  int min = count;
  int possible_min;
  Node* temp;
  for (int i = 0; i < capacity; ++i)
  {
    possible_min = 0;
    temp = table[i];
    while (temp != nullptr)
    {
      ++possible_min;
      temp = temp->next;
    }
    if (possible_min < min && possible_min > 0)
    {
      min = possible_min;
    }
  }
  return min;
}

template<typename K, typename V>
int HashMap<K,V>::max_chain_length() const
{
  int max = 0;
  int possible_max;
  Node* temp;
  for (int i = 0; i < capacity; ++i)
  {
    possible_max = 0;
    temp = table[i];
    while (temp != nullptr)
    {
      ++possible_max;
      temp = temp->next;
    }
    if (possible_max > max && possible_max > 0)
    {
      max = possible_max;
    }
  }
  return max;
}

template<typename K, typename V>
double HashMap<K,V>::avg_chain_length() const
{
  int non_empty_count = 0;
  Node* temp = nullptr;
  for (int i = 0; i < capacity; ++i)
  {
    temp = table[i];
    while (temp != nullptr)
    {
      ++non_empty_count;
      temp = temp->next;
    }
  }
  return (double)non_empty_count / capacity;
  // return (double)count / (capacity / 2);
}

template<typename K, typename V>
int HashMap<K,V>::hash(const K& key) const
{
  std::hash<K> hashFunction;
  return hashFunction(key);
}

template<typename K, typename V>
void HashMap<K,V>::resize_and_rehash()
{
  // create new table with double capacity
  Node** new_table = new Node* [capacity * 2];
  for (int i = 0; i < capacity * 2; ++i)
  {
    new_table[i] = nullptr;
  }
  // fill old table contents into the new table
  for(int i = 0; i < capacity; ++i)
  {
    if (table[i] != nullptr)
    {
      Node* curr = table[i];
      while (curr != nullptr)
      {
        Node* new_node = curr;
        new_node->key = curr->key;
        new_node->value = curr->value;
        new_node->next = nullptr;
        // rehash into new table
        K new_key = curr->key;
        int ndx = hash(new_key) % (capacity * 2);

        if (new_table[ndx] == nullptr)
        {
          new_table[ndx] = new_node;
        }
        else
        {
          new_node->next = new_table[ndx];
          new_table[ndx]->next = new_node;
        }
        curr = curr->next;
      }
    }
  }
  // delete old table
  capacity = capacity * 2;
  delete[] table;
  table = new_table;
}

template<typename K, typename V>
void HashMap<K,V>::init_table()
{
  delete table;
  table = new Node* [capacity];
  for (int i = 0; i < capacity; ++i)
  {
    table[i] = nullptr;
  }
}

#endif
