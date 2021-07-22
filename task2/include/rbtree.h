#ifndef _STL_RB_TREE_H_
#define _STL_RB_TREE_H_

#include "alloctor.h"
#include "construct.h"
#include "queue.h"		
#include <utility>			
#include <string.h>


namespace lfp {
namespace detail {


	typedef bool __rb_tree_color_type;
	const __rb_tree_color_type __rb_tree_red = true;
	const __rb_tree_color_type __rb_tree_black = false;



	struct __rb_tree_node_base {
		typedef __rb_tree_color_type color_type;
		typedef __rb_tree_node_base* base_ptr;

		color_type color;	
		base_ptr parent;	
		base_ptr left;		
		base_ptr right;		

		static base_ptr get_min_node(base_ptr x) {
			while (x->left != nullptr)
				x = x->left;
			return x;
		}
		static base_ptr get_max_node(base_ptr x) {
			while (x->right != nullptr)
				x = x->right;
			return x;
		}
		
		
		base_ptr get_brother() {
			if (this->parent->left == this)
				return this->parent->right;
			return this->parent->left;
		}
	};


	template<class Value>
	struct __rb_tree_node : public __rb_tree_node_base {
		typedef __rb_tree_node<Value>* node_pointer;
		Value value_field;		
	};






	struct __rb_tree_iterator_base {
		typedef __rb_tree_node_base::base_ptr	base_ptr;
		typedef bidirectional_iterator_tag		iterator_category;		
		typedef ptrdiff_t						difference_type;

		base_ptr node;		

		__rb_tree_iterator_base(base_ptr n) : node(n) { }

		
		void incrment() {
			if (node->right == node)
				return;

			if (node->right != nullptr) {		
				node = node->right;
				while (node->left != nullptr) {
					node = node->left;
				}
			}
			else {			
				base_ptr parent = node->parent;		
				while (parent->right == node) {		
					node = parent;					
					parent = node->parent;
				}

				if (node->right != parent) {		
					node = parent;					
				}
				
				
			}
		}

		
		void decrment() {
			if (node->left == node)
				return;
			if (node->color == __rb_tree_red &&		
				node->parent->parent == node)		
			{
				node = node->right;		
			}
			else if (node->left != nullptr) {	
				node = node->left;				
				while (node->right != nullptr)	
					node = node->right;					
			}
			else {	
				base_ptr parent = node->parent;	

				while (node == parent->left) {			
					node = parent;						
					parent = node->parent;
				}
				node = parent;			
			}
		}
	};


	template<class Value, class Ref, class Ptr>
	struct __rb_tree_iterator : public __rb_tree_iterator_base {
		typedef Value	value_type;
		typedef Ref		reference;
		typedef Ptr		pointer;
		typedef __rb_tree_node<Value>*	node_pointer;
		typedef __rb_tree_iterator<Value, Ref, Ptr> self;
		typedef __rb_tree_iterator<Value, Value&, Value*> iterator;
		typedef __rb_tree_iterator<Value, const Value&, const Value*> const_iterator;		

		__rb_tree_iterator() : __rb_tree_iterator_base(nullptr) { }
		__rb_tree_iterator(node_pointer x) : __rb_tree_iterator_base(x) { }
		__rb_tree_iterator(const iterator& x) : __rb_tree_iterator_base(x.node) { }

		reference operator*() const { return node_pointer(node)->value_field; }
#ifndef __SGI_STL_NO_ARROW_OPERATOR
		pointer operator->() const { return &(operator*()); }
#endif
		
		self& operator++() {
			incrment();
			return *this;
		}
		
		self operator++(int) {
			self tmp = *this;
			incrment();
			return tmp;
		}
		
		self& operator--() {
			decrment();
			return *this;
		}
		
		self operator--(int) {
			self tmp = *this;
			decrment();
			return tmp;
		}

		bool operator==(const iterator& x) const { return (node == x.node); }
		bool operator!=(const iterator& x) const { return (node != x.node); }
		bool operator==(const const_iterator& x) const { return (node == x.node); }
		bool operator!=(const const_iterator& x) const { return (node != x.node); }
	};



