#ifndef _STL_ALGOBASE_H_
#define _STL_ALGOBASE_H_

#include "iterator.h"		
#include "type_traits.h"	
#include <string.h>				

namespace lfp {		


	template<class ForwardIterator, class T>
	inline void fill(ForwardIterator first, ForwardIterator last, const T& value) {
		for (; first != last; ++first) {
			*first = value;
		}
	}


	template<class OutputIterator, class Size, class T>
	inline OutputIterator fill_n(OutputIterator first, Size n, const T& value) {
		for (; n > 0; --n, ++first) {
			*first = value;
		}
		return first;
	}

	template<class T>
	inline const T& max(const T& a, const T& b) {
		return (a < b) ? b : a;		
	}


	template<class T>
	inline const T& min(const T& a, const T& b) {
		return (a < b) ? a : b;		
	}


	template<class T>
	void swap(T& lhs, T& rhs) {
		T tmp = lhs;
		lhs = rhs;
		rhs = tmp;
	}


	template<class InputIterator, class T>
	inline InputIterator find(InputIterator first, InputIterator last, const T& value) {
		while (first != last && *first != value)
			++first;
		return first;
	}


namespace detail {		


	template<class InputIterator, class OutputIterator>
	struct __copy_dispatch {
		OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) {
			return __copy(first, last, result, iterator_category(first));
		}
	};

	template<class T>
	struct __copy_dispatch<T*, T*> {	
		T* operator()(T* first, T* last, T* result) {
			typedef typename __type_traits<T>::has_trivial_assignment_operator has_trivial;
			return __copy_t(first, last, result, has_trivial());
			
		}
	};

	template<class T>
	struct __copy_dispatch<const T*, T*> {	
		T* operator()(const T* first, const T* last, T* result) {
			typedef typename __type_traits<T>::has_trivial_assignment_operator has_trivial;
			return __copy_t(first, last, result, has_trivial());
		
		}
	};


	template<class InputIterator, class OutputIterator>
	inline OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag) {
		for (; first != last; ++first, ++result)	
			*result = *first;
		return result;
	}

	template<class RandomAccessIterator, class OutputIterator>
	inline OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, random_access_iterator_tag) {
		return __copy_d(first, last, result, distance_type(first));
	}
	
	template<class RandomAccessIterator, class OutputIterator, class Distance>
	inline OutputIterator __copy_d(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, Distance*) {
		Distance n = last - first;
		for (; n > 0; --n, ++first, ++result)	
			*result = *first;
		return result;
	}


	template<class T>
	inline T* __copy_t(const T* first, const T* last, T* result, __true_type) {
		memmove(result, first, sizeof(T) * (last - first));
		return result + (last - first);
	}

	template<class T>
	inline T* __copy_t(const T* first, const T* last, T* result, __false_type) {
		
		return __copy_d(first, last, result, (ptrdiff_t*)0);
	}

}


	template<class InputIterator, class OutputIterator>
	inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result) {
		return detail::__copy_dispatch<InputIterator, OutputIterator>()(first, last, result);
	}

	inline char* copy(const char* first, const char* last, char* result) {
		memmove(result, first, last - first);
		return result + (last - first);
	}

	inline wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result) {
		memmove(result, first, sizeof(wchar_t) * (last - first));
		return result + (last - first);
	}


namespace detail {		


	template<class BidirectionalIterator1, class BidirectionalIterator2>
	struct __copy_backward_dispatch {
		BidirectionalIterator2 operator()(BidirectionalIterator1 first,
										  BidirectionalIterator1 last,
										  BidirectionalIterator2 result)
		{
			return __copy_backward(first, last, result, iterator_category(first));

		}
	};

	template<class T>
	struct __copy_backward_dispatch<T*, T*> {		
		T* operator()(T* first, T* last, T* result) {
			typedef typename __type_traits<T>::has_trivial_assignment_operator has_trivial;
			return __copy_backward_t(first, last, result, has_trivial());
		
		}
	};

	template<class T>
	struct __copy_backward_dispatch<const T*, T*> {		
		T* operator()(const T* first, const T* last, T* result) {
			typedef typename __type_traits<T>::has_trivial_assignment_operator has_trivial;
			return __copy_backward_t(first, last, result, has_trivial());
			
		}
	};


	template<class BidirectionalIterator1, class BidirectionalIterator2>
	inline BidirectionalIterator2 __copy_backward(BidirectionalIterator1 first,
												  BidirectionalIterator1 last,
												  BidirectionalIterator2 result,
												  bidirectional_iterator_tag)
	{
		while (last != first)			
			*(--result) = *(--last);
		return result;
	}

	template<class RandomAccessIterator, class BidirectionalIterator>
	inline BidirectionalIterator __copy_backward(RandomAccessIterator first,
												 RandomAccessIterator last,
												 BidirectionalIterator result,
												 random_access_iterator_tag)
	{
		return __copy_backward_d(first, last, result, distance_type(first));
	}

