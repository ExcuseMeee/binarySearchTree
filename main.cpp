/*
main.cpp
Oscar Lin, COP 3415
3/27/2024
*/
#include <iostream>
#include <string>
#include <functional>
#include <fstream>
#include <sstream>
#include <vector>

// NV = Name & Value. All nodes have a string name and a V data (V is template type)
template<typename V>
class NV_Node {
public:
  std::string name;
  V data;
  NV_Node<V>* left;
  NV_Node<V>* right;

  NV_Node(std::string name = "", const V& val = V(), NV_Node<V>* left = nullptr, NV_Node<V>* right = nullptr) {
    this->name = name;
    this->data = val;
    this->left = left;
    this->right = right;
  }

};

template<typename V>
class NV_BST {
public:

  NV_BST() {
    root = nullptr;
  }

  bool isEmpty() const {
    return root == nullptr;
  }

  // print each node's name in tree structure
  void print() const {
    std::cout << "[print]" << std::endl;
    if (isEmpty()) std::cout << "empty" << std::endl;
    else print("", this->root, false);
  }

  // insert NV_Node, node has (N)ame and (V)alue
  void insert_NV(const std::string& name, const V& val) {
    insert_NV(name, val, this->root);
  }

  // find and return node that matches name. returns nullptr if not found.
  NV_Node<V>* search(const std::string& name) const {
    return search(name, this->root);
  }

  // remove node that matches name
  void remove(const std::string& name) {
    remove(name, this->root);
  }

  // get height of subtree starting at node. if node is nullptr, height is 0
  int getHeight(const NV_Node<V>* node) const {
    if (!node) return 0;

    int left = getHeight(node->left) + 1;
    int right = getHeight(node->right) + 1;

    return left > right ? left : right;

  }

  // Higher Order Function. Traverse the tree inorder, and call func for each node
  void forEach_inorder(std::function<void(NV_Node<V>*)> func) {
    forEach_inorder(func, this->root);
  }

  // return root node. empty tree returns nullptr
  const NV_Node<V>* getRoot() const {
    return this->root;
  }

private:
  NV_Node<V>* root;

  // return the NV_Node with the smallest data value, starting at node
  const NV_Node<V>* getMinNode(NV_Node<V>* node) {
    if (!node) return nullptr;

    NV_Node<V>* cur = node;
    while (cur->left != nullptr) {
      cur = cur->left;
    }
    return cur;
  }
  void insert_NV(const std::string& name, const V& val, NV_Node<V>*& node) {
    if (!node) {
      node = new NV_Node<V>(name, val);
    }
    else {
      if (name > node->name) insert_NV(name, val, node->right);
      else if (name < node->name) insert_NV(name, val, node->left);
    }

  }
  void print(const std::string& prefix, const NV_Node<V>* node, bool isRight) const {
    if (node != nullptr) {
      std::cout << prefix;
      std::cout << (isRight ? "R--" : "L--");
      std::cout << node->name << std::endl;

      // enter next tree level
      print(prefix + "   ", node->right, true);
      print(prefix + "   ", node->left, false);

    }
  }
  NV_Node<V>* search(const std::string& name, NV_Node<V>* node) const {
    if (!node) return nullptr;
    if (node->name == name) return node;

    NV_Node<V>* found = nullptr;
    if (name > node->name) found = search(name, node->right);
    else if (name < node->name) found = search(name, node->left);

    return found;
  }
  void remove(const std::string& name, NV_Node<V>*& node) {
    if (!node) return;

    if (name > node->name) {
      remove(name, node->right);
    }
    else if (name < node->name) {
      remove(name, node->left);
    }
    else {
      if (node->left && node->right) {
        const NV_Node<V>* min = getMinNode(node->right);
        node->name = min->name;
        node->data = min->data;
        remove(min->name, node->right);
      }
      else {
        NV_Node<V>* to_delete = node;
        node = node->left ? node->left : node->right;
        delete to_delete;
      }
    }

  }
  void forEach_inorder(std::function<void(NV_Node<V>*)> func, NV_Node<V>* node) {
    if (!node) return;

    forEach_inorder(func, node->left);
    func(node);
    forEach_inorder(func, node->right);
  }
};

