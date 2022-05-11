//---------------------------------------------------------------------------
// NAME: Jonathan Smoley
// FILE: btreemap.h
// DATE: Spring 2022
// DESC: Map implementation using a 2-3-4 B-Tree 
//---------------------------------------------------------------------------

#ifndef BTreeMAP_H
#define BTreeMAP_H

#include "map.h"
#include "arrayseq.h"

template<typename K, typename V>
class BTreeMap : public Map<K,V>
{
public:

  // default constructor
  BTreeMap();

  // copy constructor
  BTreeMap(const BTreeMap& rhs);

  // move constructor
  BTreeMap(BTreeMap&& rhs);

  // copy assignment
  BTreeMap& operator=(const BTreeMap& rhs);

  // move assignment
  BTreeMap& operator=(BTreeMap&& rhs);  

  // destructor
  ~BTreeMap();
  
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

  // Removes all key-value pairs from the map.
  void clear();
  
  // Returns the height of the binary search tree
  int height() const;

  // for debugging the tree
  void print() const {
    print("  ", root, height());
  }

  
private:

  // node for 2-3-4 tree
  struct Node {
    ArraySeq<std::pair<K,V>> keyvals;
    ArraySeq<Node*> children;
    // helper functions
    bool full() const {return keyvals.size() == 3;}
    bool leaf() const {return children.empty();}
    K key(int i) const {return keyvals[i].first;}
    V& val(int i) {return keyvals[i].second;}
    Node* child(int i) const {return children[i];}
  };

  // number of key-value pairs in map
  int count = 0;

  // root node
  Node* root = nullptr;

  // print helper function
  void print(std::string indent, Node* st_root, int levels) const;
  
  // clean up the tree memory 
  void clear(Node* st_root);

  // helper function for copy assignment
  Node* copy(const Node* rhs_st_root) const;

  // split the parent's i-th child
  void split(Node* parent, int i);
  
  // erase helpers
  void erase(Node* st_root, const K& key);
  void remove_internal(Node* st_root, int key_idx);
  void rebalance(Node* st_root, int key_idx, int& child_idx);
  
  // find_keys helper
  void find_keys(const K& k1, const K& k2, const Node* st_root,
                 ArraySeq<K>& keys) const;

  // sorted_keys helper
  void sorted_keys(const Node* st_root, ArraySeq<K>& keys) const;

  // height helper
  int height(const Node* st_root) const;
  
};

template<typename K, typename V>
void BTreeMap<K,V>::print(std::string indent, Node* st_root, int levels) const {
  if (levels == 0)
    return;
  if (!st_root)
    return;
  std::cout << indent << "("; 
  for (int i = 0; i < 3; ++i) {
    if (i != 0)
      std::cout << ",";
    if (st_root->keyvals.size() > i)
      std::cout << st_root->key(i); 
    else
      std::cout << "-";
  }
  std::cout << ")" << std::endl;
  if (levels > 1) {
    for (int i = 0; i < st_root->children.size(); ++i)
      print(indent + " ", st_root->child(i), levels - 1);
  }
}

// * BTREEMAP INTERFACE

// default constructor
template<typename K, typename V>
BTreeMap<K,V>::BTreeMap()
{
}

// copy constructor
template<typename K, typename V>
BTreeMap<K,V>::BTreeMap(const BTreeMap<K,V>& rhs)
{
  count = rhs.count;
  root = copy(rhs.root);
}

// move constructor
template<typename K, typename V>
BTreeMap<K,V>::BTreeMap(BTreeMap<K,V>&& rhs)
{
  count = rhs.count;
  root = std::move(rhs.root);
}

// copy assignment
template<typename K, typename V>
BTreeMap<K,V>& BTreeMap<K,V>::operator=(const BTreeMap<K,V>& rhs)
{
  if (this != &rhs)
  {
    clear(root);
    root = copy(rhs.root);
    count = rhs.count;
  }
  return *this;
}

// move assignment
template<typename K, typename V>
BTreeMap<K,V>& BTreeMap<K,V>::operator=(BTreeMap<K,V>&& rhs)
{
  if (this != &rhs)
  {
    clear(root);
    count = rhs.count;
    root = rhs.root;
    rhs.root = nullptr;
    rhs.count = 0;
  }
  return *this;
}

// destructor
template<typename K, typename V>
BTreeMap<K,V>::~BTreeMap()
{
  clear();
}
  
// size (i.e. number of key-value pairs)
template<typename K, typename V>
int BTreeMap<K,V>::size() const
{
  return count;
}

// empty (i.e. whether the count of key-value pairs is 0)
template<typename K, typename V>
bool BTreeMap<K,V>::empty() const
{
  return count == 0;
}

