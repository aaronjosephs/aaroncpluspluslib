#include <vector>
#include <utility>
#include <iterator>
#include <algorithm>
#include <iostream>
namespace aaron {
    template <typename Iterator, typename Function>
        Function enumerate(Iterator begin, 
                Iterator end, 
                Function func,
                typename std::iterator_traits<Iterator>::difference_type start=0)
        {
            for (;begin != end; ++begin, ++start)
                func(start, *begin);
            return func;
        }
    template <typename Iterator>
        void advance_all (Iterator & iterator) {
            ++iterator;
        }
    template <typename Iterator, typename ... Iterators>
        void advance_all (Iterator & iterator, Iterators& ... iterators) {
            ++iterator;
            advance_all(iterators...);
        } 
    template <typename Function, typename Iterator, typename ... Iterators>
        Function zip (Function func, Iterator begin, 
                Iterator end, 
                Iterators ... iterators)
        {
            for(;begin != end; ++begin, advance_all(iterators...))
                func(*begin, *(iterators)... );
            //could also make this a tuple
            return func;
        }
}
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
