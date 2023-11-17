#include <cstdio>
#include <iostream>
#include <cstring>
#include <memory>

static constexpr size_t MAX_LEN = 1 << 10;
static constexpr size_t MOD = 1e9 + 7;

auto hash(const char* s) {
  size_t h = 0, p = 1;
  while (s++) {
    h = (h + (*s - '0' + 1) * p) % MOD;
    p = (p * 31) % MOD;
  }
  return h; 
}

auto words(char* str) {
  std::vector<char*> h;
  char *pch = strtok(str, " ");
  while (pch) {
    h.push_back(pch);
    pch = strtok(NULL, " ");
  }
  return h;
}

int main() {
  auto* s1 = new char[MAX_LEN];
  auto* s2 = new char[MAX_LEN];
  scanf("%1023[^\n]s", s1); (void) getchar();
  scanf("%1023[^\n]s", s2); (void) getchar();

  auto w1 = words(s1), w2 = words(s2);
  char* maxs = nullptr; size_t maxl = 0;
  for (auto* sub1 : w1)
    for (auto* sub2 : w2)
      if (!strcmp(sub1, sub2) && strlen(sub1) > maxl)
        maxs = sub1, maxl = strlen(sub1);
  printf("%s", maxs);

  delete[] s1;
  delete[] s2;
}