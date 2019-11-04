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

#ifndef MGR_TYPE_MANIP_HEADER
#define MGR_TYPE_MANIP_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif



namespace memory_mgr
{
	/**
	   @brief Types manipulation sub-library
	   @details Thanks to Andrei Alexandrescu for the original version of the library
	*/
	namespace type_manip
	{
		/**
		   @brief Converts each integral constant into a unique type
		   @details Used for functions overriding based on integer
		   values
		   
		   Invocation: int2type\< v \> -  where v is a compile-time constant
		   integral
		   
		   Defines 'value', an enum that evaluates to v             
		*/
		template <int v>
		struct int2type
		{
			/**
			   @brief an enum that evaluates to v
			*/
			enum { value = v /**< evaluates to v*/};
		};

		/**
		  @brief Converts each type into a unique, insipid type
		  @details	Invocation type2type\< T \> where T is a type
					Defines the type original_type which maps back to T
		*/
		template <typename T>
		struct type2type
		{   
			/**
			   @brief maps back to template parameter T
			*/
			typedef T original_type;
			
			/**
			   @brief Default constructor, to get rid of VC7 warning
			*/
			type2type(){}
		};

		/**
		   @brief Types manipulation sub-library implementation details
		*/
		namespace detail
		{
			/**
			   @brief "select" implementation class, generic template, used
			   when flag==true                                             
			*/
			template<bool, typename T, typename U>
			struct select_impl 
			{ /**
			   @brief Select type T
			*/
			typedef T result; };

			/**
			   @brief "select" implementation class, template specialization
			   for flag==false
			                                                                
			*/
			template<typename T, typename U>
			struct select_impl<false, T, U>
			{ /**
			     @brief Select type U
			  */
			typedef U result; };
		}

		/**
		   @brief Selects one of two types based upon a boolean constant
		   @details Invocation: select::Result
		   @tparam flag  a compile\-time boolean constant
		   @tparam T     Type, selected if flag==true
		   @tparam U     Type, selected if flag==false                   
		*/
		template <bool flag, typename T, typename U>
		struct select
		{
			typedef typename detail::select_impl<flag, T, U>::result result;
		};


		namespace detail
		{
			/**
			   @brief "is_same_type" implementation class, generic template, used
			   when T and U different types 
			   @tparam T Type to check on equality
			   @tparam U Type to check on equality
			*/
			template<typename T, typename U>
			struct is_same_type_impl 
			{ enum { value = false }; };

			/**
			   @brief "is_same_type" implementation class, template specialization
			   for same types
			   @tparam U Type to check on equality (T is the same type)
			*/
			template<typename U>
			struct is_same_type_impl<U, U>
			{ enum { value = true }; };
		}

		/**
		   @brief Checks given types on equality
		   @details Invocation: is_same_type\<T, U\>::value
					
		   result evaluates to true if U == T (types equal) 
		   @tparam T Type to check on equality
		   @tparam U Type to check on equality
		*/
		template <typename T, typename U>
		struct is_same_type
		{
			enum { value /**< true if U == T, false otherwise*/ 
				= detail::is_same_type_impl<T, U>::value  };
		};

		namespace detail
		{
			/**
			   @brief Helper type. Used in pair with big_type
			   @details guarantee that sizeof(small_type) \<
			   sizeof(big_type)                              
			*/
			typedef char small_type;
			/**
			   @brief Helper type. Used in pair with small_type
			   @details guarantee that sizeof(small_type) \<
			   sizeof(big_type)                                
			*/
			class big_type { char dummy[2]; };

			/**
			   @brief Checks is type T a "void" type
			   @tparam T Type to check on equality to "void"
			*/
			template<typename T>
			struct is_void
			{
				enum { result = /**< result value, true if T is void, false otherwise*/
					is_same_type<T, void>::value          ||
					is_same_type<T, const void>::value    ||
					is_same_type<T, volatile void>::value ||
					is_same_type<T, const volatile void>::value
				};
			};
		}

