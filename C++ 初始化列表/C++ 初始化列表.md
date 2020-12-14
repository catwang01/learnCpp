[toc]

# C++ 初始化列表

C++构造函数的初始化列表使得代码更加简洁，请看下面的例子：

和初始化列表有关对知识点如下：

1. 使用构造函数初始化列表并没有效率上的优势，仅仅是书写方便，尤其是成员变量较多时，这种写法非常简单明了。
2. 成员变量的初始化顺序与初始化列表中列出的变量的顺序无关，它只与成员变量在类中声明的顺序有关.
3. 初始化 const 成员变量的唯一方法就是使用初始化列表

##

```
class  Student{
private:
    char *m_name;
    int m_age;
    float m_score;
public:
     Student(char *name, int age, float score);
     void show();
 };

 //采用初始化列表
 Student::Student(char *name, int age, float score): m_name(name), m_age(age), m_score(score){
}
```

}


如本例所示，定义构造函数时并没有在函数体中对成员变量一一赋值，其函数体为空（当然也可以有其他语句），而是在函数首部与函数体之间添加了一个冒号 `:`，后面紧跟`m_name(name), m_age(age), m_score(score)`语句，这个语句的意思相当于函数体内部的`m_name = name; m_age = age; m_score = score;`语句，也是赋值的意思。

初始化列表可以用于全部成员变量，也可以只用于部分成员变量。下面的示例只对 m_name 使用初始化列表，其他成员变量还是一一赋值：

```
Student::Student(char *name, int age, float score): m_name(name){
    m_age = age;
    m_score = score;
}
```

这个实例中，我们对 m_name 使用了初始化列表，而其他变量选择了在函数体中进行初始化


注意，成员变量的初始化顺序与初始化列表中列出的变量的顺序无关，它只与成员变量在类中声明的顺序有关。请看代码：


### 初始化 const 成员变量

构造函数初始化列表还有一个很重要的作用，那就是初始化 const 成员变量。初始化 const 成员变量的唯一方法就是使用初始化列表。例如 VS/VC 不支持变长数组（数组长度不能是变量），我们自己定义了一个 VLA 类，用于模拟变长数组，请看下面的代码：

```
class VLA{
private:
    const int m_len;
    int *m_arr;
public:
    VLA(int len);
};

//必须使用初始化列表来初始化 m_len
VLA::VLA(int len): m_len(len){
    m_arr = new int[len];
}
```

VLA 类包含了两个成员变量，m_len 和 m_arr 指针，需要注意的是 m_len 加了 const 修饰，只能使用初始化列表的方式赋值

# References
1. [C++构造函数初始化列表](http://c.biancheng.net/view/2223.html)

