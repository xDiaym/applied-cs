#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <iterator>
#include <ostream>
#include <string>

struct student {
  std::string surname;
  int dob;
  float mean_score;
};

std::istream& operator>>(std::istream& is, student& s) {
  return (is >> s.surname >> s.dob >> s.mean_score);
}

std::ostream& operator<<(std::ostream& os, const student& s) {
  return (os << s.surname << ' ' << s.dob << ' ' << s.mean_score);
}

class BTree {
  struct node_t {
    node_t(student s) : val(s), left(nullptr), right(nullptr) {}
    ~node_t() {
      delete left;
      delete right;
    }

    student val;
    node_t* left, *right;
  } *root;
  using cmp_fn = std::function<bool(student, student)>;
  cmp_fn cmp;

public:
  BTree(cmp_fn cmp_) : root(nullptr), cmp(cmp_) {}
  ~BTree() { delete root; }

  void insert(const student& s) {
    if (!root) root = new node_t(s);
    else {
      auto* n = root;
      while (n) {
        if (cmp(s, n->val)) {
          if (!n->left) {
            n->left = new node_t(s);
            break;
          } else n = n->left;
        } else {
          if (!n->right) {
            n->right = new node_t(s);
            break;
          } else n = n->right;
        }
      }
    }
  }

  void traverse() const { traverse(root); }

private:
  void traverse(node_t* n) const {
    if (!n) return;
    traverse(n->left);
    std::cout << n->val << '\n';
    traverse(n->right);
  }
};

bool cmp(const student& lhs, const student& rhs) {
  return lhs.mean_score < rhs.mean_score;
}

int main() {
  std::string fname; std::cin >> fname;
  std::ifstream ifs(fname);
  std::vector<student> v((std::istream_iterator<student>(ifs)),
                           std::istream_iterator<student>());

  auto bt = BTree(cmp);
  for (auto s : v)
    bt.insert(s);
  bt.traverse();
}