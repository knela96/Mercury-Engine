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

#ifndef MGR_WIN32_HEADER
#define MGR_WIN32_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#define MGR_WINDOWS_PLATFORM
#include <windows.h>
#include <string>
#include "detail/ptr_helpers.h"
#include "detail/types.h"
#include "detail/temp_buffer.h"

namespace memory_mgr
{
	template<class T>
	ulong ulong_cast( T val )
	{
		return static_cast<ulong>( val );
	}

	namespace osapi
	{
		typedef CRITICAL_SECTION critical_section;

		typedef void*			file_handle_t;
		typedef file_handle_t	mapping_handle_t;
		static const mapping_handle_t invalid_mapping_handle = 0;
		static void* invalid_mapping_address = 0;

		static inline void initialize_critical_section( critical_section* cs )
		{
			return InitializeCriticalSection( cs );
		}

		static inline void delete_critical_section( critical_section* cs )
		{
			return DeleteCriticalSection( cs );
		}

		static inline void enter_critical_section( critical_section* cs )
		{
			return EnterCriticalSection( cs );
		}

		static inline void leave_critical_section( critical_section* cs )
		{
			return LeaveCriticalSection( cs );
		}

		static inline file_handle_t create_file_mapping( const std::string& name, LPSECURITY_ATTRIBUTES file_mapping_attributes, ulong access, ulong low_size, ulong high_size = 0, file_handle_t hFile = INVALID_HANDLE_VALUE )
		{
			return CreateFileMappingA( hFile, file_mapping_attributes, access, high_size, low_size, name.c_str() );
		}

		static inline file_handle_t open_file_mapping( ulong access, std::string& name )
		{
			return OpenFileMappingA( access, false, name.c_str() );
		}

		static inline void* map_view_of_file(mapping_handle_t handle, ulong file_access, std::size_t numbytes, ulong highoffset = 0, ulong lowoffset = 0, void *base_addr = 0 )
		{  
			return MapViewOfFileEx(handle, file_access, highoffset, lowoffset, numbytes, base_addr);
		}

		static inline int unmap_view_of_file(void* address)
		{
			return UnmapViewOfFile(address);
		}

		static inline int close_handle(file_handle_t handle)
		{
			return CloseHandle(handle);
		}

		static inline ulong get_last_error()
		{			
			return GetLastError();
		}

		static inline std::string get_executable_path()
		{			
			detail::char_buffer path( 512 );
			while( GetModuleFileNameA( 0, path, ulong_cast( path.count() ) ) == path.count() )
			{
				path.reallocate( path.count() * 2 );
			}	

			return path.get();
		}
	}
}

#endif //MGR_WIN32_HEADER
