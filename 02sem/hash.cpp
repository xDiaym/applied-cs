#include <iostream>
#include <string_view>
#include <unordered_set>

constexpr size_t MAX_LEN = 1024;

using uset = std::unordered_set<std::string_view>;

auto words(char *s) {
    uset set;
    char *pch = strtok(s, " ");
    while (pch) {
        set.insert(pch);
        pch = strtok(NULL, " ");
    }
    return set;
} 

int main() {
    auto* str1 = new char[MAX_LEN];
    auto* str2 = new char[MAX_LEN];
    scanf("%1023[^\n]s", str1); (void) getchar();
    scanf("%1023[^\n]s", str2); (void) getchar();

    std::string_view maxs = "";
    auto s1 = words(str1), s2 = words(str2);
    for (auto s : s1) {
        if (s2.count(s) && s.size() > maxs.size())
            maxs = s;
    }
    std::cout << maxs;
}