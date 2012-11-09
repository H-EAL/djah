#include "opengl/vertex_format.hpp"

namespace djah { namespace opengl {

	//----------------------------------------------------------------------------------------------
	vertex_format::vertex_format(ePackingType pack_type)
		: packing_type_(pack_type)
		, vertex_size_(0)
	{
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	vertex_format& vertex_format::record(ePackingType pack_type)
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
		return packing_type_ == ePT_Packed;
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


	//----------------------------------------------------------------------------------------------
	bool operator ==(const vertex_format &lhs, const vertex_format &rhs)
	{
		bool result = false;

		if( lhs.vertex_attributes_.size() == rhs.vertex_attributes_.size() )
		{
			result = true;
			unsigned int nbAttributes = lhs.vertex_attributes_.size();
			for(unsigned int i = 0; i < nbAttributes && result; ++i)
			{
				result = (lhs.vertex_attributes_[i] == rhs.vertex_attributes_[i]);
			}
		}

		return result;
	}
	//----------------------------------------------------------------------------------------------

} /*opengl*/ } /*djah*/
