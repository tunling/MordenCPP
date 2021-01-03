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
