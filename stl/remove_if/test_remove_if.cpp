#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>

void printVector(const std::vector<int>& vec)
{
    for (int x : vec) std::cout << x << " ";
    std::cout << "\n";
}

int main()
{
    std::vector<int> vec {1, 2, 3, 4, 5};
    std::cout << "orignal vector:\n";
    printVector(vec);

    std::vector<int>::iterator it = remove_if(vec.begin(), vec.end(), [](int x) { return x % 2 == 0;});
    std::cout << "vector after remove_if:\n";
    printVector(vec);

    vec.erase(it, vec.end());
    std::cout << "vector after erase:\n";
    printVector(vec);
}
