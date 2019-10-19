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

#ifndef MGR_SINGLETON_MANAGER_HEADER
#define MGR_SINGLETON_MANAGER_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include "detail/singleton.h"
#include "manager_traits.h"
#include "new.h"
#include "manager_category.h"

namespace memory_mgr
{	
	//Forward declaration
	template
	< 
		class Mgr, 
		class SyncObj,
		template <class> class ThreadingModel
	>
	class  singleton_manager;

	//Forward declaration
	template 
	<
		class Mgr,
		class SyncObj, 
		template <class> class ThreadingModel
	>
	struct manager_traits< singleton_manager< Mgr, SyncObj, ThreadingModel > >;

	/**
	   @brief Singleton decorator for memory manager. Implements SingletonManagerConcept
	   @details Transforms memory manager to singleton.\n
				Adds instance() static method to manager and locks it's constructor
	   @tparam MemMgr  memory_manager class, must have default constructor.
					must support MemoryManagerConcept 
	   @tparam SyncObj	Synchronization object that will be used to
						synchronize instance creation
	   @tparam ThreadingModel synchronization model policy, one of class_level_locable or pseudo_lockable policies
	
	*/
	template 
	<
		class Mgr,
		class SyncObj = sync::critical_section, 
		template <class> class ThreadingModel = sync::class_level_lockable
	>
	class  singleton_manager : public singleton< /*typename manager_traits<*/Mgr/*>::manager_type*/, Mgr, SyncObj, ThreadingModel >
	{
	private:
		/**
		   @brief Private constructor, to prevent creation of instances of this class
		*/
		singleton_manager();

		/**
		   @brief Private destructor, to prevent creation of instances of this class
		*/
		~singleton_manager();

		/**
		   @brief Private copy constructor, to prevent creation of instances of this class
		*/
		singleton_manager(const singleton_manager&);

		/**
		   @brief Private copy operator, to prevent copying of instances of this class
		*/
		singleton_manager& operator=(const singleton_manager&);
	};

	/**
	   @brief memory_manager + singleton_manager traits
	   @details Adds singleton_manager_tag to manager_category
	*/
	template 
	<
		class MemMgr,
		class SyncObj, 
		template <class> class ThreadingModel
	>
	struct manager_traits< singleton_manager< MemMgr, SyncObj, ThreadingModel > > 
		: public manager_traits< MemMgr >
	{
		/**
		   @brief Memory manager class, that was decorated
		*/
		typedef MemMgr base_manager_type;

		/**
		   @brief Adds singleton_manager_tag to manager_category
		*/
		struct manager_category 
			: public virtual manager_traits<MemMgr>::manager_category,
			public virtual singleton_manager_tag
		{};
	};

	/**
	   @brief Helper function for global operator new overloads
	   @details used to pass memory manager object to operator new	   
	   @exception newer throws
	   @return helper object that should be passed as parameter to operator new
	*/	
	template 
	<
		class SingletonManager
	>
	static inline detail::mem_mgr_helper< typename manager_traits< SingletonManager >::base_manager_type > mem_mgr()
	{
		typedef SingletonManager mgr_type;
		typedef is_category_supported< mgr_type, singleton_manager_tag > singleton_support;
		STATIC_ASSERT( singleton_support::value, Memeory_manager_must_implement_singleton_concept );
		return detail::mem_mgr_helper< typename manager_traits< mgr_type >::base_manager_type >( mgr_type::instance() );
	};
}

using memory_mgr::mem_mgr;

#endif// MGR_SINGLETON_MANAGER_HEADER