	void __rb_tree_rotate_left(__rb_tree_node_base*, __rb_tree_node_base*&);
	void __rb_tree_rotate_right(__rb_tree_node_base*, __rb_tree_node_base*&);
	void __rb_tree_rebalance(__rb_tree_node_base*, __rb_tree_node_base*&);





	
	
	
	
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc = alloc>
	struct rb_tree {
	protected:
		typedef __rb_tree_color_type	color_type;
		typedef __rb_tree_node_base*	base_ptr;
		typedef __rb_tree_node<Value>	rb_tree_node;
		typedef rb_tree<Key, Value, KeyOfValue, Compare, Alloc> self;

		
		typedef simple_alloc<rb_tree_node, Alloc> rb_tree_node_allocator;
	public:
		typedef Key					key_type;
		typedef Value				value_type;
		typedef value_type*			pointer;
		typedef const value_type*	const_pointer;
		typedef value_type&			reference;
		typedef const value_type&	const_reference;
		typedef rb_tree_node*		node_pointer;
		typedef size_t				size_type;
		typedef ptrdiff_t			difference_type;
		typedef typename __rb_tree_iterator<value_type, reference, pointer>::iterator		 iterator;
		typedef typename __rb_tree_iterator<value_type, const reference, const pointer>::const_iterator	 const_iterator;

	protected:
		
		node_pointer header;		
		size_type node_count;		
		Compare key_compare;		


		
		node_pointer& root() const { return (node_pointer&) header->parent; }
		node_pointer& leftmost() const { return (node_pointer&) header->left; }
		node_pointer& rightmost() const { return (node_pointer&) header->right; }

		
		static node_pointer& left(node_pointer x) { return (node_pointer&) (x->left); }
		static node_pointer& right(node_pointer x) { return (node_pointer&) (x->right); }
		static node_pointer& parent(node_pointer x) { return (node_pointer&) (x->parent); }
		static reference value(node_pointer x) { return x->value_field; }
		static const Key& key(node_pointer x) { return KeyOfValue()(x->value_field); }	  
		static color_type& color(node_pointer x) { return (color_type&) (x->color); }

		
		static node_pointer& left(base_ptr x) { return (node_pointer&) (x->left); }
		static node_pointer& right(base_ptr x) { return (node_pointer&) (x->right); }
		static node_pointer& parent(base_ptr x) { return (node_pointer&) (x->parent); }
		static reference value(base_ptr x) { return ((node_pointer)x)->value_field; }
		static const Key& key(base_ptr x) { return KeyOfValue()(value(x)); }
		static color_type& color(base_ptr x) { return (color_type&)(node_pointer(x)->color); }

		
		static node_pointer get_min_node(node_pointer x) {
			return (node_pointer)__rb_tree_node_base::get_min_node(x);
		}
		static node_pointer get_max_node(node_pointer x) {
			return (node_pointer)__rb_tree_node_base::get_max_node(x);
		}

	protected:
		node_pointer get_node() { return rb_tree_node_allocator::allocate(); }
		void put_node(node_pointer p) { rb_tree_node_allocator::deallocate(p); }

		
		node_pointer create_node(const value_type& x) {
			node_pointer node = get_node();			
			try {
				construct(&(node->value_field), x);	
			}
			catch (...) {
				put_node(node);
			}
			return node;
		}
		
		node_pointer clone_node(const node_pointer x) {
			node_pointer node = create_node(x->value_field);
			node->color = x->color;
			node->left = nullptr;
			node->right = nullptr;
			return node;
		}
		
		void destroy_node(node_pointer x) {
			destroy(&x->value_field);
			put_node(x);
		}

	private:
		
		iterator __insert(base_ptr x, base_ptr y, const value_type& v);
		
		
		node_pointer __copy(node_pointer x);

		
		void __erase_node(node_pointer x);

		
		void __erase_node_case2_reblance(node_pointer x);


		void init() {
			header = get_node();			
			color(header) = __rb_tree_red;	
			root() = nullptr;				
			leftmost() = header;			
			rightmost() = header;		
		}


	public:
		rb_tree(const Compare& comp = Compare()) : node_count(0), key_compare(comp) {
			init();
		}
		rb_tree(const self& x) {		
			init();
			*this = x;					
		}
		~rb_tree() {
			clear();
			put_node(header);
		}

