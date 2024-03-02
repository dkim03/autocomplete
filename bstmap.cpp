// Yusuf Pisan pisan@uw.edu
// 17 Jan 2021

// BST class
// Creates a BST to store values
// Uses Node which holds the data

#include "bstmap.h"
#include <algorithm>
#include <cassert>

using namespace std;

// default constructor to initialize num nodes
BSTMap::BSTMap() : num_nodes(0) {}

// copy constructor
BSTMap::BSTMap(const BSTMap &bst) {
  clear(); // delete whatever is in the tree before assignment
  if (!bst.empty()) {
    // put copies of all elements in bst to vector
    vector<value_type> vec;
    bstToVector(vec, bst.root); // convert to vector
    root = vectorToBST(vec, 0, static_cast<int>(vec.size() - 1)); // convert vector to bst and assign
    num_nodes = static_cast<int>(vec.size());
  }
}

// given an array of length n
// create a tree to have all items in that array
// with the minimum height (uses same helper as rebalance)
BSTMap::BSTMap(const vector<value_type> &vec) {
  if (!vec.empty()) {
    num_nodes = static_cast<int>(vec.size()); // set number of nodes to number of elements in vec

    // create a deep copy of vector
    vector<value_type> vecCopy;
    vecCopy.assign(vec.begin(), vec.end());

    // sort vector in ascending order
    sort(vecCopy.begin(), vecCopy.end(), sortStringAscending);

    // // FOR TESTING
    // cout << "sorted: " << endl;
    // for (value_type elem : vecCopy) {
    //     cout << elem.first << endl;
    // }
  
    // using sorted vector, convert to bst to get balanced tree
    root = vectorToBST(vecCopy, 0, static_cast<int>(vecCopy.size() - 1));
  }
}

// destructor
BSTMap::~BSTMap() { clear(); }

// delete all nodes in tree
void BSTMap::clear() {
  clearHelper(root);
  root = nullptr;
  num_nodes = 0;
}

// traverses bst and deletes nodes in postorder
void BSTMap::clearHelper(Node* &root) const {
  if (root == nullptr) { return; }
  clearHelper(root->left);
  clearHelper(root->right);
  // cout << "before" << endl;
  delete root;
  // cout << "after" << endl;
}

// helper for copy constructor
void BSTMap::bstToVector(vector<value_type> &vec, const Node* root) const {
  copyHelper(vec, root);
}

// traverses bst and puts elements into given vector in order
void BSTMap::copyHelper(vector<value_type> &vec, const Node* root) const {
  if (root != nullptr) {
    copyHelper(vec, root->left);
    vec.push_back(root->data);
    copyHelper(vec, root->right);
  }
}

// helper function for rebalance and bstmap vector constructor
BSTMap::Node* BSTMap::vectorToBST(const vector<value_type> &vec, int start, int end) {
    if (start > end) {
        return nullptr;
    }
    int mid = (start + end) / 2;

    // create new node for every node
    Node* root = new Node(vec[mid]);

    root->left = vectorToBST(vec, start, mid - 1);
    root->right = vectorToBST(vec, mid + 1, end);
    return root;
}

// balance the BST by saving all nodes to a vector inorder
// and then recreating the BST from the vector
void BSTMap::rebalance() {
  vector<value_type> vec;
  bstToVector(vec, root);
  clear(); // delete anything tied to the root before assignment
  root = vectorToBST(vec, 0, static_cast<int>(vec.size() - 1));
}

// // returns true if item successfully erased - BONUS
// bool BSTMap::erase(const key_type &k) {
//   if (empty()) { return false; } // if tree is empty
//   if (!contains(k)) { return false; } // if k does not exist in bst
//   return eraseHelper(k, root);
// }

// // helper function for erase, works recursively
// bool BSTMap::eraseHelper(const key_type &key, Node* root) {

// }

// true if no nodes in BST
bool BSTMap::empty() const {
  return (root == nullptr);
}

