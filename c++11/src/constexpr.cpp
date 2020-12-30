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