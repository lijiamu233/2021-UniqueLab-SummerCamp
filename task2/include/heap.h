#ifndef _STL_HEAP_H_
#define _STL_HEAP_H_

#include "iterator.h"	 

namespace lfp {
namespace detail {


	template<class RandomAccessIterator, class Distance, class T, class Compare>
	void percolate_up(RandomAccessIterator first,		
					  Distance topIndex,				
					  Distance holeIndex,				
					  T value,							
					  Compare comp)						
	{
		Distance holeParent = (holeIndex - 1) / 2;		

		
		while (holeIndex > topIndex && comp(*(first + holeParent), value))
		{
			*(first + holeIndex) = *(first + holeParent);	
			holeIndex = holeParent;							
			holeParent = (holeIndex - 1) / 2;				
		}
		
		*(first + holeIndex) = value;
	}


	template<class RandomAccessIterator, class Distance, class T, class Compare>
	void percolate_down(RandomAccessIterator first,		
						Distance holeIndex,				
						Distance len,					
						T value,						
						Compare comp)					
	{
		Distance largerChild = 2 * (holeIndex + 1);		
		while (largerChild < len)		
		{
			
			if (comp(*(first + largerChild), *(first + largerChild - 1))) {
				--largerChild;
			}
			
			
			if (!comp(value, *(first + largerChild)))
				break;

			*(first + holeIndex) = *(first + largerChild);		
			holeIndex = largerChild;				
			largerChild = 2 * (holeIndex + 1);		
		}
		if (largerChild == len) {		
			if (comp(value, *(first + largerChild - 1))) {			
				*(first + holeIndex) = *(first + largerChild - 1);	
				holeIndex = largerChild - 1;		
			}
		}
		
		*(first + holeIndex) = value;
	}



	template<class RandomAccessIterator, class Distance, class T, class Compare>
	inline void __push_heap_aux(RandomAccessIterator first,
								RandomAccessIterator last,
								Distance*, T*,
								Compare comp)
	{
		percolate_up(first, Distance(0), Distance(last - first - 1), *(last - 1), comp);
		
	}



	template<class RandomAccessIterator, class Distance, class T, class Compare>
	inline void __pop_heap_aux(RandomAccessIterator first,
							   RandomAccessIterator last,
							   Distance*, T*,
							   Compare comp)
	{
		T value = *(last - 1);
		*(last - 1) = *first;			

		percolate_down(first, Distance(0), Distance(last - first - 1), value, comp);
		
	}



	template<class RandomAccessIterator, class Distance, class T, class Compare>
	inline void __make_heap_aux(RandomAccessIterator first,	
								RandomAccessIterator last,	
								Distance*, T*,
								Compare comp)					
	{
		Distance len = last - first;
		if (len < 2) return;		

		
		Distance holeIndex = (len - 2) / 2;
		while (holeIndex >= 0) {
			percolate_down(first, holeIndex, len, T(*(first + holeIndex)), comp);
			--holeIndex;
		}
	}

} 







	
	template<class RandomAccessIterator, class Compare>
	inline void push_heap(RandomAccessIterator first,	
						  RandomAccessIterator last,	
						  Compare comp)					
	{
		detail::__push_heap_aux(first, last, distance_type(first), value_type(first), comp);
	}



	
	
	template<class RandomAccessIterator, class Compare>
	inline void pop_heap(RandomAccessIterator first,	
						 RandomAccessIterator last,		
						 Compare comp)					
	{
		detail::__pop_heap_aux(first, last, distance_type(first), value_type(first), comp);
	}



	
	template<class RandomAccessIterator, class Compare>
	inline void make_heap(RandomAccessIterator first,	
						  RandomAccessIterator last,	
						  Compare comp)					
	{
		detail::__make_heap_aux(first, last, distance_type(first), value_type(first), comp);
	}



	
	template<class RandomAccessIterator, class Compare>
	inline void sort_heap(RandomAccessIterator first,	
						  RandomAccessIterator last,	
						  Compare comp)					
	{
		while (last - first > 1) {			
			pop_heap(first, last, comp);
			--last;
		}
	}


} 

#endif	