NV_BST<int>* searchNameNode(NV_BST<NV_BST<int>*>* mainTree, const std::string& name);
const std::vector<std::string> tokenize(std::string line);
void initializeQueries(std::ifstream& inFile, int& N, int& I, int& Q);
NV_BST<NV_BST<int>*>* buildMainTree(std::ifstream& inFile, int N, std::ofstream& outFile);
void processInserts(std::ifstream& inFile, NV_BST<NV_BST<int>*>*& mainTree, int I, std::ofstream& outFile);
void processQueries(std::ifstream& inFile, NV_BST<NV_BST<int>*>*& mainTree, int Q, std::ofstream& outFile);
std::string traverseInTraverse(NV_BST<NV_BST<int>*>* mainTree);
std::string searchQuery(NV_BST<NV_BST<int>*>* mainTree, std::vector<std::string>& line_tokens);
std::string itemBeforeQuery(NV_BST<NV_BST<int>*>* mainTree, std::vector<std::string>& line_tokens);
std::string heightBalanceQuery(NV_BST<NV_BST<int>*>* mainTree, std::vector<std::string>& line_tokens);
std::string countQuery(NV_BST<NV_BST<int>*>* mainTree, std::vector<std::string>& line_tokens);
void writeAndDisplay(std::ofstream& outFile, const std::string& str);

int main() {
  // MAKE SURE in.txt EXISTS!!!
  std::ifstream inFile("in.txt");
  std::ofstream outFile("out.txt");

  int N, I, Q;

  initializeQueries(inFile, N, I, Q);

  NV_BST<NV_BST<int>*>* mainTree = buildMainTree(inFile, N, outFile);

  processInserts(inFile, mainTree, I, outFile);
  writeAndDisplay(outFile, traverseInTraverse(mainTree)); // display itemTrees after building
  processQueries(inFile, mainTree, Q, outFile);


  delete mainTree;

  return 0;
}

// traverse mainTree inorder, then traverse each itemTree inorder. return string result of traversal
std::string traverseInTraverse(NV_BST<NV_BST<int>*>* mainTree) {
  std::stringstream ss; // stream of strings to write to. write using << operator, similar to std::cout
  if (!mainTree || mainTree->isEmpty()) {
    ss << "[traverseInTraverse] no / empty tree" << std::endl;
    return ss.str();
  }

  mainTree->forEach_inorder([&ss](NV_Node<NV_BST<int>*>* mainNode) {
    ss << "***" << mainNode->name << "***" << std::endl;
    // mainNode->data is itemTree
    if (!mainNode->data)
      ss << "Tree dne";
    else if (mainNode->data->isEmpty())
      ss << mainNode->name << " is empty";
    else {
      mainNode->data->forEach_inorder([&ss](NV_Node<int>* node) {
        // for each NV_Node within itemTree, write to std::stringstream
        ss << node->name << " ";
      });
    }
    ss << std::endl;
  });

  return ss.str(); // convert stream to string and return

}

// search main tree for node of specified name. return item tree associated with it, else return nullptr
NV_BST<int>* searchNameNode(NV_BST<NV_BST<int>*>* mainTree, const std::string& name) {
  if (!mainTree) return nullptr;

  NV_Node<NV_BST<int>*>* node = mainTree->search(name);

  return (node ? node->data : nullptr);

}

// split a line (a string delimted by whitespace) into a vector of tokens
const std::vector<std::string> tokenize(std::string line) {
  std::vector<std::string> v;
  std::istringstream iss(line); // iss will treat each word delimited by a whitespace as its own string
  std::string token;

  // push tokens to vec 
  while (iss >> token) {
    v.push_back(token);
  }

  return v; // return by value. v should be small so no performance concerns
}

// read the next N lines from infile to create main tree. main tree nodes contain pointers to item trees. displays mainTree inorder after building
NV_BST<NV_BST<int>*>* buildMainTree(std::ifstream& inFile, int N, std::ofstream& outFile) {
  if (!inFile) return nullptr;

  NV_BST<NV_BST<int>*>* mainTree = new NV_BST<NV_BST<int>*>();

  for (N; N > 0; N--) {
    if (inFile.eof()) break; // stop if at end of file

    // get line from infile
    std::string line;
    std::getline(inFile, line);

    // split line into tokens
    std::vector<std::string> line_tokens = tokenize(line);
    if (line_tokens.empty()) continue; // if empty line skip it

    try {
      mainTree->insert_NV(line_tokens.at(0), new NV_BST<int>()); // insert node (contains treename and pointer to tree)
    }
    catch (const std::exception& e) {
      std::cout << "[buildMainTree] issue occured, skipping line..." << std::endl;
      continue;
    }

  }

  // print after building
  mainTree->forEach_inorder([&outFile](NV_Node<NV_BST<int>*>* node) {
    writeAndDisplay(outFile, node->name + " ");
  });
  writeAndDisplay(outFile, "\n");


  return mainTree;
}

