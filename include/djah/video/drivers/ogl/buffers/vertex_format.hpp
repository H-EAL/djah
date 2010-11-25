#ifndef DJAH_VIDEO_DRIVERS_OGL_VERTEX_FORMAT_HPP
#define DJAH_VIDEO_DRIVERS_OGL_VERTEX_FORMAT_HPP

#include <vector>
#include "vertex_attrib.hpp"

namespace djah { namespace video { namespace drivers { namespace ogl {

	class vertex_format
	{
	public:

		typedef std::vector<format::vertex_attrib_base> attr_list_t;

		// Indicates the way attributes are stored in memory
		enum E_PACKING_TYPE
		{
			EPT_PACKED,
			EPT_INTERLEAVED
		};

		// Begin the definition of the vertex format
		vertex_format& record(E_PACKING_TYPE pack_type = EPT_INTERLEAVED);

		// Convenient way to register a vertex format
		vertex_format& operator <<(const format::vertex_attrib_base &attrib);

		// Indicates whether attributes are tightly packed or not
		bool isPacked() const;

		// Retrieve the offset between consecutive vertex attributes
		unsigned int stride() const;

		// Retrieve the length in bytes of a complete vertex
		unsigned int vertexSize() const;

		// Retrieve attributes
		const attr_list_t& attributes() const;

	private:

		E_PACKING_TYPE  packing_type_;
		attr_list_t		vertex_attributes_;
		unsigned int	vertex_size_;
	};

} /*ogl*/ } /*drivers*/ } /*video*/ } /*djah*/

#endif /* DJAH_VIDEO_DRIVERS_OGL_VERTEX_FORMAT_HPP */