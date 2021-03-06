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
    template <int N,typename Tuple>
        auto zip_get(Tuple & t)->decltype(*std::get<N>(t))&
        {
            return *std::get<N>(t);
        }
    template <typename ... Rest>
        struct zip_iter;
    template <typename First, typename Second>
        struct zip_iter<First,Second> {
            First iter1;
            Second iter2;
            using Elem1_t = decltype(*iter1);
            using Elem2_t = decltype(*iter2);
            zip_iter(const First & f, const Second & s) :
                iter1(f),iter2(s) { }

            auto operator*() -> decltype(std::make_tuple(iter1,iter2))
            {
                return std::make_tuple(iter1,iter2);
            }

/*
            auto  operator*() -> decltype(std::make_tuple(*iter1,*iter2))
            {
                return std::make_tuple(*iter1,*iter2);
            }
*/
            zip_iter & operator++() {
                ++iter1;
                ++iter2;
                return *this;
            }
            bool operator!=(const zip_iter & rhs) {
                return (this->iter1 != rhs.iter1) && (this->iter2 != rhs.iter2);
            }
        };
    template <typename First, typename ... Rest>
        struct zip_iter<First,Rest...> {
            First iter;
            using Elem_t = decltype(*iter);
            //Elem_t elem;
            zip_iter<Rest...> inner_iter;
            zip_iter(const First & f, const Rest & ... rest) :
                iter(f),
                inner_iter(rest...) {}
                        
              //this is for returning a tuple of iterators
            using tuple_t = 
                decltype(std::tuple_cat(std::make_tuple(iter),*inner_iter));

            tuple_t operator*() {
                return std::tuple_cat(std::make_tuple(iter),*inner_iter);
            }
             
/*
            using tuple_t = 
                decltype(std::tuple_cat(std::make_tuple(*iter),*inner_iter));
            //data will not be changed because these are not references

            tuple_t operator*() {
                return std::tuple_cat(std::make_tuple(*iter),*inner_iter);
            };
*/
            zip_iter & operator++() {
                ++iter;
                ++inner_iter;
                return *this;
            }
            bool operator!=(const zip_iter & rhs) {
                return (this->iter != rhs.iter) && (this->inner_iter != rhs.inner_iter);
            }
        };
   template <typename Iterator>
      struct iterator_range {
          Iterator begin_;
          Iterator end_;
          iterator_range(Iterator begin, Iterator end) :
              begin_(begin),end_(end) {}
          Iterator begin() {
              return begin_;
          }
          Iterator end() {
              return end_;
          }
      };
   template <typename ... Containers>
       auto myzip(Containers & ... containers) ->
               iterator_range<zip_iter<decltype(containers.begin())...>>
        {
            auto begin = zip_iter<decltype(containers.begin())...>(containers.begin()...);
            auto end = zip_iter<decltype(containers.end())...>(containers.end()...);
            return iterator_range<decltype(begin)>(begin,end);
        }
   template <typename Container>
       auto reverse(const Container & container) -> iterator_range<decltype(container.rbegin())>
       {
           return 
               iterator_range<decltype(container.rbegin())>(container.rbegin(),container.rend());
       }
   template <typename Container>
       auto slice(
               Container container,
               typename std::iterator_traits<decltype(container.begin())>::difference_type begin,
               typename std::iterator_traits<decltype(container.begin())>::difference_type end
               ) -> iterator_range<decltype(container.begin())>
       {
           return iterator_range<decltype(container.begin())>(
                   container.begin()+begin,
                   container.begin()+end);
       }

   //chain operates only as a range, it has no iterator
   //this is because each iterator would have to be aware of
   //it's end for this to work
   template <typename ... Containers>
       struct chain_iter;
   template <typename Container>
       struct chain_iter<Container> {
           using Iterator = decltype(((Container*)nullptr)->begin());
           Iterator begin;
           Iterator end;//never really used but kept it for consistency
           chain_iter(Container & container, bool is_end=false) :
               begin(container.begin()),end(container.end()) {
                   //std::cout << *begin<<"hi" << std::endl;
                   if(is_end) begin = container.end();
           }
           chain_iter & operator++()
           {
               ++begin;
               return *this;
           }
           auto operator*()->decltype(*begin)
           {
               return *begin;
           }
           bool operator!=(const chain_iter & rhs) {
               return this->begin != rhs.begin;
           }
       };
   template <typename Container, typename ... Containers>
       struct chain_iter<Container,Containers...>
       {
           using Iterator = decltype(((Container*)nullptr)->begin());
           Iterator begin;
           Iterator end;
           bool end_reached = false;
           chain_iter<Containers...> next_iter;
           chain_iter(Container & container, Containers& ... rest, bool is_end=false) :
               begin(container.begin()),
               end(container.end()),
               next_iter(rest...,is_end) {
                   //std::cout << *begin << std::endl;
                   if(is_end)
                       begin = container.end();
               }
           chain_iter & operator++()
           {
               if (begin == end) {
                   /*
                   //std::cout << "fuck" << std::endl;
                   if(end_reached) {
                       //std::cout << "++next_iter" << std::endl;
                       ++next_iter;
                   }
                   else {
                       //std::cout << "++" << std::endl;
                       end_reached = true;
                   }
                   */
                   ++next_iter;
               }
               else {
                   ++begin;
               }
               return *this;               
           }
           auto operator*()->decltype(*begin)
           {
               //std::cout << *begin << std::endl;
               if (begin == end) {
                   //std::cout << "*" << std::endl;
                   return *next_iter;
               }
               else {
                   return *begin;
               }
           }   
           bool operator !=(const chain_iter & rhs) {
               if (begin == end) {
                   //std::cout << "!=" << std::endl;
                   return this->next_iter != rhs.next_iter;
               }
               else
                   return this->begin != rhs.begin;
           }
        };
   template <typename ... Containers>
       iterator_range<chain_iter<Containers...>> chain(Containers& ... containers)
       {
           auto begin = 
               chain_iter<Containers...>(containers...);
           auto end =
               chain_iter<Containers...>(containers...,true);
           return 
               iterator_range<chain_iter<Containers...>>(begin,end);
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
        auto boost_reverse(const Container & container) -> 
            decltype(boost::make_iterator_range(container.rbegin(),container.rend()))
            {
                return boost::make_iterator_range(container.rbegin(),container.rend());
            }

    //slice type range good for use in range style for loops
    template <typename Container>
        auto boost_slice(
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
    //nth product generator
    template <typename Iter>                                                           
        int multiply_range(Iter begin, Iter end) {                                         
            int i = 1;                                                                     
            for (;begin != end; ++begin) {                                                 
                i *= *begin;                                                               
            }                                                                              
            return i;                                                                      
        }                                                                                  

    std::vector<int> nth_product(std::initializer_list<int> list, int n) {         
        std::vector<int> v;                                                            
        int range_prod = multiply_range(list.begin(),list.end());                      
        for (auto iter = list.begin(); iter != list.end(); ++iter) {                   
            range_prod /= *iter;                                                       
            auto i = (n / range_prod) % *iter;                                         
            v.push_back(i);                                                            
        }                                                                              
        return v;                                                                      
    } 
}
#endif
