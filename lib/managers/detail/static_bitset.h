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

#ifndef MGR_STATIC_BITSET_HEADER
#define MGR_STATIC_BITSET_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <limits>
#include <ostream>
#include <assert.h>
#include "static_assert.h"
#include "helpers.h"
#include "type_manip.h"

namespace memory_mgr
{
	enum array_type{ static_array, dynamic_array, custom_array };
	namespace detail
	{
		template<size_t BitsPerBlock>
		static inline size_t block_index(size_t pos) 
		{ 
			return pos / BitsPerBlock;
		}

		//Structure used to calculate number of blocks
		//required to store requested number of bits
		template<size_t BitsCount, size_t BitsPerBlock>
		struct calc_num_blocks
		{
			enum {result = (BitsCount / BitsPerBlock) + (BitsCount % BitsPerBlock ? 1 : 0)};
		};

		//Structure used to calculate bit index
		//in block
		template< size_t Pos, size_t BitsPerBlock >
		struct bit_index
		{
			enum{ result = Pos % BitsPerBlock };
		};

		//Structure used to calculate number of extra bits in block
		template<size_t BitsCount, size_t BitsPerBlock>
		struct extra_bits
		{
			enum { result = bit_index< BitsCount, BitsPerBlock >::result };
		};

		//Set strategy
		struct set_op 
		{
			template<class T>
			static void update( T& lhs, const T rhs )
			{
				lhs |= rhs;
			}
		};

		//Reset strategy
		struct reset_op 
		{
			template<class T>
			static void update( T& lhs, const T rhs )
			{
				lhs &= ~rhs;
			}
		};

		template< class BlockType, class ExtraBits >
		struct do_zero_unused_bits		
		{
			void operator()( BlockType& highest_block )
			{
				typedef BlockType block_type;
				enum {highest_bits_mask = ~(~block_type(0) << ExtraBits::value ) };
				highest_block &= highest_bits_mask;
			}
		};

 		template< class BlockType >
		struct do_zero_unused_bits< BlockType, ::memory_mgr::type_manip::int2type<0> > 
 		{
			void operator()( BlockType& )
			{}
		};

		//Type traits for all array types
		template< class BlockType, int BitsCount >
		struct array_traits
		{
			typedef BlockType		block_type;
			typedef block_type&		block_ref_type;
			typedef const block_type&	const_block_ref_type;

			
			typedef block_type*		block_ptr_type;
			typedef const block_type*	const_block_ptr_type;

			/**
			   @brief compile time computed constants
			*/
			enum
			{			
				bits_per_block	= std::numeric_limits<block_type>::digits,
				num_bits		= BitsCount,
				num_blocks		= detail::calc_num_blocks<num_bits, bits_per_block>::result,
				max_bits		= num_blocks * bits_per_block,
				memory_usage	= sizeof(block_type) * num_blocks
			};
		};

		//Basic array structure for array specializations 
		template< class BlockType, size_t BitsCount, array_type Type >
		struct array{};

		//Array specializations for static array
		template< class BlockType, size_t BitsCount >
		struct array< BlockType, BitsCount, static_array> : public array_traits<BlockType, BitsCount>
		{		
			typedef array_traits<BlockType, BitsCount> 		array_traits;
			typedef typename array_traits::block_type		block_type;
			typedef typename array_traits::block_ref_type		block_ref_type;
			typedef typename array_traits::const_block_ref_type	const_block_ref_type;

			typedef typename array_traits::block_ptr_type		block_ptr_type;
			typedef typename array_traits::const_block_ptr_type	const_block_ptr_type;
				
			/**
			   @brief compile time computed constants
			*/
			enum
			{			
				bits_per_block	=	array_traits::bits_per_block,
				num_bits		=	array_traits::num_bits,
				num_blocks		=	array_traits::num_blocks,
				max_bits		=	array_traits::max_bits,
				memory_usage	=	array_traits::memory_usage
			};
			
			block_type m_bits[num_blocks];
		};

