#include <iostream>
#include <cstring>

using namespace std;

int main()
{
    char s1[10] = {'1', '\0'};
    char* s2 = "467";
    char* s3 = "892";
    strcat(s1, s2);
    strcat(s1, s3);
    printf("%s\n", s1);
}
