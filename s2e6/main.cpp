#include <iostream>
#include <ostream>
#include <string>
#include <vector>

template <typename T>
std::ostream& operator<<(std::ostream& os, std::vector<T> v) {
  for (auto i : v)
    std::cout << i << ' ';
  return os;
}

template <typename KT, typename VT>
class Table {
protected:
  struct Elem {
    KT key;
    VT value;
    Elem* next = nullptr;
  };
public:
  Elem* head;

public:
  Table()
    : head(nullptr) {}

  ~Table() {
    while (head) {
      auto next = head->next;
      delete head;
      head = next;
    }
  }

  template<typename KT_, typename VT_>
  friend std::ostream& operator<<(std::ostream& os, const Table<KT_, VT_>& t);

  virtual void insert(KT key, VT value) {
    head = new Elem{key, value, head};
  }
};

template <typename KT, typename VT>
std::ostream& operator<<(std::ostream& os, const Table<KT, VT>& t) {
  if (!t.head)
    os << "<empty>";
  else {
    auto it = t.head;
    while (it) {
      os << it->key << ": " << it->value << "\n";
      it = it->next; 
    }
  }
  return os;
}


struct Vec2 { float x, y; };
std::ostream& operator<<(std::ostream& os, Vec2 v) {
  return (os << '(' << v.x << ", " << v.y << ')');
}


int main() {
  Table<int, std::string> ist;
  ist.insert(777, "Moscow");
  ist.insert(52, "Saint Petersberg");
  std::cout << ist << '\n';

  Table<std::string, float> sst;
  sst.insert("Russia", 17.1f);
  sst.insert("China", 9.6f);
  std::cout << sst << '\n';

  Table<Vec2, std::vector<int>> vvt;
  vvt.insert({0, 0}, {0, 1, 2, 3});
  vvt.insert({1, 12}, {3, 2, 1});
  std::cout << vvt << '\n';
}