// read the next I lines from inFile and insert nodes into mainTree
void processInserts(std::ifstream& inFile, NV_BST<NV_BST<int>*>*& mainTree, int I, std::ofstream& outFile) {
  if (!mainTree || !inFile) return;

  for (I; I > 0; I--) {
    if (inFile.eof()) break;

    std::string line;
    std::getline(inFile, line); // read line

    std::vector<std::string> line_tokens = tokenize(line); // should be {itemTreeName, itemName, amount}
    if (line_tokens.size() < 3) continue; // if not enough tokens skip line

    try {
      NV_BST<int>* itemTree = searchNameNode(mainTree, line_tokens.at(0));
      if (!itemTree) continue;
      itemTree->insert_NV(line_tokens.at(1), std::stoi(line_tokens.at(2)));
    }
    catch (const std::exception& e) { // skip line if error occurs when processing line
      writeAndDisplay(outFile, "[processInserts] issue encountered, skipping line...\n");
      continue;
    }

  }

}

// read the next Q lines from inFile and call queries on mainTree
void processQueries(std::ifstream& inFile, NV_BST<NV_BST<int>*>*& mainTree, int Q, std::ofstream& outFile) {
  if (!mainTree || !inFile) return;

  for (Q; Q > 0; Q--) {
    if (inFile.eof()) break;

    std::string line;
    std::getline(inFile, line);

    std::vector<std::string> line_tokens = tokenize(line);
    if (line_tokens.size() < 2) continue; // if less than 2 tokens don't bother trying


    // Queries: search, item_before, height_balance, count
    try {
      std::string query = line_tokens.at(0); // query should be the first token

      if (query == "search")
        writeAndDisplay(outFile, searchQuery(mainTree, line_tokens));
      else if (query == "item_before")
        writeAndDisplay(outFile, itemBeforeQuery(mainTree, line_tokens));
      else if (query == "height_balance")
        writeAndDisplay(outFile, heightBalanceQuery(mainTree, line_tokens));
      else if (query == "count")
        writeAndDisplay(outFile, countQuery(mainTree, line_tokens));
      else
        throw std::string("[processQueries] unknown query");
    }
    catch (const std::string& msg) {
      // queries throw string errors
      writeAndDisplay(outFile, msg);
      continue;
    }
    catch (const std::exception& e) {
      // uncaught error?
      writeAndDisplay(outFile, "[processQueries] issue occured, skipping line...\n");
      continue;
    }

  }

}

// line_tokens: {query, itemTreeName, itemName}. query itemTreeName for itemName (itemTreeName is found in mainTree). returns query result as string, throws string on error
std::string searchQuery(NV_BST<NV_BST<int>*>* mainTree, std::vector<std::string>& line_tokens) {
  if (!mainTree) throw std::string("[searchQuery] no mainTree");

  if (line_tokens.size() < 3) throw std::string("[searchQuery] invalid number of tokens");

  std::string itemTreeName = line_tokens.at(1);
  std::string name = line_tokens.at(2);

  NV_BST<int>* itemTree = searchNameNode(mainTree, itemTreeName); // search for itemTree in mainTree

  std::stringstream ss; // create stream to write to, similar to std::cout
  if (!itemTree) {
    ss << itemTreeName << " does not exist" << std::endl;
  }
  else {
    NV_Node<int>* node = itemTree->search(name); // search for item within itemTree

    if (!node) {
      ss << name << " not found in " << itemTreeName << std::endl;
    }
    else {
      ss << node->data << " " << node->name << " found in " << itemTreeName << std::endl;
    }

  }

  return ss.str();

}

