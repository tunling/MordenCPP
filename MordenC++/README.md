# 环境
```
clang++
clang version 6.0.0-1ubuntu2 (tags/RELEASE_600/final)
Target: x86_64-pc-linux-gnu
Thread model: posix

-std=c++2a
```
# 1. 常量
## 1.1 nullptr
用于区分指针、0，其类型为nullptr_t，是能隐式转换为任何指针或成员指针的类型，并能和他们进行相等或者不等的比较。<br>
运行下面代码可以看到，NULL与0、void*、nullptr均不相同，对于空指针，应该直接使用nullptr。
```cpp
#include <iostream>
#include <type_traits>
void test(char *);
void test(int);
int main()
{
    if (std::is_same<decltype(NULL), decltype(0)>::value)
        std::cout << "NULL == 0" << std::endl;
    else
        std::cout << "NULL != 0" << std::endl;
    if (std::is_same<decltype(NULL), decltype((void *)0)>::value)
        std::cout << "NULL == (void *)0" << std::endl;
    else
        std::cout << "NULL != (void *)0" << std::endl;
    if (std::is_same<decltype(NULL), std::nullptr_t>::value)
        std::cout << "NULL == nullptr" << std::endl;
    else
        std::cout << "NULL != nullptr" << std::endl;
    test(nullptr);
    //test(NULL); //不能通过编译
    test(0);
    return 0;
}
void test(char *)
{
    std::cout << "call test(char*)" << std::endl;
}
void test(int i)
{
    std::cout << "call test(int)" << std::endl;
}

```
## 1.2 constexpr
常量表达式，常用于数组定义阶段。<br>
此外constexpr修饰的函数支持递归（且从C++14开始，可以在内部使用局部变量、循环和分支等简单语句）
```cpp
#include <iostream>
#define LEN 10
int len()
{
    int i = 2;
    return i;
}
const int len_constexpr()
{
    return 5;
}
const int fib(const int n)
{
    return n == 1 || n == 2 ? 1 : fib(n - 1) + fib(n - 2);
}
int main()
{
    char a1[10];  //合法
    char a2[LEN]; //合法
    int len1 = 10;
    //char a3[len1]; //非法
    const int len2 = len1 + 1;
    //char a4[len2]; //非法
    constexpr int len2_constexpr = 1 + 2 + 3;
    char a5[len2_constexpr]; //合法
    //char a6[len()]; //非法
    char a7[len_constexpr() + 1]; //合法
    std::cout << fib(10) << std::endl;
    //55
    return 0;
}
```
# 2. 变量及其初始化
## 2.1 if / switch变量声明强化
在if条件判断内使用变量声明。
```cpp
#include <iostream>
#include <vector>
#include <algorithm>
int main()
{
    std::vector<int> vec = {1, 2, 3, 4};
    // 将临时变量放到 if 语句内
    if (const std::vector<int>::iterator itr = std::find(vec.begin(), vec.end(), 3);
        itr != vec.end())
    {
        *itr = 4;
    }
    //将输出1 2 4 4
    for (std::vector<int>::iterator element = vec.begin(); element != vec.end(); ++element)
        std::cout << *element << std::endl;
    return 0;
}
```
## 2.2 初始化列表
用于对于类对象的列表初始化。<br>
将初始化列表作为一种类型，称为std::initializer_list。
```cpp
#include <iostream>
#include <initializer_list>
#include <vector>
class init_list_test
{
private:
    std::vector<int> vec;

public:
    init_list_test(std::initializer_list<int> list)
    {
        for (std::initializer_list<int>::iterator it = list.begin();
             it != list.end(); ++it)
            vec.push_back(*it);
    }
    void show()
    {
        for (std::vector<int>::iterator it = vec.begin();
             it != vec.end(); ++it)
            std::cout << *it << std::endl;
    }
};
int main()
{
    //或者init_list_test test = {1, 2, 3, 4, 5};
    init_list_test test{1, 2, 3, 4, 5};
    //1 2 3 4 5
    test.show();
    return 0;
}
```
## 2.3 结构化绑定
结构化绑定提供了类似其他语言（例如python）中提供的多返回值的功能。
```cpp
#include <iostream>
#include <tuple>
std::tuple<int, double, std::string> test()
{
    return std::make_tuple(1, 3.14, "486");
}
int main()
{
    auto [x, y, z] = test();
    //1, 3.14, 486
    std::cout << x << ", " << y << ", " << z << std::endl;
    return 0;
}
```
# 3. 类型推导
## 3.1 auto
对变量进行类型推导<br>
常用于迭代器<br>
不能用于函数传参，不能用于推导数组类型
```cpp
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
```
## 3.2 decltype
可以对表达式进行类型推导<br>
用于声明而不定义，保留const，推断出引用
```cpp
#include <iostream>
using namespace std;
int main()
{
    auto x = 1;
    auto y = 2;
    decltype(x + y) z;
    //type x == int
    //type z == type x
    if (std::is_same<decltype(x), int>::value)
        std::cout << "type x == int" << std::endl;
    if (std::is_same<decltype(x), float>::value)
        std::cout << "type x == float" << std::endl;
    if (std::is_same<decltype(x), decltype(z)>::value)
        std::cout << "type z == type x" << std::endl;
    return 0;
}
```
## 3.3 尾返回类型推导
C++14
```cpp
#include <iostream>
using namespace std;
template <typename T, typename U>
auto add(T a, U b)
{
    return a + b;
}
int main()
{
    double a = 3.14;
    int b = 6;
    //9.14
    cout << add(a, b) << endl;
    return 0;
}
```
## 3.4 decltype(auto)
C++14，用于对转发函数或封装的返回类型进行推导
```cpp
#include <iostream>
#include <string>
using namespace std;
string lookup1();
string &lookup2();
//string look_up_a_string_1()
decltype(auto) look_up_string_1()
{
    return lookup1();
}
//string& look_up_a_string_1()
decltype(auto) look_up_string_2()
{
    return lookup2();
}
```
# 4. 控制流
## 4.1 if constexpr
编译时完成分支判断
```cpp
#include <iostream>
template <typename T>
auto print_type_info(const T &t)
{
    if constexpr (std::is_integral<T>::value)
    {
        return t + 1;
    }
    else
    {
        return t + 0.001;
    }
}
int main()
{
    std::cout << print_type_info(5) << std::endl;
    std::cout << print_type_info(3.14) << std::endl;
    return 0;
}
```
## 4.2 区间for迭代
C++11，简洁的循环语句
```cpp
#include <iostream>
#include <vector>
#include <algorithm>
int main()
{
    std::vector<int> vec = {1, 2, 3, 4};
    if (auto itr = std::find(vec.begin(), vec.end(), 3);
        itr != vec.end())
        *itr = 4;
    for (auto element : vec) //readonly
        std::cout << element << std::endl;
    for (auto &element : vec) //writeable
        element += 1;
    for (auto element : vec) //readonly
        std::cout << element << std::endl;
    //1 2 4 4 2 3 5 5
    return 0;
}
```
## 模板