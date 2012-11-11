#ifndef DJAH_OPENGL_VERTEX_ATTRIB_HPP
#define DJAH_OPENGL_VERTEX_ATTRIB_HPP

#include <string>

#include "../types.hpp"
#include "gl_types.hpp"

namespace djah { namespace opengl {

	namespace format {

		//------------------------------------------------------------------------------------------
		class vertex_attrib_base
		{
		public:
			vertex_attrib_base(const std::string &_name, u32 _size, u32 _count, u32 _valueType, u32 _divisor)
				: name_(_name), size_(_size), count_(_count), valueType_(_valueType), divisor_(_divisor) {}

			// Read-only accessors
			const std::string& name()	const { return name_;		}
			u32 size()					const { return size_;       }
			u32 count()					const { return count_;      } 
			u32 valueType()				const { return valueType_;	}
			u32 divisor()				const { return divisor_;	}

		protected:
			// Attribute name
			std::string name_;
			// Size of an element
			u32 size_;
			// Number of elements
			u32 count_;
			// Elements' type
			u32 valueType_;
			// Divisor
			u32 divisor_;
		};
		//------------------------------------------------------------------------------------------
		// Comparison
		inline bool operator ==(const vertex_attrib_base &lhs, const vertex_attrib_base &rhs)
		{
			return 
			(
				   lhs.name()      == rhs.name()
				&& lhs.size()      == rhs.size()
				&& lhs.count()     == rhs.count()
				&& lhs.valueType() == rhs.valueType()
				&& lhs.divisor()   == rhs.divisor()
			);
		}
		//------------------------------------------------------------------------------------------


		//------------------------------------------------------------------------------------------
		template<u32 Count, typename T>
		struct vertex_attrib : public vertex_attrib_base
		{
			vertex_attrib(const std::string &name)
				: vertex_attrib_base(name, sizeof(T), Count, gl_type<T>::Type, 0) {}
		};
		//------------------------------------------------------------------------------------------


		//------------------------------------------------------------------------------------------
		template<u32 Count, typename T>
		struct position : public vertex_attrib<Count,T>
		{
			position()
				: vertex_attrib<Count,T>("Position") {}
		};
		//------------------------------------------------------------------------------------------


		//------------------------------------------------------------------------------------------
		template<u32 Count, typename T>
		struct normal : public vertex_attrib<Count,T>
		{
			normal()
				: vertex_attrib<Count,T>("Normal") {}
		};
		//------------------------------------------------------------------------------------------


		//------------------------------------------------------------------------------------------
		template<u32 Count, typename T>
		struct color : public vertex_attrib<Count,T>
		{
			color()
				: vertex_attrib<Count,T>("Color") {}
		};
		//------------------------------------------------------------------------------------------


		//------------------------------------------------------------------------------------------
		template<u32 Count, typename T>
		struct tex_coord : public vertex_attrib<Count,T>
		{
			tex_coord()
				: vertex_attrib<Count,T>("TexCoord") {}
		};
		//------------------------------------------------------------------------------------------

	} /*format*/

} /*opengl*/ } /*djah*/

#endif /* DJAH_OPENGL_VERTEX_ATTRIB_HPP */