// number of nodes in BST
int BSTMap::size() const {
  return num_nodes;
}

// true if item is in BST
bool BSTMap::contains(const key_type &key) const {
  return containsHelper(key, root);
}

// traverses bst and compares key to each element
bool BSTMap::containsHelper(const key_type &key, const Node* root) const {
  if (root == nullptr) {
      return false;
  }

  // compare key with the data at current node
  if (key == root->data.first) { // if key is found
      return true;
  }
  if (key < root->data.first) { // if less than, go left
      return containsHelper(key, root->left);
  } 
  return containsHelper(key, root->right); // if greater, go right

  // // compare key with the data at current node
  // if (key == root->data.first) { // if key is found
  //     return true;
  // } else if (key < root->data.first) { // if less than, go left
  //     return containsHelper(key, root->left);
  // } else { // if greater, go right
  //     return containsHelper(key, root->right);
  // }
}

// If k matches the key returns a reference to its mapped value
// If k does not match any key, inserts a new element
// with that key and returns a reference to its mapped value.
BSTMap::mapped_type &BSTMap::operator[](const key_type &k) {
  if (root == nullptr) {
      // if the tree is empty, create a new root node
      root = new Node({k, mapped_type{}});

      // the bane of my existence is below this line
      // root = new Node; // 
      // root->data = {k, mapped_type{}};
      // -> leads to segmentation fault

      num_nodes++;
      return root->data.second;
  }

  if (contains(k)) { // if key is found, return reference to the mapped value of that key element
    return retrieve(k, root);
  } // if bst is not empty and k does not match
  Node* newNode = new Node({k, mapped_type{}}); // create new node and data and store k value
  
  Node* nodePtr = root; // create pointer to traverse bst
  bool placed = false;
  while (!placed) {
    // if there exists an empty spot, place accordingly
    // otherwise, keep traversing
    if (k < nodePtr->data.first && nodePtr->left == nullptr) {
      nodePtr->left = newNode;
      placed = true;
    } else if (k > nodePtr->data.first && nodePtr->right == nullptr) {
      nodePtr->right = newNode;
      placed = true;
    } else if (k < nodePtr->data.first) { // go left if k is less than current element
      nodePtr = nodePtr->left;
    } else { // go right if k is greater than element
      nodePtr = nodePtr->right;
    }
  }

  num_nodes++; // increment due to insertion of new element
  return newNode->data.second;
}

// helper function for operator[], returns address of mapped_type corresponding
// to a given key
BSTMap::mapped_type& BSTMap::retrieve(const key_type &key, Node* root) const {
  if (root != nullptr) {
    if (key == root->data.first) { // if found, return data
      return root->data.second;
    }
    if (key < root->data.first) { // if key is less than current element, go left
      return retrieve(key, root->left);
    }
    if (key > root->data.first) { // if key is greater than current element, go right
      return retrieve(key, root->right);
    }
  }
  return root->data.second;
}

// returns a vector of key-value pairs that partially match the key
// Main function used by autocomplete
// Might traverse both left and right subbranches of a node
// Example: getall("hel")
// Return: { (hello, 10), (help, 20)}
vector<BSTMap::value_type> BSTMap::getAll(const key_type &k) const {
  vector<value_type> v; // vector to store matching phrases
  getAllHelper(k, v, root);

  // // FOR TESTING
  // cout << v.size() << endl;
  // for (BSTMap::value_type elem : v) {
  //   cout << "[" << elem.first << ", " << elem.second << "]" << endl;
  // }

  return v;
}

