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