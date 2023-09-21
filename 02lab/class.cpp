#include <iostream>
#include <cstdlib>

class Array {
public:
    Array(int size) : arr(new int[size]), n(size) {}
    
    void init() {
        for (int i = 0; i < n; ++i)
            arr[i] = rand() % 100;
    }
    
    int search(int v) {
        for (int i = 0; i < n; ++i)
            if (arr[i] == v) return i;
        return -1;
    }

    int min() {
        int m = arr[0];
        for (int i = 0; i < n; ++i)
            if (m > arr[i]) m = arr[i];
        return m;
    }

    int max() {
        int m = arr[0];
        for (int i = 0; i < n; ++i)
            if (m < arr[i]) m = arr[i];
        return m;
    }

    Array merge(const Array& other) {
        Array c(n + other.n);
        for (int i = 0; i < n; ++i) c.arr[i] = arr[i];
        for (int j = 0; j < other.n; ++j) c.arr[n + j] = other.arr[j];
        return c;  
    }

    void sort() {
        for (int i = 0; i < n; ++i)
            for (int j = i + 1; j < n; ++j)
                if (arr[i] > arr[j])
                    std::swap(arr[i], arr[j]);
    }

    void print() {
        for (int i = 0; i < n; ++i)
            std::cout << arr[i] << ' ';
        std::cout << '\n';
    }
    
    ~Array() { delete[] arr; } 

private:
    int *arr;
    int n;
};

int main() {
    srand(0);
    Array a(10);
    a.init();
    a.print();
    a.sort();
    a.print();

    Array b(7);
    b.init();
    b.print();

    Array c = a.merge(b);
    c.print();
    std::cout << c.min() << ' ' << c.max() << '\n';

    return 0;
}
