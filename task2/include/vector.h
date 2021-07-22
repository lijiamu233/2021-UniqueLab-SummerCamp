#ifndef _STL_VECTOR_H_
#define _STL_VECTOR_H_

#include "alloctor.h"		
#include "construct.h"		
#include "iterator.h"		
#include "uninitialized.h"	
#include "algobase.h"		
#include <initializer_list>		

namespace lfp {


	
	template<class T, class Alloc = alloc>
	class vector {
	public:
		
		typedef T					value_type;
		typedef value_type*			pointer;
		typedef const value_type*	const_pointer;
		typedef value_type*			iterator;
		typedef const value_type*	const_iterator;
		typedef value_type&			reference;
		typedef const value_type&	const_reference;
		typedef size_t				size_type;
		typedef ptrdiff_t			difference_type;

	private:
		
		typedef simple_alloc<value_type, Alloc> data_allocator;
		iterator start;				
		iterator finish;			
		iterator end_of_storage;	

		
		void deallocate() {
			if (start) {
				data_allocator::deallocate(start, end_of_storage - start);
			}
			start = 0;
			finish = 0;
			end_of_storage = 0;
		}

		
		void fill_initialize(size_type n, const T& value) {
			start = allocate_and_fill(n, value);
			finish = start + n;
			end_of_storage = finish;
		}

		
		void insert_aux(iterator pos, size_type n, const T& x);

	public:		
		vector() : start(0), finish(0), end_of_storage(0) { }	
		explicit vector(size_type n) { fill_initialize(n, T()); }	
		vector(int n, const T& value) { fill_initialize(n, value); }	
		vector(size_type n, const T& value) { fill_initialize(n, value); }
		template<class Iterator>
		vector(Iterator first, Iterator last) {		
			size_type n = distance(first, last);
			start = data_allocator::allocate(n);
			uninitialized_copy(first, last, start);
			finish = start + n;
			end_of_storage = finish;
		}
		vector(const vector<T>& rhs) {		
			start = data_allocator::allocate(rhs.size());
			uninitialized_copy(rhs.start, rhs.finish, start);
			finish = start + rhs.size();
			end_of_storage = finish;
		}
		vector(std::initializer_list<T> init_list) {		
			start = data_allocator::allocate(init_list.size());
			uninitialized_copy(init_list.begin(), init_list.end(), start);
			finish = start + init_list.size();
			end_of_storage = finish;
		}
		~vector() {
			if (start)
				destroy(start, finish);		
			deallocate();			
		}

		iterator begin() { return start; }
		const_iterator begin() const { return start; }
		iterator end() { return finish; }
		const_iterator end() const { return finish; }
		size_type size() const { return finish - start; }
		size_type capacity() const { return end_of_storage - start; }
		reference operator[](size_type n) { return *(start + n); }
		const_reference operator[](size_type n) const { return *(start + n); }
		reference front() { return *begin(); }
		const_reference front() const { return *begin(); }
		reference back() { return *(end() - 1); }
		const_reference back() const { return *(end() - 1); }

		
		void insert(iterator pos, size_type n, const T& x) {
			insert_aux(pos, n, x);
		}

		
		void insert(iterator pos, const T& x) {
			insert_aux(pos, 1, x);
		}

		void push_front(const T& x) {
			insert_aux(begin(), 1, x);
		}

		void push_back(const T& x) {
			if (finish != end_of_storage) {		
				construct(finish, x);
				++finish;
			}
			else {
				insert_aux(end(), 1, x);		
			}
		}

		void pop_front() {
			erase(begin());
		}

		void pop_back() {
			--finish;
			destroy(finish);
		}

		
		iterator erase(iterator pos) {		
			if (pos + 1 != end()) {		
				copy(pos + 1, finish, pos);	
			}
			--finish;
			destroy(finish);
			return pos;
		}

		
		iterator erase(iterator first, iterator last) {
			
			iterator it = copy(last, finish, first);

			
			destroy(it, finish);
			finish = it;		
			return first;
		}

		void resize(size_type new_sz, const T& x) {
			if (new_sz < size()) {
				erase(begin() + new_sz, end());
			}
			else {
				insert_aux(end(), new_sz - size(), x);
			}
		}

		void resize(size_type new_sz) {
			resize(new_sz, (T)0);
		}

		
		void reserve(size_type reser_size) {
			if (reser_size <= capacity())
				return;

			
			const size_type old_capacity = capacity();
			const size_type new_capacity = 2 * max(old_capacity, reser_size);

			
			iterator new_start = data_allocator::allocate(new_capacity);
			iterator new_finish = new_start;
			try{
				
				new_finish = uninitialized_copy(start, finish, new_start);

			} catch (...) {
				
				
				data_allocator::deallocate(new_start, new_capacity);
				throw;		
			}

			
			destroy(begin(), end());
			deallocate();

			
			start = new_start;
			finish = new_finish;
			end_of_storage = new_start + new_capacity;
		}

		void swap(vector<T>& rhs) {
			lfp::swap(start, rhs.start);
			lfp::swap(finish, rhs.finish);
			lfp::swap(end_of_storage, rhs.end_of_storage);
		}

		void clear() {
			erase(begin(), end());
		}

	protected:
		
		iterator allocate_and_fill(size_type n, const T& value) {
			iterator result = data_allocator::allocate(n);
			uninitialized_fill_n(result, n, value);
			return result;
		}
	};	






















































	
	template<class T, class Alloc>
	void vector<T, Alloc>::insert_aux(iterator pos, size_type n, const T& x) {
		if (n > 0)
		{
			if (size_type(end_of_storage - finish) >= n) {
				
				T x_copy = x;
				
				
				const size_type elems_after = finish - pos;
				iterator old_finish = finish;
				if (elems_after > n) {
					
					uninitialized_copy(finish - n, finish, finish);		
					finish += n;		
					copy_backward(pos, old_finish - n, old_finish);	
					
					fill(pos, pos + n, x_copy);
				}
				else {
					
					uninitialized_fill_n(finish, n - elems_after, x_copy);
					finish = finish + n - elems_after;
					uninitialized_copy(pos, old_finish, finish);
					finish += elems_after;
					fill(pos, old_finish, x_copy);
				}
			}
			else {		
				
				const size_type old_capacity = capacity();
				const size_type new_capacity = 2 * max(old_capacity, n);

				
				iterator new_start = data_allocator::allocate(new_capacity);
				iterator new_finish = new_start;
				try{
					
					new_finish = uninitialized_copy(start, pos, new_start);
					
					new_finish = uninitialized_fill_n(new_finish, n, x);
					
					new_finish = uninitialized_copy(pos, finish, new_finish);
				} catch (...) {
					
					
					data_allocator::deallocate(new_start, new_capacity);
					throw;		
				}

				
				destroy(begin(), end());
				deallocate();

				
				start = new_start;
				finish = new_finish;
				end_of_storage = new_start + new_capacity;
			}
		}
	}

}

#endif 