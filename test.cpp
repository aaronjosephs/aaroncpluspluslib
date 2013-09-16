#include "aaron.hpp"
//#include "itertools.hpp"
#include <vector>
#include <array>

int main () {
    std::vector<int> v1{1,2,3};
    std::vector<int> v2{4,5,6};
    std::vector<float> v3{1.2,2.4,9.0,6.7};
    std::vector<float> v4{1.2,2.4,9.0};
    std::array<int,3> a5{{8,9,10}};
    std::vector<std::string> s6{"apple","pear","bannananan","fruity"};
    std::cout << "zip with lambda test" << std::endl;
    aaron::zip (
            [](int i,int j,float k,float l){
                std::cout << i << " " << j << " " << k << " " << l << std::endl;
            },
            v1.begin(),v1.end(),v2.begin(),v3.begin(),v4.begin());
    
    std::cout << std::endl << "Macro foreach enumerator test" << std::endl;
    foreach(i,j,v1)
        std::cout << i << " " << j << std::endl;
    endforeach;
    std::cout << std::endl << "Boost aided zip test" << std::endl;
    /*
    for (auto i : aaron::zip(v1,v2,v3)) {
        std::cout << i.get<0>() << " " 
            << i.get<1>() << " "
            << i.get<2>() << std::endl;
    }
    */
    std::cout << std::endl << "Reverse test" << std::endl;
    for (auto i : aaron::reverse(v1)) {
        std::cout << i << std::endl;
    }
    std::cout << std::endl << "slice test" << std::endl;
    for (auto i : aaron::slice(v1,0,2)) {
        std::cout << i << std::endl;
    }
    
    std::cout << std::endl << "Variadic template zip iterator" << std::endl;
    for (auto i : aaron::myzip(v1,v2,v3,s6)) {
        std::cout << aaron::zip_get<0>(i) << " " 
            << (aaron::zip_get<1>(i)=22) << " "
            << aaron::zip_get<2>(i) << 
            aaron::zip_get<3>(i) << std::endl;
    }
    for (auto i : aaron::myzip(v1,v2,v3,s6)) {
        std::cout << aaron::zip_get<0>(i) << " " 
            << aaron::zip_get<1>(i) << " "
            << aaron::zip_get<2>(i) << 
            aaron::zip_get<3>(i) << std::endl;
    }
    std::cout << std::endl << "Chain iter test" << std::endl;
    auto range = aaron::chain(v1,v2,a5);
    for (auto iter = range.begin(); iter != range.end();++iter) {
        std::cout << (*iter=1) << std::endl;
    }
    //std::cout << *(++(++(++(range.begin())))) << std::endl;
    for (auto i : range) {
        std::cout << i << std::endl;
    }
    /*
    auto zippy = aaron::zip_iter<decltype(v1.begin()),decltype(v2.begin()),decltype(v3.begin())>
            (v1.begin(),v2.begin(),v3.begin());
    auto zippend = aaron::zip_iter<decltype(v1.begin()),decltype(v2.begin()),decltype(v3.begin())>
            (v1.end(),v2.end(),v3.end());
    auto range = aaron::iterator_range<decltype(zippy)>(zippy,zippend);
    for (auto i : range){
        std::cout << std::get<0>(i) << " " 
            << std::get<1>(i) << " "
            << std::get<2>(i) << std::endl;
    }
    */

    //++zippy;
    //auto t = *zippy;
    //auto zippy2(zippy);
    //std::cout << (zippy2 != ++zippy) << std::endl;
    //std::cout << std::get<0>(*zippy) << " " << std::get<1>(t) << " " << std::get<2>(t) << std::endl;
    /*
    v1={1,2,3};
    v2={4,5,6};
    v3={1.2,2.4,9.0,6.7};
    v4={1.2,2.4,9.0};
    a5={{8,9,10}};
    s6={"apple","pear","bannananan","fruity"};
    std::cout << std::endl << "Reverse test" << std::endl;
    for (auto i : iter::reverse(v1)) {
        std::cout << i << std::endl;
    }
    std::cout << std::endl << "slice test" << std::endl;
    for (auto i : iter::slice(v1,0,2)) {
        std::cout << i << std::endl;
    }
    
    std::cout << std::endl << "Variadic template zip iterator" << std::endl;
    for (auto i : iter::zip(v1,v2,v3,s6)) {
        std::cout << iter::zip_get<0>(i) << " " 
            << (iter::zip_get<1>(i)=22) << " "
            << iter::zip_get<2>(i) << 
            iter::zip_get<3>(i) << std::endl;
    }
    for (auto i : iter::zip(v1,v2,v3,s6)) {
        std::cout << iter::zip_get<0>(i) << " " 
            << iter::zip_get<1>(i) << " "
            << iter::zip_get<2>(i) << 
            iter::zip_get<3>(i) << std::endl;
    }
    std::cout << std::endl << "Chain iter test" << std::endl;
    auto rang = iter::chain(v1,v2,a5);
    for (auto iter = rang.begin(); iter != rang.end();++iter) {
        std::cout << (*iter=1) << std::endl;
    }
    //std::cout << *(++(++(++(range.begin())))) << std::endl;
    for (auto i : iter::chain(v1,v2,a5)) {
        std::cout << i << std::endl;
    } 
    */
  //test of same stuff in different namespace and file  
    return 0;
}