		self& operator=(const self& x);	

		
		void clear();

		
		void swap(self& x) {
			node_pointer tmp = header;
			*(&header) = x.header;
			*(&(x.header)) = tmp;
		}

		
		const_iterator find(const Key& k) const;
		
		iterator find(const Key& k) {
			const_iterator it = static_cast<const self&>(*this).find(k);
			node_pointer p = static_cast<node_pointer>(it.node);
			return iterator(p);
		}

		
		size_type count(const key_type& x) const {
			size_type x_count = 0;
			const_iterator it = find(x);		

			if (it != end()) {
				++x_count;
				
				
				while (++it != end()) {
					if (!key_compare(x, key(it.node)) && !key_compare(key(it.node), x))	 
						++x_count;
					else
						break;
				}
			}
			return x_count;
		}

		Compare key_comp() const { return key_compare; }
		iterator begin() { return leftmost(); }		
		const_iterator begin() const { return leftmost(); }
		iterator end() { return header; }			
		const_iterator end() const { return header; }
		bool empty() const { return (node_count == 0); }
		size_type size() const { return node_count; }
		size_type max_size() const { return size_type(-1); }


		
		std::pair<iterator, bool> insert_unique(const value_type& v);

		template<class InputIterator>
		void insert_unique(InputIterator first, InputIterator last) {
			while (first != last) {
				insert_unique(*first);
				++first;
			}
		}

		
		iterator insert_equal(const value_type& v);

		template<class InputIterator>
		void insert_equal(InputIterator first, InputIterator last) {
			while (first != last) {
				insert_equal(*first);
				++first;
			}
		}

		
		void erase(node_pointer x) { __erase_node(x); }
		void erase(iterator x) { __erase_node(node_pointer(x.node)); }
		size_type erase(const Key& x) {
			iterator it = find(x);
			size_type count = 0;

			while (it != end()) {
				__erase_node(node_pointer(it.node));
				it = find(x);
				++count;
			}

			return count;
		}
		void erase(iterator first, iterator last) {
			while (first != last) {
				iterator tmp = first++;
				erase(tmp);
			}
		}