// update operator
template<typename K, typename V>
V& BTreeMap<K,V>::operator[](const K& key)
{
  Node* curr = root;
  while (curr)
  {
    for (int i = 0; i < curr->keyvals.size(); ++i)
    {
      if (curr->key(i) == key)
      {
        return curr->val(i);
      }
      else if (curr->key(i) > key)
      {
        curr = curr->child(i);
      }
      else if ((i == 3) && (curr->key(i + 1)))
      {
        curr = curr->child(i + 1);
      }
    }
  }
  throw std::out_of_range("Update[]: key not found");
}

// access operator
template<typename K, typename V>
const V& BTreeMap<K,V>::operator[](const K& key) const
{
  Node* curr = root;
  while (curr)
  {
    for (int i = 0; i < curr->keyvals.size(); ++i)
    {
      if (curr->key(i) == key)
      {
        return curr->val(i);
      }
      else if (curr->key(i) > key)
      {
        curr = curr->child(i);
      }
      else if ((i == 3) && (curr->key(i + 1)))
      {
        curr = curr->child(i + 1);
      }
    }
  }
  throw std::out_of_range("Access[]: key not found");
}

// insert base
template<typename K, typename V>
void BTreeMap<K,V>::insert(const K& key, const V& value)
{
  // base case: empty tree
  if (!root)
  {
    // create new root and return
    Node* temp = new Node;
    temp->keyvals.insert({key, value}, 0);
    ++count;
    root = temp;
    return;
  }
  // base case: root is full, so split
  if (root->full())
  {
    Node* left = root;
    root = new Node();
    root->children.insert(left, 0);
    split(root, 0);
  }
  // naviage to leaf, split full nodes as necessary
  Node* prev = root;
  Node* curr = root;
  while (curr->full() || !(curr->leaf()))
  {
    // split the node
    if (curr->full())
    {
      // split the node and restart search from prev
      for (int i = 0; i < prev->keyvals.size(); ++i)
      {
        if (key < prev->key(i))
        {
          split(prev, i);
          i = prev->children.size();
        }
        else if (i == prev->children.size() - 1)
        {
          split(prev, i + 1);
          i = prev->children.size();
        }
      }
      curr = prev;
    }
    prev = curr;
    // follow appropriate child
    for (int i = 0; i < curr->keyvals.size(); ++i)
    {
      if (key < curr->key(i))
      {
        curr = curr->child(i);
        i = curr->keyvals.size();
      }
      else if (i == curr->keyvals.size() - 1)
      {
        curr = curr->child(i + 1);
        i = curr->keyvals.size();
      }
    }
  }
  // insert the key-value pair
  for (int i = 0; i < curr->keyvals.size(); ++i)
  {
    if (key < curr->key(i))
    {
      curr->keyvals.insert({key, value}, i);
      ++count;
      return;
    }
    else if (i == curr->keyvals.size() - 1)
    {
      curr->keyvals.insert({key, value}, i + 1);
      ++count;
      return;
    }
  }
}

// erase base
template<typename K, typename V>
void BTreeMap<K,V>::erase(const K& key)
{
  erase(root, key);
  if (root->keyvals.empty())
  {
    Node* left = nullptr;
    // check if one child left
    if (root->children.size() > 0)
    {
      left = root->child(0);
    }
    delete root;
    root = left;
  }
  --count;
}

// contains (i.e. whether the key is in the map)
template<typename K, typename V>
bool BTreeMap<K,V>::contains(const K& key) const
{
  Node* curr = root;
  while (curr != nullptr)
  {
    for (int i = 0; i < curr->keyvals.size(); ++i)
    {
      if (curr->key(i) == key)
      {
        return true;
      }
    }
    curr = curr->child(key);
  }
  return false;
}

// find_keys base
template<typename K, typename V>
ArraySeq<K> BTreeMap<K,V>::find_keys(const K& k1, const K& k2) const
{
  ArraySeq<K> keys;
  find_keys(k1, k2, root, keys);
  return keys;
}

// sorted_keys base
template<typename K, typename V>
ArraySeq<K> BTreeMap<K,V>::sorted_keys() const
{
  ArraySeq<K> keys;
  sorted_keys(root, keys);
  return keys;
}

