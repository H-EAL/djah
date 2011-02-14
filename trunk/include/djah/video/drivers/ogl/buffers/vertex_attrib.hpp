#ifndef DJAH_VIDEO_DRIVERS_OGL_VERTEX_ATTRIB_HPP
#define DJAH_VIDEO_DRIVERS_OGL_VERTEX_ATTRIB_HPP

#include <string>

#include "../gl_types.hpp"

namespace djah { namespace video { namespace drivers { namespace ogl {

	namespace format {

		//------------------------------------------------------------------------------------------
		class vertex_attrib_base
		{
		public:

			vertex_attrib_base(const std::string &name, unsigned int size, unsigned int count, unsigned int value_type)
				: name_(name), size_(size), count_(count), value_type_(value_type) {}

			// Read-only accessors
			const std::string&  name()      const { return name_;       }
			unsigned int		size()      const { return size_;       }
			unsigned int		count()     const { return count_;      } 
			unsigned int		valueType() const { return value_type_; } 

		protected:

			// Attribute name
			std::string  name_;
			// Size of an element
			unsigned int size_;
			// Number of elements
			unsigned int count_;
			// Elements' type
			unsigned int value_type_;
		};
		//------------------------------------------------------------------------------------------


		//------------------------------------------------------------------------------------------
		template<typename T>
		struct vertex_attrib : public vertex_attrib_base
		{
			vertex_attrib(const std::string &name, unsigned int count)
			: vertex_attrib_base(name, sizeof(T), count, gl_type<T>::Type) {}
		};
		//------------------------------------------------------------------------------------------


		//------------------------------------------------------------------------------------------
		template<typename T>
		struct position : public vertex_attrib<T>
		{
			position(unsigned int count)
				: vertex_attrib<T>("Position", count) {}
		};
		//------------------------------------------------------------------------------------------


		//------------------------------------------------------------------------------------------
		template<typename T>
		struct normal : public vertex_attrib<T>
		{
			normal(unsigned int count)
				: vertex_attrib<T>("Normal", count) {}
		};
		//------------------------------------------------------------------------------------------


		//------------------------------------------------------------------------------------------
		template<typename T>
		struct color : public vertex_attrib<T>
		{
			color(unsigned int count)
				: vertex_attrib<T>("Color", count) {}
		};
		//------------------------------------------------------------------------------------------


		//------------------------------------------------------------------------------------------
		template<typename T>
		struct tex_coord : public vertex_attrib<T>
		{
			tex_coord(unsigned int count)
				: vertex_attrib<T>("TexCoord", count) {}
		};
		//------------------------------------------------------------------------------------------

	} /*format*/

} /*ogl*/ } /*drivers*/ } /*video*/ } /*djah*/

#endif /* DJAH_VIDEO_DRIVERS_OGL_VERTEX_ATTRIB_HPP */