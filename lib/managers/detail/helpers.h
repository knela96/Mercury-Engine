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

#ifndef MGR_HELPERS_HEADER
#define MGR_HELPERS_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif 

#include <assert.h>

namespace memory_mgr
{
	/**
	@brief Helper classes
	*/
	namespace helpers 
	{
		/**
		@brief Helper classes implementation details
		*/
		namespace detail 
		{
			template <typename T>
			int integer_log2_impl(T x, int n) 
			{
				int result = 0;
				while (x != 1) 
				{
					const T t = static_cast< const T >( x >> n );
					if (t) {
						result += n;
						x = t;
					}
					n /= 2;
				}
				return result;
			}

			// helper to find the maximum power of two
			// less than p (more involved than necessary,
			// to avoid PTS)
			//
			template <int p, int n>
			struct max_pow2_less 
			{
				enum { c = 2*n < p };
				enum { value = c ? (max_pow2_less< c*p, 2*c*n>::value) : n};
			};

			template <>
			struct max_pow2_less<0, 0> 
			{
				enum { value = 0};
			};


		} //helpers detail

		// ---------
		// integer_log2
		// ---------------
		//
		template <typename T>
		int integer_log2(T x) 
		{
			assert(x > 0); // PRE

			const int n = detail::max_pow2_less<std::numeric_limits<T>::digits, 4>::value;

			return detail::integer_log2_impl(x, n);

		}

		template <typename T>
		int lowest_bit(T x) 
		{
			assert(x >= 1); // PRE

			// clear all bits on except the rightmost one,
			// then calculate the logarithm base 2
			return integer_log2<T>( x - ( x & (x-1) ) );

		}

		static inline void throw_bad_alloc()
		{ throw std::bad_alloc(); }

		static inline void do_nothing()
		{}

		template<class StringT>
		static inline StringT& add_leading_slash( StringT& str )
		{
			if(str[0] != '/')
			{
				str.insert( str.begin(), '/');
			}
			return str;
		}

		template<class StringT>
		static inline StringT& add_trailing_slash( StringT& str )
		{
			if( str.length() >= 1 )
			{
				const typename StringT::traits_type::char_type last = *(str.end() - 1);
				if(last != '/' || last != '\\')
				{
					str.insert( str.end(), '/');
				}
			}
			return str;
		}

		template<class StringT> static inline StringT get_parent_dir( const StringT& file_path )
		{
			size_t pos = file_path.find_last_of( "\\/" );
			return file_path.substr( 0, (pos == -1 ? 0 : pos) );
		}
	}//helpers

}//memory_mgr


#endif// MGR_HELPERS_HEADER
