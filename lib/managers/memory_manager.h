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

#ifndef MGR_MEMORY_MANAGER_HEADER
#define MGR_MEMORY_MANAGER_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include "detail/ptr_helpers.h"
#include "detail/bit_manager.h"
#include "sync/critical_section.h"
#include "sync/locks.h"
#include "detail/offset_traits.h"
#include "manager_category.h"

/**
   @brief Main namespace
*/
namespace memory_mgr
{
	/**
	   @brief Encapsulates work with memory
	   @tparam BlockType   integral type, that will be used in
	                      static_bitset
	   @tparam MemorySize  memory size in bytes which will be
	                      available to manager
	   @tparam ChunkSize   size of minimum block in bytes, which
	                      manager can allocate
	   @tparam OffsetType  type that will be used to store memory offset
	   @tparam SyncObj     Synchronization object that will be used to
	                      synchronize access to internal structures
	                      during management operations               
	*/
	template
	<
		class BlockType, 
		size_t MemorySize,
 		size_t ChunkSize,
		class OffsetType = size_t,
		class SyncObj = sync::critical_section
	>
	class memory_manager : protected sync::object_level_lockable<SyncObj>
	{
	public:
		/**
		   @brief compile time computed constants
		*/
		enum
		{
			chunk_size = ChunkSize /**< size of memory chunk*/,
			memory_size = MemorySize /**< memory size in bytes*/,
			num_chunks = memory_size / chunk_size /**< number of memory chunks that can be allocated*/
		};
		
	private:		
		/**
		   @brief lock type, used for synchronization
		*/
		typedef typename sync::object_level_lockable<SyncObj>::lock lock;
		
		/**
		   @brief bit manager type, used to manipulate chunks bitmap
		*/
		typedef detail::bit_manager<BlockType, num_chunks, detail::mcNone> bitmgr_type;

		/**
		   @brief Bit Manager used to store information about allocated
		   memory chunks                                               
		*/
		bitmgr_type m_bitmgr;

		/**
		   @brief Pointer to memory base address from which offset is
		   calculated                                                
		*/
		char* m_membase;
		
	public:
		/**
		   @brief Type of this
		*/
		typedef memory_manager								self_type;
		/**
		   @brief memory manager category tag
		*/
		typedef memory_manager_tag							manager_category;

		/**
		   @brief memory block pointer type
		   @see static_bitset::block_ptr_type
		*/
		typedef typename bitmgr_type::block_ptr_type		block_ptr_type;

		/**
		   @brief Type used to store size, commonly std::size_t
		   @see static_bitset::size_type
		*/
		typedef typename bitmgr_type::size_type				size_type;
		
		/**
		   @brief Type of synchronization object passed as template
		   parameter                                               
		*/
		typedef SyncObj										sync_object_type;
		/**
		   @brief memory offset type passed as template parameter
		*/
		typedef OffsetType									offset_type;
		

		/**
		   @brief Constructor, performs initialization of manager and
					memory passed as parameter
		   @param mem_base  Pointer to memory which will be managed by
		                    manager, before first used memory must be
		                    zero filled                               
		*/
		explicit memory_manager( void* mem_base )
			:m_bitmgr( static_cast< block_ptr_type >( mem_base ) )
		{
			m_membase = detail::char_cast( detail::shift( mem_base, bitmgr_type::memory_usage ) );
		}
		
		/**
		   @brief Call this method to allocate memory block
		   @param size size of memory block in bytes
		   @exception bad_alloc if manager went out of memory
		   @return offset in bytes from memory base address.
		*/
		offset_type allocate( size_type size )
		{			
			return do_allocate( size, helpers::throw_bad_alloc );
		}


		/**
		   @brief Call this method to allocate memory block
		   @param size    size of memory block in bytes 
		   @param nothrow  unused parameter, just to overload existing
		                   function
		   
		   @exception newer  throws
		   @return offset in bytes from memory base address.          
		*/
		offset_type allocate( size_type size, const std::nothrow_t& /*nothrow*/ )/*throw()*/
		{			
			return do_allocate( size, helpers::do_nothing );
		}

		/**
		   @brief Call this method to deallocate memory block
		   @param offset  offset returned by allocate method
		   @param size   size of memory block in bytes
		   @exception newer  throws
		*/
 		void deallocate( const offset_type offset, size_type size )
 		{
			lock l(*this);
 			m_bitmgr.deallocate( chunk_index( offset ), chunks_required( size ) );
 		}

		/**
		   @brief Call this method to get memory base address from which offset
		   is calculated
		   @exception newer  throws
		   @return pointer to memory base address                               
		*/
		char* get_base() const
		{
			return m_membase;
		}

		/**
		   @brief Call this method to know is there available memory in
		   manager
		   
		   @exception newer  throws
		   @retval true   if there is no more free memory to
		                  allocate
		   @retval false  otherwise                                    
		*/
		bool empty()
		{
			return m_bitmgr.empty();
		}

		/**
		   @brief Call this method to know is there any allocated blocks
		   @exception newer  throws
		   @retval true   no blocks are allocated by this manager
		   @retval false  otherwise                                     
		*/
		bool free()
		{
			return m_bitmgr.free();
		}
	private:
		/**
		   @brief Returns offset by chunk index
		   @param chunk_ind  chunk index
		   @exception newer  throws
		*/
		static inline size_type calc_offset( size_type chunk_ind )
		{ return chunk_ind * chunk_size; }

		/**
		   @brief Returns chunk index by offset
		   @param offset  memory offset in bytes
		   @exception newer  throws
		*/
		static inline size_type chunk_index( size_type offset )
		{ return offset / chunk_size; }

		/**
		   @brief Returns chunks count by bytes count
		   @param size size in bytes
		   @exception newer  throws
		*/
		static inline size_type chunks_count( size_type size )
		{ return chunk_index( size ); }

		/**
		   @brief Returns number of extra bytes
		   @exception newer  throws
		*/
		static inline size_type extra_bytes( size_type size )
		{ return size % chunk_size; }

		/**
		   @brief Returns number of chunks required to store requested
		   bytes number		   
		   @param size size in bytes
		   @exception newer  throws
		*/
		static inline size_type chunks_required( size_type size )
		{
			return chunks_count( size ) + (extra_bytes( size ) ? 1 : 0);
		};

		

		/**
		   @brief Call this method to allocate memory block
		   @return offset in bytes from memory base address.
		   @param size         size of memory block in bytes
		   @param OnNoMemoryOp  Callback for "out of memory" event
		   @exception n/a Callback function can throw an exception, 
		   but by itself method never throws
		*/
		template< class OnNoMemory >
		inline offset_type do_allocate( size_type size, OnNoMemory OnNoMemoryOp )
		{
			lock l(*this);
			size_type chunk_ind = m_bitmgr.allocate( chunks_required( size ) );
			if( chunk_ind == bitmgr_type::npos )
			{
				OnNoMemoryOp();
				return offset_traits<offset_type>::invalid_offset;
			}
			return calc_offset( chunk_ind );
		}
	};	
}


#endif// MGR_MEMORY_MANAGER_HEADER
