//---------------------------------------------------------------------------
// NAME: Jonathan Smoley
// FILE: avlmap.h
// DATE: Spring 2022
// DESC: AVL Tree implemented using the map class as an extention of a BST
//      to include a way to keep the tree balanced.
//---------------------------------------------------------------------------

#ifndef AVLMAP_H
#define AVLMAP_H

#include "map.h"
#include "arrayseq.h"

template<typename K, typename V>
class AVLMap : public Map<K,V> 
{
public:

  // default constructor
  AVLMap();

  // copy constructor
  AVLMap(const AVLMap& rhs);

  // move constructor
  AVLMap(AVLMap&& rhs);

  // copy assignment
  AVLMap& operator=(const AVLMap& rhs);

  // move assignment
  AVLMap& operator=(AVLMap&& rhs);  

  // destructor
  ~AVLMap();
  
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

  // helper to print the tree for debugging
  void print() const;

private:

  // node for linked-list separate chaining
  struct Node {
    K key;
    V value;
    int height;
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

  // insert helper
  Node* insert(const K& key, const V& value, Node* st_root);
  
  // erase helper
  Node* erase(const K& key, Node* st_root);

  // find_keys helper
  void find_keys(const K& k1, const K& k2, const Node* st_root,
                 ArraySeq<K>& keys) const;

  // sorted_keys helper
  void sorted_keys(const Node* st_root, ArraySeq<K>& keys) const;

  // rotations
  Node* rotate_right(Node* k2);
  Node* rotate_left(Node* k2);

  // rebalance
  Node* rebalance(Node* st_root);

