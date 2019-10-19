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

#ifndef MGR_SCOPED_PTR_HEADER
#define MGR_SCOPED_PTR_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <stdexcept>

namespace memory_mgr
{
	namespace detail
	{
		template<typename T>
		struct ptr_deleter
		{
			static void do_delete( const T* const ptr)
			{
				delete ptr;
			}
		};

		template<typename T>
		struct array_deleter
		{
			static void do_delete( const T* const ptr)
			{
				delete[] ptr;
			}
		};


		template<typename T, template <class> class Deleter = detail::ptr_deleter>
		class scoped_ptr
		{
			scoped_ptr( const scoped_ptr& );
			scoped_ptr& operator=( const scoped_ptr& );
		public:
			typedef T				elem_t;
			typedef elem_t*			pointer_t;
			typedef const pointer_t const_pointer_t;

			typedef elem_t&			reference_t;
			typedef const elem_t&	const_reference_t;

			typedef Deleter<T>		deleter_t;

			scoped_ptr( pointer_t ptr = NULL )
				:m_ptr( ptr )
			{
			}

			void own( pointer_t ptr )
			{
				if( m_ptr )
				{
					throw std::logic_error( "scoped_ptr already owns an object" );
				}

				if( !ptr )
				{
					throw std::logic_error( "scoped_ptr unable to own NULL pointer" );
				}
				m_ptr = ptr;
			}

			pointer_t operator->()
			{
				return m_ptr;
			}

			const_pointer_t operator->() const
			{
				return m_ptr;
			}

			reference_t operator*()
			{
				assert( m_ptr != NULL && "Dereferencing NULL pointer" );
				return *m_ptr;
			}

			const_reference_t operator*() const
			{
				assert( m_ptr != NULL && "Dereferencing NULL pointer" );
				return *m_ptr;
			}

			~scoped_ptr()
			{
				cleanup();
			}
		private:
			void cleanup()
			{
				deleter_t::do_delete( m_ptr );
			}
			pointer_t m_ptr;
		};

	}
}

#endif// MGR_SCOPED_PTR_HEADER