	template<class RandomAccessIterator, class BidirectionalIterator, class Distance>
	inline BidirectionalIterator __copy_backward_d(RandomAccessIterator first,
												   RandomAccessIterator last,
												   BidirectionalIterator result,
												   Distance*)
	{
		Distance n = last - first;
		for (; n > 0; --n)	
			*(--result) = *(--last);
		return result;
	}


	template<class T>
	inline T* __copy_backward_t(const T* first, const T* last, T* result, __true_type) {
		result = result - (last - first);
		memmove(result, first, sizeof(T) * (last - first));
		return result;
	}

	template<class T>
	inline T* __copy_backward_t(const T* first, const T* last, T* result, __false_type) {
		
		return __copy_backward_d(first, last, result, (ptrdiff_t*)0);
	}

}


	template<typename BidirectionalIterator1, typename BidirectionalIterator2>
	inline BidirectionalIterator2 copy_backward(BidirectionalIterator1 first,
		BidirectionalIterator1 last,
		BidirectionalIterator2 result)
	{
		return detail::__copy_backward_dispatch<BidirectionalIterator1, BidirectionalIterator2>()(first, last, result);
	}
	
	inline char* copy_backward(const char* first, const char* last, char* result) {
		result = result - (last - first);
		memmove(result, first, last - first);
		return result;
	}

	inline wchar_t* copy_backward(const wchar_t* first, const wchar_t* last, wchar_t* result) {
		result = result - (last - first);
		memmove(result, first, sizeof(wchar_t) * (last - first));
		return result;
	}


namespace detail {		


	template<class ForwarIterator, class T, class Distance>
	ForwarIterator __lower_bound(ForwarIterator first, ForwarIterator last,
								 const T& value, Distance*, forward_iterator_tag)
	{
		Distance len = distance(first, last);		
		Distance half = 0;
		ForwarIterator mid;

		while (len > 0) {
			half = (len >> 1);		
			mid = first;
			advance(mid, half);		
			if (*mid < value) {
				first = ++mid;
				len = len - half - 1;	
			}
			else {
				len = half;
			}
		}
		return first;
	}
	
	template<class	RandomAccessIterator, class T, class Distace>
	RandomAccessIterator __lower_bound(RandomAccessIterator first, RandomAccessIterator last,
									   const T& value, Distace*, random_access_iterator_tag)
	{
		Distace len = last - first;			
		Distace half = 0;
		RandomAccessIterator mid;

		while (len > 0) {
			half = (len >> 1);		
			mid = first + half;		

			if (*mid < value) {
				first = mid + 1;
				len = len - half - 1;
			}
			else {
				len = half;
			}
		}
		return first;
	}


	
	template<class ForwarIterator, class T, class Compare, class Distance>
	ForwarIterator __lower_bound(ForwarIterator first, ForwarIterator last,
								 const T& value, Compare comp, Distance*, forward_iterator_tag)
	{
		Distance len = distance(first, last);		
		Distance half = 0;
		ForwarIterator mid;

		while (len > 0) {
			half = (len >> 1);		
			mid = first;
			advance(mid, half);		
			if (comp(*mid, value)) {
				first = ++mid;
				len = len - half - 1;	
			}
			else {
				len = half;
			}
		}
		return first;
	}
	
	template<class	RandomAccessIterator, class T, class Compare, class Distace>
	RandomAccessIterator __lower_bound(RandomAccessIterator first, RandomAccessIterator last,
									   const T& value, Compare comp, Distace*, random_access_iterator_tag)
	{
		Distace len = last - first;			
		Distace half = 0;
		RandomAccessIterator mid;

		while (len > 0) {
			half = (len >> 1);		
			mid = first + half;		

			if (comp(*mid, value)) {
				first = mid + 1;
				len = len - half - 1;
			}
			else {
				len = half;
			}
		}
		return first;
	}



	
	template<class ForwarIterator, class T, class Distance>
	ForwarIterator __upper_bound(ForwarIterator first, ForwarIterator last,
								 const T& value, Distance*, forward_iterator_tag)
	{
		Distance len = distance(first, last);		
		Distance half = 0;
		ForwarIterator mid;

		while (len > 0) {
			half = (len >> 1);		
			mid = first;
			advance(mid, half);		
			if (value < *mid) {
				len = half;
			}
			else {
				first = ++mid;
				len = len - half - 1;	
			}
		}
		return first;
	}
	
