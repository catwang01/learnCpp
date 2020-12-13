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
