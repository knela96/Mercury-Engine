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

#ifndef MGR_LOCKS_HEADER
#define MGR_LOCKS_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

namespace memory_mgr
{
	/**
	   @brief Synchronization principles
	*/
	namespace sync
	{
		//-------------------------------------
		// Locks do nothing
		template<class SyncObjT>
		class pseudo_lockable
		{  
		public:
			pseudo_lockable()
			{}

			class lock
			{
			public:
				lock() 
				{};

				lock( const pseudo_lockable &c ) 
				{};
				~lock()
				{};

			private:
				lock(const lock &c);
				lock& operator=(const lock &c);
			};

		private:
			pseudo_lockable( const pseudo_lockable& );
			pseudo_lockable& operator=( const pseudo_lockable& );
		};

		//-------------------------------------
		// Object level blocking
		template<class SyncObjT>
		class object_level_lockable
		{  
		public:
			friend class lock;

			object_level_lockable()
			{}

			class lock
			{
			public:
				lock( const object_level_lockable &c ) 
					: m_lackable(c)
				{ 
					m_lackable.m_syncObj.Enter();
				};

				~lock()
				{ 
					m_lackable.m_syncObj.Leave();
				};

			private:
				object_level_lockable const &m_lackable;

				lock(const lock &c);
				lock& operator=(const lock &c);
			};
		private:
			mutable SyncObjT m_syncObj;
			object_level_lockable( const object_level_lockable& );
			object_level_lockable& operator=( const object_level_lockable& );
		};
		//-------------------------------------

		template <class SyncObjT>
		class class_level_lockable
		{
			static SyncObjT sm_syncObj;

		public:
			//class lock;
			friend class lock;

			class lock
			{
				lock(const lock&);
				lock& operator=(const lock&);
			public:
				lock()
				{
					sm_syncObj.Enter();
				}

				~lock()
				{
					sm_syncObj.Leave();
				}
			};
		};

		template <class SyncObjT>
		SyncObjT class_level_lockable<SyncObjT>::sm_syncObj;

	}//sync

}//memory_mgr


#endif// MGR_LOCKS_HEADER
