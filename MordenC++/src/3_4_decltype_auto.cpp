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