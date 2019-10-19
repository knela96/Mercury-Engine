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

#ifndef MGR_TEMP_BUFFER_HEADER
#define MGR_TEMP_BUFFER_HEADER

#include <assert.h>
#include <stdexcept>

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

namespace memory_mgr
{
	namespace detail
	{

		/**
		   @brief Exception class for use by temp_buffer<> class
		   @details This exception will be thrown by temp_buffer<> class 
		   if you will try to allocate new buffer while class owns another one
		*/
		class multiple_allocation: public std::runtime_error
		{
		public:
			multiple_allocation()
				:std::runtime_error("Multiple allocation is not allowed in temp_buffer.")
			{}
		};


		//Template class for temporary buffers
		template<typename ItemType>
		class temp_buffer
		{	
		public:
			typedef ItemType			item_type;
			typedef item_type*			buffer_type;
			typedef const item_type*	const_buffer_type;
			typedef size_t				size_type;
			typedef temp_buffer			self_type;
			typedef self_type&			self_ref_type;
			typedef const self_type&	const_self_ref_type;

			enum {
				item_size = sizeof( item_type ) /**< Size of buffer element in bytes*/
			};

			/**
			   @brief Default constructor, creates null size buffer
			   @exception newer throws
			*/
			temp_buffer()
				:m_buffer( 0 ), m_size( 0 )
			{}

			/**
			   @brief Constructor, allocates memory for buffer with 'count' elements
			  
			   @param	count	buffer size in elements	
			   @exception newer throws		 
			*/
			temp_buffer( size_type count )
				:m_buffer( 0 ), m_size( 0 )
			{
				this->allocate( count );
			}

			/**
			   @brief Copy constructor, creates independent buffer copy			 
			   @param	buf	buffer to copy from
			   @exception newer throws
			*/
			temp_buffer(const_self_ref_type buf)
				:m_buffer( 0 ), m_size( 0 )
			{
				*this = buf;
			}

			/**
			   @brief Destructor, frees buffer.
			   @exception newer throws
			*/
			~temp_buffer()
			{
				this->free_buffer();
			}

			/**
			   @brief Copy operator, creates independent buffer copy
			   @param	rhs	buffer to copy from
			   @exception newer throws
			   @return Reference to self
			*/
			inline self_ref_type operator=(const_self_ref_type rhs)
			{	
				this->put( rhs.m_buffer, rhs.m_size );
				return *this;
			}

			/**
			   @brief Implicit cast operator
			   @exception newer throws
			   @return Pointer to buffer 
			*/
			inline operator buffer_type()
			{
				return this->m_buffer;
			}

			/**
			   @brief Call this method to get pointer to buffer
			   @exception newer throws
			   @return Pointer to buffer
			*/
			inline buffer_type get()
			{
				return this->m_buffer;
			}

			/**
			   @brief Call this method to get buffer size
			   @exception newer throws
			   @return Buffer size in bytes
			*/
			inline size_type size()
			{
				return this->m_size * item_size;
			}

			/**
			   @brief Call this method to get items count
			   @exception newer throws
			   @return Buffer size in elements
			*/
			inline size_type count()
			{
				return this->m_size;
			}

			/**
			   @brief Call this method to put data from array with count items to buffer
			   @param	array	array to copy from
			   @param	count	number of items to copy
			   @exception newer throws
			   @return Pointer to new buffer
			*/
			inline buffer_type put( const_buffer_type array, size_type count )
			{
				if( array && count )
				{
					if( count > m_size )
					{
						reallocate( count );
					}
					zero_buffer();
					buffer_type tmp = m_buffer;
					while( count-- )
					{
						*tmp++ = *array++;
					}

					return m_buffer;
				}
				return 0;
			}

			/**
			  @brief Call this method to allocate memory buffer
			  @param	count	Size of buffer in elements, must be greater than NULL
			  @exception multiple_allocation if object already owns another memory buffer
			  @return Pointer to new buffer
			*/
			inline buffer_type allocate( size_type count )
			{	
				if(m_buffer) 
				{
					throw multiple_allocation();
				}

				assert( count != 0 && "Invalid buffer size!" );
				m_size = count;	

				m_buffer = new item_type[ m_size ];
				zero_buffer();
				return m_buffer;
			}

			/**
			   @brief Call this method to reallocate memory buffer
			   @param	count	Size of new buffer in elements, must be greater than NULL
			   @exception newer throws
			   @return Pointer to new buffer
			*/
			inline buffer_type reallocate( size_type count )
			{	
				if( count )
				{
					free_buffer();
					return allocate( count );
				}	

				return m_buffer;
			}

		private:

			/**
			   @brief Pointer to memory buffer
			*/
			buffer_type	m_buffer;
			/**
			   @brief Size of memory buffer in elements
			*/
			size_type m_size;

			/**
			   @brief Call this method to free memory buffer
			   @exception newer throws
			*/
			inline void free_buffer()
			{
				if( m_buffer )
				{
					delete[] m_buffer;
					m_buffer = NULL;
					m_size = 0;
				}
			}

			/**
			   @brief Call this method to zero fill buffer's memory
			   @exception newer throws 
			*/
			inline void zero_buffer()
			{
				std::fill( m_buffer, m_buffer + m_size, 0 );
			}
		};

		typedef temp_buffer<char> char_buffer;
		typedef temp_buffer<wchar_t> wchar_buffer;
	}
}
#endif//MGR_TEMP_BUFFER_HEADER

