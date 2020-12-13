[toc]


# C++ const

## const 修饰变量

```cpp
#include<iostream>
using namespace std;
int main() {
    int const a1 = 3;   // const data
    const int a2 = a1;    // const data

    const int *a3 = 3;   // const data, non-const pointer
    int const *a4 = 3;   // const data, non-const pointer
    int * const a5 = 3;   // non-const data,const pointer

    int const * const a6 = 3;   // const data,const pointer
    const int * const a7 = 3;   // const data,const pointer

    return 0;
}
```

# References
1. [C++ const用法 尽可能使用const - 旭东的博客 - 博客园](https://www.cnblogs.com/xudong-bupt/p/3509567.html)
