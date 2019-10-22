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

#ifndef MGR_SIZE_TRACKING_HEADER
#define MGR_SIZE_TRACKING_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include "manager_traits.h"
#include "manager_category.h"
#include "pointer_convert.h"
#include "detail/ptr_helpers.h"

namespace memory_mgr
{
	namespace detail
	{
		/**
		   @brief Base class for size_tracking implementation classes
		   @tparam MemMgr memory_manager class, with or w/o decorators
		*/
		template< class MemMgr >
		class size_tracking_impl_base
		{
			/**
			   @brief Memory manager class that should be decorated
			*/
			typedef MemMgr									mgr_type;
		public:	

			/**
			   @brief Type used to store size, commonly std::size_t
			   @see static_bitset::size_type
			*/
 			typedef typename manager_traits<mgr_type>::size_type			size_type;
			
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

		protected:
			explicit size_tracking_impl_base( void* mem_base )
				:m_mgr( mem_base )
			{}

			/**
			   @brief Adds size of auxiliary data to 'size' parameter
			  
			   @param	size	variable that should be updated
			   @exception newer throws
			*/
			void update_size( size_type& size )
			{
				//Additional memory for size storing
				size += sizeof( size_type );
			}

			/**
			   @brief Puts memory block size at the beginning of memory block
			  
			   @param	ptr	pointer to allocated memory block
			   @param	size	size of allocated memory block 
			   @exception newer throws
			  
			   @return pointer to first byte after stored size
			*/
			void* store_size( void* ptr, size_type size )
			{
				//Store size
				size_type* psize = detail::size_cast( ptr );
				*psize = size;
				return ++psize;
			}

			/**
			   @brief Memory Manager instance that is decorated be size tracking
			*/
			mgr_type m_mgr;
		};

		/**
		   @brief Base class for size tracking implementation template classes specializations
		*/
		template<class MemMgr, class pointer_conv_flag >
		class size_tracking_impl;

		/**
		   @brief Size tracking implementation for memory manager that supports PointerConverterConcept
		*/
		template< class MemMgr >
		class size_tracking_impl<
			MemMgr,
			yes_type /* PointerConverterConcept supported*/ >
			: public size_tracking_impl_base< MemMgr >
		{
			/**
			   @brief Memory manager class that should be decorated
			*/
			typedef MemMgr								mgr_type;

			/**
			   @brief size tracking implementation base class
			*/
			typedef size_tracking_impl_base< mgr_type >	impl_base_type;

		protected:
			/**
			   @brief Protected constructor, simply passes parameters to base class' constructor
			  
			   @param	mem_base	Pointer to memory which will be managed by
									manager
			   @exception newer throws
			*/
			explicit size_tracking_impl( void* mem_base )
				:impl_base_type( mem_base )
			{}

		public:
			/**
			   @brief Type used to store size, commonly std::size_t
			   @see static_bitset::size_type
			*/
			typedef typename impl_base_type::size_type			size_type;

			/**
			  @brief Call this method to allocate memory block
			  @param size size of memory block in bytes
			  @exception bad_alloc if manager went out of memory
			  @return pointer to allocated memory block
			*/
			void* allocate( size_type size )
			{			
				update_size( size );
				return store_size( this->m_mgr.allocate( size ), size );
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
				update_size( size );
				return store_size( this->m_mgr.allocate( size, nothrow ), size );
			}

			/**
			   @brief Call this method to deallocate memory block
			   @param ptr  pointer to memory block, returned by allocate method
			   @param size   this value is ignored
			   @exception newer  throws
			*/
			void deallocate( void* ptr, size_type /*size*/ = 0)
			{
				assert( ptr >= this->m_mgr.get_base() && "Invalid pointer value");
				assert( ptr < ( this->m_mgr.get_base() + manager_traits<mgr_type>::memory_size )  && "Invalid pointer value" );

				size_type *ps = detail::size_cast( ptr );
				--ps;
				this->m_mgr.deallocate( ps, *ps );
			}
		};

