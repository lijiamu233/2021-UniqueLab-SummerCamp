#ifndef _STL_QUEUE_H_
#define _STL_QUEUE_H_

#include "list.h"		//以list作为其底层容器

namespace lfp {

	//省缺以list作为底层容器，用户也可以自己指定
	template<class T, class Container = list<T>>
	class queue {
	public:
		typedef typename Container::value_type		value_type;
		typedef typename Container::size_type		size_type;
		typedef typename Container::reference		reference;
		typedef typename Container::const_reference	const_reference;
	protected:
		Container cont;		//底层容器

		/* 以下利用底层容器的操作实现queue的对外接口 */
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

}// end of namespace lfp

#endif	//! _STL_QUEUE_H_