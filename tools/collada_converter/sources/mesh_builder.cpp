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
enum E_VERTEX_ATTRIBUTE
{
	EVA_POSITION,
	EVA_NORMAL,
	EVA_TEX_COORD,
	EVA_TANGENT,
	EVA_BONES,
	EVA_WEIGHTS
};
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
class skeleton
{
public:
	skeleton(const skin &skn);
	~skeleton();

	void addBonesInfluences(unsigned int vertex_index, std::vector<float> &buffer) const;
	void addBonesWeights(unsigned int vertex_index, std::vector<float> &buffer) const;


	static const unsigned int max_influences_ = 4;
	struct skin_attribute
	{
		float bones[max_influences_];
		float weights[max_influences_];
	};
	std::vector<skin_attribute> skin_attributes_;
};
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
class submesh
{
public:
	submesh(const mesh &m, const triangles &tris, skeleton *skel);
	~submesh();

	std::vector<float>	buffer_;
	size_t				vertex_size_;
};
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
mesh_builder::mesh_builder(const proxy &obj)
{
	skeleton *skel = obj.getControllersLib() ? new skeleton(*obj.getControllers()[0]->skin_) : 0;

	size_t geo = obj.getGeometries().size();
	for(size_t i = 0; i < geo; ++i)
	{
		const mesh						&m		= *(obj.getGeometries()[i]->mesh_);
		const triangles_list_t			&tris	= m.triangles_;
		triangles_list_t::const_iterator it		= tris.begin();
		triangles_list_t::const_iterator it_end	= tris.end();
		while(it != it_end)
		{
			submesh *sm = new submesh(m, *(*it), skel);
			submeshes_.push_back(sm);
			++it;
		}
	}

	delete skel;
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
skeleton::skeleton(const skin &skn)
{
	input *ipt = get_input_by_semantic(ESS_WEIGHT, skn.vertex_weights_->inputs_);

	// Retrieve weights source
	const source *weights = ipt ? get_source_by_id(ipt->source_, skn.sources_) : 0;

	if(weights)
	{
		/*
		const unsigned short *influences = skn.vertex_weights_->vcount_;
		max_influences_ = *std::max_element(influences, influences+skn.vertex_weights_->count_);
		*/
		skin_attributes_.resize(skn.vertex_weights_->count_);
		int w = 0;
		for(unsigned int v = 0; v < skn.vertex_weights_->count_; ++v)
		{
			const int influences = skn.vertex_weights_->vcount_[v];
			for(unsigned short i = 0; i < max_influences_; ++i)
			{
				if(i < influences)
				{
					skin_attributes_[v].bones[i] = static_cast<float>(skn.vertex_weights_->v_[w++]);
					const unsigned short weight_index = skn.vertex_weights_->v_[w++];
					skin_attributes_[v].weights[i] = weights->float_array_->data_[weight_index];
				}
				else
				{
					skin_attributes_[v].bones[i] = -1.0f;
					skin_attributes_[v].weights[i] = 0.0f;
				}
			}
		}
	}
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
skeleton::~skeleton()
{
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
void skeleton::addBonesInfluences(unsigned int vertex_index, std::vector<float> &buffer) const
{
	for(unsigned int w = 0; w < max_influences_; ++w)
		buffer.push_back( skin_attributes_[vertex_index].bones[w] );
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
void skeleton::addBonesWeights(unsigned int vertex_index, std::vector<float> &buffer) const
{
	for(unsigned int w = 0; w < max_influences_; ++w)
		buffer.push_back( skin_attributes_[vertex_index].weights[w] );
}
//--------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------
submesh::submesh(const mesh &m, const triangles &tris, skeleton *skel)
	: buffer_(0)
	, vertex_size_(0)
{
	// Retrieve active inputs
	const input *inputs[] =
	{
		get_input_by_semantic(ESS_VERTEX,	   tris.inputs_),
		get_input_by_semantic(ESS_NORMAL,	   tris.inputs_),
		get_input_by_semantic(ESS_TEXCOORD,    tris.inputs_),
		get_input_by_semantic(ESS_TEXTANGENT,  tris.inputs_),
		get_input_by_semantic(ESS_TEXBINORMAL, tris.inputs_),
	};

	// For each active input retieve its source
	std::string pos_src_id = get_source_id_by_semantic(ESS_POSITION, m.vertices_->inputs_);
	int ipt = 0;
	const source *srcs[] =
	{
		get_source_by_id(pos_src_id, m.sources_),
		inputs[++ipt] ? get_source_by_id(inputs[ipt]->source_, m.sources_)	: 0,
		inputs[++ipt] ? get_source_by_id(inputs[ipt]->source_, m.sources_)	: 0,
		inputs[++ipt] ? get_source_by_id(inputs[ipt]->source_, m.sources_)	: 0,
		inputs[++ipt] ? get_source_by_id(inputs[ipt]->source_, m.sources_)	: 0
	};

	// Count the number of actual vertex attributes by counting non null sources
	const int nb_sources = sizeof(srcs)/sizeof(source*);
	const unsigned int vertex_attrib_count = static_cast<unsigned int>
	( std::count_if(srcs, srcs+nb_sources, std::bind1st(std::not_equal_to<const source*>(), (source*)0)) );

	// Resize buffer for better performances
	buffer_.reserve(tris.real_count_ * vertex_attrib_count * 3);

	// For each vertex
	for(unsigned int i = 0; i < tris.real_count_; i += vertex_attrib_count)
	{
		// For each vertex attribute
		for(unsigned int v = 0; v < nb_sources; ++v)
		{
			if( inputs[v] && srcs[v] )
			{
				// Offset of the current vertex attribute in the source data
				const unsigned int attrib_offset = inputs[v]->offset_;
				// Number of components of the current vertex attribute
				const unsigned int attrib_stride = srcs[v]->technique_common_->accessor_->stride_;
				// Index of the current vertex attribute's first component
				const unsigned int index = attrib_stride * tris.indices_[i + attrib_offset];
				// Vertex attribute components
				for(unsigned int s = 0; s < attrib_stride; ++s)
					buffer_.push_back( srcs[v]->float_array_->data_[index + s] );
			}
		}

		// Add skinning attributes if needed
		if( skel )
		{
			skel->addBonesInfluences(tris.indices_[i], buffer_);
			skel->addBonesWeights(tris.indices_[i], buffer_);
		}
	}

	// Compute vertex size
	for(unsigned int v = 0; v < nb_sources; ++v)
		if( inputs[v] && srcs[v] )
			vertex_size_ += srcs[v]->technique_common_->accessor_->stride_;
	
	// Skinning attributes size
	if( skel )
		vertex_size_ += skel->max_influences_ * 2;
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
submesh::~submesh()
{
}
//--------------------------------------------------------------------------------------------------