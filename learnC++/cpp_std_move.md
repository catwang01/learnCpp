[toc] 

# c++11 std::move() 的使用

std::move函数可以以非常简单的方式将左值引用转换为右值引用。
通过std::move，可以避免不必要的拷贝操作。
std::move是为性能而生。
std::move是将对象的状态或者所有权从一个对象转移到另一个对象，只是转移，没有内存的搬迁或者内存拷贝。

最典型的应用是将一个 string 对象 push_back 到一个 vector 对象中时可以使用 move。

```
#include <iostream>
#include <utility>
#include <vector>
#include <string>
int main()
{
    std::string str = "Hello";
    std::vector<std::string> v;

    v.push_back(str); //调用常规的拷贝构造函数，新建字符数组，拷贝数据
                    // v 中的 "Hello" 和 str 中的 "Hello" 不是同一个对象，v 中的是 str 中的 copy 出来的
    std::cout << "After copy, str is \"" << str << "\"\n";

    v.push_back(std::move(str)); //调用移动构造函数，掏空str，掏空后，最好不要使用str
    std::cout << "After move, str is \"" << str << "\"\n"; // 此时 str 中没有内容了
    std::cout << "The contents of the vector are \"" << v[0]
                                         << "\", \"" << v[1] << "\"\n";
}
```
 
 输出结果

```
After copy, str is "Hello"
After move, str is ""
The contents of the vector are "Hello", "Hello"
```

# References
1. [c++11 std::move() 的使用 - 那一剑的風情 - 博客园](https://www.cnblogs.com/SZxiaochun/p/8017349.html)

