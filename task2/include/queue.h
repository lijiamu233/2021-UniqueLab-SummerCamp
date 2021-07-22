#ifndef _STL_QUEUE_H_
#define _STL_QUEUE_H_

#include "list.h"		

namespace lfp {

	
	template<class T, class Container = list<T>>
	class queue {
	public:
		typedef typename Container::value_type		value_type;
		typedef typename Container::size_type		size_type;
		typedef typename Container::reference		reference;
		typedef typename Container::const_reference	const_reference;
	protected:
		Container cont;		

		
	public:
		queue() : cont() {}
		bool empty() const { return cont.empty(); }
		size_type size() const { return cont.size(); }
		reference front() { return cont.front(); }
		const_reference front() const { return cont.front(); }
		reference back() { return cont.back(); }
		const_reference back() const { return cont.back(); }
		void push(const value_type& x) { cont.push_back(x); }
		void pop() { cont.pop_front(); }
	};

}

#endif	