toc]

# C++ streambuf 输入输出


一般情况下，在C++中，iostream内的cin和cout是比scanf和printf慢的，这主要是为了同时兼容iostream和stdio，iostream与stdio的缓冲区被绑到了一起，以及cin和cout的stream是绑定在一起的，这使得cin和cout有额外的开销

为了提高cin和cout的效率，我们可以取消iostream与stdio的同步，以及cin和cout的stream的绑定：

```
std::ios::sync_with_stdio(false); 
cin.tie(NULL); 
cout.tie(NULL);
```

这样cin与cout就比scanf和printf快了。在本机测试上，iostream甚至比stdio快了6倍左右。然而这样做之后，就不可以将iostream与stdio混用了，然而输入量较大的时候，这种方法仍然无能为力

在stdin中，我们有getchar，想要追求更快的速度，我们有fread

在iostream中我们同样可以达到同样的效率，甚至更快，我们需要使用 streambuf 对象

## streambuf

获取cin的streambuf：

```
std::streambuf *fb = cin.rdbuf();
```

### sbumpc

然后我们就可以进行类似stdio中的操作：

对于getchar，我们有sbumpc，它的用法等同于getchar，速度略快于getchar：

```
char ch = fb -> sbumpc();
```

### sgetn

对于fread，我们有sgetn，它的用法与fread类似，速度略快于fread：

```
#define MAX_INPUT 1000000
char buf[MAX_INPUT];
 int main() {
    fb -> sgetn(buf, MAX_INPUT);
    return 0;
}
```

### sputc


对于putchar，我们有sputc，它的用法等同于putchar，速度略快于putchar：

```
char ch = 'n';
fb -> sputc(ch);
```

### sputn

对于fwrite，我们有sputn，它的用法与fwrite类似，速度略快于fwrite

```
#define MAX_OUTPUT 1000000
char buf[MAX_OUTPUT]; 
int main() {
    fb -> sputn(buf, MAX_OUTPUT);
    return 0;
}
```


这样我们就有了一份完整的快速输入输出 丢人代码（因为sputn的那份比sputc还慢，所以这里就用了sputc的代码）：

# References
1. [丢人笔记：黑科技——使用streambuf加速读入输出 - hinanawi - 博客园](https://www.cnblogs.com/hinanawitenshi/p/8830097.html)