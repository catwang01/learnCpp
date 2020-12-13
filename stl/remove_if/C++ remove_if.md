[toc]

# C++ remove_if


函数 `remove_if` 移除序列 ``[start, end)` `中所有应用于谓词p返回true的元素.

此函数返回一个指向被修剪的序列的最后一个元素迭代器.

记住, `remove_if` 并不会实际移除序列 `[start, end)` 中的元素; 如果在一个容器上应用 `remove_if`, 容器的长度并不会改变(remove_if()不可能仅通过迭代器改变容器的属性), 所有的元素都还在容器里面. 实际做法是, remove_if()将所有应该移除的元素都移动到了容器尾部并返回一个分界的迭代器. 移除的所有元素仍然可以通过返回的迭代器访问到. 为了实际移除元素, 你必须对容器自行调用erase()以擦除需要移除的元素. 这也是[erase-remove idiom](http://en.wikipedia.org/wiki/Erase-remove_idiom "http://en.wikipedia.org/wiki/Erase-remove_idiom")名称的由来:

```
container.erase(remove_if(container.begin(), container.end(), pred), container.end());
```

1. `remove_if` 类似于[partition](http://huycwork.blog.163.com/blog/static/136751999201042095626262/)(), 但有两点不同: 1) 它们使用的谓词条件刚好相反. 2) remove_if只强调前面部分(第二部分不再需要了)
2. `remove_if` 以线性时间运行.
3. `remove_if` 不能用于关联容器如set<>或map<>.

```cpp
#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>

void printVector(const std::vector<int>& vec)
{
    for (int x : vec) std::cout << x << " ";
    std::cout << "\n";
}

int main()
{
    std::vector<int> vec {1, 2, 3, 4, 5};
    std::cout << "orignal vector:\n";
    printVector(vec);

    std::vector<int>::iterator it = remove_if(vec.begin(), vec.end(), [](int x) { return x % 2 == 0;});
    std::cout << "vector after remove_if:\n";
    printVector(vec);

    vec.erase(it, vec.end());
    std::cout << "vector after erase:\n";
    printVector(vec);
}
```

# References
1. [c++ remove_if - 张东升 - 博客园](https://www.cnblogs.com/zhangdongsheng/p/8590221.html)
