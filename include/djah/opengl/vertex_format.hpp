#ifndef DJAH_OPENGL_VERTEX_FORMAT_HPP
#define DJAH_OPENGL_VERTEX_FORMAT_HPP

#include <vector>
#include "vertex_attrib.hpp"

namespace djah { namespace opengl {

	class vertex_format
	{
	public:
		typedef std::vector<format::vertex_attrib_base> attr_list_t;

		// Indicates the way attributes are stored in memory
		enum ePackingType
		{
			ePT_Packed,
			ePT_Interleaved
		};

		// Constructor
		vertex_format(ePackingType pack_type = ePT_Interleaved);

		// Begin the definition of the vertex format
		vertex_format& record(ePackingType pack_type = ePT_Interleaved);

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

		// Compute the offset of a given attribute
		unsigned int attributeOffset(const std::string &attributeName, unsigned int vertexCount) const;

		// Compare two vertex formats
		friend bool operator ==(const vertex_format &lhs, const vertex_format &rhs);

	private:
		ePackingType packing_type_;
		attr_list_t	 vertex_attributes_;
		unsigned int vertex_size_;
	};

} /*opengl*/ } /*djah*/

#endif /* DJAH_OPENGL_VERTEX_FORMAT_HPP */
