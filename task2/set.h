#ifndef _STL_SET_H_
#define _STL_SET_H_

#include "alloctor.h"
#include "algobase.h"		
#include "rbtree.h"

namespace lfp {
	template<class Key, class Compare = std::less<Key>, class Alloc = alloc>
	class set {
	public:
		typedef Key			key_type;
		typedef Key			value_type;
		typedef Compare		key_compare;
		typedef Compare		value_compare;
	private:
		
		typedef detail::rb_tree<key_type, value_type, identity<value_type>, key_compare, Alloc> rep_type;
		typedef typename rep_type::iterator rep_iterator;

		rep_type temp;		

	public:
	
		typedef typename rep_type::const_iterator		iterator;
		typedef typename rep_type::const_iterator		const_iterator;
		typedef typename rep_type::const_pointer		pointer;
		typedef typename rep_type::const_pointer		const_pointer;
		typedef typename rep_type::const_reference		reference;
		typedef typename rep_type::const_reference		const_reference;
		typedef typename rep_type::size_type			size_type;
		typedef typename rep_type::difference_type		difference_type;
		

		set() : temp(Compare()) { }
		explicit set(const Compare& comp) : temp(comp) { }
		template<class InputIterator>
		set(InputIterator first, InputIterator last) : temp(Compare()) {
			temp.insert_unique(first, last);
		}
		set(const set<Key, Compare, Alloc>& x) : temp(x.temp) { }

		set<Key, Compare, Alloc>& operator=(const set<Key, Compare, Alloc>& x) {
			temp = x.temp;
			return *this;
		}
		bool empty() const { return temp.empty(); }
		size_type size() const { return temp.size(); }
		size_type max_size() const { return temp.max_size(); }
		

		
		std::pair<iterator, bool> insert(const value_type& x) {
			std::pair<rep_iterator, bool> p = temp.insert_unique(x);
			return std::pair<iterator, bool>(p.first, p.second);
		}
	
		template<class InputIterator>
		void insert(InputIterator first, InputIterator last) {
			temp.insert_unique(first, last);
		}

		
		void erase(iterator pos) {
			temp.erase((rep_iterator&)pos);
		}
		size_type erase(const key_type& x) {
			return temp.erase(x);
		}
		void erase(iterator first, iterator last) {
			temp.erase((rep_iterator&)first, (rep_iterator&)last);
		}
		void clear() { temp.clear(); }

		std::pair<iterator, iterator> equal_range(const key_type& x) const { return temp.equal_range(x); }



	};
}

#endif