	template<class	RandomAccessIterator, class T, class Distace>
	RandomAccessIterator __upper_bound(RandomAccessIterator first, RandomAccessIterator last,
									   const T& value, Distace*, random_access_iterator_tag)
	{
		Distace len = last - first;			
		Distace half = 0;
		RandomAccessIterator mid;

		while (len > 0) {
			half = (len >> 1);		
			mid = first + half;		

			if (value < *mid) {
				len = half;
			}
			else {
				first = mid + 1;
				len = len - half - 1;
			}
		}
		return first;
	}


	
	template<class ForwarIterator, class T, class Compare, class Distance>
	ForwarIterator __upper_bound(ForwarIterator first, ForwarIterator last,
								 const T& value, Compare comp, Distance*, forward_iterator_tag)
	{
		Distance len = distance(first, last);		
		Distance half = 0;
		ForwarIterator mid;

		while (len > 0) {
			half = (len >> 1);		
			mid = first;
			advance(mid, half);		
			if (comp(value, *mid)) {
				len = half;
			}
			else {
				first = ++mid;
				len = len - half - 1;	
			}
		}
		return first;
	}
	
	template<class	RandomAccessIterator, class T, class Compare, class Distace>
	RandomAccessIterator __upper_bound(RandomAccessIterator first, RandomAccessIterator last,
									   const T& value, Compare comp, Distace*, random_access_iterator_tag)
	{
		Distace len = last - first;			
		Distace half = 0;
		RandomAccessIterator mid;

		while (len > 0) {
			half = (len >> 1);		
			mid = first + half;		

			if (comp(value, *mid)) {
				len = half;
			}
			else {
				first = mid + 1;
				len = len - half - 1;
			}
		}
		return first;
	}

}


 
	template<class ForwarIterator, class T>
	inline ForwarIterator lower_bound(ForwarIterator first, ForwarIterator last, const T& value) {
		return detail::__lower_bound(first, last, value, distance_type(first), iterator_category(first));
	}


	template<class ForwarIterator, class T, class Compare>
	inline ForwarIterator lower_bound(ForwarIterator first, ForwarIterator last, const T& value, Compare comp) {
		return detail::__lower_bound(first, last, value, comp, distance_type(first), iterator_category(first));
	}


	template<class ForwarIterator, class T>
	inline ForwarIterator upper_bound(ForwarIterator first, ForwarIterator last, const T& value) {
		return detail::__upper_bound(first, last, value, distance_type(first), iterator_category(first));
	}


	template<class ForwarIterator, class T, class Compare>
	inline ForwarIterator upper_bound(ForwarIterator first, ForwarIterator last, const T& value, Compare comp) {
		return detail::__upper_bound(first, last, value, comp, distance_type(first), iterator_category(first));
	}



	
	template<class Arg, class Result>
	struct unary_function {
		typedef Arg			argument_type;
		typedef Result		result_type;
	};


	
	template<class Arg1, class Arg2, class Result>
	struct binary_function {
		typedef Arg1		first_argument_type;
		typedef Arg2		second_argument_type;
		typedef Result		result_type;
	};

	template<class T>
	struct negate : public unary_function<T, T> {
		T operator()(const T& x) const { return -x; }
	};



	template<class T>
	struct plus : public binary_function<T, T, T> {
		T operator()(const T& x, const T& y) const {
			return x + y;
		}
	};
	template<class T>
	struct minus : public binary_function<T, T, T> {
		T operator()(const T& x, const T& y) const {
			return x - y;
		}
	};
	template<class T>
	struct multiplies : public binary_function<T, T, T> {
		T operator()(const T& x, const T& y) const {
			return x * y;
		}
	};
	template<class T>
	struct divides : public binary_function<T, T, T> {
		T operator()(const T& x, const T& y) const {
			return x / y;
		}
	};
	template<class T>
	struct modulus : public binary_function<T, T, T> {
		T operator()(const T& x, const T& y) const {
			return x % y;
		}
	};
	template<class T>
	struct less : public binary_function<T, T, bool> {
		bool operator()(const T& x, const T& y) const {
			return x < y;
		}
	};
	template<class T>
	struct greater : public binary_function<T, T, bool> {
		bool operator()(const T& x, const T& y) const {
			return x > y;
		}
	};



	template<class T>
	struct identity : public unary_function<T, T> {
		const T& operator()(const T& x) const {
			return x;
		}
	};

	template<class Pair>
	struct select1st : public unary_function<Pair, typename Pair::first_type> {
		const typename Pair::first_type& operator()(const Pair& x) const {
			return x.first;
		}
	};

	template<class Pair>
	struct select2nd : public unary_function<Pair, typename Pair::second_type> {
		const typename Pair::second_type& operator()(const Pair& x) const {
			return x.second;
		}
	};


	template<class Arg1, class Arg2>
	struct project1st : public binary_function<Arg1, Arg2, Arg1> {
		Arg1 operator()(const Arg1& x, const Arg2&) {
			return x;
		}
	};


	template<class Arg1, class Arg2>
	struct project2nd : public binary_function<Arg1, Arg2, Arg2> {
		Arg2 operator()(const Arg1&, const Arg2& y) {
			return y;
		}
	};

	template<class T>
	struct equal_to {
		bool operator()(const T& x, const T& y) const {
			return (x == y);
		}
	};

	
	struct str_equal {
		bool operator()(const char* s1, const char* s2) const {
			return strcmp(s1, s2) == 0;
		}
	};

}

#endif 