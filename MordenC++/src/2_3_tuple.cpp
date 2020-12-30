#include<iostream>
#include<tuple>
std::tuple<int, double, std::string> test(){
    return std::make_tuple(1, 3.14, "486");
}
int main(){
    auto [x, y, z] = test();
    //1, 3.14, 486
    std::cout << x << ", " << y << ", " << z << std::endl;
    return 0;
}