		/**
		   @brief Size tracking implementation for memory manager that doesn't support PointerConverterConcept
		*/
		template< class MemMgr >
		class size_tracking_impl<
			MemMgr,
			no_type /* PointerConverterConcept not supported*/ >
			: public size_tracking_impl< 
				pointer_convert< MemMgr >,
				yes_type /* PointerConverterConcept supported*/
			>
		{
			/**
			   @brief Decorating memory manager class using pointer_convert decorator,
			   @details	so now internally we can use implementation of size tracking for
					memory managers that supports PointerConverterConcept
			*/
			typedef pointer_convert< MemMgr >				mgr_type;

			/**
			   @brief size tracking implementation base class
			   @details this class is size tracking implementation
						that supports PointerConverterConcept, it implements main functionality
						of size tracking
			*/
			typedef size_tracking_impl< 
				mgr_type,
				yes_type /* PointerConverterConcept supported*/
			>	impl_base_type;
		protected:
			/**
			   @brief Protected constructor, simply passes parameters to base class' constructor
			  
			   @param	mem_base	Pointer to memory which will be managed by
									manager
			   @exception newer throws
			*/
			explicit size_tracking_impl( void* mem_base )
				:impl_base_type( mem_base )
			{}
		public:
			/**
			   @brief Type used to store size, commonly std::size_t
			   @see static_bitset::size_type
			*/
			typedef typename impl_base_type::size_type				size_type;

			/**
			   @brief type that used to store memory offset
			   @see memory_manager::offset_type
			*/
			typedef typename manager_traits<mgr_type>::offset_type	offset_type;

			/**
			   @brief Call this method to allocate memory block
			   @param size size of memory block in bytes
			   @exception bad_alloc if manager went out of memory
			   @return offset in bytes from memory base address.
			*/
			offset_type allocate( size_type size )
			{	
				//Just calls allocate method of size tracking implementation that supports PointerConverterConcept
				//and converts returned pointer to offset
				return detail::pointer_to_offset( impl_base_type::allocate( size ), this->m_mgr );
			}

			/**
			   @brief Call this method to allocate memory block
			   @param size    size of memory block in bytes 
			   @param nothrow  unused parameter, just to overload existing
				function

			   @exception newer  throws
			   @return offset in bytes from memory base address.          
			*/
			offset_type allocate( size_type size, const std::nothrow_t& nothrow )/*throw()*/
			{		
				//Calls allocate method of size tracking implementation that supports PointerConverterConcept
				//and converts returned pointer to offset
				return detail::pointer_to_offset( impl_base_type::allocate( size, nothrow ), this->m_mgr );
			}

			/**
			   @brief Call this method to deallocate memory block
			   @param offset  offset returned by allocate method
			   @param size   this value is ignored
			   @exception newer  throws
			*/
			void deallocate( const offset_type offset, size_type /*size*/ = 0 )
			{
				//Converts passed offset into pointer and calls deallocate method
				//of size tracking implementation that supports PointerConverterConcept
				impl_base_type::deallocate( detail::offset_to_pointer( offset, this->m_mgr ) );
			}
		};

	}

	/**
	   @brief Size tracking decorator for memory manager.
	   @details Tracks size of allocated memory blocks, so when
	   deallocating them there is no need to specify size of memory
	   block
	   @tparam MemMgr  memory_manager class, with or w/o decorators,
					must support MemoryManagerConcept.
	                If supports PointerConverterConcept then tracking
	                operations will work faster                    
	*/
	template< class MemMgr >
	class size_tracking 
		: public detail::size_tracking_impl
		< 
			MemMgr, 
			typename is_category_supported<MemMgr, pointer_conversion_tag>::result 
		>
	{
		/**
		   @brief Memory manager class that should be decorated
		*/
		typedef MemMgr									memmgr_type;

		/**
		   @brief Size tracking implementation type
		*/
		typedef detail::size_tracking_impl
		< 
			MemMgr, 
			typename is_category_supported<MemMgr, pointer_conversion_tag>::result 
		>	impl_type;

	public:
		/**
		   @brief Constructor, creates memory manager with specified
		   base address
		   @param mem_base  Pointer to memory which will be managed by
		                    manager, before first used memory must be
		                    zero filled
		   @see memory_manager::memory_manager                        
		*/
		explicit size_tracking( void* mem_base )
			:impl_type( mem_base )
		{}
	};

	/**
	   @brief memory_manager + size_tracking traits
	   @details Adds size_tracking_tag to manager_category
	*/
	template<class MemMgr>
	struct manager_traits< size_tracking< MemMgr > > 
		: public manager_traits< MemMgr >
	{
		/**
		   @brief Memory manager class, that was decorated
		*/
		typedef MemMgr base_manager_type;

		/**
		   @brief Add size_tracking_tag to manager_category
		*/
		struct manager_category 
			: public virtual manager_traits<MemMgr>::manager_category,
			public virtual size_tracking_tag
		{};
	};
}

#endif //MGR_SIZE_TRACKING_HEADER
