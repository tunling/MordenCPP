#include <vector>
#include <iostream>
class List
{
public:
    std::vector<int> vec;
    List(std::initializer_list<int> list)
    {
        for (auto it = list.begin(); it != list.end(); ++it)
        {
            vec.push_back(*it);
        }
    }
};
int main()
{
    List list = {1, 2, 3, 4, 5, 6};
    std::cout << "List: ";
    for (auto it = list.vec.begin(); it != list.vec.end(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    return 0;
}