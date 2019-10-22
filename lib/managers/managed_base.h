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

#ifndef MGR_MANAGED_BASE_HEADER
#define MGR_MANAGED_BASE_HEADER

#include "detail/static_assert.h"
#include "detail/ptr_helpers.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

namespace memory_mgr
{
	namespace detail
	{
		/**
		   @brief Call this method to get full size of memory block 
					that was used to store array
		  
		   @param	p	pointer to memory block 
					that was used to store array
		   @param	size	size passed as parameter to delete[] function
		   @exception newer throws
		  
		   @return size of memory block 
		*/
		static inline size_t calc_full_size( void* p, size_t size )
		{
#ifdef _MSC_VER
			size = *size_cast(p) * size;
			return size + sizeof( size_t );
#else
			return size;
#endif
		}
	}

	/**
	   @brief Helper class, used to pass object's name to new/new[] operators
	*/
	class object_name			
	{	
		/**
		   @brief Pointer to null terminated string that stores object's name
		*/
		const wchar_t* m_name;
	public:
		/**
		   @brief Constructor 
		   @param name Pointer to null terminated string that stores object's name

		   @warning Pointer, passed as name parameter, must 
					be valid until the end of allocation operation.
		*/
		object_name( const wchar_t* name )
			: m_name( name )
		{}


		/**
		   @brief Call this method to get stored name
		  
		   @exception newer throws
		  
		   @return Pointer to null terminated string that stores object's name 
		*/
		const wchar_t* get_name()
		{
			return m_name;
		}
	};

	/**
	   @brief 
	   @details 
	   @tparam MemMgr Memory manager class, must support PointerConverterConcept, SingletonManagerConcept
	*/
	template< class MemMgr >
	class managed_base
	{
	protected:
		~managed_base()
		{}

		typedef MemMgr mem_mgr;
	public:
		/**
		   @brief Overloaded operator new, allocates memory block in memory managed by mem_mgr
		   @param size size of memory block required to store object of derived class,
						this parameter is passed by compiler automatically
		   @exception bad_alloc if manager went out of memory
		   @return pointer to allocated memory block
		*/
		static void* operator new( size_t size )/*throw( std::bad_alloc )*/
		{
			return mem_mgr::instance().allocate( size );			
		}

		/**
		   @brief Overloaded operator new, allocates memory block in memory managed by mem_mgr
		   @param size size of memory block required to store object of derived class,
					this parameter is passed by compiler automatically
		   @param nothrow  unused parameter, just to overload existing
		                   function
		   @exception newer  throws
		   @return pointer to allocated memory block
		*/
		static void* operator new( size_t size, const std::nothrow_t& nothrow ) /*throw()*/
		{
			return mem_mgr::instance().allocate( size, nothrow );			
		}

		/**
		   @brief Overloaded placement operator new
		   @param p pointer to memory where to place object
		   @exception newer  throws
		   @return pointer passed as second parameter
		*/
		static void* operator new(  size_t, void* p ) /*throw()*/
		{
			return p;
		}
		
		/**
		   @brief Overloaded operator new for named objects,
					allocates memory block in memory managed by mem_mgr
		   @param size size of memory block required to store object of derived class,
					this parameter is passed by compiler automatically
		   @param name name of the object
		   @exception bad_alloc if manager went out of memory
		   @return pointer to allocated memory block
		*/
		static void* operator new( size_t size, const object_name& name )/*throw( std::bad_alloc )*/
		{
			name;
			/**
			   @todo implement correct logic in this method
			   mem_mgr::instance().allocate( size, name );
			*/
			STATIC_ASSERT( false, named_objects_not_supported );
			return 0;
		}

		//////////////////////////////////////////////////////////////////////////
		/**
		   @brief Overloaded operator delete, deallocates memory block in memory managed by mem_mgr
		   @param p pointer to memory block
		   @param size size of memory block that should be deallocated
		   @exception newer throws
		   @remark all parameters are passed by compiler automatically
		*/
		static void operator delete( void* p, size_t size )
		{
			mem_mgr::instance().deallocate( p, size );
		}

		/**
		   @brief Overloaded operator delete for named objects,
					deallocates memory block in memory managed by mem_mgr
		   @param p pointer to memory block
		   @param size size of memory block that should be deallocated
		   @param name name of the object that should be deallocated
		   @exception newer  throws
		   @remark all parameters are passed by compiler automatically
		*/
		static void operator delete( void* p, size_t size, const object_name& name )
		{
			p;
			name;
			/**
			   @todo implement correct logic in this method
			   mem_mgr::instance().deallocate( size, name );
			*/
			STATIC_ASSERT( false, named_objects_not_supported );
		}

		//////////////////////////////////////////////////////////////////////////
		/**
		   @brief Overloaded operator new[], allocates memory block in memory managed by mem_mgr
		   @param size size of memory block required to store array of objects of derived class,
						this parameter is passed by compiler automatically
		   @exception bad_alloc if manager went out of memory
		   @return pointer to allocated memory block
		*/
		static void* operator new[]( size_t size )/*throw( std::bad_alloc )*/
		{			
			return mem_mgr::instance().allocate( size );			
		}

		/**
		   @brief Overloaded operator new[], allocates memory block in memory managed by mem_mgr
		   @exception newer  throws
		   @return pointer to allocated memory block
		*/
		static void* operator new[]( size_t size, const std::nothrow_t& nothrow ) /*throw()*/
		{
			
			return mem_mgr::instance().allocate( size, nothrow );			
		}

		/**
		   @brief Overloaded placement operator new[]
		   @param p pointer to memory where to place object
		   @exception newer  throws
		   @return pointer passed as second parameter
		*/
		static void* operator new[](  size_t, void* p ) /*throw()*/
		{
			return p;
		}

		/**
		   @brief Overloaded operator new[] for named objects,
					allocates memory block in memory managed by mem_mgr
		   @param size size of memory block required to store object of derived class,
					this parameter is passed by compiler automatically
		   @param name name of the object that should be allocated
		   @exception bad_alloc if manager went out of memory
		   @return pointer to allocated memory block
		*/
		static void* operator new[]( size_t size, const object_name& name )/*throw( std::bad_alloc )*/
		{
			name;
			/**
			   @todo implement correct logic in this method
			   mem_mgr::instance().allocate( size, name );
			*/
			STATIC_ASSERT( false, named_objects_not_supported );
			return 0;
		}

		//////////////////////////////////////////////////////////////////////////
		/**
		   @brief Overloaded operator delete[], deallocates memory block in memory managed by mem_mgr
		   @param p pointer to memory block
		   @param size size of memory block that should be deallocated
		   @exception newer throws
		   @remark all parameters are passed by compiler automatically
		*/
		static void operator delete[]( void* p, size_t size )
		{
			mem_mgr::instance().deallocate( p, detail::calc_full_size( p, size) );
		}

		/**
		   @brief Overloaded operator delete[] for named objects,
					deallocates memory block in memory managed by mem_mgr
		   @param p pointer to memory block
		   @param size size of memory block that should be deallocated
		   @param name name of the object that should be deallocated
		   @exception newer  throws
		   @remark all parameters are passed by compiler automatically
		*/
		static void operator delete[]( void* p, size_t size, const object_name& name )
		{
			p;
			name;
			/**
			   @todo implement correct logic in this method
			   mem_mgr::instance().deallocate( size, name );
			*/
			STATIC_ASSERT( false, named_objects_not_supported );
		}
	};
	
	
}

#endif// MGR_MANAGED_BASE_HEADER
