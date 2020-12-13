#include <string>
#include <iostream>

using namespace std;

int main()
{
    char tmp[10];
    sprintf(tmp, "%d %d %d\n", 1, 2, 3);
    printf(tmp);
}
