#ifndef _STL_ALLOCATOR_H_
#define _STL_ALLOCATOR_H_

#include <stdlib.h>

#if 0
	#include <new>
	#define __THROW_BAD_ALLOC throw std::exception("bad_alloc")
#elif !defined(__THROW_BAD_ALLOC)
    #include <iostream>
	#define __THROW_BAD_ALLOC std::cerr << "out of memory\n"; exit(1);
#endif

namespace lfp{
    namespace detail{
        template<int inst>
        class __malloc_alloc_template{
            private:
                static void *oom_malloc(size_t);
                static void *oom_realloc(void*, size_t);
                static void(*__malloc_alloc_oom_handler)();
            public:
                static void* allocate(size_t n){
                    void* result = ::malloc(n);
                    if (nullptr == result)
                        result = oom_malloc(n);	
                    return result;
                }
                static void deallocate(void* p, size_t n){
                    ::free(p);	
                }
                static void* reallocate(void* p, size_t old_sz, size_t new_sz) {
                    void* result = ::realloc(p, new_sz);
                    if (nullptr == result)
				        result = oom_realloc(p, new_sz);
                    return result;
                }
                static void(*set_malloc_handler( void(*f)() ))() {
			        void(*old)() = __malloc_alloc_oom_handler;
			        __malloc_alloc_oom_handler = f;
			        return (old);
		        }               
        }    
        template<int inst>   
        void(*__malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = 0;
        template<int inst>
        void* __malloc_alloc_template<inst>::oom_malloc(size_t n){
            void(*my_malloc_handler)();
		    void* result;
            for (int i = 0; i < 32; ++i){
                my_malloc_handler = __malloc_alloc_oom_handler;
                if (0 == my_malloc_handler)
                    __THROW_BAD_ALLOC;
                (*my_malloc_handler)();	
                result = ::malloc(n);
                if (result)
				    return (result);
            }
            __THROW_BAD_ALLOC;
        }
        template<int inst>
        void* __malloc_alloc_template<inst>::oom_realloc(void* p, size_t n){
            void(*my_malloc_handler)();
		    void* result;
            for (int i = 0; i < 32; ++i){
                my_malloc_handler = __malloc_alloc_oom_handler;
                if (0 == my_malloc_handler)
                    __THROW_BAD_ALLOC;
                (*my_malloc_handler)();	
                result = ::realloc(p, n);
                if (result)
				    return (result);
            }
            __THROW_BAD_ALLOC;
        }
        typedef __malloc_alloc_template<0> malloc_alloc;
        enum { __ALIGN = 8 };		
        enum { __MAX_BYTES = 128 };	
        enum { __NFREELISTS = __MAX_BYTES / __ALIGN };
        template<bool threads, int inst>
        class __default_alloc_template {
            private:
                union obj{
                    union obj* free_list_link;
                    char client_data[1];
                }
                static obj* volatile free_lists[__NFREELISTS];
                static size_t ROUND_UP(size_t bytes) {
			        return ((bytes + __ALIGN - 1) & ~(__ALIGN - 1));
		        }
                static size_t FREELIST_INDEX(size_t bytes) {
			        return ((bytes + __ALIGN - 1) / __ALIGN - 1);
		        }
                static void* refill(size_t n);
                static char* chunk_alloc(size_t size, int &nobjs);
                static char*  start_free;	
                static char*  end_free;	
                static size_t heap_size;
            public:
                static void* allocate(size_t);
                static void deallocate(void*, size_t);
                static void* reallocate(void*, size_t, size_t);
        }
        template<bool threads, int inst>
	    char* __default_alloc_template<threads, inst>::start_free = 0;
	    template<bool threads, int inst>
	    char* __default_alloc_template<threads, inst>::end_free = 0;
	    template<bool threads, int inst>
	    size_t __default_alloc_template<threads, inst>::heap_size = 0;
        template<bool threads, int inst>
	    typename __default_alloc_template<threads, inst>::obj*
	    volatile __default_alloc_template<threads, inst>::free_lists[__NFREELISTS] ={ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        template<bool threads, int inst>
        void* __default_alloc_template<threads, inst>::allocate(size_t total_byte) {
            if (total_byte > (size_t)__MAX_BYTES) {
                return malloc_alloc::allocate(total_byte);
            }
            obj* volatile* pp_free_list = free_lists + FREELIST_INDEX(total_byte);
            obj* result = *pp_free_list;
            if (0 == result) {
                void* r = refill(ROUND_UP(total_byte));
                return r;
            }
            *pp_free_list = result->free_list_link;
            return result;
        }

        
        template<bool threads, int inst>
        void __default_alloc_template<threads, inst>::deallocate(void* p, size_t n) {
        
            if (n > (size_t)__MAX_BYTES) {
                malloc_alloc::deallocate(p, n);
                return;
            }

            
            obj* pObj = (obj*)p;
            obj* volatile* pp_free_list = free_lists + FREELIST_INDEX(n);
            pObj->free_list_link = *pp_free_list;
            *pp_free_list = pObj;
        }


        template<bool threads, int inst>
        void* __default_alloc_template<threads, inst>::reallocate(void* p, size_t old_size, size_t new_size) {

            return malloc_alloc::reallocate(p, old_size, new_size);
        }

        template<bool threads, int inst>
        void* __default_alloc_template<threads, inst>::refill(size_t size) {
            char* chunk = chunk_alloc(size, nobjs);

            if (1 == nobjs)
                return (chunk);

            obj* result = (obj *)chunk;	  


            obj* volatile* pp_free_list = free_lists + FREELIST_INDEX(size);
            *pp_free_list = (obj*)(chunk + size);	

            obj* cur_obj = *pp_free_list;
            for (int i = 2; i < nobjs; ++i)		
            {
                obj* next_obj = (obj *)((char *)cur_obj + size);
                cur_obj->free_list_link = next_obj;
                cur_obj = next_obj;
            }
            cur_obj->free_list_link = 0;	

            return (result);
        }


        template<bool threads, int inst>
        char* __default_alloc_template<threads, inst>::chunk_alloc(size_t size, int& nobjs) {
            char* result;
            size_t total_bytes = size * nobjs;			
            size_t bytes_left = end_free - start_free;	

            if (bytes_left >= total_bytes) {		
                result = start_free;
                start_free += total_bytes;
                return result;
            }
            else if (bytes_left >= size) {		
                result = start_free;
                nobjs = (int)(bytes_left / size);	
                total_bytes = size * nobjs;			
                start_free += total_bytes;
                return result;
            }
            else {		
            
                size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 2);
                
            
                if (bytes_left > 0) {
                    

                    
                    obj* volatile* pp_free_list = free_lists + FREELIST_INDEX(bytes_left);

                    
                    ((obj*)start_free)->free_list_link = *pp_free_list;
                    *pp_free_list = (obj *)start_free;
                }

            
                start_free = (char *)malloc(bytes_to_get);
                if (0 == start_free)	
                {
                    obj* volatile* pp_free_list;
                    
                    for (int i = size; i <= __MAX_BYTES; i += __ALIGN)
                    {
                        pp_free_list = free_lists + FREELIST_INDEX(i);
                        obj* pObj = *pp_free_list;
                        if (pObj != 0) {		
                            
                            *pp_free_list = pObj->free_list_link;
                            start_free = (char *)pObj;
                            end_free = start_free + i;

                            return chunk_alloc(size, nobjs);

                        }
                    }
                    end_free = 0;		
                    
                    start_free = (char *)malloc_alloc::allocate(bytes_to_get);
                }

                heap_size += bytes_to_get;
                end_free = start_free + bytes_to_get;

            
                return chunk_alloc(size, nobjs);
            }
        }
        #define __NODE_ALLOCATER_THREADS false
    }
#ifdef __USE_MALLOC
    typedef detail::malloc_alloc alloc;
#else
    typedef detail::__default_alloc_template<__NODE_ALLOCATER_THREADS, 0> alloc;
#endif
    template<typename T, class Alloc>
	class simple_alloc {
	public:
		static T* allocate(size_t n) {
			return 0 == n ? 0 : (T*)Alloc::allocate(n * sizeof(T));
		}
		static T* allocate(void) {
			return (T*)Alloc::allocate(sizeof(T));
		}
		static void deallocate(T* p, size_t n) {
			if (0 != n)
				Alloc::deallocate(p, n * sizeof(T));
		}
		static void deallocate(T* p) {
			Alloc::deallocate(p, sizeof(T));
		}
	};
}
#endif