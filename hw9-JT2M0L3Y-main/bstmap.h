//---------------------------------------------------------------------------
// NAME: Jonathan Smoley
// FILE: bstmap.h
// DATE: Spring 2022
// DESC: Binary Search Tree implementation of the Map class.
//---------------------------------------------------------------------------

#ifndef BSTMAP_H
#define BSTMAP_H

#include "map.h"
#include "arrayseq.h"

template<typename K, typename V>
class BSTMap : public Map<K,V>
{
public:

  // default constructor
  BSTMap();

  // copy constructor
  BSTMap(const BSTMap& rhs);

  // move constructor
  BSTMap(BSTMap&& rhs);

  // copy assignment
  BSTMap& operator=(const BSTMap& rhs);

  // move assignment
  BSTMap& operator=(BSTMap&& rhs);  

  // destructor
  ~BSTMap();
  
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
  bool prev_key(const K& key, K& prev_key) const; 

  // Removes all key-value pairs from the map.
  void clear();

  // Returns the height of the binary search tree
  int height() const;
  
private:

  // node for linked-list separate chaining
  struct Node {
    K key;
    V value;
    Node* left;
    Node* right;
  };

  // number of key-value pairs in map
  int count = 0;

  // array of linked lists
  Node* root = nullptr;

  // clean up the tree and reset count to zero given subtree root
  void clear(Node* st_root);

  // copy assignment helper
  Node* copy(const Node* rhs_st_root) const;
  
  // erase helper
  Node* erase(const K& key, Node* st_root);

  // find_keys helper
  void find_keys(const K& k1, const K& k2, const Node* st_root,
                 ArraySeq<K>& keys) const;

  // sorted_keys helper
  void sorted_keys(const Node* st_root, ArraySeq<K>& keys) const;

  // height helper
  int height(const Node* st_root) const;
  
};

template<typename K, typename V>
BSTMap<K,V>::BSTMap()
{
}

template<typename K, typename V>
BSTMap<K,V>::BSTMap(const BSTMap<K,V>& rhs)
{
  count = rhs.count;
  root = copy(rhs.root);
}

template<typename K, typename V>
BSTMap<K,V>::BSTMap(BSTMap<K,V>&& rhs)
{
  count = rhs.count;
  *this = std::move(rhs);
}

template<typename K, typename V>
BSTMap<K,V>& BSTMap<K,V>::operator=(const BSTMap<K,V>& rhs)
{
  // check for self-assignment
  if (this != &rhs)
  {
    // clear current tree
    clear(root);
    count = rhs.count;
    root = copy(rhs.root);
  }
  return *this;
}

template<typename K, typename V>
BSTMap<K,V>& BSTMap<K,V>::operator=(BSTMap<K,V>&& rhs)
{
  // check for self-assignment
  if (this != &rhs)
  {
    // clear current tree
    clear(root);
    root = rhs.root;
    rhs.root = nullptr;
    count = rhs.count;
    rhs.count = 0;
    clear(rhs.root);
  }
  return *this;
} 

template<typename K, typename V>
BSTMap<K,V>::~BSTMap()
{
  clear(root);
  count = 0;
  root = nullptr;
}
  
template<typename K, typename V>
int BSTMap<K,V>::size() const
{
  return count;
}

template<typename K, typename V>
bool BSTMap<K,V>::empty() const
{
  return count == 0;
}

template<typename K, typename V>
V& BSTMap<K,V>::operator[](const K& key)
{
  if (contains(key))
  {
    Node* curr = root;
    while (curr != nullptr)
    {
      if (curr->key == key)
      {
        break;
      }
      else if (curr->key < key)
      {
        curr = curr->right;
      }
      else
      {
        curr = curr->left;
      }
    }
    return curr->value;
  }
  else
  {
    throw std::out_of_range("Update[]: key not in map");
  }
}

template<typename K, typename V>
const V& BSTMap<K,V>::operator[](const K& key) const
{
  if (contains(key))
  {
    Node* curr = root;
    while (curr != nullptr)
    {
      if (curr->key == key)
      {
        break;
      }
      else if (curr->key < key)
      {
        curr = curr->right;
      }
      else
      {
        curr = curr->left;
      }
    }
    return curr->value;
  }
  else
  {
    throw std::out_of_range("Access[]: key not in map");
  }
}

template<typename K, typename V>
void BSTMap<K,V>::insert(const K& key, const V& value)
{
  Node* new_node = new Node;
  new_node->key = key;
  new_node->value = value;
  new_node->left = nullptr;
  new_node->right = nullptr;

  Node* curr = root;
  Node* prev = nullptr;
  while (curr != nullptr)
  {
    prev = curr;
    if (curr->key < key)
    {
      curr = curr->right;
    }
    else
    {
      curr = curr->left;
    }
  }

  if (prev == nullptr)
  {
    root = new_node;
  }
  else if (prev->key < key)
  {
    prev->right = new_node;
  }
  else
  {
    prev->left = new_node;
  }
  ++count;
}

template<typename K, typename V>
void BSTMap<K,V>::erase(const K& key)
{
  if (contains(key))
  {
    root = erase(key, root);
  }
  else
  {
    throw std::out_of_range("Erase(): key not in map");
  }
}

template<typename K, typename V>
bool BSTMap<K,V>::contains(const K& key) const
{
  Node* curr = root;
  while (curr != nullptr)
  {
    if (curr->key == key)
    {
      return true;
    }
    else if (curr->key < key)
    {
      curr = curr->right;
    }
    else
    {
      curr = curr->left;
    }
  }
  return false;
}

