#include "mesh_builder.hpp"

#include <algorithm>
#include <functional>
#include <iostream>

#include "collada/proxy.hpp"
#include "collada/utils.hpp"
#include "collada/library/common.hpp"
#include "collada/library/geometries.hpp"
#include "collada/library/controllers.hpp"

using namespace collada;
using namespace collada::library;

//--------------------------------------------------------------------------------------------------
class submesh
{
public:
	submesh(const mesh &m, const triangles &tris, const skin &skn);
	~submesh();

	std::vector<float>			buffer_;
	std::vector<float>			abuffer_;
	std::vector<unsigned short>	indices_;
	size_t						vertex_size_;
};
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
mesh_builder::mesh_builder(const proxy &obj)
{
	const skin						&skn	= *(obj.getControllers()[0]->skin_);
	const mesh						&m		= *(obj.getGeometries()[0]->mesh_);
	const triangles_list_t			&tris	= m.triangles_;
	triangles_list_t::const_iterator it		= tris.begin();
	triangles_list_t::const_iterator it_end	= tris.end();
	while(it != it_end)
	{
		submesh *sm = new submesh(m, *(*it), skn);
		submeshes_.push_back(sm);
		++it;
	}
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
mesh_builder::~mesh_builder()
{
	delete_content(submeshes_);
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
size_t mesh_builder::getVertexSize(int i) const
{
	return submeshes_[i]->vertex_size_;
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
size_t mesh_builder::getBufferCount(int i) const
{
	return submeshes_[i]->buffer_.size();
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
const std::vector<float>& mesh_builder::getBuffer(int i) const
{
	return submeshes_[i]->buffer_;
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
size_t mesh_builder::getSubMeshesCount() const
{
	return submeshes_.size();
}
//--------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------
submesh::submesh(const mesh &m, const triangles &tris, const skin &skn)
	: buffer_(0)
	, indices_(0)
	, vertex_size_(0)
{

	const input *inputs[] =
	{
		get_input_by_semantic(ESS_VERTEX,	tris.inputs_),
		get_input_by_semantic(ESS_NORMAL,	tris.inputs_),
		get_input_by_semantic(ESS_TEXCOORD, tris.inputs_),
		get_input_by_semantic(ESS_WEIGHT,	skn.vertex_weights_->inputs_)
	};

	std::string pos_src_id = get_source_id_by_semantic(ESS_POSITION, m.vertices_->inputs_);
	int ipt = 0;
	const source *srcs[] =
	{
		get_source_by_id(pos_src_id, m.sources_),
		inputs[++ipt] ? get_source_by_id(inputs[ipt]->source_, m.sources_)	: 0,
		inputs[++ipt] ? get_source_by_id(inputs[ipt]->source_, m.sources_)	: 0
	};

	// Retrieve weights source
	const source *weights = inputs[++ipt] ? get_source_by_id(inputs[ipt]->source_, skn.sources_) : 0;
	unsigned short max_influences = 0;
	if(weights)
	{
		const unsigned short *influences = skn.vertex_weights_->vcount_;
		max_influences = *std::max_element(influences, influences+skn.vertex_weights_->count_);
	}

	// Compute the number of vertex attributes
	const unsigned int vertex_attrib_count = static_cast<unsigned int>
	( std::count_if(srcs, srcs+3, std::bind1st(std::not_equal_to<const source*>(), (source*)0)) );

	// Resize buffer for better performances
	buffer_.reserve(tris.real_count_ * vertex_attrib_count);

	// For each vertex
	for(unsigned int i = 0; i < tris.real_count_; i += vertex_attrib_count)
	{
		// For each vertex attribute
		for(unsigned int v = 0; v < vertex_attrib_count; ++v)
		{
			const unsigned int attrib_offset = inputs[v]->offset_;
			const unsigned int attrib_stride = srcs[v]->technique_common_->accessor_->stride_;

			// Vertex attribute components
			for(unsigned int s = 0; s < attrib_stride; ++s)
			{
				const unsigned int index = attrib_stride * tris.indices_[i + attrib_offset] + s;
				buffer_.push_back( srcs[v]->float_array_->data_[index] );
			}
		}
	}

	// Compute vertex size
	for(unsigned int v = 0; v < vertex_attrib_count; ++v)
		vertex_size_ += srcs[v]->technique_common_->accessor_->stride_;
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
submesh::~submesh()
{
}
//--------------------------------------------------------------------------------------------------