		//Array specializations for dynamic array
		template< class BlockType, size_t BitsCount >
		struct array< BlockType, BitsCount, dynamic_array> : public array_traits<BlockType, BitsCount>
		{		
			typedef array_traits<BlockType, BitsCount> 		array_traits;
			typedef typename array_traits::block_type		block_type;
			typedef typename array_traits::block_ref_type		block_ref_type;
			typedef typename array_traits::const_block_ref_type	const_block_ref_type;

			typedef typename array_traits::block_ptr_type		block_ptr_type;
			typedef typename array_traits::const_block_ptr_type	const_block_ptr_type;

			/**
			   @brief compile time computed constants
			*/
			enum
			{			
				bits_per_block	=	array_traits::bits_per_block,
				num_bits	=	array_traits::num_bits,
				num_blocks	=	array_traits::num_blocks,
				max_bits	=	array_traits::max_bits,
				memory_usage	=	array_traits::memory_usage
			};
			
			array()
				:m_bits( 0 )
			{ this->m_bits = new block_type[num_blocks]; }

			~array()
			{ delete[] this->m_bits; }

			block_type* m_bits;
		};

		//Array specializations for custom array
		template< class BlockType, size_t BitsCount >
		struct array< BlockType, BitsCount, custom_array> : public array_traits<BlockType, BitsCount>
		{		
			typedef array_traits<BlockType, BitsCount> 		array_traits;
			typedef typename array_traits::block_type		block_type;
			typedef typename array_traits::block_ref_type		block_ref_type;
			typedef typename array_traits::const_block_ref_type	const_block_ref_type;

			typedef typename array_traits::block_ptr_type		block_ptr_type;
			typedef typename array_traits::const_block_ptr_type	const_block_ptr_type;
			
			/**
			   @brief compile time computed constants
			*/
			enum {			
				bits_per_block	=	array_traits::bits_per_block,
				num_bits		=	array_traits::num_bits,
				num_blocks		=	array_traits::num_blocks,
				max_bits		=	array_traits::max_bits,
				memory_usage	=	array_traits::memory_usage
			};
			
			array( block_ptr_type arr_ptr )
				:m_bits( arr_ptr )
			{}

			block_type* m_bits;		
		};

	}

	template< class BlockType, size_t BitsCount, array_type StaticArr = static_array >
	class static_bitset: public detail::array< BlockType, BitsCount, StaticArr >
	{		
		static_bitset( const static_bitset& );
		static_bitset& operator=( const static_bitset& );
	public:	
		typedef static_bitset	self_type;		
		typedef self_type&	self_ref_type;

		typedef detail::array< BlockType, BitsCount, StaticArr >	base_type;

		/**
		   @brief memory block type
		*/
		typedef typename base_type::block_type			block_type;
		
		/**
		   @brief memory block reference type
		*/
		typedef typename base_type::block_ref_type		block_ref_type;

		/**
		   @brief memory block const reference type
		*/
		typedef typename base_type::const_block_ref_type	const_block_ref_type;
		
		/**
		   @brief memory block pointer type
		*/
		typedef typename base_type::block_ptr_type		block_ptr_type;

		/**
		   @brief memory block const pointer type
		*/
		typedef typename base_type::const_block_ptr_type	const_block_ptr_type;

		/**
		   @brief Type used to store size, commonly std::size_t
		*/
		typedef size_t		size_type;
		typedef size_type	block_width_type;

		/**
		   @brief invalid index value (null pos)
		*/
		const static size_type npos = ~size_type(0);

		/**
		   @brief compile time computed constants
		*/
		enum
		{			
			bits_per_block	=	base_type::bits_per_block,
			num_bits		=	base_type::num_bits,
			num_blocks		=	base_type::num_blocks,
			max_bits		=	base_type::max_bits,
			memory_usage	=	base_type::memory_usage
		};
		
		//Default constructor
		//Resets all bits
		static_bitset()
		{ 
			STATIC_ASSERT( num_bits != 0, Bitset_cant_be_empty );
		}

