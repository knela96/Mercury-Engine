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

#ifndef MGR_POINTER_CONVERT_HEADER
#define MGR_POINTER_CONVERT_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <vector>
#include "detail/ptr_helpers.h"
#include "manager_traits.h"
#include "manager_category.h"


namespace memory_mgr
{	
	/**
	   @brief Pointer converting decorator for memory manager. Implements PointerConverterConcept
	   @details Replaces memory_manager allocate/deallocate methods to work with pointers instead of offsets
	   @tparam MemMgr  memory_manager class, with or w/o decorators.
					must support MemoryManagerConcept
	*/
	template< class MemMgr >	
	class pointer_convert 
	{
		/**
		   @brief Memory manager class that should be decorated
		*/
		typedef MemMgr				mgr_type;

		/**
		   @brief Memory Manager instance that is decorated be pointer converter
		*/
		mgr_type m_mgr;
	public:	
		/**
		   @brief Type used to store size, commonly std::size_t
		   @see static_bitset::size_type
		*/
 		typedef typename manager_traits<mgr_type>::size_type			size_type;

		/**
		   @brief type that used to store memory offset
		   @see memory_manager::offset_type
		*/
 		typedef typename manager_traits<mgr_type>::offset_type		offset_type;

		/**
		   @brief Constructor, creates memory manager with specified
		   base address
		   @param mem_base  Pointer to memory which will be managed by
		                    manager, before first used memory must be
		                    zero filled
		   @see memory_manager::memory_manager                        
		*/
		pointer_convert( void* mem_base )
			:m_mgr( mem_base )
		{}

		/**
		  @brief Call this method to allocate memory block
		  @param size size of memory block in bytes
		  @exception bad_alloc if manager went out of memory
		  @return pointer to allocated memory block
		*/
		void* allocate( size_type size )
		{			
			return detail::offset_to_pointer( m_mgr.allocate( size ), m_mgr );
		}

		/**
		   @brief Call this method to allocate memory block
		   @param size    size of memory block in bytes 
		   @param nothrow  unused parameter, just to overload existing
		                   function
		   
		   @exception newer  throws
		   @return pointer to allocated memory block         
		*/
		void* allocate( size_type size, const std::nothrow_t& nothrow )/*throw()*/
		{			
			return detail::offset_to_pointer( m_mgr.allocate( size, nothrow ), m_mgr );
		}

		/**
		   @brief Call this method to deallocate memory block
		   @param p  pointer to memory block, returned by allocate method
		   @param size   size of memory block in bytes
		   @exception newer  throws
		*/
 		void deallocate( const void* p, size_type size )
 		{
			m_mgr.deallocate( detail::pointer_to_offset( p, m_mgr ), size );
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
			return m_mgr.empty();
		}

		/**
		   @brief Call this method to know is there any allocated blocks
		   @exception newer  throws
		   @retval true   no blocks are allocated by this manager
		   @retval false  otherwise                                     
		*/
		bool free()
		{
			return m_mgr.free();
		}

		/**
		   @brief Call this method to get memory base address from which offset
		   is calculated
		   @exception newer  throws
		   @return pointer to memory base address                               
		*/
		const char* get_base() const
		{
			return m_mgr.get_base();
		}

		
	};

	/**
	   @brief memory_manager + pointer_convert traits
	   @details Adds pointer_conversion_tag to manager_category
	*/
	template< class MemMgr >
	struct manager_traits< pointer_convert< MemMgr > > 
		: public manager_traits< MemMgr >
	{
		/**
		   @brief Memory manager class, that was decorated
		*/
		typedef MemMgr base_manager_type;

		/**
		   @brief Add pointer_conversion_tag to manager_category
		*/
		struct manager_category 
			: public virtual manager_traits<MemMgr>::manager_category,
			public virtual pointer_conversion_tag
		{};
	};
}
#endif// MGR_POINTER_CONVERT_HEADER
