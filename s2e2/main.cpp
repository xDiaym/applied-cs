#include <iostream>
#include <ostream>

class String {
 private:
  static const int n = 255;
  char str[n + 1];

 public:
  friend std::ostream& operator<<(std::ostream& os, const String& s);

  String() { str[0] = 0; }

  String(char s[]) {
    str[0] = 0;
    for (int i = 0; s[i] != '\0'; i++) add(s[i]);
  }

  void add(char x) {
    ++str[0];
    str[str[0]] = x;
  }

  int strlen() const { return int(str[0]); }  // длина строки

  // Конкатенация строк
  void strcat(const String& other) {
    for (int i = 1; i <= other.strlen(); ++i) add(other.str[i]);
  }

  // Разделение строки
  void slice(int k, String& s) {
    s.str[0] = 0;
    for (int i = k; i <= strlen(); ++i) s.add(str[i]);
    str[0] = k - 1;
  }

  // Контекстный поиск
  int find(const String& needle) {
    for (int i = 1; i <= strlen() - needle.strlen() + 1; ++i) {
      bool match = true;
      for (int j = 1; j < needle.strlen(); ++j)
        if (str[i + j - 1] != needle.str[j]) {
          match = false;
          break;
        }
      if (match) return i;
    }
    return -1;
  }

  // Контекстная замена
  void replace(const String& pattern, const String& replacement) {
    String dirty_buff, buff;
    int pos = find(pattern);
    while (pos != -1) {
      // pattern = bb, replacement = cc, *this = aaaabbsomestr
      slice(pos, dirty_buff);                        // *this = aaaa, dirty_buff = bbsomestr
      dirty_buff.slice(pattern.strlen() + 1, buff);  // dirty_buff = bb, buff = somestr;
      strcat(replacement);                           // *this = aaaacc
      strcat(buff);                                  // *this = aaaaccsomestr
      pos = find(pattern);
    }
  }
};

std::ostream& operator<<(std::ostream& os, const String& s) {
  for (int i = 1; i <= s.strlen(); ++i) os << s.str[i];
  return os;
}

int main() {
  using std::cout, std::endl;
  String a("hello world bye"), b(" bye"), c("!");
  a.replace(b, c);
  cout << a << endl;
}