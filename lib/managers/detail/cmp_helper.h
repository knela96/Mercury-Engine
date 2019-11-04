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

#ifndef MGR_CMP_HELPER_HEADER
#define MGR_CMP_HELPER_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

namespace memory_mgr
{
	namespace detail
	{
		template< class ClassToHelp >
		class cmp_helper
		{
			typedef ClassToHelp op_type;
			inline const op_type& get_lhs() const
			{
				return static_cast<const op_type&>( *this );
			}
		public:
			bool operator!=( const op_type& rhs ) const
			{
				return !( get_lhs() == rhs);
			}

			bool operator<=( const op_type& rhs ) const
			{
				return !(get_lhs() > rhs);
			}

			bool operator>=( const op_type& rhs ) const
			{
				return !(get_lhs() < rhs);
			}
		};
	}//namespace detail

}//namespace memory_mgr


#endif// MGR_CMP_HELPER_HEADER
