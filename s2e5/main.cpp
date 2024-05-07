#include <cctype>
#include <iostream>
#include <string_view>

class Tree {
  struct Node {
    Node *left, *right;
    char op;

    Node(char op_) : op(op_), left(nullptr), right(nullptr) {}
    Node(char op_, Node* l, Node* r) : op(op_), left(l), right(r) {}
    ~Node() {
      delete left;
      delete right;
    }
  };
  Node* root = nullptr;

  expr ::= 'a' | 'b' ...
  op ::= + | - | / | *
  expr ::= '(' expr op expr ')'
  Node* parse(std::string_view s, int& pos) {
    if (isalnum(s[pos])) {
      return new Node(s[pos++]);
    }
    if (s[pos] == '(') {
      ++pos;
      Node* left = parse(s, pos);
      char op = s[pos++];
      Node* right = parse(s, pos);
      ++pos;  // ')'
      return new Node(op, left, right);
    }
    return nullptr;
  }

  void print(Node* n) {
    if (!n) return;
    if (n->left && n->right) {
      std::cout << '(';
      print(n->left);
      std::cout << n->op;
      print(n->right);
      std::cout << ')';
    } else {
      std::cout << n->op;
    }
  }

public:
  static Tree parse(std::string_view s) {
    int pos = 0;
    Tree t;
    t.root = t.parse(s, pos);
    return t;
  }

  void print() {
    print(root);
  }

  ~Tree() {
    delete root;
  }
};


int main() {
  std::string s = "((1*4)+(3/a))";
  std::cout << s << '\n';
  Tree t = Tree::parse(s);
  t.print();
  std::cout << '\n';
}
