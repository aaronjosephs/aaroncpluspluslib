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
    //Math functions
    //Sieve of Eratostothenes
    std::vector<int> generate_sieve(const ulong & max) {
        std::vector<int> sieve{2};
        std::vector<bool> not_prime(max+1);
        ulong current_prime = sieve.back();
        bool done = false;
        while(!done){ 
            for (int i = 2; i * current_prime <= max;i++) {
                not_prime[i*current_prime]=true;
            }
            for (int j = current_prime+1;true;j++) {
                if (not_prime[j] == false) {
                    if( j >= max) {
                        done = true;
                        break;
                    }
                    else {
                        sieve.push_back(j);
                        current_prime = j;
                        break;
                    }
                }
            }
        }
        return sieve;
    }
    //Prime factorization
    std::vector<ulong> prime_factors(ulong number) {
        auto primes = generate_sieve(number/2+1);
        std::vector<ulong> factors;
        for (auto iter = primes.begin();number!=1;++iter) {
            while(number % *iter == 0) {
                factors.push_back(*iter);
                number /= *iter;
            }
        }
        return factors;
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
