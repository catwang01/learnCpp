[toc]

class EX{
	public:
		void constFunction() const;
};

如上所示，通过将类成员函数声明为const,以表示这个函数不可以修改类中的成员变量。
建议将不可以修改数据成员的函数声明为const，如果在编写const成员函数时，不慎修改了数据成员， 或者调用了其他的非const函数，则此时编译器会指出错误，这样做提高了程序的健壮性。

在相同的函数参数及相同的名字的情况下，const函数与非const函数可以构成重载函数，但是const成员函数不能改变任何的非静态变量

## const函数与非const函数的调用规则

1. const对象默认调用const成员函数，非const对象默认调用非const成员函数；
2. 在同时存在const函数和非const重载函数的前提下，若非const对象想调用const成员函数，则需要显示的转化，例如 `(const Student&)obj.getAge();`
若const对象想调用非const成员函数，同理进行强制类型转换const_cast < Student&>(constObj).getAge();(注意constObj一定要加括号)
当类中只有一种函数存在的情况

非const对象可以调用const成员函数或者非const成员函数
const对象只能调用const成员函数，若直接调用非const成员函数编译器会报错。
