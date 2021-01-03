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
此外constexpr修饰的函数支持递归（且从C++14开始，可以在内部使用局部变量、循环和分支等简单语句）。
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
对变量进行类型推导。<br>
常用于迭代器。<br>
不能用于函数传参，不能用于推导数组类型。
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
可以对表达式进行类型推导。<br>
用于声明而不定义，保留const，推断出引用。
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
C++14。
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
C++14，用于对转发函数或封装的返回类型进行推导。
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
编译时完成分支判断。
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
C++11，简洁的循环语句。
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
# 5. 模板
## 5.1 外部模板
避免重复实例化导致编译时间增加。<br>
用于显式通知编译器何时进行模板的实例化。
```cpp
template class std::vector<bool>;          //强行实例化
extern template class std::vector<double>; //不在该当前编译文件中实例化模板
```
## 5.2 嵌套模板
传统C++不支持嵌套模板（连续右尖括号不合法），C++11开始可用。
```cpp
//true_type and false_type
template<bool T>
class Test
{
    bool test = T;
}
std::vector<Test<(1>2)>> test; //合法，但不建议写这样的代码
```
##  5.3 类别名模板
传统C++中，typedef不可为模板定义新的名称，C++11中使用using可以实现，且using可以实现和typedef原有功能一样的功能。
```cpp
//代替typedef
//FP为别名
typedef void (*FP) (int, const std::string&);
using FP = void (*) (int, const std::string&);
```
```cpp
//类别名模板
template<typename T, typename U>
class MagicType 
{
public:
    T dark;
    U magic;
};
//TrueDarkMagic是别名
template<typename T>
using TrueDarkMagic = MagicType<std::vector<T>, std::string>;
```
## 5.4 默认模板参数
指定模板的默认参数。
```cpp
template<typename T = int, typename U = int>
auto add(T x, U y) -> decltype(x+y) 
{   //拖尾返回类型
    return x+y;
}
auto ret = add(1,3); //无需 auto ret = add<int, int>(1,3);
```
## 5.5 变长参数模板
C++17支持变参模板展开，可用于实现内存池等。
```cpp
template<typename T0, typename... T>
void printf1(T0 t0, T... t) 
{
    std::cout << t0 << std::endl;
    if constexpr(sizeof...(t) > 0)  //sizeof...(args)用于计算参数个数
        printf1(t...);
}
```
等价于以下递归模板函数写法。
```cpp
template<typename T0>
void printf2(T0 value) 
{
    std::cout << value << std::endl;
}
template<typename T, typename... Ts>
void printf2(T value, Ts... args) 
{
    std::cout << value << std::endl;
    printf2(args...);
}
```
也可使用以下初始化列表展开方法，无需终止条件。
```cpp
template<typename T, typename... Ts>
auto printf3(T value, Ts... args) 
{
    std::cout << value << std::endl;
    (void) std::initializer_list<T>
    {
    ([&args]{std::cout << args << std::endl;}(), value)...
    }; //使用了lambda表达式
}
```
## 5.6 折叠表达式
C++17中将变长参数这种特性进一步带给了表达式。
```cpp
#include <iostream>
using namespace std;
template <typename... T>
auto sum(T... t)
{
    return (t + ...);
}
int main()
{
    cout << sum(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) << endl;
    return 0;
}
```
## 5.7 非类型模板参数推导
非类型模板参数形式。
```cpp
template <typename T, int BufSize>
class buffer_t 
{
public:
    T& alloc();
    void free(T& item);
private:
    T data[BufSize];
}
buffer_t<int, 100> buf; // 100 作为模板参数
```
非类型模板参数推导。
```cpp
template <auto value> 
void foo() 
{
    std::cout << value << std::endl;
    return;
}
int main() 
{
    foo<10>();  // value 被推导为 int 类型
}
```
# 6. 面向对象
## 6.1 委托构造
在同一个类中一个构造函数调用另一个构造函数。
```cpp
#include <iostream>
class Base
{
public:
    int value1;
    int value2;
    Base()
    {
        value1 = 1;
    }
    Base(int value) : Base() //委托Base()构造函数
    { 
        value2 = value;
    }
};
int main()
{
    Base b(2);
    std::cout << b.value1 << std::endl;
    std::cout << b.value2 << std::endl;
}
```
## 6.2 继承构造
C++11使用using引入继承构造函数。
```cpp
#include <iostream>
class Base
{
public:
    int value1;
    int value2;
    Base()
    {
        value1 = 1;
    }
    Base(int value) : Base()
    { // 委托 Base() 构造函数
        value2 = value;
    }
};
class Subclass : public Base
{
public:
    using Base::Base; // 继承构造
};
int main()
{
    Subclass s(3);
    std::cout << s.value1 << std::endl;
    std::cout << s.value2 << std::endl;
}
```
## 6.3 显式虚函数重载
避免意外重载虚函数。<br>
override: 显式告知编译器进行重载，编译器将检查基函数是否存在这样的虚函数，否则将无法通过编译。
```cpp
struct Base 
{
    virtual void foo(int);
};
struct SubClass: Base 
{
    virtual void foo(int) override; // 合法
    virtual void foo(float) override; // 非法, 父类没有此虚函数
};
```
final:防止类被继续继承以及终止虚函数继续重载而引入。
```cpp
struct Base 
{
    virtual void foo() final;
};
struct SubClass1 final: Base {}; // 合法

struct SubClass2 : SubClass1 {}; // 非法, SubClass1 已 final

struct SubClass3: Base 
{
    void foo(); // 非法, foo 已 final
};
```
## 6.4 显式禁用默认函数
C++11允许显式的声明采用或拒绝编译器自带的函数。
```cpp
class Magic 
{
public:
    Magic() = default; // 显式声明使用编译器生成的构造
    Magic& operator=(const Magic&) = delete; // 显式声明拒绝编译器生成构造
    Magic(int magic_number);
}
```
## 6.5 强类型枚举
C++11引入枚举类。实现了类型安全，首先他不能够被隐式的转换为整数，同时也不能够将其与整数数字进行比较，更不可能对不同的枚举类型的枚举值进行比较。但相同枚举值之间如果指定的值相同，那么可以进行比较。
```cpp
enum class new_enum : unsigned int
{
    value1,
    value2,
    value3 = 100,
    value4 = 100
};
if (new_enum::value3 == new_enum::value4) 
{
    // 会输出
    std::cout << "new_enum::value3 == new_enum::value4" << std::endl;
}
```
希望获得枚举值的值时，将必须显式的进行类型转换，不过我们可以通过重载 << 这个算符来进行输出，可以收藏下面这个代码段。
```cpp
#include <iostream>
template<typename T>
std::ostream& operator<<(typename std::enable_if<std::is_enum<T>::value, std::ostream>::type& stream, const T& e)
{
    return stream << static_cast<typename std::underlying_type<T>::type>(e);
}
std::cout << new_enum::value3 << std::endl
```