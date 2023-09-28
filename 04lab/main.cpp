#include <iostream>
#include <cstring>

double vowels_percentage(const char* str) {
    const int len = strlen(str);
    int vowels = 0;
    for (int i = 0; i < len; ++i)
        if (strchr("aeiou", str[i]))
            ++vowels;
    return vowels / static_cast<double>(len);
}

char* find(char *str) {
    char* p_max_vows = NULL;
    double v_max_vows = 0;
    char* pch = strtok(str, " ");
    while (pch) {
        double p = vowels_percentage(pch);
        if (p > v_max_vows)
            v_max_vows = p, p_max_vows = pch;
        pch = strtok(NULL, " ");
    }
    return p_max_vows;
}

int main() {
    char* str = new char[1024];
    scanf("%1023[^\n]s", str);
    std::cout << find(str);
}