		//Constructor used only by custom arrays to initialize
		//array's pointer
		explicit static_bitset( block_ptr_type bits_ptr )
			:base_type(bits_ptr)
		{
			STATIC_ASSERT( num_bits != 0, Bitset_cant_be_empty );
		}

		~static_bitset()
		{}

		size_type size() const
		{ return num_bits; }

		bool test( size_type pos ) const
		{
			assert(pos < num_bits);
			return unchecked_test(pos);
		}

		bool test( size_type pos, size_type count ) const
		{
			assert(pos < num_bits);
			assert(count > 0);

			size_type blk = block_index( pos );
			size_type ind = bit_index( pos );
			ptrdiff_t left = count;
			while( left > 0 )
			{
				if ( contains_bits(blk, ind, left) )
				{
					++blk;
					left -= bits_per_block - ind;
					ind = 0;
				}
				else
				{
					return false;
				}
			}
			return true;
		}
		
		self_ref_type set()
		{
			std::fill(this->m_bits, this->m_bits + num_blocks, ~block_type(0));
			zero_unused_bits();
			return *this;
		}

		self_ref_type set( size_type pos)
		{
			assert(pos < num_bits);

			this->m_bits[block_index(pos)] |= bit_mask(pos);
			return *this;
		}


		self_ref_type set( size_type pos, size_type count )
		{			
			return do_set<detail::set_op>( pos, count );
		}

		self_ref_type reset(size_type pos)
		{
			assert(pos < num_bits);
			this->m_bits[block_index(pos)] &= ~bit_mask(pos);
			return *this;
		}

		self_ref_type reset( size_type pos, size_type count )
		{			
			return do_set<detail::reset_op>( pos, count );
		}

		self_ref_type reset()
		{
			std::fill(this->m_bits, this->m_bits + num_blocks, block_type(0));
			return *this;
		}

		self_ref_type flip(size_type pos)
		{
			assert(pos < num_bits);
			this->m_bits[block_index(pos)] ^= bit_mask(pos);
			return *this;
		}

		self_ref_type flip()
		{
			for (size_type i = 0; i < num_blocks; ++i)
			{
				this->m_bits[i] = ~this->m_bits[i];
			}
			zero_unused_bits();
			return *this;
		}


		size_type find_n( size_type count,  size_type first_block = 0 )
		{
			if( count == 0 )
			{
				return npos;
			}
			size_type lowest_bit = do_find_from(first_block);

			size_type blk = first_block;
			size_type ind = lowest_bit;
			ptrdiff_t left = count;
			while( (left > 0) && (lowest_bit != npos) )
			{
				if ( contains_bits(blk, ind, left) )
				{
					++blk;
					left -= bits_per_block - ind;
					ind = 0;
				}
				else
				{
					lowest_bit = ind = do_find_next( ind + 1, blk );
					left = count;
					first_block = blk;
				}
			}
			
			return (lowest_bit != npos) ? first_block * bits_per_block + lowest_bit : npos;
		}

		size_type find_first() const
		{
			size_type blk_index = 0;
			return blk_index * bits_per_block + do_find_from( blk_index );
		}

		size_type find_next( size_type pos ) const
		{			
			++pos;

			size_type blk_index = block_index(pos);
			return blk_index * bits_per_block + do_find_next( bit_index(pos), blk_index );
		}
		
		//Return true if all bits are set to 0
		bool empty()
		{
			//Is there at least one block with bit set to 1
			return find_first_block(0) >= num_blocks;
		}

	private:
		static inline size_type block_index(size_type pos) 
		{ return pos / bits_per_block; }
		
		static inline block_width_type bit_index(size_type pos) 
		{ return pos % bits_per_block; }

		static inline block_type bit_mask(size_type pos) 
		{ return static_cast<block_type>( block_type(1) << bit_index(pos) ); }
		
