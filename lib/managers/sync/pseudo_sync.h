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

#ifndef MGR_PSEUDO_SYNC_HEADER
#define MGR_PSEUDO_SYNC_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif


namespace memory_mgr
{
	namespace sync
	{
		class pseudo_sync
		{
		public:
			pseudo_sync()	{}
			~pseudo_sync()	{}
			void Enter() const	{}
			void Leave() const	{}
		private:
			pseudo_sync(const pseudo_sync &);
			pseudo_sync & operator=(const pseudo_sync &);
		};

	}//sync

}//memory_mgr


#endif// MGR_PSEUDO_SYNC_HEADER