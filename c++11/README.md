# 环境
clang++
clang version 6.0.0-1ubuntu2 (tags/RELEASE_600/final)
Target: x86_64-pc-linux-gnu
Thread model: posix

-std=c++2a

# 1. 常量
## nullptr
用于区分指针、0，其类型为nullptr_t，是能隐式转换为任何指针或成员指针的类型，并能和他们进行相等或者不等的比较。
运行下面代码可以看到，NULL与0、void*、nullptr均不相同，对于空指针，应该直接使用nullptr。
```cpp
#include<iostream>
#include<type_traits>
void test(char*);
void test(int);
int main(){
    if(std::is_same<decltype(NULL), decltype(0)>::value)
	std::cout << "NULL == 0" << std::endl;
    else
	std::cout << "NULL != 0" << std::endl;
    if (std::is_same<decltype(NULL), decltype((void*)0)>::value)
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
void test(char*){
    std::cout << "call test(char*)" << std::endl;
}
void test(int i){
    std::cout << "call test(int)" << std::endl;
}
```
## constexpr
常量表达式，常用于数组定义阶段。
此外constexpr修饰的函数支持递归（且从c++14开始，可以在内部使用局部变量、循环和分支等简单语句）
```cpp
#include<iostream>
#define LEN 10
int len(){
    int i = 2;
    return i;
}
const int len_constexpr(){
    return 5;
}
const int fib(const int n){
    return n == 1 || n == 2 ? 1 : fib(n-1)+fib(n-2);
}
int main(){
    char a1[10]; //合法
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