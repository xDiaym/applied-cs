#include <algorithm>
#include <iostream>
#include <ostream>

class Table {
protected:
  struct Elem {
    int key;
    int value;
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

  friend std::ostream& operator<<(std::ostream& os, const Table& t);

  virtual void insert(int key, int value) {
    head = new Elem{key, value, head};
  }
};

std::ostream& operator<<(std::ostream& os, const Table& t) {
  if (!t.head)
    os << "<empty>";
  else {
    auto it = t.head;
    while (it) {
      os << it->key << ": " << it->value << "; ";
      it = it->next; 
    }
  }
  return os;
}

class TableKey : public Table {
public:
  TableKey() : Table() {}
  TableKey(const Table& t) {
    auto it = t.head;
    while (it) {
      insert(it->key, it->value);
      it = it->next;
    }
  }

  virtual void insert(int key, int value) {
    if (!head || key < head->value) {
      head = new Elem{key, value, head};
    } else {
      const auto par = find_parent(key);
      par->next = new Elem{key, value, par->next};
    }
  }

private:
  Elem* find_parent(int key) const {
    if (!head) return nullptr;
    auto it = head;
    while (it->next && key > it->next->key)
      it = it->next;
    return it;
  }
};


int main() {
  Table* t = new Table;
  std::cout << *t << "\n";
  for (int i = 0; i < 10; ++i)
    t->insert(i, i);
  std::cout << *t << "\n";
  delete t;

  t = new TableKey;
  std::cout << *t << "\n";
  for (int i = 15; i > 10; --i)
    t->insert(i, i);
  std::cout << *t << "\n";
  delete t;
}