// helper function for getAll()
void BSTMap::getAllHelper(const key_type &key, vector<value_type>& vec, const Node* root) const {
  if (root != nullptr) { // check if null
  // if the length of the element string is greater or equal
  // to the prefix length, begin comparing characters up to
  // the prefix length
  // if any mismatching characters are found or the size of the
  // string is less than the prefix, traverse tree and repeat process
    bool isMatch = true;
    if (root->data.first.size() >= key.size()) {
      for (int i = 0; i < key.size(); i++) {
        if (root->data.first[i] != key[i]) {
          isMatch = false;
          break;
        }
      }
    } else { // if prefix is larger than element, do not add element
      isMatch = false;
    }
    if (isMatch) { vec.push_back(root->data); }
    getAllHelper(key, vec, root->left);
    getAllHelper(key, vec, root->right);
  }
}

// 0 if empty, 1 if only root, otherwise
// height of root is max height of subtrees + 1
int BSTMap::height() const {
  if (empty()) { return 0; }
  if (root->left == nullptr && root->right == nullptr) { return 1; }
  return heightHelper(root) + 1;
}

// helper function for height
int BSTMap::heightHelper(Node* root) const {
  if (root == nullptr) { return -1; }
  int leftHeight = heightHelper(root->left);
  int rightHeight = heightHelper(root->right);

  return 1 + max(leftHeight, rightHeight);
}

// same as contains, but returns 1 or 0
// compatibility with std::map
size_t BSTMap::count(const string &key) const {
  if (contains(key)) { return 1; }
  return 0;
}

// inorder traversal: left-root-right
// takes a function that takes a single parameter of type T
void BSTMap::inorder(void visit(const value_type &item)) const {
  inorderHelper(root, visit);
}

// helper function for recursive inrder traversal
void BSTMap::inorderHelper(const Node* root, void visit(const value_type &item)) const {
  if (root != nullptr) { // check if null
    inorderHelper(root->left, visit);
    visit(root->data);
    inorderHelper(root->right, visit);
  }
}

// preorder traversal: root-left-right
void BSTMap::preorder(void visit(const value_type &item)) const {
  preorderHelper(root, visit);
}

// helper function for recursive preorder traversal
void BSTMap::preorderHelper(const Node* root, void visit(const value_type &item)) const {
  if (root != nullptr) { // check if null
    visit(root->data);
    preorderHelper(root->left, visit);
    preorderHelper(root->right, visit);
  }
}

// postorder traversal: left-right-root
void BSTMap::postorder(void visit(const value_type &item)) const {
  postorderHelper(root, visit);
}

// helper function for recursive postorder traversal
void BSTMap::postorderHelper(const Node* root, void visit(const value_type &item)) const {
  if (root != nullptr) { // check if null
    postorderHelper(root->left, visit);
    postorderHelper(root->right, visit);
    visit(root->data);
  }
}

// trees are equal if they have the same structure
// AND the same item values at all the nodes
bool BSTMap::operator==(const BSTMap &other) const {
  // check for nullptr and return false if one of bstmaps is nullptr
  if (root == nullptr && other.root != nullptr) { return false; }
  if (root != nullptr && other.root == nullptr) { return false; }
  if (height() != other.height()) { return false; }
  if (num_nodes != other.num_nodes) { return false; }
  
  // convert both trees to vectors and compare keys and mapped_types
  vector<value_type> thisVec;
  vector<value_type> otherVec;
  bstToVector(thisVec, root);
  bstToVector(otherVec, other.root);

  // compare all elements of this to other element
  for (int i = 0; i < thisVec.size(); i++) {
    if (thisVec[i].first != otherVec[i].first) { return false; }
    if (thisVec[i].second != otherVec[i].second) { return false; }
  }
  return true;
}

// not == to each other
bool BSTMap::operator!=(const BSTMap &other) const {
  return !(*this == other);
}

// height of a Node, nullptr is 0, root is 1, static, no access to 'this'
// helper function to height(), used by printVertical
int BSTMap::getHeight(const Node *n) {
  if (n == nullptr) { return 0; }
  return 1;
}

bool BSTMap::sortStringAscending(BSTMap::value_type &a, BSTMap::value_type &b) {
  return a.first < b.first;
}