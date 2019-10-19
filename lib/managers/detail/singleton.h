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

#ifndef MGR_SINGLETON_HEADER
#define MGR_SINGLETON_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include "sync/locks.h"
#include "sync/critical_section.h"

namespace memory_mgr
{
	/**
	   @brief Policy based implementation of a Singleton design
	   pattern  
	   @tparam T reference to this type will be returned by instance() method
	   @tparam InstT instance of this type will be created, and returned by instance method
	   @tparam SyncObj	Synchronization object that will be used to
						synchronize instance creation
	   @tparam ThreadingModel synchronization model policy, one of class_level_locable or pseudo_lockable policies
	*/
	template 
	<
		class T,
		class InstT = T,
		class SyncObj = sync::critical_section, 
		template <class> class ThreadingModel = sync::class_level_lockable
	>
	class  singleton : private ThreadingModel< SyncObj >
	{
		/**
		   @brief Scoped lock type
		*/
		typedef typename ThreadingModel<SyncObj>::lock lock;
	public:
		/**
		   @brief Call this method to get instance of singleton class, creates
		   a new instance of the class if one does not exist		   
		   
		   @return Reference to instance of singleton class                   
		*/
		static T& instance()
		{
			if ( !m_instance )
			{
				lock l;
				if ( !m_instance )
				{
					m_instance = new InstT();
					atexit( destruct );
				}
			}
			return *m_instance;
		}


		/**
		   @brief Call this method to destroy singleton instance and
		   free associated resources.                               
		*/
		static void destruct()
		{
			if ( m_instance )
			{
				lock l;
				if ( m_instance )
				{
					delete m_instance;
					m_instance = NULL;
				}
			}
		}

	protected:
		/**
		   @brief Protected default constructor, to prevent outside sources
		   from creating or destroying a singleton instance.
		*/
		singleton(){}

		/**
		   @brief Protected destructor, to prevent outside sources from
		   creating or destroying a singleton instance.                
		*/
		~singleton(){}
	private:			
		/**
		   @brief Pointer to singleton instance
		*/
		static InstT* m_instance;
		
		/**
		   @brief Copy protection
		*/
		singleton & operator=(const singleton &);
	};
	//------------------------------------------------------

	template 
	<
		class T,
		class InstT,
		class SyncObj,
		template <class> class ThreadingModel
	>
	InstT* singleton<T, InstT, SyncObj, ThreadingModel>::m_instance = NULL;

}


#endif// MGR_SINGLETON_HEADER
