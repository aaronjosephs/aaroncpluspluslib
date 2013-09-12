#include <vector>
#include <iterator>
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
        auto inc_deref (Iterator & iterator) -> decltype(*iterator){
            Iterator temp(iterator);
            ++iterator;
            return *temp;
        }
    template <typename Function, typename Iterator, typename ... Iterators>
        Function zip (Function func, Iterator begin, 
                Iterator end, 
                Iterators ... iterators)
        {
            for(;begin != end; ++begin)
                func(*begin, inc_deref(iterators)... );
            return func;
        }
}
int main () {
    std::vector<int> v1{1,2,3};
    std::vector<int> v2{3,2,1};
    std::vector<float> v2{1.2,2.4,9.0};
    aaron::zip (
            [](int i,int j,float k){
                std::cout << i << " " << j << " " << k << std::endl;
            },
            v1.begin(),v1.end(),v2.begin());
}
