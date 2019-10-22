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

#ifndef MGR_CONFIG_HEADER
#define MGR_CONFIG_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#if defined(linux) || defined(__linux) || defined(__linux__)
// linux:
#  define MGR_PLATFORM_CONFIG "../config/platform/linux.h"
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#	define MGR_PLATFORM_CONFIG "../config/platform/win32.h"
#else 
#	error "Unsupported platform. In future all the platforms will be supported."
#endif

#include MGR_PLATFORM_CONFIG

#include "detail/helpers.h"

namespace memory_mgr
{
	/**
	   @brief OS specific API wrappers
	*/
	namespace osapi
	{
		/**
		   @brief Call this function to get path to folder from which executable was launched
		   @return path to folder from which executable was launched
		*/
		static inline std::string get_exe_dir()
		{
			return helpers::get_parent_dir( get_executable_path() );
		}
	}
}

#endif// MGR_CONFIG_HEADER
