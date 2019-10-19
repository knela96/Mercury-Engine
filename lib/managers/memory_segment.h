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

#ifndef MGR_MEMORY_SEGMENT_HEADER
#define MGR_MEMORY_SEGMENT_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include "manager_traits.h"
#include "manager_category.h"
#include "segment_traits.h"

namespace memory_mgr
{	
	/**
	   @brief Default segment allocator traits
	*/
	template<class SegmentAllocator>
	struct segment_alloc_traits
	{
		/**
		   @brief memory type tag, e.g. heap, shared memory, memory mapped file etc.
		*/
		typedef typename SegmentAllocator::memory_type	memory_type;
	};

	/**
	   @brief Adapter, used to link memory_manager and memory segment
	   @tparam SegmentAllocator memory segment allocator, must support SegmentAllocatorConcept
	   @tparam MemMgr memory manager type, must support MemoryManagerConcept
	*/
	template< class SegmentAllocator, class MemMgr >	
	class memory_segment : private SegmentAllocator, public MemMgr
	{
		/**
		   @brief Memory manager that will be linked with allocated memory segment
		*/
		typedef MemMgr				memmgr_type;
		
		/**
		   @brief Memory segment allocator type
		   @details Will be used to allocate memory segment
		*/
		typedef	SegmentAllocator	memory;

	public:
		/**
		   @brief Default constructor
		   @details Creates memory segment and memory manager linked to this segment
		*/
		memory_segment()
			:memory( manager_traits<memmgr_type>::memory_size ),
			memmgr_type( memory::segment_base() )
		{}
	};

	/**
	   @brief memory manager traits for memory managers decorated by memory_segment class
	*/
	template< class SegmentAllocator, class MemMgr >
	struct manager_traits< memory_segment< SegmentAllocator, MemMgr > > 
		: public manager_traits< MemMgr >
	{
		/**
		   @brief Memory manager class, that was decorated
		*/
		typedef MemMgr base_manager_type;
	};

	/**
	   @brief Default memory segment traits
	*/
	template< class SegmentAllocator, class MemMgr >
	struct segment_traits< memory_segment< SegmentAllocator, MemMgr > > 
	{
		/**
		   @brief memory type tag
		   @details retrieves memory type from segment allocator traits
		*/
		typedef typename segment_alloc_traits<SegmentAllocator>::memory_type	memory_type;
	};

}
#endif// MGR_MEMORY_SEGMENT_HEADER