		iterator lower_bound(const key_type& x) { return lower_bound(begin(), end(), x); }
		const_iterator lower_bound(const key_type& x) const { return lower_bound(begin(), end(), x); }
		iterator upper_bound(const key_type& x) { return upper_bound(begin(), end(), x); }
		const_iterator upper_bound(const key_type& x) const { return upper_bound(begin(), end(), x); }

		
		std::pair<iterator, iterator> equal_range(const key_type& x) {
			iterator first = find(x);
			iterator last = find(x);
			while (last != end()) {
				if (!key_compare(x, key(last.node)) && !key_compare(key(last.node), x))
					++last;
				else
					break;
			}

			return std::pair<iterator, iterator>(first, last);
		}
	};



	
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>&
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::operator=(const self& x) {
		clear();		

		key_compare = x.key_compare;
		header->parent = __copy(x.root());
		if (header->parent != nullptr)
		{
			root()->parent = header;
			node_count = x.node_count;

			leftmost() = get_min_node(root());
			rightmost() = get_max_node(root());
		}

		return *this;
	}


	
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::clear()
	{
		if (header->parent != nullptr) {
			queue<base_ptr> node_queue;

			node_queue.push(header->parent);
			while (!node_queue.empty()) {
				base_ptr cur = node_queue.front();
				node_queue.pop();
				
				if (cur->left != nullptr)
					node_queue.push(cur->left);
				if (cur->right != nullptr)
					node_queue.push(cur->right);
				destroy_node(node_pointer(cur));
			}

			
			node_count = 0;
			root() = nullptr;		
			leftmost() = header;	
			rightmost() = header;
		}
	}


	
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const Key& k) const {
		node_pointer y = header;
		node_pointer x = root();

		

		









		while (x != nullptr) {
			if (key_compare(key(x), k))
				x = right(x);			
			else if ( key_compare(k, key(x)) )
				x = left(x);			
			else
				return iterator(x);		
		}

		return end();
	}


	
	
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	std::pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool>
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const value_type& v) {
		node_pointer y = header;
		node_pointer x = root();
		bool comp = true;

		while (x != nullptr) {		
			y = x;

			comp = key_compare(KeyOfValue()(v), key(x));		
			x = comp ? left(x) : right(x);
		} 

		iterator j = iterator(y);

		
		if (comp) {
			if (j == begin())	
				return std::pair<iterator, bool>(__insert(x, y, v), true);
			else		
				--j;
		}
		
		if (key_compare(key(j.node), KeyOfValue()(v))) {
			return std::pair<iterator, bool>(__insert(x, y, v), true);
		}

		
		
		
		return std::pair<iterator, bool>(j, false);
	}

	
	
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const value_type& v) {

		

		node_pointer y = header;
		node_pointer x = root();		
		while (x != nullptr) {
			y = x;
			x = key_compare(KeyOfValue()(v), key(x)) ? left(x) : right(x);		
		}
		return __insert(x, y, v);
		
	}


	
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__insert(base_ptr _x, base_ptr _y, const value_type& v) {
		node_pointer x = (node_pointer)_x;
		node_pointer y = (node_pointer)_y;

		node_pointer new_node = create_node(v);			
		if (y == header || x != 0 || key_compare(KeyOfValue()(v), key(y)))
		{
			left(y) = new_node;		
			if (y == header) {
				root() = new_node;
				rightmost() = new_node;
			}
			else if (y == leftmost()) {
				leftmost() = new_node;
			}
		}
		else {
			right(y) = new_node;		
			if (y == rightmost()) {
				rightmost() = new_node;		
			}
		}
		parent(new_node) = y;		
		left(new_node) = 0;
		right(new_node) = 0;		

		
		__rb_tree_rebalance(new_node, header->parent);		
		++node_count;
		return iterator(new_node);		
	}

	
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::node_pointer
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__copy(node_pointer x) {
		if (x == nullptr)
			return nullptr;

		node_pointer x_copy = clone_node(x);
		x_copy->left = __copy(left(x));
		x_copy->right = __copy(right(x));

		if (x_copy->left != nullptr)
			x_copy->left->parent = x_copy;
		if (x_copy->right != nullptr)
			x_copy->right->parent = x_copy;

		return x_copy;
	}

	
	

	
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__erase_node(node_pointer x)
	{
		if (x->left == nullptr && x->right == nullptr)	
		{
			if (x->color == __rb_tree_red)		
			{
				if (leftmost() == x)
					leftmost() = (node_pointer)x->parent;
				else if (rightmost() == x)
					rightmost() = (node_pointer)x->parent;
			}
			else {									
				if (root() == x) {	
					root() = nullptr;
					leftmost() = header;
					rightmost() = header;
				}
				else {		
					__erase_node_case2_reblance(x);

					if (leftmost() == x)
						leftmost() = (node_pointer)x->parent;
					if (rightmost() == x)
						rightmost() = (node_pointer)x->parent;
				}
			}

			if (x->parent->left == x)
				x->parent->left = nullptr;
			else
				x->parent->right = nullptr;

			destroy_node(x);
			--node_count;
		}
		else if (x->left == nullptr) {		
			node_pointer son = (node_pointer)x->right;
			son->parent = x->parent;
			son->color = __rb_tree_black;

			if (x->parent->left == x)
				x->parent->left = son;
			else
				x->parent->right = son;

			if (root() == x) {		
				root() = son;
			}
			if (leftmost() == x)
				leftmost() = son;

			destroy_node(x);
			--node_count;
		}
		else if (x->right == nullptr) {		
			node_pointer son = (node_pointer)x->left;
			son->parent = x->parent;
			son->color = __rb_tree_black;

			if (x->parent->left == x)
				x->parent->left = son;
			else
				x->parent->right = son;
			
			if (root() == x) {		
				root() = son;
			}
			if (rightmost() == x)
				rightmost() = son;

			destroy_node(x);
			--node_count;
		}
		else {								
			iterator it = iterator(x);
			++it;	

			memmove(&(x->value_field), &(((node_pointer)it.node)->value_field), sizeof(x->value_field));
			

			__erase_node((node_pointer)it.node);	
		}
	}

	
	
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__erase_node_case2_reblance(node_pointer x) {
		node_pointer brother = (node_pointer)x->get_brother();

		if (brother->color == __rb_tree_black)		
		{
			if ((brother->left && brother->left->color == __rb_tree_red) ||		
				(brother->right && brother->right->color == __rb_tree_red))
			{
				if (brother == x->parent->right)
				{
					if (brother->right != nullptr) {	
						__rb_tree_rotate_left(x->parent, header->parent);
						brother->right->color = __rb_tree_black;
						brother->color = x->parent->color;
						x->parent->color = __rb_tree_black;
					}
					else {							
						__rb_tree_rotate_right(brother, header->parent);
						brother->color = __rb_tree_red;
						brother->parent->color = __rb_tree_black;  

						__erase_node_case2_reblance(x);	  
					}
				}
				else	
				{
					if (brother->left != nullptr) {	  
						__rb_tree_rotate_right(x->parent, header->parent);
						brother->left->color = __rb_tree_black;
						brother->color = x->parent->color;
						x->parent->color = __rb_tree_black;
					}
					else {							
						__rb_tree_rotate_left(brother, header->parent);
						brother->color = __rb_tree_red;
						brother->parent->color = __rb_tree_black;  

						__erase_node_case2_reblance(x);	
					}
				}
			}
			else					
			{
				if (x == root())	
					return;
				if (x->parent->color == __rb_tree_red)	
				{
					x->parent->color = __rb_tree_black;
					brother->color = __rb_tree_red;
				}
				else {				
					brother->color = __rb_tree_red;

					__erase_node_case2_reblance((node_pointer)x->parent);	

					
					
				}
			}
		}
		else				
		{
			if (brother == x->parent->right) {
				__rb_tree_rotate_left(x->parent, header->parent);
			}
			else {
				__rb_tree_rotate_right(x->parent, header->parent);
			}

			brother->color = __rb_tree_black;
			x->parent->color = __rb_tree_red;

			__erase_node_case2_reblance(x);		
		}
	}






	inline void __rb_tree_rebalance(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
		x->color = __rb_tree_red;		

		while (x != root && x->parent->color == __rb_tree_red)	
		{
			if (x->parent == x->parent->parent->left)		
			{
				__rb_tree_node_base* y = x->parent->parent->right;	

				if (y && y->color == __rb_tree_red)			
				{
					y->color = __rb_tree_black;				
					x->parent->color = __rb_tree_black;		
					x->parent->parent->color = __rb_tree_red;	

					x = x->parent->parent;		
				}
				else	
				{
					if (x == x->parent->right)			
					{
						x = x->parent;					
						__rb_tree_rotate_left(x, root);
					}
					x->parent->color = __rb_tree_black;			
					x->parent->parent->color = __rb_tree_red;	
					__rb_tree_rotate_right(x->parent->parent, root);	

					
				}
			}
			else		
			{
				__rb_tree_node_base* y = x->parent->parent->left;	

				if (y && y->color == __rb_tree_red)		
				{
					y->color = __rb_tree_black;				
					x->parent->color = __rb_tree_black;		
					x->parent->parent->color = __rb_tree_red;	

					x = x->parent->parent;		
				}
				else	
				{
					if (x == x->parent->left)		
					{
						x = x->parent;				
						__rb_tree_rotate_right(x, root);
					}
					x->parent->color = __rb_tree_black;			
					x->parent->parent->color = __rb_tree_red;	
					__rb_tree_rotate_left(x->parent->parent, root);		

					
				}
			}
		}		
		root->color = __rb_tree_black;		
	}



	inline void __rb_tree_rotate_left(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
		__rb_tree_node_base* y = x->right;		
		x->right = y->left;
		if (y->left != nullptr) {
			y->left->parent = x;		
		}
		y->parent = x->parent;

		
		if (x == root) {					
			root = y;
			x->parent->parent = y;			
		}
		else if (x == x->parent->left)		
			x->parent->left = y;
		else								
			x->parent->right = y;
		y->left = x;
		x->parent = y;
	}



	inline void __rb_tree_rotate_right(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
		__rb_tree_node_base* y = x->left;		
		x->left = y->right;
		if (y->right != nullptr) {
			y->right->parent = x;		
		}
		y->parent = x->parent;

		
		if (x == root) {					
			root = y;
			x->parent->parent = y;			
		}
		else if (x == x->parent->right)		
			x->parent->right = y;
		else								
			x->parent->left = y;
		y->right = x;
		x->parent = y;
	}

} 
} 

#endif	