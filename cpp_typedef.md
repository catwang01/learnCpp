[toc]

# C++ typedef

不管实在C还是C++代码中，typedef这个词都不少见，当然出现频率较高的还是在C代码中。typedef与#define有些相似，但更多的是不同，特别是在一些复杂的用法上，就完全不同了，看了网上一些C/C++的学习者的博客，其中有一篇关于typedef的总结还是很不错，由于总结的很好，我就不加修改的引用过来了，以下是引用的内容(红色部分是我自己写的内容)。

## 用途一：

定义一种类型的别名，而不只是简单的宏替换。可以用作同时声明指针型的多个对象。比如：

```
char* pa, pb; // 这多数不符合我们的意图，它只声明了一个指向字符变量的指针，和一个字符变量；
```

以下则可行：

```
typedef char* PCHAR;

PCHAR pa, pb;  
```

## 用途二:

用在旧的C代码中，帮助struct。以前的代码中，声明struct新对象时，必须要带上struct，即形式为： struct 结构名对象名，如：

```
struct tagPOINT1
{
    int x;
    int y;
};

struct tagPOINT1 p1;
```

使用 `typedef` 

```
typedef struct tagPOINT
{
    int x;
    int y;
} POINT;

POINT p1; // 这样就比原来的方式少写了一个struct，比较省事，尤其在大量使用的时
```

而在C++中，则可以直接写：结构名对象名，即：

```
tagPOINT1 p1;
```

或许，在C++中，typedef的这种用途二不是很大，但是理解了它，对掌握以前的旧代码还是有帮助的，毕竟我们在项目中有可能会遇到较早些年代遗留下来的代码。

## 用途三 用typedef来定义与平台无关的类型。

比如定义一个叫 REAL 的浮点类型，在目标平台一上，让它表示最高精度的类型为：

```
typedef long double REAL;
```


在不支持 long double 的平台二上，改为：

```
typedef double REAL;
```

在连 double 都不支持的平台三上，改为：

```
typedef float REAL;
```

也就是说，当跨平台时，只要改下 typedef 本身就行，不用对其他源码做任何修改。

标准库就广泛使用了这个技巧，比如size_t。另外，因为typedef是定义了一种类型的新别名，不是简单的字符串替换，所以它比宏来得稳健。
这个优点在我们写代码的过程中可以减少不少代码量哦！

# References
1. [关于typedef的用法总结 - Bigcoder - 博客园](https://www.cnblogs.com/csyisong/archive/2009/01/09/1372363.html)
