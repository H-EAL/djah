#ifndef DJAH_UTILS_HIERARCHY_GENERATOR_HPP
#define DJAH_UTILS_HIERARCHY_GENERATOR_HPP

#include "typelist.hpp"

namespace djah { namespace utils {

	//----------------------------------------------------------------------------------------------
	// Class definition
	//----------------------------------------------------------------------------------------------
	template<typename TList, template<typename> class Unit>
	class gen_scatter_hierarchy;
	//----------------------------------------------------------------------------------------------
	// Generates hierarchy nodes
	//----------------------------------------------------------------------------------------------
	template<typename T1, typename T2, template<typename> class Unit>
	class gen_scatter_hierarchy<typelist<T1,T2>, Unit>
		: public gen_scatter_hierarchy<T1, Unit>
		, public gen_scatter_hierarchy<T2, Unit>
	{
	public:
		typedef typelist<T1,T2> TypeList;
		typedef gen_scatter_hierarchy<T1, Unit> LeftBase;
		typedef gen_scatter_hierarchy<T2, Unit> RightBase;
	};
	//----------------------------------------------------------------------------------------------
	// Generates hierarchy leaves
	//----------------------------------------------------------------------------------------------
	template<typename AtomicType, template<typename> class Unit>
	class gen_scatter_hierarchy : public Unit<AtomicType>
	{
	public:
		typedef Unit<AtomicType> LeftBase;
	};
	//----------------------------------------------------------------------------------------------
	// Stop hierarchy generation
	//----------------------------------------------------------------------------------------------
	template<template<typename> class Unit>
	class gen_scatter_hierarchy<nulltype, Unit>
	{
	};
	//----------------------------------------------------------------------------------------------


} /*utils*/ } /*djah*/

#endif /* DJAH_UTILS_HIERARCHY_GENERATOR_HPP */