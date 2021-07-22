#ifndef _STL_LIST_H_
#define _STL_LIST_H_

#include "alloctor.h"		
#include "construct.h"		
#include "iterator.h"		
#include "algobase.h"		
#include <cstddef>				

namespace lfp {
namespace detail {


	template<class T>
	struct __node_type {
		typedef __node_type<T>* node_pointer;
		node_pointer prev;		
		node_pointer next;
		T data;
	};


	template<class T, class Ref = T&, class Ptr = T*>
	struct __list_iterator {
	public:
		typedef __list_iterator<T, Ref, Ptr>		self;
		typedef __list_iterator<T, T&, T*>			iterator;
		typedef const __list_iterator<T, T&, T*>	const_iterator;
		typedef bidirectional_iterator_tag			iterator_category;		
		typedef T									value_type;
		typedef Ptr									pointer;
		typedef const Ptr							const_pointer;
		typedef Ref									reference;
		typedef const Ref							const_reference;
		typedef size_t								size_type;
		typedef ptrdiff_t							difference_type;
		typedef __node_type<T>*						node_pointer;

		node_pointer node;		

		
		__list_iterator() { }
		__list_iterator(node_pointer x) : node(x) { }
		__list_iterator(const iterator& x) : node(x.node) { }

		bool operator==(const self& x) const { return node == x.node; }
		bool operator!=(const self& x) const { return node != x.node; }
		
		reference operator*() const { return (*node).data; }
		
		pointer operator->() const { return &(operator*()); }		
		
		self& operator++() {	
			node = (*node).next;
			return *this;
		}
		self operator++(int) {	
			self tmp = *this;
			++*this;
			return tmp;
		}
		
		self& operator--() {	
			node = (*node).prev;
			return *this;
		}
		self operator--(int) {	
			self tmp = *this;
			--*this;
			return tmp;
		}
	};

}





	template<class T, class Alloc = alloc>		
	struct list {
	protected:
		typedef detail::__node_type<T> node_type;
	public:
		typedef typename detail::__list_iterator<T, T&, T*>::node_pointer		node_pointer;

		typedef typename detail::__list_iterator<T, T&, T*>::iterator			iterator;
		typedef typename detail::__list_iterator<T, T&, T*>::const_iterator		const_iterator;
		typedef typename detail::__list_iterator<T, T&, T*>::iterator_category	category;
		typedef typename detail::__list_iterator<T, T&, T*>::value_type			value_type;
		typedef typename detail::__list_iterator<T, T&, T*>::pointer			pointer;
		typedef typename detail::__list_iterator<T, T&, T*>::const_pointer		const_pointer;
		typedef typename detail::__list_iterator<T, T&, T*>::reference			reference;
		typedef typename detail::__list_iterator<T, T&, T*>::const_reference	const_reference;
		typedef typename detail::__list_iterator<T, T&, T*>::size_type			size_type;
		typedef typename detail::__list_iterator<T, T&, T*>::difference_type	difference_type;
	protected:
		size_type size_;
		node_pointer node;
		
		

		
		typedef simple_alloc<node_type, Alloc> node_type_allocator;
	
		
		node_pointer get_node() { return node_type_allocator::allocate(); }
		
		void put_node(node_pointer p) { node_type_allocator::deallocate(p); }
		
		node_pointer create_node(const T& x) {
			node_pointer p = get_node();
			construct(&p->data, x);		
			return p;
		}
		
		void destroy_node(node_pointer p) {
			destroy(&p->data);	
			put_node(p);		
		}
		
		void empty_initialize() {
			node = get_node();		
			node->next = node;
			node->prev = node;		
			size_ = 0;
		}

		
		
		
		void transfer(iterator first, iterator last, iterator pos);

		
		iterator QuickSortCore(iterator low, iterator high);
		void QuickSort(iterator low, iterator high);

	public:
		list() { empty_initialize(); }
		~list() {
			clear();
			destroy_node(node);
		}

		
		iterator begin() { return (*node).next; }
		const_iterator begin() const { return (*node).next; }
		iterator end() { return node; }
		const_iterator end() const { return node; }
		bool empty() const { return node->next == node;	}
		size_type size() const { return size_;  }

		reference front() { return *begin(); }
		const_reference front() const { return *begin(); }
		reference back() { return *(--end()); }
		const_reference back() const { return *(--end()); }

		
		iterator insert(iterator pos, const T& x);

		
		iterator erase(iterator pos);

		void push_front(const T& x) { insert(begin(), x); }
		void push_back(const T& x) { insert(end(), x); }
		void pop_front() { erase(begin()); }
		void pop_back() { erase(--end()); }

		
		void clear();
		
		void remove(const T& value);
		
		void unique();
		
		
		void splice(iterator pos, list& x);
		
		void splice(iterator pos, iterator i);
		
		
		void splice(iterator pos, iterator first, iterator last);

		
		void merge(list<T, Alloc>& x);
		
		void reverse();

		
		
