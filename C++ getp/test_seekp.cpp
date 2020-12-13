#include <iostream>
#include <fstream>

using namespace std;
int main()
{
    ifstream in("test.txt");

    //基地址为文件结束处，偏移地址为0，于是指针定位在文件结束处
    in.seekg(0, ios::end);
    //sp为定位指针，因为它在文件结束处，所以也就是文件的大小
    streampos sp = in.tellg();
    cout << "file size:" << sp << endl;
}