// next_key (i.e. in order successor of key parameter)
template<typename K, typename V>
bool BTreeMap<K,V>::next_key(const K& key, K& next_key) const
{
  if (count > 0)
  {
    Node* curr = root;
    bool found = false;
    K candidate = next_key;
    while (curr != nullptr)
    {
      int i;
      for (i = 0; i < curr->keyvals.size(); ++i)
      {
        if (found)
        {
          if (curr->key(i) < candidate && curr->key(i) > key)
          {
            candidate = curr->key(i);
          }
        }
        else
        {
          if (curr->key(i) > key)
          {
            candidate = curr->key(i);
            found = true;
          }
        }
      }

      if (curr->key(i) > key)
      {
        curr = curr->child(i);
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

// prev_key (i.e. in order predecessor of key parameter)
template<typename K, typename V>
bool BTreeMap<K,V>::prev_key(const K& key, K& next_key) const
{
  if (count > 0)
  {
    Node* curr = root;
    bool found = false;
    K candidate = next_key;
    while (curr != nullptr)
    {
      int i;
      for (i = 0; i < curr->keyvals.size(); ++i)
      {
        if (found)
        {
          if (curr->key(i) > candidate && curr->key(i) < key)
          {
            candidate = curr->key(i);
          }
        }
        else
        {
          if (curr->key(i) < key)
          {
            candidate = curr->key(i);
            found = true;
          }
        }
      }

      if (curr->key(i) < key)
      {
        curr = curr->child(i);
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

// clear base
template<typename K, typename V>
void BTreeMap<K,V>::clear()
{
  clear(root);
  root = nullptr;
  count = 0;
}

// height base
template<typename K, typename V>
int BTreeMap<K,V>::height() const
{
  return height(root);
}

// * BTREEMAP IMPLEMENTATION

// clear helper
template<typename K, typename V>
void BTreeMap<K,V>::clear(Node* st_root)
{
  if (st_root)
  {
    for (int i = 0; i < st_root->children.size(); ++i)
    {
      clear(st_root->child(i));
    }
    delete st_root;
  }
}

// copy assignment helper
template<typename K, typename V>
typename BTreeMap<K,V>::Node* BTreeMap<K,V>::copy(const Node* rhs_st_root) const
{
  if (rhs_st_root)
  {
    Node* st_root = new Node(*rhs_st_root);
    for (int i = 0; i < rhs_st_root->children.size(); ++i)
    {
      st_root->children[i] = copy(rhs_st_root->child(i));
    }
    return st_root;
  }
  return nullptr;
}

// split (i.e. split a full node into two)
template<typename K, typename V>
void BTreeMap<K,V>::split(Node* parent, int i)
{
  Node* left = parent->child(i);
  std::pair<K,V> mid_pair {left->key(1), left->val(1)};

  Node* right = new Node;
  std::pair<K,V> right_pair {left->key(2), left->val(2)};

  right->keyvals.insert(right_pair, 0);

  parent->keyvals.insert(mid_pair, i);

  parent->children.insert(right, i + 1);

  left->keyvals.erase(1);
  left->keyvals.erase(1);

  if (left->children.size() >= 3)
  {
    right->children.insert(left->child(2), 0);
    left->children.erase(2);
  }
  if (left->children.size() >= 3)
  {
    right->children.insert(left->child(2), 1);
    left->children.erase(2);
  }
}

// erase helper
/*
  CASES:
  1) leaf node & key is inside
      - just remove key from leaf node
  2) internal node & key is inside
    a) left child has at least 2 keys
      - replace key with in order predecessor
      - erase predecessor from left child
    b) left child has only 1 key, but the right
      child has 2 or more keys
      - replace key with in order successor
      - erase successor from right child
    c) left and right children each have 1 key (merge case)
      - remove key from node (x)
      - merge key & right child key into left child
      - delete right child (check cases again)
  3) internal node & key is not there
      - determine the subtree, rooted at c_i, that
        the key should be in
      - if c_i has only 1 key, do case a or b
        (a and b are rebalance cases)
      - continue search from c_i
    a) c_i has an immediate left/right sibling w/
      at least 2 keys
    b) c_i's immediate left & right each only have 1 key
*/
template<typename K, typename V>
void BTreeMap<K,V>::erase(Node* st_root, const K& key)
{
  while (st_root != nullptr)
  {
    // case 1: leaf node
    if (st_root->leaf())
    {
      // do leaf case stuff
    }
    else
    {
      // find the k_i <= key
      // check if case 2:
      //  case 2: key is internal node
      //    call remove_internal(st_root, key_index);
      // else keep searching, rebalancing as needed
      //    use rebalance to check case 3
      //    rebalance(st_root, key_index, child_index);
      //    resume search from child
      //    st_root = st_root->child(child_index);
    }
  }
  throw std::out_of_range("Erase(): key not found");
}

// remove_internal (i.e. merge a node with its sibling) case 2 helper
template<typename K, typename V>
void BTreeMap<K,V>::remove_internal(Node* st_root, int key_idx)
{

}

// rebalance (i.e. keep node heights balanced) case 3 helper
template<typename K, typename V>
void BTreeMap<K,V>::rebalance(Node* st_root, int key_idx, int& child_idx)
{

}

// find_keys helper
template<typename K, typename V>
void BTreeMap<K,V>::find_keys(const K& k1, const K& k2, const Node* st_root,
                    ArraySeq<K>& keys) const
{

}

// sorted_keys helper
template<typename K, typename V>
void BTreeMap<K,V>::sorted_keys(const Node* st_root, ArraySeq<K>& keys) const
{

}

// height helper
template<typename K, typename V>
int BTreeMap<K,V>::height(const Node* st_root) const
{
  if (st_root)
  {
    int max_height = 0;
    for (int i = 0; i < st_root->children.size(); ++i)
    {
      int ht = height(st_root->child(i));
      if (ht > max_height)
      {
        max_height = ht;
      }
    }
    return max_height + 1;
  }
  return 0;
}

#endif
