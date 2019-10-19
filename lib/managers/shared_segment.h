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

#ifndef MGR_SHARED_SEGMENT_HEADER
#define MGR_SHARED_SEGMENT_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <stdexcept>
#include "config/config.h"
#include "detail/helpers.h"
#include "memory_segment.h"
#include "manager_traits.h"
#include "segment_traits.h"

namespace memory_mgr
{	
	namespace detail
	{
		class shared_allocator_base
		{
		public:
			shared_allocator_base()
				:m_mapping( osapi::invalid_mapping_handle ),
				m_base( osapi::invalid_mapping_address )
			{

			}
			//Returns address of allocated segment
			void* segment_base()
			{ return m_base; }

			typedef shared_memory_tag	memory_type;

		protected:
			osapi::mapping_handle_t m_mapping;
			void*  m_base;
		};
	}

	

#ifdef MGR_WINDOWS_PLATFORM
	//Windows shared memory allocator to SegmentAllocatorConcept 
	template<class SegNameOp>
	class shared_allocator: public detail::shared_allocator_base
	{
	public:
		//Default constructor, allocates mem_size bytes
		//in segment with name returned by SegNameOp function		
		shared_allocator( const size_t mem_size )
		{
			std::string name = "Global\\";
			name += SegNameOp::get_name();
			m_mapping = osapi::create_file_mapping( name, 0,
				PAGE_READWRITE, ulong_cast( mem_size ) );

			if( m_mapping == osapi::invalid_mapping_handle  )
			{
				throw std::runtime_error( "file mapping creation failed" );
			}

			m_base = osapi::map_view_of_file( m_mapping, FILE_MAP_ALL_ACCESS,
				mem_size);
			if( m_base == osapi::invalid_mapping_address)
			{
				throw std::runtime_error( "memory mapping failed" );
			}			
		}
		
		~shared_allocator()
		{
			if( m_base != osapi::invalid_mapping_address)
			{
				osapi::unmap_view_of_file(m_base);
			}

			if( m_mapping != osapi::invalid_mapping_handle )
			{
				osapi::close_handle(m_mapping);
			}
		}
	};

#elif defined( MGR_LINUX_PLATFORM )
	
	//Posix shared memory allocator to SegmentAllocatorConcept 
	template<class SegNameOp>
	class shared_allocator: public detail::shared_allocator_base
	{
		typedef detail::shared_allocator_base base_type;
		std::string m_name;
		const size_t m_size;
	public:			
		//Default constructor, allocates mem_size bytes
		//in segment with name returned by SegNameOp function		
		shared_allocator( const size_t mem_size )
			:m_size( mem_size ),
			m_name( SegNameOp::get_name() )
		{
			helpers::add_leading_slash( m_name );
			int open_flag = O_RDWR //read-write mode
				| O_CREAT;//Create new or open existent

			mode_t access_mode = S_IRWXO //read, write, execute/search by others 
				| S_IRWXG	//read, write, execute/search by group
				| S_IRWXU;	//read, write, execute/search by owner

			m_mapping = osapi::create_file_mapping(m_name, open_flag, access_mode);
			if( base_type::m_mapping == osapi::invalid_mapping_handle )
			{
				throw std::runtime_error( "file mapping creation failed" );
			}

			//Resize file mapping
			if( osapi::resize_file_mapping( base_type::m_mapping, m_size ) != 0 )
			{
				throw std::runtime_error( "failed to resize file mapping" );
			}

			//Map file to memory
			m_base = osapi::map_view_of_file( base_type::m_mapping, 
				PROT_READ | PROT_WRITE, mem_size );

			if( m_base == osapi::invalid_mapping_address )
			{
				throw std::runtime_error( "memory mapping failed" );
			}
		}

		~shared_allocator()
		{		
			if( m_base != osapi::invalid_mapping_address )
			{
				osapi::unmap_view_of_file( m_base, m_size );
			}			 
			
			if( base_type::m_mapping != osapi::invalid_mapping_handle )
			{
				osapi::close_file_mapping( m_name, m_mapping );
			}

		}
	};
#endif

#define MGR_SEGMENT_NAME( var_name ) var_name##_name_returner

#define MGR_DECLARE_SEGMENT_NAME( var_name, segment_name )\
	struct MGR_SEGMENT_NAME(var_name)\
	{\
		static inline const char* get_name( const size_t/* id*/= 0)\
		{ return "memory_mgr-"segment_name; }\
	};

MGR_DECLARE_SEGMENT_NAME( default, "default_segment" );
	
	//MemMgr - must support MemoryManagerConcept
	template< class MemMgr, class SegNameOp = MGR_SEGMENT_NAME( default ) >	
	class shared_segment 
		: public memory_segment< shared_allocator<SegNameOp>, MemMgr >
	{
	};

	template< class MemMgr, class SegNameOp >
	struct manager_traits< shared_segment< MemMgr, SegNameOp > > 
		: public manager_traits< MemMgr >
	{
		/**
		   @brief Memory manager class, that was decorated
		*/
		typedef MemMgr base_manager_type;
	};
}

#endif// MGR_SHARED_SEGMENT_HEADER