template<typename K, typename V>
ArraySeq<K> BSTMap<K,V>::find_keys(const K& k1, const K& k2) const
{
  ArraySeq<K> foundKeys;
  find_keys(k1, k2, root, foundKeys);
  return foundKeys;
}

template<typename K, typename V>
ArraySeq<K> BSTMap<K,V>::sorted_keys() const
{
  ArraySeq<K> keysSorted;
  sorted_keys(root, keysSorted);
  return keysSorted;
} 

template<typename K, typename V>
bool BSTMap<K,V>::next_key(const K& key, K& next_key) const
{
  if (count > 0)
  {
    Node* curr = root;
    bool found = false;
    K candidate = next_key;
    while (curr != nullptr)
    {
      if (found)
      {
        if (curr->key < candidate && curr->key > key)
        {
          candidate = curr->key;
        }
      }
      else
      {
        if (curr->key > key)
        {
          candidate = curr->key;
          found = true;
        }
      }
      if (curr->key <= key)
      {
        curr = curr->right;
      }
      else if (curr->key > key)
      {
        curr = curr->left;
      }
    }
    if (found)
    {
      next_key = candidate;
      return true;
    }
    else
    {
      return false;
    }
  }
  return false;
}

template<typename K, typename V>
bool BSTMap<K,V>::prev_key(const K& key, K& prev_key) const
{
  if (count > 0)
  {
    Node* curr = root;
    bool found = false;
    K candidate = prev_key;
    while (curr != nullptr)
    {
      if (found)
      {
        if (curr->key > candidate && curr->key < key)
        {
          candidate = curr->key;
        }
      }
      else
      {
        if (curr->key < key)
        {
          candidate = curr->key;
          found = true;
        }
      }

      if (curr->key >= key)
      {
        curr = curr->left;
      }
      else if (curr->key < key)
      {
        curr = curr->right;
      }
    }

    if (found)
    {
      prev_key = candidate;
      return true;
    }
    else
    {
      return false;
    }
  }
  return false;  
} 

template<typename K, typename V>
void BSTMap<K,V>::clear()
{
  clear(root);
}

template<typename K, typename V>
int BSTMap<K,V>::height() const
{
  return height(root);
}

template<typename K, typename V>
void BSTMap<K,V>::clear(BSTMap<K,V>::Node* st_root)
{
  if (st_root != nullptr)
  {
    clear(st_root->left);
    clear(st_root->right);
    st_root = nullptr;
  }
}

template<typename K, typename V>
typename BSTMap<K,V>::Node* BSTMap<K,V>::copy(const BSTMap<K,V>::Node* rhs_st_root) const
{
  if (rhs_st_root == nullptr)
  {
    return nullptr;
  }
  Node* new_node = new Node;
  new_node->key = rhs_st_root->key;
  new_node->value = rhs_st_root->value;
  new_node->left = copy(rhs_st_root->left);
  new_node->right = copy(rhs_st_root->right);
  return new_node;
}
  
template<typename K, typename V>
typename BSTMap<K,V>::Node* BSTMap<K,V>::erase(const K& key, BSTMap<K,V>::Node* st_root)
{
  if (st_root == nullptr)
  {
    return st_root;
  }
  if (st_root->key == key)
  {
    Node* temp = st_root;
    if (st_root->left == nullptr)
    {
      st_root = st_root->right;
    }
    else if (st_root->right == nullptr)
    {
      st_root = st_root->left;
    }
    else
    {
      Node* curr = st_root->right;
      Node* prev = st_root;
      while (curr->left != nullptr)
      {
        prev = curr;
        curr = curr->left;
      }
      temp->key = curr->key;
      temp->value = curr->value;
      if (prev == st_root)
      {
        prev->right = curr->right;
      }
      else
      {
        prev->left = curr->right;
      }
    }
    --count;
  }
  else if (st_root->key < key)
  {
    st_root->right = erase(key, st_root->right);
  }
  else
  {
    st_root->left = erase(key, st_root->left);
  }
  return st_root;
}

template<typename K, typename V>
void BSTMap<K,V>::find_keys(const K& k1, const K& k2, const BSTMap<K,V>::Node* st_root,
                 ArraySeq<K>& keys) const
{
  if (st_root != nullptr)
  {
    if (st_root->key >= k1)
    {
      find_keys(k1, k2, st_root->left, keys);
    }
    if (st_root->key >= k1 && st_root->key <= k2)
    {
      keys.insert(st_root->key, keys.size());
    }
    if (st_root->key <= k2)
    {
      find_keys(k1, k2, st_root->right, keys);
    }
  }
}

template<typename K, typename V>
void BSTMap<K,V>::sorted_keys(const BSTMap<K,V>::Node* st_root, ArraySeq<K>& keys) const
{
  if (st_root != nullptr)
  {
    const Node* curr = st_root;
    Node* temp = curr->left;
    while (temp != nullptr)
    {
      curr = temp;
      temp = curr->left;
    }
    K min = curr->key;

    curr = st_root;
    temp = curr->right;
    while (temp != nullptr)
    {
      curr = temp;
      temp = curr->right;
    }
    K max = curr->key;

    keys = find_keys(min, max);
  }
}

template<typename K, typename V>
int BSTMap<K,V>::height(const BSTMap<K,V>::Node* st_root) const
{
  if (st_root == nullptr)
  {
    return 0;
  }
  else
  {
    return 1 + std::max(height(st_root->left), height(st_root->right));
  }
}

#endif