		void sort();
	};



	
	template<class T, class Alloc>
	typename list<T, Alloc>::iterator
		list<T, Alloc>::insert(iterator pos, const T& x) {
		node_pointer tmp = create_node(x);

		
		tmp->next = pos.node;
		tmp->prev = pos.node->prev;
		(pos.node->prev)->next = tmp;
		pos.node->prev = tmp;
		++size_;
		return tmp;
	}

	
	template<class T, class Alloc>
	typename list<T, Alloc>::iterator
		list<T, Alloc>::erase(iterator pos) {
		node_pointer next_node = pos.node->next;
		node_pointer prev_node = pos.node->prev;
		prev_node->next = next_node;
		next_node->prev = prev_node;
		destroy_node(pos.node);
		--size_;
		return iterator(next_node);
	}

	
	template<class T, class Alloc>
	void list<T, Alloc>::clear() {
		node_pointer cur = node->next;	
		while (cur != node) {		
			node_pointer tmp = cur;
			cur = cur->next;
			destroy_node(tmp);		
		}
		
		node->next = node;
		node->prev = node;
		size_ = 0;
	}

	
	template<class T, class Alloc>
	void list<T, Alloc>::remove(const T& value) {
		iterator first = begin();
		iterator last = end();
		while (first != last)
		{
			if (*first == value) {
				first = erase(first);
				continue;
			}
			++first;
		}
	}

	
	template<class T, class Alloc>
	void list<T, Alloc>::unique() {
		if (size_ == 0)
			return;

		iterator first = begin();
		iterator last = --end();
		while (first != last) {
			iterator tmp = first++;
			if (*first == *tmp) {
				erase(tmp);
			}
		}
	}

	
	template<class T, class Alloc>
	void list<T, Alloc>::splice(iterator pos, list<T, Alloc>& x) {
		if (!x.empty()) {
			transfer(pos, x.begin(), x.end());
			size_ += x.size();
		}
	}

	
	template<class T, class Alloc>
	void list<T, Alloc>::splice(iterator pos, iterator i) {
		iterator j = i;
		++j;
		if (pos == i || pos == j)
			return;
		transfer(pos, i, j);
		++size_;
	}

	
	
	template<class T, class Alloc>
	void list<T, Alloc>::splice(iterator pos, iterator first, iterator last) {
		if (first != last) {
			tranfer(pos, first, last);
			size_ += distance(first, last);
		}
	}

	
	template<class T, class Alloc>
	void list<T, Alloc>::merge(list<T, Alloc>& x) {
		iterator first1 = begin();
		iterator last1 = end();
		iterator first2 = x.begin();
		iterator last2 = x.end();
		size_ += x.size();

		
		while (first1 != last1 && first2 != last2) {
			if (*first2 < *first1) {
				iterator next = first2;
				transfer(first1, first2, ++next);
				first2 = next;
			}
			else {
				++first1;
			}
		}
		if (first2 != last2) {
			transfer(last1, first2, last2);
		}
	}

	
	template<class T, class Alloc>
	void list<T, Alloc>::reverse() {
		
		if (node->next == node || (node->next)->next == node)
			return;

		iterator first = begin();
		++first;
		while (first != end()) {
			iterator old = first;
			++first;
			transfer(begin(), old, first);
		}
	}

	
	template<class T, class Alloc>
	void list<T, Alloc>::sort() {
		
		if (node->next == node || (node->next)->next == node)
			return;
		QuickSort(begin(), --end());
	}




	
	
	
	template<class T, class Alloc>
	void list<T, Alloc>::transfer(iterator pos, iterator first, iterator last) {
		if (pos != last)
		{
			(last.node)->prev->next = pos.node;
			(first.node)->prev->next = last.node;
			(pos.node)->prev->next = first.node;
			node_pointer tmp = (pos.node)->prev;
			(pos.node)->prev = (last.node)->prev;
			(last.node)->prev = (first.node)->prev;
			(first.node)->prev = tmp;

			
			
			
			
			
			
			
		}
	}


	
	
	template<class T, class Alloc>
	typename list<T, Alloc>::iterator
		list<T, Alloc>::QuickSortCore(iterator low, iterator high) {
		value_type pivotkey = (low.node)->data;
		while (low != high) {
			while (low != high && (high.node)->data >= pivotkey)
				--high;
			(low.node)->data = (high.node)->data;
			while (low != high && (low.node)->data <= pivotkey)
				++low;
			(high.node)->data = (low.node)->data;
		}
		(low.node)->data = pivotkey;
		return low;		
	}

	template<class T, class Alloc>
	void list<T, Alloc>::QuickSort(iterator low, iterator high) {
		if (low != high) {
			iterator pivot = QuickSortCore(low, high);
			if (pivot == low) {
				QuickSort(++pivot, high);
			}
			else if (pivot == high) {
				QuickSort(low, --high);
			}
			else {
				iterator pivot2 = pivot;
				QuickSort(low, --pivot2);		
				QuickSort(++pivot, high);		
			}
		}
	}

}

#endif 