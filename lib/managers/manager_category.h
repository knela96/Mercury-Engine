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

#ifndef MGR_MANAGER_CATEGORY_HEADER
#define MGR_MANAGER_CATEGORY_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include "manager_traits.h"
#include "detail/type_manip.h"

namespace memory_mgr
{
	//Tags to categorize memory managers capabilities

	/**
	   @brief Simple memory manager tag
	*/
	struct memory_manager_tag{};

	/**
	   @brief Singleton memory manager tag
	*/
	struct singleton_manager_tag{};

	/**
	   @brief Memory manager with size tracking capability tag
	*/
	struct size_tracking_tag{};

	/**
	   @brief Memory manager with pointer conversion capability tag
	*/
	struct pointer_conversion_tag{};

	/**
	   @brief Tag for memory manager with capability to allocate named objects 
	*/
	struct named_objects_manager_tag{};

	//Tags to categorize memory types

	/**
	   @brief Tag for memory segments of unknown type
	*/
	struct unknown_memory_tag{};

	/**
	   @brief Tag for heap memory segments
	*/
	struct heap_memory_tag{};
	
	/**
	   @brief Tag for shared memory segments
	*/
	struct shared_memory_tag{};

	
	/**
	   @brief Yes type, indicates success
	*/
	struct yes_type{};

	/**
	   @brief No type, indicates failure
	*/
	struct no_type{};

	/**
	   @brief Checks at compile time is specified memory manager class supports
				specified category
	   @details Invocation:\n
				is_category_supported<my_mgr, size_tracking_tag>::value\n
				or\n
				is_category_supported<my_mgr, size_tracking_tag>::result\n
	   @tparam MemMgr memory manager class that should be checked
	   @tparam Category category tag that should be checked
	*/
	template< class MemMgr, class Category >
	struct is_category_supported
	{
		/**
		   @brief Stores the result of check
		   @details if memory manager supports specified category 
					then value will be equal to 'true'
					otherwise it will be equal to 'false'
		*/
		enum {value = type_manip::super_subclass
			< 
				Category,
				typename manager_traits<MemMgr>::manager_category			 
			>::value};

		/**
		   @brief Stores the result of check
		   @details if memory manager supports specified category 
					then result will be yes_type
					otherwise it will be no_type
		*/
		typedef typename type_manip::select
			< 
				value, 
				yes_type,
				no_type
			>::result result;
	};
}


#endif //MGR_MANAGER_CATEGORY_HEADER
