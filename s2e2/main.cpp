#include <iostream>
#include <ostream>
#include <utility>
#include <vector>

size_t next_power_of_2(size_t n) {
  size_t p = 1;
  while (p < n) p *= 2;
  return p;
}

template <typename T>
class List {
  struct Node {
    Node* next;
    T data_;
  } *head_ = nullptr, *tail_ = nullptr;

  friend std::ostream& operator<<(std::ostream& os, const List<T>& l);

public:
  void add(T&& t) {
    if (!tail_) head_ = tail_ = new Node{nullptr, std::move(t)};
    else {
      tail_->next = new Node{nullptr, std::move(t)};
      tail_ = tail_->next;
    }
  }
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const List<T>& l) {
  typename List<T>::Node* n = l.head_;
  while (n) {
    os << n->data_ << ' ';
    n = n->next_;
  }
  return os;
}


// TODO: Small String Optimization + CharTraits + Allocator
class String {
 private:
  size_t size_, capacity_;
  char* data_;

 private:
  void reserve(size_t n) {
    if (n > capacity_) {
      capacity_ = n;
      // FIXME: realloc with new[]
      data_ = static_cast<char*>(realloc(data_, n));
    }
  }

 public:
  static constexpr size_t npos = size_t(-1);
  friend std::ostream& operator<<(std::ostream& os, const String& s);

  String() noexcept
    : size_(0), capacity_(8), data_(new char[8]) {}

  explicit String(const char* s) {
    size_ = ::strlen(s);
    capacity_ = next_power_of_2(size_ + 1); // +1 for \0
    data_ = new char[capacity_];
    memcpy(data_, s, size_);
    data_[size_] = '\0';
  }

  String(const String& s) {
    size_ = s.size_;
    capacity_ = s.capacity_;
    data_ = new char[capacity_];
    memcpy(data_, s.data_, size_ + 1); // +1 for \0
  }

  String(String&& s) noexcept {
    size_ = std::exchange(s.size_, 0);
    capacity_ = std::exchange(s.capacity_, 0);
    data_ = std::exchange(s.data_, nullptr);
  }

  String& operator=(const String& s) {
    if (this != &s)
      String(s).swap(*this);
    return *this;
  }

  String& operator=(String&& s) noexcept {
    swap(s);
    return *this;
  }

  void swap(String& s) noexcept {
    std::swap(size_, s.size_);
    std::swap(capacity_, s.capacity_);
    std::swap(data_, s.data_);
  }

  ~String() {
    delete[] data_;
  }

  void add(char x) {
    if (size_ + 1 == capacity_)
      reserve(capacity_ * 2);
    data_[size_] = x;
    ++size_;
    data_[size_] = '\0';
  }

  size_t size() const noexcept { return size_; }  // длина строки

  // Конкатенация строк
  void strcat(const String& other) {
    reserve(size_ + other.size_ + 1);  // +1 for \0
    memcpy(data_ + size_, other.data_, other.size_);
    size_ += other.size_;
    data_[size_] = '\0';
  }

  // Разделение строки
  String slice(size_t k) {
    String tail(&data_[k]);
    size_ = k;
    data_[k] = '\0';
    return tail;
  }

  // Контекстный поиск
  size_t find(const String& needle) const {
    for (size_t i = 0; i < size_ - needle.size_ + 1; ++i) {
      bool match = true;
      for (size_t j = 0; j < needle.size_; ++j)
        if (data_[i + j] != needle.data_[j]) {
          match = false;
          break;
        }
      if (match) return i;
    }
    return npos;
  }

  // Контекстная замена
  void replace(const String& pattern, const String& replacement) {
    String dirty_buff, buff;
    auto pos = find(pattern);
    while (pos != npos) {
      // pattern = bb, replacement = cc, *this = aaaabbsomestr
      dirty_buff = slice(pos);                        // *this = aaaa, dirty_buff = bbsomestr
      buff = dirty_buff.slice(pattern.size() + 1);  // dirty_buff = bb, buff = somestr;
      strcat(replacement);                           // *this = aaaacc
      strcat(buff);                                  // *this = aaaaccsomestr
      pos = find(pattern);
    }
  }

  bool endswith(const String& s) {
    for (size_t i = 0; i < s.size(); ++i)
      if (data_[size_ - s.size() + i] != s.data_[i])
        return false;
    return true;
  }

  List<String> split(String r) {
    while () {
    
    }
  }
};

String operator+(String left, String right) {
  String r(left);
  r.strcat(right);
  return r;
}

std::ostream& operator<<(std::ostream& os, const String& s) {
  return (os << s.data_);
}


int main() {
  using std::cin, std::cout, std::endl;
  String s("Petrov");

  if (s.endswith(String("ov"))) {
    cout << "male" << endl;
    cout << "Im:\t"  << s << endl;
    s.slice(s.size() - 2);
    cout << "Rod:\t" << s + String("ova") << endl;
    cout << "Dat:\t" << s + String("ovu") << endl;
    cout << "Vim:\t" << s + String("ova") << endl;
    cout << "Tv:\t"  << s + String("ovym") << endl;
    cout << "Pr:\t"  << s + String("ove") << endl;
  } else if (s.endswith(String("ova"))) {
    cout << "female" << endl;
    cout << "Im:\t"  << s << endl;
    s.slice(s.size() - 3);
    cout << "Rod:\t" << s + String("ovoy") << endl;
    cout << "Dat:\t" << s + String("ovoy") << endl;
    cout << "Vim:\t" << s + String("ove") << endl;
    cout << "Tv:\t"  << s + String("ovoy") << endl;
    cout << "Pr:\t"  << s + String("ovoy") << endl;
  } else
    cout << "unknown" << endl;
}