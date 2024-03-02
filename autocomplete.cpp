#include "autocomplete.h"
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

void testBSTAll();

// default constructor, initializes bstmap
Autocomplete::Autocomplete() {
  phrases = BSTMap();
}

// reads in the given .txt file and inserts into bstmap structure
void Autocomplete::readFile(const string &fileName) {
  ifstream ifs(fileName);

  // TODO(student)
  stringstream ss;
  string strLine;
  string temp;
  getline(ifs, strLine); // get the first line
  while (getline(ifs, strLine)) {
    ss << strLine; // load one line into stringstream

    // get mapped_type and key_type and insert
    ss >> temp;
    BSTMap::mapped_type map = stol(temp);
    getline(ss, temp); // extract key including all spaces
    BSTMap::key_type key = temp.substr(1, temp.size()); // remove leading space
    phrases[key] = map; // insert into bst using operator[]

    // reset stringstream
    ss.str("");
    ss.clear();
  }
  ifs.close();  
  phrases.rebalance(); // rebalance bst tree after insertions
}

// visitor function for std::sort, sorts value_type objects by weight
bool Autocomplete::sortByWeight(BSTMap::value_type &a, BSTMap::value_type &b) {
  return a.second > b.second;
}

// gets all matching phrases given a prefix by the user and stores
// in a vector of value_type which is returned
vector<BSTMap::value_type>
Autocomplete::complete(const BSTMap::key_type &prefix) const {
  // TODO(student)
  vector<BSTMap::value_type> vec = phrases.getAll(prefix); // get matching phrases in order
  sort(vec.begin(), vec.end(), sortByWeight); // sort matching phrases by weight

  // // FOR TESTING
  // cout << vec.size() << endl;
  // for (BSTMap::value_type elem : vec) {
  //   cout << "[" << elem.first << ", " << elem.second << "]" << endl;
  // }

  return vec;
}
