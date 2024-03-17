#include <iostream>
#include <assert.h>

class List {
protected:
  struct Node {
    int val;
    Node* next;
  } *head;

public:
  List() : head(nullptr) {}
  void print() const noexcept {
    auto* n = head;
    while (n) {
      std::cout << n->val << " -> ";
      n = n->next;
    }
    std::cout << std::endl;
  }

  virtual void push(int x) = 0;
  virtual int pop() = 0;
  
  virtual void init(int n) {
    for (int i = 0; i < n; ++i)
      push(i);
  }

  virtual ~List() {
    auto* h = head;
    while (h) {
      auto* n = head->next;
      delete h;
      h = n;
    }
  }
};

class Stack : virtual public List {
public:
  virtual void push(int x) {
    head = new Node{x, head};
  }

  virtual int pop() {
    int v = head->val;
    auto* n = head->next;
    delete head;
    head = n;
    return v;
  }
};

class Queue : virtual public List {
public:
  void push(int x) override {
    if (!head) head = new Node{x, nullptr};
    else {
      auto* h = head;
      while (h->next) h = h->next;
      h->next = new Node{x, nullptr};
    }
  }

  int pop() override {
    int v = head->val;
    auto* n = head->next;
    delete head;
    head = n;
    return v;
  }
};

class Deque : public Stack, public Queue {
public:
  virtual void init(int n) override {
    for (int i = 0; i < n; ++i)
      if (i % 2 == 0)
        Stack::push(i);
      else
        Queue::push(i);
  }

  void push(int x) override { Queue::push(x); }
  int pop() override { return Queue::pop(); }


  void push_front(int x) {
    Stack::push(x);
  }

  int pop_front() {
    return Stack::pop();
  }

  int pop_back() {
    if (!head->next) {
      int v = head->val;
      delete head;
      head = nullptr;
      return v;
    } else {
      auto* h = head;
      while (h->next->next) h = h->next;
      int v = head->next->val;
      delete head->next;
      head->next = nullptr;
      return v;
    }
  }
};


int main() {
  List* mass[6];
  for (int i = 0; i < 6; i++)
  switch (i % 3) {
  case 0: mass[i] = new Stack; break;
  case 1: mass[i] = new Queue; break;
  case 2: mass[i] = new Deque; break;
  }
  for (int i = 0; i < 6; i++)
    mass[i]->init(15);
  for (int i = 0; i < 6; i++)
    mass[i]->print();
  
  for (int i = 0; i < 6; i++)
    mass[i]->push(10000);
  
  for (int i = 0; i < 6; i++)
    mass[i]->print();
  
  std::cout << std::endl;
}
