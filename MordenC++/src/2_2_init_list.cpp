#include<iostream>
#include<initializer_list>
#include<vector>
class init_list_test{
private:
    std::vector<int> vec;
public:
    init_list_test(std::initializer_list<int> list){
        for(std::initializer_list<int>::iterator it = list.begin();
            it != list.end(); ++it)
            vec.push_back(*it);
    }
    void show(){
        for(std::vector<int>::iterator it = vec.begin();
            it != vec.end(); ++it) 
            std::cout << *it << std::endl;
    }
};
int main(){
    //或者init_list_test test = {1, 2, 3, 4, 5};
    init_list_test test{1, 2, 3, 4, 5};
    //1 2 3 4 5
    test.show();
    return 0;
}