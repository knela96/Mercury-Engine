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

#ifndef MGR_MANAGER_TRAITS_HEADER
#define MGR_MANAGER_TRAITS_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

namespace memory_mgr
{
	/**
	   @brief memory_manager traits
	   @details all associated types and constants are accessible using this type
	*/
	template<class MemMgr>
	struct manager_traits
	{
		/**
		   @brief Memory manager type
		*/
		typedef MemMgr										manager_type;

		/**
		   @brief Memory manager class, that was decorated
		*/
		typedef void										base_manager_type;

		/**
		   @brief memory manager category tag
		*/
		typedef typename manager_type::manager_category		manager_category;

		/**
		   @brief memory block pointer type
		   @see static_bitset::block_ptr_type
		*/
		typedef typename manager_type::block_ptr_type		block_ptr_type;		
		
		/**
		   @brief Type used to store size, commonly std::size_t
		   @see static_bitset::size_type
		*/
		typedef typename manager_type::size_type			size_type;
		
		/**
		   @brief Type of synchronization object passed as template
		   parameter                                               
		*/
		typedef typename manager_type::sync_object_type		sync_object_type;

		/**
		   @brief memory offset type passed as template parameter
		*/
		typedef typename manager_type::offset_type			offset_type;

		enum
		{
			chunk_size	= manager_type::chunk_size /**< size of memory chunk*/,
			memory_size = manager_type::memory_size /**< memory size in bytes available to manager*/,
			num_chunks	= manager_type::num_chunks /**< number of memory chunks that can be allocated*/
		};
	};
}

#endif //MGR_MANAGER_TRAITS_HEADER
