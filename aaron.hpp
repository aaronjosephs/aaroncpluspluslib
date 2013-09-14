#ifndef AARON_HPP
#define AARON_HPP
#include <vector>
#include <utility>
#include <iterator>
#include <algorithm>
#include <tuple>
//#ifdef __BOOST__
#include <boost/iterator/zip_iterator.hpp>
#include <boost/range.hpp>
//#endif //__BOOST__

//macro for index
#define foreach(index,elem,range) \
{\
    int index = -1;\
    for (auto elem : range) {\
        ++index;

#define endforeach }}

namespace aaron {

    using ulong = unsigned long long;

    //Struct allows function to be called on each element
    //of param pack in order like this:
    //pass{ (function(args), 1)...}; // the comma operator allows you
    //to use a function that returns void
    struct pass {
        template <typename ... Types>
            pass (Types...) {}
    };

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
                func(*begin, (*iterators)... );
            //could also make this a tuple
            return func;
        }

//#ifdef __BOOST__
    //Boost aided functions
    template <typename... Containers>
        auto zip(const Containers&... containers) -> 
            boost::iterator_range<
                boost::zip_iterator
                    <decltype(boost::make_tuple(std::begin(containers)...))>
            >
        {
            auto zip_begin = 
                boost::make_zip_iterator(boost::make_tuple(std::begin(containers)...));
            auto zip_end = 
                boost::make_zip_iterator(boost::make_tuple(std::end(containers)...));
            return boost::make_iterator_range(zip_begin, zip_end);
        }
    template <typename Container>
        auto reverse(const Container & container) -> 
            decltype(boost::make_iterator_range(container.rbegin(),container.rend()))
            {
                return boost::make_iterator_range(container.rbegin(),container.rend());
            }

    //slice type range good for use in range style for loops
    template <typename Container>
        auto slice(
                Container container,
                typename std::iterator_traits<decltype(container.begin())>::difference_type begin,
                typename std::iterator_traits<decltype(container.begin())>::difference_type end
                ) -> decltype(boost::make_iterator_range(container.begin(),container.begin()))
        {
            return boost::make_iterator_range(
                    container.begin()+begin,
                    container.begin()+end);
        }

//#endif //__BOOST__

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
    int func (int i) {
        return i;
    }
}
#endif
