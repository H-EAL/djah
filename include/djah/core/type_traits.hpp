#ifndef DJAH_UTILS_TYPE_TRAITS_HPP
#define DJAH_UTILS_TYPE_TRAITS_HPP

namespace djah { namespace utils {
	
	//----------------------------------------------------------------------------------------------
	template<typename T>
	struct type_traits
	{
	private:
		
		template<typename T> struct base_type			{ typedef T BaseType; };
		template<typename T> struct base_type<T*>		{ typedef T BaseType; };
		template<typename T> struct base_type<T&>		{ typedef T BaseType; };
		template<typename T> struct base_type<const T>	{ typedef T BaseType; };
		template<typename T> struct base_type<const T&> { typedef T BaseType; };
		template<typename T> struct base_type<const T*>	{ typedef T BaseType; };

	public:

		typedef typename base_type<T>::BaseType BaseType;

		typedef typename BaseType&				ReferenceType;
		typedef typename const BaseType&		ConstRefType;
		typedef typename BaseType*				PointerType;
		typedef typename const BaseType*		ConstPtrType;
	};
	//----------------------------------------------------------------------------------------------

	
	//----------------------------------------------------------------------------------------------
	// Determine whether two types are equal
	//----------------------------------------------------------------------------------------------
	template<typename T, typename U>	struct same_type		{ enum { result = false }; };
	template<typename T>				struct same_type<T,T>	{ enum { result = true  }; };
	//----------------------------------------------------------------------------------------------

} /*utils*/ } /*djah*/

#endif /* DJAH_UTILS_TYPE_TRAITS */