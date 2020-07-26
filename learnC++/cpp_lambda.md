[toc]

# C++ lambda  表达式 demo

```
#include <iostream>
#include <vector>

using namespace std;

int main()
{
    int maxsize = 100;
    vector<int> table = [&]() {vector<int> tmp(maxsize); for (int i = 0; i < maxsize; i++) tmp[i] = i; return tmp;}
    ();
    for (auto x : table)
        cout << x << endl;
}

```


# References
1. [word2vec/word2vec.h at 3b1e8880915b5d04f74c739f640c3eacafb88464 · jdeng/word2vec](https://github.com/jdeng/word2vec/blob/3b1e8880915b5d04f74c739f640c3eacafb88464/word2vec.h#L528)
