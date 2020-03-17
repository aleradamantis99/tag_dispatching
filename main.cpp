#include <iostream>
#include <vector>
#include <tuple>
//CONSTRUCTORES
struct PiecewiseConstruct_t {};

constexpr PiecewiseConstruct_t PiecewiseConstruct;

template <typename T1, typename T2>
struct Pair
{
	T1 first;
	T2 second;

	constexpr Pair(): first{}, second{} {};
	
	constexpr Pair(const T1& x, const T2& y): first(x), second(y) {}
	
	template <class U1, class U2>
	constexpr Pair(U1&& x, U2&& y): 
		first(std::forward<U1>(x)),
		second(std::forward<U2>(y))  {}
	
	template <class U1, class U2>
	constexpr Pair(const Pair<U1, U2>& p): first(p.first), second(p.second){}
	
	template <class U1, class U2>
	constexpr Pair(Pair<U1, U2>&& p):
		first(std::forward<U1>(p.first)),
		second(std::forward<U2>(p.second))
	{}
    
    template<typename... Args1, typename... Args2>
	Pair(PiecewiseConstruct_t,
	   std::tuple<Args1...> first, std::tuple<Args2...> second)
      : Pair(first, second,
	     typename std::make_index_sequence<sizeof...(Args1)>{},
	     typename std::make_index_sequence<sizeof...(Args2)>{})
      { 
      }
    
    Pair(const Pair&) = default;
	Pair(Pair&&) = default;

private:
	template<typename... Args1, std::size_t... Indexes1,
             typename... Args2, std::size_t... Indexes2>
	Pair(std::tuple<Args1...>& tuple1, std::tuple<Args2...>& tuple2,
	   std::index_sequence<Indexes1...>, std::index_sequence<Indexes2...>)
      : first(std::forward<Args1>(std::get<Indexes1>(tuple1))...),
        second(std::forward<Args2>(std::get<Indexes2>(tuple2))...)
      { }
};

/* Kind of works if you want to remove the Pair(const T1&, const T2&) constructor
template <typename U1, typename U2>
Pair(U1&&, U2&&) -> Pair<std::remove_cvref_t<U1>, std::remove_cvref_t<U2>>;
*/

//Partial Specialization

namespace detail
{
template <typename>
struct Type{};

template <size_t N, typename T>
bool func(Type<T>)
{
	return N>static_cast<T>(2);
}

template <size_t N>
bool func(Type<int>)
{
	return N>1;
}
}

template <typename T, size_t N>
bool func()
{
	using namespace detail;
	return func<N>(Type<T>{});
}


//Selecting function
template<typename BidirectionalIterator>
constexpr void reverse(BidirectionalIterator first, BidirectionalIterator last,
		  std::bidirectional_iterator_tag)
{
	while (true)
	{
		if (first == last || first == --last)
		{
  			return;
  		}
		else
		{
			std::iter_swap(first, last);
			++first;
		}
	}
}

template<typename RandomAccessIterator>
constexpr void reverse(RandomAccessIterator first, RandomAccessIterator last, std::random_access_iterator_tag)
{
	if (first == last)
		return;
	--last;
	while (first < last)
	{
		std::iter_swap(first, last);
		++first;
		--last;
	}
}


template<typename BidirectionalIterator>
constexpr void reverse(BidirectionalIterator first, BidirectionalIterator last)
{
	using IteratorCategory = std::iterator_traits<BidirectionalIterator>::iterator_category;
	std::reverse(first, last, IteratorCategory{});
}
template <typename...>
struct TD;
int main()
{
	int a=1, b=2;
	Pair p{b, 3};
	
	Pair c(p);
	using MyPair = Pair<std::vector<int>, std::vector<float>>;
	MyPair vs(PiecewiseConstruct, std::forward_as_tuple(4, 1),
	 								std::forward_as_tuple(5, 3.5));
	 								
	std::cout << func<size_t, 2>();
	return 0;
}
