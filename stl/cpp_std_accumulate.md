[toc]

# C++ std::accumulate

accumulate定义在#include<numeric>中，是用来进行 reduce 操作的。默认进行 +，也可以换成别的操作符。

## 1. 累加求和

```cpp
int sum = accumulate(vec.begin() , vec.end() , 42);
```

accumulate带有三个形参：头两个形参指定要累加的元素范围，第三个形参则是累加的初值。

accumulate函数将它的一个内部变量设置为指定的初始值，然后在此初值上累加输入范围内所有元素的值。accumulate算法返回累加的结果，其返回类型就是其第三个实参的类型。

可以使用accumulate把string型的vector容器中的元素连接起来：

```cpp
vector<string> v {"edward", "elric"};
string sum = accumulate(v.begin() , v.end() , string(""));
```

这个函数调用的效果是：从空字符串开始，把vec里的每个元素连接成一个字符串。

## 2.自定义数据类型的处理

C++ STL中有一个通用的数值类型计算函数— accumulate(),可以用来直接计算数组或者容器中C++内置数据类型，例如：

```cpp
#include <numeric>int arr[]={10,20,30,40,50};vector<int> va(&arr[0],&arr[5]);int sum=accumulate(va.begin(),va.end(),0);  //sum = 150
```

但是对于自定义数据类型，我们就需要自己动手写一个回调函数来实现自定义数据的处理，然后让它作为accumulate()的第四个参数，accumulate()的原型为

```cpp
template<class _InIt,	class _Ty,	class _Fn2> inline	_Ty _Accumulate(_InIt _First, _InIt _Last, _Ty _Val, _Fn2 _Func)	{	// return sum of _Val and all in [_First, _Last), using _Func	for (; _First != _Last; ++_First)		_Val = _Func(_Val, *_First);	return (_Val);	}
```

【例1】

```cpp
#include <vector>#include <string>using namespace std; struct Grade{	string name;	int grade;}; int main(){	Grade subject[3] = {		{ "English", 80 },		{ "Biology", 70 },		{ "History", 90 }	}; 	int sum = accumulate(subject, subject + 3, 0, [](int a, Grade b){return a + b.grade; });	cout << sum << endl; 	system("pause");	return 0;}
```

# References
1. [std::accumulate - cppreference.com](https://zh.cppreference.com/w/cpp/algorithm/accumulate)
2. [C++的STL中accumulate的用法_Ethan的博客-CSDN博客_c++ accumulate](https://blog.csdn.net/u011499425/article/details/52756242)