  // print helper
  void print(std::string indent, const Node* st_root) const;
};

template<typename K, typename V>
void AVLMap<K,V>::print() const
{
  print(std::string(""), root);
}

template<typename K, typename V>
void AVLMap<K,V>::print(std::string indent, const Node* st_root) const
{
  if (!st_root)
    return;
  std::cout << st_root->key << " (" << st_root->height << ")" <<  std::endl;
  if (st_root->left) {
    std::cout << indent << " lft: ";
    print(indent + " ", st_root->left);
  }
  if (st_root->right) {
    std::cout << indent << " rgt: ";
    print(indent + " ", st_root->right);
  }
}

// public AVL function definitions

template<typename K, typename V>
AVLMap<K,V>::AVLMap()
{
}

// copy constructor
template<typename K, typename V>
AVLMap<K,V>::AVLMap(const AVLMap<K,V>& rhs)
{
  count = rhs.count;
  root = copy(rhs.root);
}

// move constructor
template<typename K, typename V>
AVLMap<K,V>::AVLMap(AVLMap<K,V>&& rhs)
{
  count = rhs.count;
  *this = std::move(rhs);
}

// copy assignment
template<typename K, typename V>
AVLMap<K,V>& AVLMap<K,V>::operator=(const AVLMap<K,V>& rhs)
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
AVLMap<K,V>& AVLMap<K,V>::operator=(AVLMap<K,V>&& rhs)
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
AVLMap<K,V>::~AVLMap()
{
  clear();
}
  
// Returns the number of key-value pairs in the map
template<typename K, typename V>
int AVLMap<K,V>::size() const
{
  return count;
}

// Tests if the map is empty
template<typename K, typename V>
bool AVLMap<K,V>::empty() const
{
  return count == 0;
}

// Allows values associated with a key to be updated. Throws
// out_of_range if the given key is not in the collection.
template<typename K, typename V>
V& AVLMap<K,V>::operator[](const K& key)
{
  Node* curr = root;
  while (curr != nullptr)
  {
    if (key == curr->key)
    {
      return curr->value;
    }
    else if (key < curr->key)
    {
      curr = curr->left;
    }
    else
    {
      curr = curr->right;
    }
  }
  throw std::out_of_range("Update[]: key not in map");
}

// Returns the value for a given key. Throws out_of_range if the
// given key is not in the collection.
template<typename K, typename V> 
const V& AVLMap<K,V>::operator[](const K& key) const
{
  Node* curr = root;
  while (curr != nullptr)
  {
    if (key == curr->key)
    {
      return curr->value;
    }
    else if (key < curr->key)
    {
      curr = curr->left;
    }
    else
    {
      curr = curr->right;
    }
  }
  throw std::out_of_range("Access[]: key not in map");
}

// Extends the collection by adding the given key-value pair.
// Expects key to not exist in map prior to insertion.
template<typename K, typename V>
void AVLMap<K,V>::insert(const K& key, const V& value)
{
  root = insert(key, value, root);
  ++count;
}

// Shrinks the collection by removing the key-value pair with the
// given key. Does not modify the collection if the collection does
// not contain the key. Throws out_of_range if the given key is not
// in the collection.
template<typename K, typename V>
void AVLMap<K,V>::erase(const K& key)
{
  if (contains(key))
  {
    root = erase(key, root);
    --count;
  }
  else
  {
    throw std::out_of_range("Erase(): key not in map");
  }
}

// Returns true if the key is in the collection, and false otherwise.
template<typename K, typename V>
bool AVLMap<K,V>::contains(const K& key) const
{
  Node* curr = root;
  while (curr != nullptr)
  {
    if (key == curr->key)
    {
      return true;
    }
    else if (key < curr->key)
    {
      curr = curr->left;
    }
    else
    {
      curr = curr->right;
    }
  }
  return false;
}

// Returns the keys k in the collection such that k1 <= k <= k2
template<typename K, typename V>
ArraySeq<K> AVLMap<K,V>::find_keys(const K& k1, const K& k2) const
{
  ArraySeq<K> keys;
  find_keys(k1, k2, root, keys);
  return keys;
}

// Returns the keys in the collection in ascending sorted order
template<typename K, typename V>
ArraySeq<K> AVLMap<K,V>::sorted_keys() const
{
  ArraySeq<K> keys;
  sorted_keys(root, keys);
  return keys;
}

// Gives the key (as an ouptput parameter) immediately after the
// given key according to ascending sort order. Returns true if a
// successor key exists, and false otherwise.
template<typename K, typename V>
bool AVLMap<K,V>::next_key(const K& key, K& next_key) const
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

// Gives the key (as an ouptput parameter) immediately before the
// given key according to ascending sort order. Returns true if a
// predecessor key exists, and false otherwise.
template<typename K, typename V>
bool AVLMap<K,V>::prev_key(const K& key, K& prev_key) const
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

// Removes all key-value pairs from the map.
template<typename K, typename V>
void AVLMap<K,V>::clear()
{
  clear(root);
  count = 0;
}

// Returns the height of the binary search tree
template<typename K, typename V>
int AVLMap<K,V>::height() const
{
  if (root != nullptr)
  {
    return root->height;
  }
  else
  {
    return 0;
  }
}

// private AVL helper function definitions

// clean up the tree and reset count to zero given subtree root
template<typename K, typename V>
void AVLMap<K,V>::clear(Node* st_root)
{
  if (st_root != nullptr)
  {
    clear(st_root->left);
    clear(st_root->right);
    st_root = nullptr;
  }
}

// copy assignment helper
template<typename K, typename V>
typename AVLMap<K,V>::Node* AVLMap<K,V>::copy(const Node* rhs_st_root) const
{
  if (rhs_st_root == nullptr)
  {
    return nullptr;
  }
  Node* new_node = new Node;
  new_node->key = rhs_st_root->key;
  new_node->value = rhs_st_root->value;
  new_node->height = rhs_st_root->height;
  new_node->left = copy(rhs_st_root->left);
  new_node->right = copy(rhs_st_root->right);
  return new_node;
}

// insert helper
template<typename K, typename V>
typename AVLMap<K,V>::Node* AVLMap<K,V>::insert(const K& key, const V& value, Node* st_root)
{
  if (st_root == nullptr)
  {
    Node* new_node = new Node;
    new_node->key = key;
    new_node->value = value;
    new_node->left = nullptr;
    new_node->right = nullptr;
    new_node->height = 1;
    return new_node;
  }
  else
  {
    if (key < st_root->key)
    {
      st_root->left = insert(key, value, st_root->left);
    }
    else
    {
      st_root->right = insert(key, value, st_root->right);
    }
    // update height via backtrack to root
    if (st_root->left != nullptr && st_root->right != nullptr)
    {
      st_root->height = std::max(st_root->left->height, st_root->right->height) + 1;
    }
    else if (st_root->left != nullptr && st_root->right == nullptr)
    {
      st_root->height = st_root->left->height + 1;
    }
    else
    {
      st_root->height = st_root->right->height + 1;
    }
  }
  return rebalance(st_root);
}

// erase helper
template<typename K, typename V>
typename AVLMap<K,V>::Node* AVLMap<K,V>::erase(const K& key, Node* st_root)
{
  if (key < st_root->key)
  {
    st_root->left = erase(key, st_root->left);
  }
  else if (key > st_root->key)
  {
    st_root->right = erase(key, st_root->right);
  }
  // if key is at current node, replace with one child
  else if (key == st_root->key)
  {
    if (st_root->left == nullptr)
    {
      // replace current node with right child
      Node* temp = st_root;
      st_root = st_root->right;
      delete temp;
    }
    else if (st_root->right == nullptr)
    {
      // replace current node with left child
      Node* temp = st_root;
      st_root = st_root->left;
      delete temp;
    }
    // otherwise, do complicated erase
    else
    {
      // find inorder successor
      Node* successor = st_root->right;
      Node* prev = st_root;
      while (successor->left != nullptr)
      {
        prev = successor;
        successor = successor->left;
      }
      st_root->key = successor->key;
      st_root->value = successor->value;
      // erase successor in right subtree
      st_root->right = erase(successor->key, st_root->right);
      if (prev != st_root)
      {
        prev->left = successor->right;
      }
      else
      {
        prev->right = successor->right;
      }
    }
  }
  // update height via backtrack to root
  if (st_root != nullptr)
  {
    if (st_root->left != nullptr && st_root->right != nullptr)
    {
      st_root->height = std::max(st_root->left->height, st_root->right->height) + 1;
    }
    else if (st_root->left != nullptr && st_root->right == nullptr)
    {
      st_root->height = st_root->left->height + 1;
    }
    else if (st_root->left == nullptr && st_root->right != nullptr)
    {
      st_root->height = st_root->right->height + 1;
    }
    else
    {
      st_root->height = 1;
    }
  }
  return rebalance(st_root);
}

// find_keys helper
template<typename K, typename V>
void AVLMap<K,V>::find_keys(const K& k1, const K& k2, const Node* st_root,
              ArraySeq<K>& keys) const
{
  if (st_root != nullptr)
  {
    if (k1 <= st_root->key)
    {
      find_keys(k1, k2, st_root->left, keys);
    }
    if (k1 <= st_root->key && k2 >= st_root->key)
    {
      keys.insert(st_root->key, keys.size());
    }
    if (k2 >= st_root->key)
    {
      find_keys(k1, k2, st_root->right, keys);
    }
  }
}

// sorted_keys helper
template<typename K, typename V>
void AVLMap<K,V>::sorted_keys(const Node* st_root, ArraySeq<K>& keys) const
{
  if (st_root != nullptr)
  {
    sorted_keys(st_root->left, keys);
    keys.insert(st_root->key, keys.size());
    sorted_keys(st_root->right, keys);
  }
}

// rotation right
template<typename K, typename V>
typename AVLMap<K,V>::Node* AVLMap<K,V>::rotate_right(Node* k2)
{
  // perform rotation on k2 (current root)
  Node* k1 = k2->left;
  k2->left = k1->right;
  k1->right = k2;
  // update height of k2 (k1 right subtree)
  int l_height = 0;
  int r_height = 0;
  if (k2->left != nullptr)
  {
    l_height = k2->left->height;
  }
  if (k2->right != nullptr)
  {
    r_height = k2->right->height;
  }
  if (l_height > r_height)
  {
    k2->height = l_height + 1;
  }
  else
  {
    k2->height = r_height + 1;
  }
  // update height of k1 (new root of tree)
  l_height = 0;
  r_height = 0;
  if (k1->left != nullptr)
  {
    l_height = k1->left->height;
  }
  if (k1->right != nullptr)
  {
    r_height = k1->right->height;
  }
  if (l_height > r_height)
  {
    k1->height = l_height + 1;
  }
  else
  {
    k1->height = r_height + 1;
  }
  return k1;
}

// rotation left
template<typename K, typename V>
typename AVLMap<K,V>::Node* AVLMap<K,V>::rotate_left(Node* k2)
{
  // perform rotation on k2 (current root)
  Node* k1 = k2->right;
  k2->right = k1->left;
  k1->left = k2;
  // update height of k2 (k1 left subtree)
  int l_height = 0;
  int r_height = 0;
  if (k2->left != nullptr)
  {
    l_height = k2->left->height;
  }
  if (k2->right != nullptr)
  {
    r_height = k2->right->height;
  }
  if (l_height > r_height)
  {
    k2->height = l_height + 1;
  }
  else
  {
    k2->height = r_height + 1;
  }
  // update height of k1 (new root of tree)
  l_height = 0;
  r_height = 0;
  if (k1->left != nullptr)
  {
    l_height = k1->left->height;
  }
  if (k1->right != nullptr)
  {
    r_height = k1->right->height;
  }
  if (l_height > r_height)
  {
    k1->height = l_height + 1;
  }
  else
  {
    k1->height = r_height + 1;
  }
  return k1;
}

// rebalance
template<typename K, typename V>
typename AVLMap<K,V>::Node* AVLMap<K,V>::rebalance(Node* st_root)
{
  if (st_root == nullptr)
  {
    return st_root;
  }

  // separate left and right subtrees
  Node* left_sub = st_root->left;
  Node* right_sub = st_root->right;
  // store heights of left and right subtrees
  int bal_fact = 0;
  int l_height = 0;
  int r_height = 0;
  if (left_sub != nullptr)
  {
    l_height = left_sub->height;
  }
  if (right_sub != nullptr)
  {
    r_height = right_sub->height;
  }
  // update balance factor
  bal_fact = l_height - r_height;
  // if left exists and right does not and left height is more than 1
  if (left_sub != nullptr && right_sub == nullptr && left_sub->height > 1)
  {
    if (left_sub->right != nullptr)
    {
      left_sub = rotate_left(left_sub);
      st_root->left = left_sub;
    }
    st_root = rotate_right(st_root);
  }
  // if right exists and left does not and right height is more than 1
  else if (right_sub != nullptr && left_sub == nullptr && right_sub->height > 1)
  {
    if (right_sub->left != nullptr)
    {
      right_sub = rotate_right(right_sub);
      st_root->right = right_sub;
    }
    st_root = rotate_left(st_root);
  }
  // if both left and right exist and balance factor is more than 1
  else if (left_sub != nullptr && right_sub != nullptr && bal_fact > 1)
  {
    if (left_sub->right->height > left_sub->left->height)
    {
      left_sub = rotate_left(left_sub);
      st_root->left = left_sub;
    }
    st_root = rotate_right(st_root);
  }
  // if both left and right exist and balance factor is less than -1
  else if (left_sub != nullptr && right_sub != nullptr && bal_fact < -1)
  {
    if (right_sub->left->height > right_sub->right->height)
    {
      right_sub = rotate_right(right_sub);
      st_root->right = right_sub;
    }
    st_root = rotate_left(st_root);
  }
  return st_root;
}

#endif