// line_tokens: {query, itemTreeName, itemName}. query itemTreeName for number of items before itemName. returns result as string, throws string on error
std::string itemBeforeQuery(NV_BST<NV_BST<int>*>* mainTree, std::vector<std::string>& line_tokens) {
  if (!mainTree) throw std::string("[itemBeforeQuery] no mainTree");

  if (line_tokens.size() < 3) throw std::string("[itemBeforeQuery] invalid number of tokens");

  std::string itemTreeName = line_tokens.at(1);
  std::string name = line_tokens.at(2);

  NV_BST<int>* itemTree = searchNameNode(mainTree, itemTreeName);

  std::stringstream ss;
  if (!itemTree) {
    ss << itemTreeName << " does not exist" << std::endl;
  }
  else {
    int count = 0;
    bool found = false; // to prevent displaying count if itemName is not found in the tree

    itemTree->forEach_inorder([&name, &count, &found](NV_Node<int>* node) {
      // for each node in tree, increment count if it's before name
      if (found) return;
      else if (node->name == name) found = true;
      else if (node->name < name) count++;
    });

    if (found)
      ss << "item before " << name << ": " << count << std::endl;
    else
      ss << name << " not found in " << itemTreeName << std::endl;

  }

  return ss.str();

}

// line_tokens: {query, itemTreeName}. query itemTreeName for left and right subtree height, and overall balance. returns result as string, throws string on error
std::string heightBalanceQuery(NV_BST<NV_BST<int>*>* mainTree, std::vector<std::string>& line_tokens) {
  if (!mainTree) throw std::string("[heighBalanceQuery] no mainTree");

  if (line_tokens.size() < 2) throw std::string("[heightBalanceQuery] invalid number of tokens");

  std::string itemTreeName = line_tokens.at(1);

  NV_BST<int>* itemTree = searchNameNode(mainTree, itemTreeName);

  std::stringstream ss;
  if (!itemTree) {
    ss << itemTreeName << " does not exist" << std::endl;
  }
  else {
    const NV_Node<int>* root = itemTree->getRoot();

    int left = root ? itemTree->getHeight(root->left) : 0;
    int right = root ? itemTree->getHeight(root->right) : 0;
    int difference = std::abs(left - right);

    ss << itemTreeName << ": "
      << "left height " << left << ", "
      << "right height " << right << ", "
      << "difference " << difference << ", "
      << (difference > 1 ? "not balanced" : "balanced") << std::endl;

  }

  return ss.str();

}

// line_tokens: {query, itemTreeName}. query itemTreeName for total count of items. returns result as string, throws string on error
std::string countQuery(NV_BST<NV_BST<int>*>* mainTree, std::vector<std::string>& line_tokens) {
  if (!mainTree) throw std::string("[countQuery] no mainTree");

  if (line_tokens.size() < 2) throw std::string("[countQuery] invalid number of tokens");

  std::string itemTreeName = line_tokens.at(1);

  NV_BST<int>* itemTree = searchNameNode(mainTree, itemTreeName);

  std::stringstream ss;
  if (!itemTree) {
    ss << itemTreeName << " does not exist" << std::endl;
  }
  else {
    int count = 0;
    itemTree->forEach_inorder([&count](NV_Node<int>* node) {
      if (node->data) count += node->data; // for each node in tree, add node->data to count
    });

    ss << itemTreeName << " count " << count << std::endl;

  }

  return ss.str();

}

// take in a string, then write to outFile and display in console too
void writeAndDisplay(std::ofstream& outFile, const std::string& str) {
  if (!outFile) return;
  if (!outFile.is_open()) return;

  std::cout << str; // print to console
  outFile << str; // write to outFile

}

// read the first line of inFile and set values for N, I, Q to be used later
void initializeQueries(std::ifstream& inFile, int& N, int& I, int& Q) {
  if (!inFile) throw std::string("[initializeQueries] no infile");

  std::string line;
  std::getline(inFile, line);

  std::vector<std::string> line_tokens = tokenize(line);
  if (line_tokens.size() < 3) throw std::string("[initializeQueries] not enough tokens");
  try {
    N = std::stoi(line_tokens.at(0));
    I = std::stoi(line_tokens.at(1));
    Q = std::stoi(line_tokens.at(2));
  }
  catch (const std::exception& e) {
    throw std::string("[initializeQueries] invalid tokens");
  }

}