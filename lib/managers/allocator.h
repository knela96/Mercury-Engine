/* 
Generic Memory Manager (memory-mgr)
http://memory-mgr.sourceforge.net/
Copyright (c) 2007-2008 Anton (shikin) Matosov

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 3, 29 June 2007 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA <http://fsf.org/>


Please feel free to contact me via e-mail: shikin@users.sourceforge.net
*/

#ifndef MGR_ALLOCATOR_HEADER
#define MGR_ALLOCATOR_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include "simple_ptr.h"

namespace memory_mgr
{

	template< class T,	class MemMgr >
	class allocator
	{
		enum
		{
			item_size = sizeof(T)
		};
	public:
		typedef MemMgr								memmgr_t;
		typedef T									value_type;
		typedef allocator< value_type, memmgr_t >	self_type;

		//*
		typedef typename simple_ptr< value_type, memmgr_t >		pointer;
		typedef typename const simple_ptr< value_type, memmgr_t >	const_pointer;
		typedef typename simple_ptr< value_type, memmgr_t >::reference_type		reference;
		typedef typename simple_ptr< value_type, memmgr_t >::const_reference_type	const_reference;
		//*/

		/*
		typedef value_type*								pointer;
		typedef const value_type*						const_pointer;
		typedef value_type&		reference;
		typedef const value_type&	const_reference;
		//*/

		typedef typename memmgr_t::size_type size_type;
		typedef ptrdiff_t difference_type;

		template<class Other>
		struct rebind
		{	// convert an allocator<_Ty> to an allocator <_Other>
			typedef typename memory_mgr::allocator< Other, memmgr_t > other;
		};

		// return address of mutable _Val
		pointer address( reference val ) const
		{	
			return pointer(&val);
		}

		// return address of nonmutable _Val
		const_pointer address( const_reference val ) const
		{	
			return const_pointer(&val);
		}

		// construct default allocator (do nothing)
		allocator()
		{	
		}
		template<class other>
		allocator( const allocator<other, memmgr_t>& ) _THROW0()
		{	// construct from a related allocator (do nothing)
		}

		template<class other>
		self_type& operator=( const allocator<other, memmgr_t>& )
		{	// assign from a related allocator (do nothing)
			return (*this);
		}

		// deallocate object at _Ptr, ignore size
		void deallocate( pointer ptr, size_type size )
		{	
			memmgr_t::instance().deallocate( &ptr, size );
		}

		// allocate array of _Count elements
		pointer allocate(size_type count)
		{	
			return new(memmgr_t::instance()) value_type[count];
		}

		// allocate array of _Count elements, ignore hint
		pointer allocate(size_type count, const void _FARQ *)
		{	
			return (allocate(count));
		}

		// construct object at _Ptr with value _Val
		void construct(pointer ptr, const_reference val)
		{	
			::new (&*ptr) T(val);
		}

		// destroy object at _Ptr
		void destroy(pointer ptr)
		{	
			(&*ptr)->~T();
		}

		// estimate maximum array size
		size_type max_size() const 
		{	
			return memmgr_t::memory_size;
		}

	};

	// allocator TEMPLATE OPERATORS
	template<class T,
	class U, class mem_mgr > inline
		bool operator==(const allocator<T, mem_mgr>&, const allocator<U, mem_mgr>&) _THROW0()
	{	// test for allocator equality (always true)
		return (true);
	}

	template<class T,
	class U, class mem_mgr > inline
		bool operator!=(const allocator<T, mem_mgr>&, const allocator<U, mem_mgr>&) _THROW0()
	{	// test for allocator inequality (always false)
		return (false);
	}

}

#endif //MGR_ALLOCATOR_HEADER
