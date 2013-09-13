#include "aaron.hpp"

int main () {
    std::vector<int> v1{1,2,3};
    std::vector<int> v2{3,2,1};
    std::vector<float> v3{1.2,2.4,9.0};
    std::vector<float> v4{1.2,2.4,9.0};
    aaron::zip (
            [](int i,int j,float k,float l){
            std::cout << i << " " << j << " " << k << " " << l << std::endl;
            },
            v1.begin(),v1.end(),v2.begin(),v3.begin(),v4.begin());
}

