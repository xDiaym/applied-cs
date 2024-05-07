#include <exception>
#include <fstream>
#include <algorithm>
#include <iostream>

template <typename T>
class Collection {
public:
  Collection(int limit_, int delta_) :
    limit(limit_),
    delta(delta_),
    count(0),
    items(new T[limit_]) {}

  virtual void insert(T v) {
    if (count == limit)
      setLimit(count + delta);
    items[count++] = std::move(v);
  }

  T& operator[](size_t idx) { return items[idx]; }
  const T& operator[](size_t idx) const { return items[idx]; }

  void setLimit(int new_limit) {
    T* data = new T[new_limit];
    for (int i = 0; i < std::min(limit, new_limit); ++i)
      data[i] = items[i];
    delete[] items;
    limit = new_limit;
    items = data;
  }

  void load(std::istream& is) {
    while (!is.eof()) {
      T x; is >> x;
      insert(x);
    }
  }

  void show() {
    for (int i = 0; i < count; ++i)
      std::cout << items[i] << ' ';
    std::cout << '\n';
  }

  virtual ~Collection() {
    delete[] items;
  }

protected:
  T* items;
  int limit;
  int delta;
  int count;
};

template <typename T>
class SortedCollection : public Collection<T> {
public:
  using Collection<T>::count, Collection<T>::limit, Collection<T>::delta, Collection<T>::items, Collection<T>::setLimit;

  SortedCollection(int limit_, int delta_) : Collection<T>(limit_, delta_) {}

  virtual void insert(T v) {
    if (count == limit)
      setLimit(count + delta);
    auto pos = std::lower_bound(items, items + count, v) - items;
    for (int i = count; i > pos; --i)
      items[i] = items[i-1];
    items[pos] = std::move(v);
    ++count;
  }
};

struct Student {
  std::string name;
  int age;
};

std::ostream& operator<<(std::ostream& os, const Student& st) {
  return (os << '(' << st.name << ' ' << st.age << ')');
}

std::istream& operator>>(std::istream& is, Student& st) {
  return (is >> st.name >> st.age);
}

bool operator<(const Student& lhs, const Student& rhs) {
  return std::tie(lhs.name, lhs.age) < std::tie(rhs.name, rhs.age);
}

int main() try {
  {
    std::ifstream ifs("ints.txt");
    Collection<int> c(4, 2);
    c.load(ifs);
    c.show();
  }
  {
    std::ifstream ifs("studs.txt");
    SortedCollection<Student> c(4, 2);
    c.load(ifs);
    c.show();
  }
} catch (std::exception exc) {
  std::cerr << "Something went wrong\n" << exc.what();
}
