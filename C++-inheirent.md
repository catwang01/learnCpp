[toc]


# C++ public private protected 

公有继承(public)、私有继承(private)、保护继承(protected)是常用的三种继承方式。

## 1. 公有继承(public)

公有继承的特点是基类的公有成员和保护成员作为派生类的成员时，它们都保持原有的状态，而基类的私有成员仍然是私有的，不能被这个派生类的子类所访问。

## 2. 私有继承(private)

私有继承的特点是基类的公有成员和保护成员都作为派生类的私有成员，并且不能被这个派生类的子类所访问。

## 3. 保护继承(protected)

保护继承的特点是基类的所有公有成员和保护成员都成为派生类的保护成员，并且只能被它的派生类成员函数或友元访问，基类的私有成员仍然是私有的。

下面列出三种不同的继承方式的基类特性和派生类特性。

|   | public | protected | private |
| -- | -- | -- | --- |
| 公有继承 | public | protected | 不可见 |
| 私有继承 | private | private | 不可见 |
| 保护继承 | protected | protected | 不可见 |

在上图中：
1. 基类成员对派生类都是：公有和保护的成员是可见的，私有的的成员是不可见的。

2. 基类成员对派生类的对象来说：要看基类的成员在派生类中变成了什么类型的成员。如：私有继承时，基类的公有成员和私有成员都变成了派生类中的私有成员，因此对于派生类中的对象来说基类的公有成员和私有成员就是不可见的。

#### 对于公有继承方式

(1) 基类成员对其对象的可见性：

公有成员可见，其他不可见。这里保护成员同于私有成员。

(2) 基类成员对派生类的可见性：

公有成员和保护成员可见，而私有成员不可见。这里保护成员同于公有成员。

(3) 基类成员对派生类对象的可见性：

公有成员可见，其他成员不可见。

所以，在公有继承时，派生类的对象可以访问基类中的公有成员；

派生类的成员函数可以访问基类中的公有成员和保护成员。

这里，一定要区分清楚派生类的对象和派生类中的成员函数对基类的访问是不同的。

#### 对于私有继承方式

(1) 基类成员对其对象的可见性：

公有成员可见，其他成员不可见。

(2) 基类成员对派生类的可见性：

公有成员和保护成员是可见的，而私有成员是不可见的。

(3) 基类成员对派生类对象的可见性：

所有成员都是不可见的。

所以，在私有继承时，基类的成员只能由直接派生类访问，而无法再往下继承。

#### 对于保护继承方式

这种继承方式与私有继承方式的情况相同。两者的区别仅在于对派生类的成员而言，对基类成员有不同的可见性。

上述所说的可见性也就是可访问性。

关于可访问性还有另一种说法。这种规则中，称派生类的对象对基类访问为水平访问，称派生类的派生类对基类的访问为垂直访问。

### 示例

```cpp
#include<iostream>
using namespace std;
//////////////////////////////////////////////////////////////////////////
class A       //父类
{
private:
    int privatedateA;
protected:
    int protecteddateA;
public:
    int publicdateA;
};
//////////////////////////////////////////////////////////////////////////
class B :public A      //基类A的派生类B（公有继承）
{
public:
    void funct()
    {
        int b;
        b=privatedateA;   //error：基类中私有成员在派生类中是不可见的
        b=protecteddateA; //ok：基类的保护成员在派生类中为保护成员
        b=publicdateA;    //ok：基类的公共成员在派生类中为公共成员
    }
};
//////////////////////////////////////////////////////////////////////////
class C :private A  //基类A的派生类C（私有继承）
{
public:
    void funct()
    {
        int c;
        c=privatedateA;    //error：基类中私有成员在派生类中是不可见的
        c=protecteddateA;  //ok：基类的保护成员在派生类中为私有成员
        c=publicdateA;     //ok：基类的公共成员在派生类中为私有成员
    }
};
//////////////////////////////////////////////////////////////////////////
class D :protected A   //基类A的派生类D（保护继承）
{
public:
    void funct()
    {
        int d;
        d=privatedateA;   //error：基类中私有成员在派生类中是不可见的
        d=protecteddateA; //ok：基类的保护成员在派生类中为保护成员
        d=publicdateA;    //ok：基类的公共成员在派生类中为保护成员
    }
};
//////////////////////////////////////////////////////////////////////////
int main()
{
    int a; 
 
    B objB;
    a=objB.privatedateA;   //error：基类中私有成员在派生类中是不可见的,对对象不可见
    a=objB.protecteddateA; //error：基类的保护成员在派生类中为保护成员，对对象不可见
    a=objB.publicdateA;    //ok：基类的公共成员在派生类中为公共成员，对对象可见
 
    C objC;
    a=objC.privatedateA;   //error：基类中私有成员在派生类中是不可见的,对对象不可见
    a=objC.protecteddateA; //error：基类的保护成员在派生类中为私有成员，对对象不可见
    a=objC.publicdateA;    //error：基类的公共成员在派生类中为私有成员，对对象不可见
 
    D objD;
    a=objD.privatedateA;   //error：基类中私有成员在派生类中是不可见的,对对象不可见
    a=objD.protecteddateA; //error：基类的保护成员在派生类中为保护成员，对对象不可见
    a=objD.publicdateA;    //error：基类的公共成员在派生类中为保护成员，对对象不可见
 
    return 0;
}
```

# References
1. [C++继承：公有，私有，保护 - csqlwy - 博客园](https://www.cnblogs.com/qlwy/archive/2011/08/25/2153584.html)