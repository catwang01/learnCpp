#include <iostream>
#include <numeric>
#include <vector>
#include <string>
#include <numeric>

using namespace std;

void demo1()
{
    int A[] = {1, 2, 3, 4, 5};
    cout << accumulate(A, A + 5, 0) << endl;
}

void demo2()
{
    vector<string> v {"edward", "elric"};
    string ret = accumulate(v.begin(), v.end(), string(""));
    cout << ret << "\n";
}

void demo3()
{
    vector<string> v {"edward", "elric"};
    string ret = accumulate(v.begin(), v.end(), string(""), [](string x, string y) {return x + " " + y;});
    cout << ret << "\n";
}

int main()
{
    demo1();
    demo2();
    demo3();
}
