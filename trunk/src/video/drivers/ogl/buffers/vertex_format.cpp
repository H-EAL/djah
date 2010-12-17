#include "video/drivers/ogl/buffers/vertex_format.hpp"

namespace djah { namespace video { namespace drivers { namespace ogl {

	//----------------------------------------------------------------------------------------------
	vertex_format::vertex_format(E_PACKING_TYPE pack_type)
		: packing_type_(pack_type)
		, vertex_size_(0)
	{
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	vertex_format& vertex_format::record(E_PACKING_TYPE pack_type)
	{
		packing_type_ = pack_type;
		vertex_attributes_.clear();
		vertex_size_ = 0;
		return *this;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	vertex_format& vertex_format::operator <<(const format::vertex_attrib_base &attrib)
	{
		vertex_attributes_.push_back(attrib);
		vertex_size_ += attrib.count() * attrib.size();
		return *this;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	bool vertex_format::isPacked() const
	{
		return packing_type_ == EPT_PACKED;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	unsigned int vertex_format::stride() const
	{
		return isPacked() ? 0 : vertexSize();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	unsigned int vertex_format::vertexSize() const
	{
		return vertex_size_;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	const vertex_format::attr_list_t& vertex_format::attributes() const
	{
		return vertex_attributes_;
	}
	//----------------------------------------------------------------------------------------------

} /*ogl*/ } /*drivers*/ } /*video*/ } /*djah*/