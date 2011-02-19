#ifndef DJAH_VIDEO_DRIVERS_OGL_VERTEX_ATTRIB_HPP
#define DJAH_VIDEO_DRIVERS_OGL_VERTEX_ATTRIB_HPP

#include <string>

#include "../../../../types.hpp"
#include "../gl_types.hpp"

namespace djah { namespace video { namespace drivers { namespace ogl {

	namespace format {

		//------------------------------------------------------------------------------------------
		class vertex_attrib_base
		{
		public:

			vertex_attrib_base(const std::string &name, u32 size, u32 count, u32 value_type)
				: name_(name), size_(size), count_(count), value_type_(value_type) {}

			// Read-only accessors
			const std::string& name() const { return name_; }
			u32 size()      const { return size_;       }
			u32 count()     const { return count_;      } 
			u32 valueType() const { return value_type_; } 

		protected:

			// Attribute name
			std::string  name_;
			// Size of an element
			u32 size_;
			// Number of elements
			u32 count_;
			// Elements' type
			u32 value_type_;
		};
		//------------------------------------------------------------------------------------------


		//------------------------------------------------------------------------------------------
		template<u32 Count, typename T>
		struct vertex_attrib : public vertex_attrib_base
		{
			vertex_attrib(const std::string &name)
			: vertex_attrib_base(name, sizeof(T), Count, gl_type<T>::Type) {}
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

} /*ogl*/ } /*drivers*/ } /*video*/ } /*djah*/

#endif /* DJAH_VIDEO_DRIVERS_OGL_VERTEX_ATTRIB_HPP */