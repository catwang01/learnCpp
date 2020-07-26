[toc]

# C++ heap

C++可以使用优先队列 <priority_queue>，但是灵活性不如使用 <algorithm> 中的 `make_heap` 来将其一个 vector 变成堆序

<algorithm> 中的heap相关的函数类似于 python 中的 heapq 。

## 基本使用 

建堆 make_heap

### 加入元素 push_heap

插入位于位置 last-1 的元素到范围 [first, last-1) 所定义的最大堆中。函数的第一版本用 operator< 比较元素，第二版本用给定的比较函数 comp 。

### 弹出堆顶 pop_heap

交换在位置 first 的值和在位置 last-1 的值，并令子范围 [first, last-1) 变为堆。这拥有从范围 [first, last) 所定义的堆移除首个元素的效果。

###  创建大根堆

直接使用 `make_heap` 创建出来的是大根堆

``` 
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
int main(int argc, char *argv[])
{
    vector<int> v = { 3, 2, 4, 1, 5, 9 }; 
    make_heap(v.begin(), v.end(), greater<int>());
    for (int x: v) cout << x  << " ";
    cout << endl;
    return 0;
}
```

```
9 5 4 1 2 3
```

### 创建小根堆

```
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool cmp(int x, int y) {
    return x > y;
}

void print(const vector<int>& v ) {
    for(int x: v) cout << x << ' ';
    cout << endl;
}
int main(int argc, char *argv[])
{
    vector<int> v = { 3, 2, 4, 1, 5, 9 }; 
    // 建堆
    make_heap(v.begin(), v.end(), cmp);
    print(v);
    // 原来的堆顶被移动到vector末尾了
    pop_heap(v.begin(), v.end(), cmp);
    print(v);
    return 0;
}
```

``
1 2 4 3 5 9
2 3 4 9 5 1
```

