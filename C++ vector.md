[toc]

# C++ vector

## 基本使用

vector 模板在 vector 头文件中，`#include <vector>` 导入

```
1. push_back 在数组的最后添加一个数据

2. pop_back 去掉数组的最后一个数据

3. at 得到编号位置的数据

4. begin 得到数组头的指针

5. end 得到数组的最后一个单元+1的指针

6. front 得到数组头的引用

7. back 得到数组的最后一个单元的引用

8. max_size 得到vector最大可以是多大

9. capacity 当前vector分配的大小

10. size 当前使用数据的大小

11. resize 改变当前使用数据的大小，如果它比当前使用的大，则填充默认值

12. reserve 改变当前vecotr所分配空间的大小

13. erase 删除指针指向的数据项

14. clear 清空当前的vector

15. rbegin 将vector反转后的开始指针返回(其实就是原来的end-1)

16. rend 将vector反转后的结束指针返回(其实就是原来的begin-1)

17. empty 判断vector是否为空

18. swap 与另一个vector交换数据
```

### 初始化

#### 一维vector

##### 1. 默认初始化，默认值为全0； 可通过push_back()添加元素。

```cpp
vector<int> v1;  // vector为空，size为０，未开辟空间
vector<int> v2(5); // 指定vector大小为5 ，元素初始值默认为０，
vector<int> v3(5, 2); // 指定vector大小为5，元素初始值默认为2
```

**注意**：vector是动态的，因此可以先声明一个空的，再往进填东西；

```
vector<int> v1;
v1.push_back(1);
v2.push_back(2);
```

##### 2. 直接初始化

需要支持 c++11 新特性

```
vector<int> vec1 = {1, 2, 3};
```

##### 3.用数组来初始化 vector

用数组元素的指针可以用作迭代器来初始化vector

```
int x[] = {1, 2, 3, 4};
vector<int> v4(&x[1], &x[3]); // {2, 3}; 注意 x[3]没有被赋值给 v4，这类似于python的切片 v4 = x[1:3]
vector<int> v5(x+1, x+3); // 这种写法和上面的本质相同，利用了指针算术
```

##### 4. 用已存在的vector初始化新的vector

```cpp
vector<int> v(5,2);
vector<int> v_copy(v);
```

#### 二维vector

注意是

```
vector<vector <int> >
```

不是
```
vector<vector <int>>
```

否则有的编译器会报错 `a space is required between consecutive right angle brackets (use '> >')`。这是因为 `>>` 可能会和位移操作混淆；


##### 1. 全部初始化为某个数

```cpp
// 初始化二维vector，为r*c的vector，所有值为3.
vector<vector<int> > newOne(r, vector<int>(c, 3));
```

##### 2. 直接初始化

```
vector<vector<int> > vec = { {1,4,3}, {1,4,7} , {1,3,5} , {2,9,4} , {2,5,8} , {3,9,6} }
```

### size

### back

返回最后一个元素的引用。

### 遍历

#### 遍历一维向量

```
for (int i=0; i<s.size(); i++)
    cout << s[i] << endl;
```

#### 遍历二维向量

```
for(int i=0; i<s.size(); i++)
{
    for(int j=0; j<s[0].size(); j++)
    {
        cout << setw(5) << s[i][j];
    }
    cout << endl;
    }
```

### insert插入数据

需要使用指针进行操作

```
#include<iostream>
#include<vector>
using namespace std;
 
int main()
{
    vector<int> v(3);
    v[0]=2; v[1]=7; v[2]=9;
    
    //在最前面的元素前插入8
    v.insert(v.begin(),8);
    
    //在第二个元素前插入新元素1
    v.insert(v.begin()+2,1);
    
    //在末尾插入新元素1
    v.insert(v.end(),3);     
    
    for(vector<int>::iterator it=v.begin();it!=v.end();it++)
        cout<<*it<<endl;
    return 0;   
}
```

### erase 删除数据项

只需要注意是通过指针来删除的。

```
int x[3] = {1,2,3};
vector<int> a(x, x+3);
a.erase(a.begin() + 2); // 删除 a[2], 删除后 a = {1,2}
```

### 二维vector添加新项

```
int vector<int> tmp = {1, 2, 3};
vec.push_back(tmp);
```

实际上不需要重新声明一个vector，可以直接push_back

```
vec.push_back({1, 2, 3});
```

### clear

使用 clear 之后，size 会变成 0；

### find

`find` 不是vector本身的方法，而是在 algorithm 头文件中的

```
#include <iostream>
#include <algorithm>
#include <vector>
 
int main()
{
    using namespace std;
 
    vector<int> vec;
    for (int i=1; i<=6; i++) vec.push_back(i);
    vector<int>::iterator it = find(vec.begin(), vec.end(), 6);
 
    if (it != vec.end())
        cout<<*it<<endl;
    else
        cout<<"can not find"<<endl;
 
    return 0;
}
```

### vector revsere逆序

1. method 1: 使用 .rbeign() 和 .rend()

```
#include <vector>
#include <iomanip>
#include <iostream>

using namespace std;
int main() {
    int x[] = {1,2,3,4,5};
    int n = sizeof(x) / sizeof(x[0]);
    vector<int> v(x, x+5);
    vector<int> reversed(v.rbegin(), v.rend());

    for (int i=0; i<n; i++)
        cout << setw(3) << reversed[i]; 
    cout << endl;
}
```
 
2. method2 利用 swap 函数

```
#include <vector>
#include <iomanip>
#include <iostream>

using namespace std;
int main() {
    int x[] = {1,2,3,4,5};
    int n = sizeof(x) / sizeof(x[0]);
    vector<int> v(x, x+5);
    for (int i=0; i<n/2; i++) {
        swap(v[i], v[n-1-i]);
    }

    for (int i=0; i<n; i++)
        cout << setw(3) << v[i];
    cout << endl;
}
```


# References
- [关于C++中vector初始化问题 - 君只见独不见的专栏 - CSDN博客](https://blog.csdn.net/GreenHandCGL/article/details/82798049)
- [c++ vector函数说明在最前面插入数据_C/C++_计算机天堂_技术学习的天堂](http://www.jsjtt.com/bianchengyuyan/cyuyan/40.html)
- [C++ vector 容器浅析 | 菜鸟教程](https://www.runoob.com/w3cnote/cpp-vector-container-analysis.html)
- [c++中vector find使用 - test1280](https://blog.csdn.net/test1280/article/details/65937779)