		/**
		   @brief Checks is one type convertible to another
		   @details Checks is type From convertible to type To
		*/
		template <class From, class To>
		class is_convertible
		{
			/**
			   @brief Helper struct, used as replacement of void type passed as
			   template parameter, if any
			*/
			struct void_replace{};

			/**
			   @brief Validation of type From
			   @details This step is required, because sizeof(void) will cause compiler error
			*/
			typedef typename select
				<
				detail::is_void<From>::result,
				void_replace, From
				>
				::result from;

			/**
			   @brief Validation of type To
			   @details This step is required, because sizeof(void) will cause compiler error
			*/
			typedef typename select
				<
				detail::is_void<To>::result,
				void_replace, To
				>
				::result to;

			/**
			   @brief main function of conversion checking
			   @details "called" if type From is inconvertible to type To
			*/
			static detail::big_type   test(...);
			/**
			   @brief main function of conversion checking
			   @details "called" if type From is convertible to type To
			*/
			static detail::small_type test(to);

			/**
			   @brief Helper function used to create temporary object of type From
			   @return Temporary object of type From
			*/
			static from make_from();

		public:       
			enum { exists = sizeof( test( make_from() ) ) == sizeof( detail::small_type ) };
		};

		/**
		   @brief Checks is type T a class
		   @tparam T Type to check
		*/
		template <class T>
		class is_class
		{
			template<class U> static detail::big_type test(...);
			template<class U> static detail::small_type test(void(U::*)(void));			
		public:       
			enum { value = sizeof(test<T>(0)) == sizeof(detail::small_type) };
		};
		
		/**
		   @brief Figures out the conversion relationships between two types
		   @details Invocations (From and To are types):
		   
		   @li conversion<From, To>::exists
		   returns (at compile time) true if there is an implicit conversion from From
		   to To (example: Derived to Base)
		   @li conversion<From, To>::exists_2_way
		   returns (at compile time) true if there are both conversions from From
		   to To and from To to From (example: int to char and back)
		   @li conversion<From, To>::same_type
		   returns (at compile time) true if From and To represent the same type

		   @remarks Might not work if From and To are in a private inheritance hierarchy.
		*/
		template <class From, class To>
		struct conversion
		{
			enum { exists /**< true if there is an implicit conversion from From*/
				= (is_convertible<From,To>::exists) };
			enum { exists_2_way /**< true if there are both conversions from From
								to To and from To to From (example: int to char and back)*/
				= (exists && is_convertible<To, From>::exists) };
			enum { same_type /**< true if From and To represent the same type*/
				= (is_same_type<From, To>::value) };
		};

		/**
		   @brief Figures out relationships between two classes
		   @details super_subclass<B(ase), D(erived)>::value where B(ase) and D(erived) are types. 
		   @return true if B(ase) is a public base of D(erived), 
		   or if B(ase) and D(erived) are aliases of the same type.
		   @remarks Might not work if From and To are in a private inheritance hierarchy.
		*/
		template <class B/*Base*/, class D/*Derived*/>
		struct super_subclass
		{
			enum { value = (conversion<const volatile D*, const volatile B*>::exists &&
				!conversion<const volatile B*, const volatile void*>::same_type) };
		};

		/**
		   @brief Figures out strict relationships between two classes
		   @details super_subclass_strict<B(ase), D(erived)>::value where B(ase) and D(erived) are types. 
		   @return true if B(ase) is a public base of D(erived).

		   @remarks Might not work if From and To are in a private inheritance hierarchy.
		*/
		template <class B/*Base*/, class D/*Derived*/>
		struct super_subclass_strict
		{
			enum { value = (conversion<const volatile D*, const volatile B*>::exists &&
				!conversion<const volatile B*, const volatile void*>::same_type &&
				!conversion<const volatile B*, const volatile D*>::same_type) };
		};
	}
}

#endif //MGR_TYPE_MANIP_HEADER