		static inline block_type bit_mask(size_type pos, size_type count) 
		{ 
			count = (count % bits_per_block) < count ? -1 : count;
			return block_type( ((block_type(1) << count ) - 1) << bit_index(pos) ); 
		}

		static inline block_type higher_bit_mask(size_type pos) 
		{ return block_type( ~block_type(0) << bit_index(pos) ); }

		static inline block_type lower_bit_mask(size_type pos) 
		{ return ~higher_bit_mask(pos); }

		static inline size_type blocks_count( size_type bits_count ) 
		{ return block_index( bits_count ); }

		bool unchecked_test(size_type pos) const
		{ return (this->m_bits[block_index(pos)] & bit_mask(pos)) != 0; }
		

		//Find first block with at list one bit set to 1
		size_type find_first_block( size_type from ) const
		{
			// skip null blocks
			while (from < num_blocks && this->m_bits[from] == 0)
			{
				++from;
			}
			return from;
		}

		size_type do_find_from( size_type& first_block ) const
		{
			first_block = find_first_block( first_block );

			if (first_block >= num_blocks)
			{
				return npos; // not found
			}

			return helpers::lowest_bit(this->m_bits[first_block]);
		}

		size_type do_find_next( size_type bit_ind, size_type& blk_ind ) const
		{
			if ( bit_ind >= num_bits )
			{
				return npos;
			}
			const block_type fore = this->m_bits[blk_ind] & ( higher_bit_mask(bit_ind) );

			return fore ? helpers::lowest_bit(fore):
				do_find_from( ++blk_ind );
		}

		bool contains_bits( size_type block, size_type first, size_type count ) const
		{
			const block_type mask = bit_mask( first, count );
			return (this->m_bits[block] &  mask) == mask;
		}

		// If size() is not a multiple of bits_per_block
		// then not all the bits in the last block are used.
		// This function resets the unused bits (convenient
		// for the implementation of many member functions)
		//
		void zero_unused_bits()
		{
			// if != 0 this is the number of bits used in the last block
			detail::do_zero_unused_bits< block_type, type_manip::int2type< detail::extra_bits<num_bits, bits_per_block>::result > >()( highest_block() );
		}		

		// gives a reference to the highest block
		//
		block_ref_type highest_block()
		{
			return this->m_bits[num_blocks - 1];
		}

		// gives a const-reference to the highest block
		//
		const_block_ref_type highest_block() const
		{
			return this->m_bits[num_blocks - 1];
		}

		template< class set_op >
		self_ref_type do_set( size_type pos, size_type count )
		{
			assert(pos < num_bits);
			count = (count != npos ? count : num_bits - pos); 

			assert( (pos + count) <= num_bits );

			size_type block_ind = block_index(pos);
			const size_type block_end	= block_index(pos + count);
			const size_type last_index = bit_index(pos + count);


			size_type bits_fill_count = count;
			do
			{
				set_op::update(this->m_bits[block_ind++], bit_mask(pos, bits_fill_count) );
				pos = 0;
				bits_fill_count = (block_ind != block_end) ? bits_per_block : last_index;
			}
			while (block_ind <= block_end);

			return *this;
		};
	};

	template <class Ch, class Tr, class BlockType, size_t BitsCount, array_type Type >
	std::basic_ostream<Ch, Tr>& operator<<( std::basic_ostream<Ch, Tr>& ostr, const static_bitset<BlockType, BitsCount, Type>& b )
	{
		typedef Ch char_type;
		char_type one = '1';
		char_type zero = '0';
		char_type space = ' ';

		typedef static_bitset<BlockType, BitsCount, Type> bitset_type;
		typedef typename bitset_type::size_type size_type;
		
		for (size_type i = bitset_type::num_bits - 1; ( i != bitset_type::npos ) && ostr; --i) 
		{
			ostr << (b.test(i)? one : zero);
			if ( (i % 8) == 0 )
			{
				ostr << space;
			}
			
		}
		return ostr;
	}
}


#endif// MGR_STATIC_BITSET_HEADER
