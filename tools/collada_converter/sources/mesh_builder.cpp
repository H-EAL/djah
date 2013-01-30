#include "mesh_builder.hpp"

#include <algorithm>
#include <functional>
#include <iostream>

#include "collada/proxy.hpp"
#include "collada/utils.hpp"
#include "collada/library/common.hpp"
#include "collada/library/animations.hpp"
#include "collada/library/geometries.hpp"
#include "collada/library/controllers.hpp"
#include "collada/library/visual_scenes.hpp"

using namespace collada;
using namespace collada::library;

#include <djah/core/randomizer.hpp>


void set_matrix_element_from_target(const std::string &target, float value, math::matrix4f &mat)
{
	const std::string &sm = target.substr( target.find_last_of(")")  - 1, 1 );
	const std::string &sn = target.substr( target.find_first_of("(") + 1, 1 );

	int m = (int)(sm[0] - '0');
	int n = (int)(sn[0] - '0');

	mat.m(m,n) = value;
}

math::matrix4f get_matrix(node *n)
{
	math::matrix4f mat;

	if(		!n->transformations_[0].empty()
		||	!n->transformations_[1].empty()
		||	!n->transformations_[2].empty() )
	{
		mat.toIdentity();

		for(unsigned int t = 0; t < n->transformations_[collada::library::transformation::ETT_TRANSLATE].size(); ++t)
		{
			const collada::library::transformation &tr = *n->transformations_[collada::library::transformation::ETT_TRANSLATE][t];
			mat *= math::make_translation(tr.values_[0], tr.values_[1], tr.values_[2]);
		}

		for(unsigned int r = 0; r < n->transformations_[collada::library::transformation::ETT_ROTATE].size(); ++r)
		{
			const collada::library::transformation &rot = *n->transformations_[collada::library::transformation::ETT_ROTATE][r];
			mat *= math::make_rotation(math::deg_to_rad(rot.values_[2]), rot.values_[0], rot.values_[1], rot.values_[2]);
		}

		for(unsigned int s = 0; s < n->transformations_[collada::library::transformation::ETT_SCALE].size(); ++s)
		{
			const collada::library::transformation &sc = *n->transformations_[collada::library::transformation::ETT_SCALE][s];
			mat *= math::make_scale(sc.values_[0], sc.values_[1], sc.values_[2]);
		}
	}
	else
	{
		mat = n->matrix_;
	}

	return mat;
}


//--------------------------------------------------------------------------------------------------
mesh_builder::mesh_builder(const proxy &obj)
{
	if( obj.getGeometriesLib() )
	{
		geometry_list_t::const_iterator geo_it;
		geometry_list_t::const_iterator geo_it_end = obj.getGeometries().end();
		for(geo_it = obj.getGeometries().begin(); geo_it != geo_it_end; ++geo_it)
		{
			models_.push_back( new model(obj, *(*geo_it)) );
		}
	}
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
mesh_builder::~mesh_builder()
{
	delete_content(models_);
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
model::model(const proxy &obj, const geometry &geom)
	: skeleton_(nullptr)
{
	controllers *ctrl_lib = obj.getControllersLib();
	if( ctrl_lib )
	{
		controller *ctrler =  get_controller_by_skin_source(geom.id_, ctrl_lib->controllers_);
		if( ctrler && ctrler->skin_ )
		{
			skeleton_ = new skeleton(obj, *ctrler->skin_);
			if(!skeleton_->isValid())
			{
				delete skeleton_;
				skeleton_ = nullptr;
			}
		}
	}

	min_.x = min_.y = min_.z =  std::numeric_limits<float>::max();
	max_.x = max_.y = max_.z = -std::numeric_limits<float>::max();

	const mesh						&m		= *(geom.mesh_);
	const triangles_list_t			&tris	= m.triangles_;
	triangles_list_t::const_iterator it		= tris.begin();
	triangles_list_t::const_iterator it_end	= tris.end();
	while(it != it_end)
	{
		submesh *sm = new submesh(m, *(*it), skeleton_);
		meshes_.push_back(sm);
		
		min_.x = std::min(min_.x, sm->min_.x);
		min_.y = std::min(min_.y, sm->min_.y);
		min_.z = std::min(min_.z, sm->min_.z);

		max_.x = std::max(max_.x, sm->max_.x);
		max_.y = std::max(max_.y, sm->max_.y);
		max_.z = std::max(max_.z, sm->max_.z);

		++it;
	}
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
model::~model()
{
	delete_content(meshes_);
	delete skeleton_;
}
//--------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------
skeleton::skeleton(const proxy &obj, const skin &skn)
	: is_valid_(false)
{
	is_valid_ = setupBones(skn) && setupWeights(skn) && setupBonesHierarchy( *(obj.getVisualScenesLib()) );

	memcpy(bind_shape_matrix_.data, skn.bind_shape_matrix_, sizeof(bind_shape_matrix_));

	if( is_valid_ && obj.getAnimationsLib() )
	{
		bone_list_t::iterator it;
		bone_list_t::iterator it_end = bones_.end();
		for( it = bones_.begin(); it != it_end; ++it )
			it->setupAnimation( *(obj.getAnimationsLib()) );
	}
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
skeleton::~skeleton()
{
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
bool skeleton::setupBones(const skin &skn)
{
	// Retrieve inputs
	input *joints_ipt	= get_input_by_semantic(ESS_JOINT, skn.joints_->inputs_);
	input *inv_bind_ipt = get_input_by_semantic(ESS_INV_BIND_MATRIX, skn.joints_->inputs_);

	// Retrieve sources
	const source *joints_src	= joints_ipt	? get_source_by_id(joints_ipt->source_  , skn.sources_) : nullptr;
	const source *inv_bind_src	= inv_bind_ipt	? get_source_by_id(inv_bind_ipt->source_, skn.sources_) : nullptr;

	bool success = joints_src && joints_src->name_array_ && inv_bind_src && inv_bind_src->float_array_;
	if( success )
	{
		bones_.reserve(joints_src->name_array_->count_);
		for(unsigned int i = 0; i < joints_src->name_array_->count_; ++i)
		{
			bones_.push_back(bone());
			bone &b = bones_.back();
			b.sid_ = joints_src->name_array_->data_[i];
			memcpy(b.inv_bind_matrix_.data, inv_bind_src->float_array_->data_ + (i*16), sizeof(b.inv_bind_matrix_.data));
		}
	}
	return success;
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
bool skeleton::setupWeights(const skin &skn)
{
	input *weight_ipt			= get_input_by_semantic(ESS_WEIGHT, skn.vertex_weights_->inputs_);
	const source *weight_src	= weight_ipt ? get_source_by_id(weight_ipt->source_, skn.sources_) : nullptr;

	bool success = weight_src != nullptr;
	if( success )
	{
		//const unsigned short *influences = skn.vertex_weights_->vcount_;
		//max_influences_ = *std::max_element(influences, influences+skn.vertex_weights_->count_);
		skin_attributes_.resize(skn.vertex_weights_->count_);
		int w = 0;
		for(unsigned int v = 0; v < skn.vertex_weights_->count_; ++v)
		{
			const int influences = skn.vertex_weights_->vcount_[v];
			for(unsigned short i = 0; i < max_influences_; ++i)
			{
				if(i < influences)
				{
					skin_attributes_[v].bones_ids[i] = skn.vertex_weights_->v_[w++];
					const unsigned short weight_index = skn.vertex_weights_->v_[w++];
					skin_attributes_[v].weights[i] = weight_src->float_array_->data_[weight_index];
				}
				else
				{
					skin_attributes_[v].bones_ids[i] = (unsigned short)-1;
					skin_attributes_[v].weights[i] = 0.0f;
				}
			}
		}
	}
	return success;
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
bool skeleton::setupBonesHierarchy(const visual_scenes &scenes, bool bbb, float t)
{
	bool success = !bones_.empty() && !scenes.visual_scenes_.empty();

	if( success )
	{
		bone &root_bone = bones_[0];
		node *root_node = get_node_by_sid(root_bone.sid_, scenes.visual_scenes_[0]->nodes_);

		success = root_node != nullptr;
		if( success )
		{
			initBone(root_node, math::matrix4f::identity, bbb, t);
		}
	}

	return success;
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
void skeleton::initBone(node *current_node, const math::matrix4f &parent_world_mat, bool bbb, float t)
{
	//const math::matrix4f &current_mat = parent_mat * get_matrix(current_node);
	math::matrix4f jmat			= get_matrix(current_node);
	math::matrix4f current_mat	= parent_world_mat * jmat;
	const int bone_id			= getBoneIdBySid(current_node->sid_);
	if( bone_id >= 0 )
	{
		bone &current_bone = bones_[bone_id];
		current_bone.id_   = current_node->id_;
		current_bone.name_ = current_node->name_;

		if(bbb)
		{
			current_bone.key_frames_[t].transformation_._14 = jmat._14;
			current_bone.key_frames_[t].transformation_._24 = jmat._24;
			current_bone.key_frames_[t].transformation_._34 = jmat._34;
			current_bone.key_frames_[t].transformation_._44 = jmat._44;
			current_bone.world_matrix_ = parent_world_mat * current_bone.key_frames_[t].transformation_;
			current_mat = current_bone.world_matrix_;

			current_bone.start_ = current_bone.end_ = math::vector3f::null_vector;
			current_bone.start_ = math::vec4_to_vec3( math::transform(parent_world_mat, math::vec3_to_vec4(current_bone.start_))	);
			current_bone.end_	= math::vec4_to_vec3( math::transform(current_mat, math::vec3_to_vec4(current_bone.end_))	);
		}
		else
		{
			current_bone.joint_matrix_	= jmat;
			current_bone.world_matrix_	= parent_world_mat * jmat;

			current_bone.start_ = math::vector3f(parent_world_mat._14, parent_world_mat._24, parent_world_mat._34);
			current_bone.end_   = math::vector3f(current_bone.world_matrix_._14, current_bone.world_matrix_._24, current_bone.world_matrix_._34);
		}

		current_bone.skinning_matrix_ = current_bone.world_matrix_ * current_bone.inv_bind_matrix_;
	}

	node_list_t::const_iterator it;
	node_list_t::const_iterator it_end = current_node->children_.end();
	for(it = current_node->children_.begin(); it != it_end; ++it)
	{
		initBone(*it, current_mat, bbb, t);
	}
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
int skeleton::getBoneIdById(const std::string &bone_id) const
{
	int id = -1;
	const int nb_bones = static_cast<int>(bones_.size());
	for( int i = 0; i < nb_bones && id == -1; ++i )
	{
		if( bones_[i].id_ == bone_id )
			id = i;
	}
	return id;

}
//--------------------------------------------------------------------------------------------------
int skeleton::getBoneIdBySid(const std::string &bone_sid) const
{
	int id = -1;
	const int nb_bones = static_cast<int>(bones_.size());
	for( int i = 0; i < nb_bones && id == -1; ++i )
	{
		if( bones_[i].sid_ == bone_sid )
			id = i;
	}
	return id;

}
//--------------------------------------------------------------------------------------------------
int skeleton::getBoneIdByName(const std::string &bone_name) const
{
	int id = -1;
	const int nb_bones = static_cast<int>(bones_.size());
	for( int i = 0; i < nb_bones && id == -1; ++i )
	{
		if( bones_[i].name_ == bone_name )
			id = i;
	}
	return id;

}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
const skeleton::skin_attribute& skeleton::getSkinAttribute(unsigned int vertex_index) const
{
	return skin_attributes_[vertex_index];
}
//--------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------
void bone::setupAnimation(const animations &anim_lib)
{
	float times[] = {0.0f,0.033333f,0.066666f,0.1f,0.133333f,0.166667f,0.2f,0.233333f,0.266667f,0.3f,0.333333f,0.366667f,0.4f,0.433333f,0.466667f,0.5f,0.533333f,0.566667f,0.6f,0.633333f,0.666667f,0.7f,0.733333f,0.766667f,0.8f,0.833333f,0.866667f,0.9f,0.933333f,0.966667f,1.0f,1.03333f,1.06667f,1.1f,1.13333f,1.16667f};

	const animation_list_t &anims = get_animations_by_target_id(id_, anim_lib.animations_);

	animation_list_t::const_iterator it;
	animation_list_t::const_iterator it_end = anims.end();
	for(it = anims.begin(); it != it_end; ++it)
	{
		const channel_list_t &channels = (*it)->channels_;
		const sampler_list_t &samplers = (*it)->samplers_;
		const source_list_t  &sources  = (*it)->sources_;

		channel_list_t::const_iterator ch_it;
		channel_list_t::const_iterator ch_it_end = channels.end();
		for(ch_it = channels.begin(); ch_it != ch_it_end; ++ch_it)
		{
			sampler *spl = get_sampler_by_id( (*ch_it)->source_, samplers );
			if( spl )
			{
				const input *inputs[] =
				{
					get_input_by_semantic(ESS_INPUT, spl->inputs_),
					get_input_by_semantic(ESS_OUTPUT, spl->inputs_),
					get_input_by_semantic(ESS_INTERPOLATION, spl->inputs_)
				};

				int ipt = -1;
				const source *srcs[] =
				{
					inputs[++ipt] ? get_source_by_id(inputs[ipt]->source_, sources)	: 0,
					inputs[++ipt] ? get_source_by_id(inputs[ipt]->source_, sources)	: 0,
					inputs[++ipt] ? get_source_by_id(inputs[ipt]->source_, sources)	: 0
				};

				if( srcs[0] && srcs[1] && srcs[2] )
				{
					// Get target from channel
					std::string target = (*ch_it)->target_;
					target = target.substr( target.find_last_of("/")+1 );

					unsigned int nb_key_frames = 36;//srcs[0]->float_array_->count_;
					for(unsigned int iFrame = 0; iFrame < nb_key_frames; ++iFrame)
					{
						float time = ( srcs[0]->float_array_->count_ < nb_key_frames ) ? times[iFrame] : srcs[0]->float_array_->data_[iFrame];
						if( key_frames_.count(time) <= 0 )
						{
							key_frames_.insert( key_frame_list_t::value_type(time, key_frame(time)) );
						}

						if( target == "transform" )
						{
							// BAKED MATRIX
							memcpy(key_frames_[time].transformation_.data, srcs[1]->float_array_->data_ + (iFrame*16), sizeof(key_frames_[time].transformation_.data));
						}
						else if( target.find("transform") != std::string::npos )
						{
							// EXPLODED MATRIX
							float fValue = (srcs[1]->float_array_->count_ < nb_key_frames) ? srcs[1]->float_array_->data_[0] : srcs[1]->float_array_->data_[iFrame];
							set_matrix_element_from_target(target, fValue, key_frames_[time].transformation_);
						}
						// IF TRANSFORMATIONS
					}
				}
			}
		}
	}
}
//--------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------
submesh::submesh(const mesh &m, const triangles &tris, skeleton *skel)
	: vertex_size_(0)
	, vertex_count_(0)
	, skeleton_(skel)
{
	cleanUp();
	const unsigned int indicesPerVertex = setupAttributeBuffers(m, tris);
	populateAttributeBuffers(indicesPerVertex, tris, skel);
	computeVertexSize();
	computeBoundingBox();

	/*
	if( hasAnimation() )
	{
		setupBindPose();
	}
	*/
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
void submesh::cleanUp()
{
	attributeBuffers_.clear();
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
unsigned int submesh::setupAttributeBuffers(const mesh &m, const triangles &tris)
{
	attributeBuffers_.resize(tris.inputs_.size());

	unsigned int indicesPerVertex = 0;

	int i = 0;
	auto itEnd = tris.inputs_.end();
	for(auto it = tris.inputs_.begin(); it != itEnd; ++it)
	{
		source *src = nullptr;

		if( (*it)->semantic_ == "VERTEX" )
		{
			const std::string &posSrcId = get_source_id_by_semantic(ESS_POSITION, m.vertices_->inputs_);
			src = get_source_by_id(posSrcId, m.sources_);
		}
		else
		{
			src = get_source_by_id((*it)->source_, m.sources_);
		}

		assert(src != nullptr);

		indicesPerVertex = std::max(indicesPerVertex, (*it)->offset_+1);

		attributeBuffers_[i].pInput   = (*it);
		attributeBuffers_[i].pSource  = src;
		attributeBuffers_[i].semantic = (*it)->semantic_;
		attributeBuffers_[i].set      = (*it)->set_;
		attributeBuffers_[i].stride   = src->technique_common_->accessor_->stride_;
		attributeBuffers_[i].data.reserve(tris.vertex_count_ * attributeBuffers_[i].stride);
		++i;
	}

	return indicesPerVertex;
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
void submesh::populateAttributeBuffers(unsigned int indicesPerVertex, const triangles &tris, skeleton *skel)
{
	const unsigned int nbAttribs = attributeBuffers_.size();

	// For each vertex
	for(unsigned int i = 0; i < tris.elements_count_; i += indicesPerVertex)
	{
		// For each vertex attribute
		for(unsigned int v = 0; v < nbAttribs; ++v)
		{
			// Offset of the current vertex attribute in the source data
			const unsigned int attrib_offset = attributeBuffers_[v].pInput->offset_;
			// Number of components of the current vertex attribute
			const unsigned int attrib_stride = attributeBuffers_[v].pSource->technique_common_->accessor_->stride_;
			// Index of the current vertex attribute first component
			const unsigned int index = attrib_stride * tris.indices_[i + attrib_offset];
			// Vertex attribute components
			for(unsigned int s = 0; s < attrib_stride; ++s)
				attributeBuffers_[v].data.push_back( (*(attributeBuffers_[v].pSource->float_array_))[index + s] );
		}

		// Add skinning attributes if needed
		if( hasAnimation() )
		{
			const skeleton::skin_attribute &skin_attr = skel->getSkinAttribute(tris.indices_[i]);
			influences_.insert(influences_.end(), skin_attr.bones_ids, skin_attr.bones_ids + skeleton::max_influences_);
			weights_.insert(weights_.end(), skin_attr.weights, skin_attr.weights + skeleton::max_influences_);
		}

		++vertex_count_;
	}
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
void submesh::computeVertexSize()
{
	const unsigned int nbAttribs = attributeBuffers_.size();

	// Compute vertex size
	for(unsigned int v = 0; v < nbAttribs; ++v)
	{
		vertex_size_ += attributeBuffers_[v].stride;
	}

	// Add skinning attributes size
	if( hasAnimation() )
	{
		strides_[0]	  = skeleton_->max_influences_;
		strides_[1]	  = skeleton_->max_influences_;
		vertex_size_ += skeleton_->max_influences_;
		vertex_size_ += skeleton_->max_influences_;
	}
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
void submesh::computeBoundingBox()
{
	min_.x = min_.y = min_.z =  std::numeric_limits<float>::max();
	max_.x = max_.y = max_.z = -std::numeric_limits<float>::max();

	auto it = std::find_if(attributeBuffers_.begin(), attributeBuffers_.end(), [](const attribute_buffer_t &buff) { return buff.semantic == "VERTEX"; });

	if( it != attributeBuffers_.end() )
	{
		const std::vector<float> &positions = it->data;
		assert(it->stride == 3);

		const unsigned int nbPositions = positions.size();
		for(int i = 0; i < nbPositions; i += 3)
		{
			min_.x = std::min(min_.x, positions[i]);
			min_.y = std::min(min_.y, positions[i+1]);
			min_.z = std::min(min_.z, positions[i+2]);

			max_.x = std::max(max_.x, positions[i]);
			max_.y = std::max(max_.y, positions[i+1]);
			max_.z = std::max(max_.z, positions[i+2]);
		}
	}
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
bool submesh::hasAttribute(collada::E_SOURCE_SEMANTIC semantic) const
{
	std::string semStr(semantic_to_string(semantic));
	auto it = std::find_if(attributeBuffers_.begin(), attributeBuffers_.end(), [&](const attribute_buffer_t &buff) { return buff.semantic == semStr; });
	return (it != attributeBuffers_.end());
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
const submesh::attribute_buffer_t& submesh::getAttributeBufferBySemantic(E_SOURCE_SEMANTIC semantic) const
{
	std::string semStr(semantic_to_string(semantic));
	auto it = std::find_if(attributeBuffers_.begin(), attributeBuffers_.end(), [&](const attribute_buffer_t &buff) { return buff.semantic == semStr; });
	assert(it != attributeBuffers_.end());
	return (*it);
}
//--------------------------------------------------------------------------------------------------

/*
//--------------------------------------------------------------------------------------------------
submesh::submesh(const mesh &m, const triangles &tris, skeleton *skel)
	: vertex_size_(0)
	, vertex_count_(0)
	, skeleton_(skel)
{
	memset(strides_, 0, sizeof(strides_));

	// Retrieve active inputs
	const input *inputs[] =
	{
		get_input_by_semantic(ESS_VERTEX,	   tris.inputs_),
		get_input_by_semantic(ESS_NORMAL,	   tris.inputs_),
		get_input_by_semantic(ESS_TEXCOORD,    tris.inputs_, 0),
		get_input_by_semantic(ESS_TEXCOORD,    tris.inputs_, 1),
		get_input_by_semantic(ESS_TEXCOORD,    tris.inputs_, 2),
		get_input_by_semantic(ESS_TEXCOORD,    tris.inputs_, 3),
		get_input_by_semantic(ESS_TEXTANGENT,  tris.inputs_, 0),
		get_input_by_semantic(ESS_TEXTANGENT,  tris.inputs_, 1),
		get_input_by_semantic(ESS_TEXBINORMAL, tris.inputs_, 0),
		get_input_by_semantic(ESS_TEXBINORMAL, tris.inputs_, 1),
	};

	// For each active input retrieve its source
	const std::string &pos_src_id = get_source_id_by_semantic(ESS_POSITION, m.vertices_->inputs_);
	int ipt = 0;
	const source *srcs[] =
	{
		get_source_by_id(pos_src_id, m.sources_),
		inputs[++ipt] ? get_source_by_id(inputs[ipt]->source_, m.sources_)	: 0,
		inputs[++ipt] ? get_source_by_id(inputs[ipt]->source_, m.sources_)	: 0,
		inputs[++ipt] ? get_source_by_id(inputs[ipt]->source_, m.sources_)	: 0,
		inputs[++ipt] ? get_source_by_id(inputs[ipt]->source_, m.sources_)	: 0,
		inputs[++ipt] ? get_source_by_id(inputs[ipt]->source_, m.sources_)	: 0,
		inputs[++ipt] ? get_source_by_id(inputs[ipt]->source_, m.sources_)	: 0,
		inputs[++ipt] ? get_source_by_id(inputs[ipt]->source_, m.sources_)	: 0,
		inputs[++ipt] ? get_source_by_id(inputs[ipt]->source_, m.sources_)	: 0,
		inputs[++ipt] ? get_source_by_id(inputs[ipt]->source_, m.sources_)	: 0
	};

	// Count the number of actual vertex attributes by counting non null sources
	const int nb_sources = sizeof(srcs)/sizeof(source*);
	unsigned int vertex_attrib_count = 0;
	for(unsigned int v = 0; v < nb_sources; ++v)
	{
		if( inputs[v] && srcs[v] )
		{
			vertex_attrib_count = std::max(vertex_attrib_count, inputs[v]->offset_+1);
		}
	}

	std::vector<float> *buffers[] =
	{
		&positions_,
		&normals_,
		&tex_coords0_,
		&tex_coords1_,
		&tex_coords2_,
		&tex_coords3_,
		&tex_tangents0_,
		&tex_tangents1_,
		&tex_binormals0_,
		&tex_binormals1_
	};

	// Resize buffers for better performances
	for(int i = 0; i < nb_sources; ++i)
	{
		if( srcs[i] )
			buffers[i]->reserve( tris.vertex_count_ );
	}
	if( hasAnimation() )
	{
		weights_.reserve( tris.count_ * 3 * 4 );
		influences_.reserve( tris.count_ * 3 * 4 );
	}

	// For each vertex
	for(unsigned int i = 0; i < tris.elements_count_; i += vertex_attrib_count)
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
				// Index of the current vertex attribute first component
				const unsigned int index = attrib_stride * tris.indices_[i + attrib_offset];
				// Vertex attribute components
				for(unsigned int s = 0; s < attrib_stride; ++s)
					buffers[v]->push_back( (*(srcs[v]->float_array_))[index + s] );
			}
		}

		// Add skinning attributes if needed
		if( hasAnimation() )
		{
			const skeleton::skin_attribute &skin_attr = skel->getSkinAttribute(tris.indices_[i]);
			influences_.insert(influences_.end(), skin_attr.bones_ids, skin_attr.bones_ids + skeleton::max_influences_);
			weights_.insert(weights_.end(), skin_attr.weights, skin_attr.weights + skeleton::max_influences_);
		}

		++vertex_count_;
	}

	// Compute vertex size
	for(unsigned int v = 0; v < nb_sources; ++v)
	{
		if( inputs[v] && srcs[v] )
		{
			strides_[v]   = srcs[v]->technique_common_->accessor_->stride_;
			vertex_size_ += strides_[v];
		}
	}
	
	// Add skinning attributes size
	if( hasAnimation() )
	{
		strides_[EVA_WEIGHTS]			= skeleton_->max_influences_;
		strides_[EVA_BONES_INFLUENCES]	= skeleton_->max_influences_;
		vertex_size_ += strides_[EVA_WEIGHTS];
		vertex_size_ += strides_[EVA_BONES_INFLUENCES];

		setupBindPose();
	}

	computeBoundingBox();
}
//--------------------------------------------------------------------------------------------------
*/
//--------------------------------------------------------------------------------------------------
submesh::~submesh()
{
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void submesh::setupBindPose()
{
	auto itP = std::find_if(attributeBuffers_.begin(), attributeBuffers_.end(), [](const attribute_buffer_t &buff) { return buff.semantic == "VERTEX"; });
	auto itN = std::find_if(attributeBuffers_.begin(), attributeBuffers_.end(), [](const attribute_buffer_t &buff) { return buff.semantic == "NORMAL"; });
	assert(itP != attributeBuffers_.end() && itN != attributeBuffers_.end());

	const std::vector<float> &positions = itP->data;
	const std::vector<float> &normals   = itN->data;

	skinned_positions_.clear();
	skinned_normals_.clear();
	skinned_positions_.resize(positions.size(), 0.0f);
	skinned_normals_.resize(normals.size(), 0.0f);

	// Skin positions and normals
	for(int i = 0; i < vertex_count_; ++i)
	{
		const int pos_stride	= itP->stride;
		const int norm_stride	= itN->stride;
		const int weight_stride	= getWeightStride();
		const int infl_stride	= getInfluenceStride();

		math::vector4f  pos		(positions[i*pos_stride]	, positions[i*pos_stride+1]	, positions[i*pos_stride+2]	, 1.0f							);
		math::vector4f  norm;
		if( !normals.empty() )
			norm = math::vector4f(normals[i*norm_stride]	, normals[i*norm_stride+1]		, normals[i*norm_stride+2]		, 1.0f							);
		math::vector<6,unsigned short> bones;
		memcpy(bones.data, &influences_[i*infl_stride], sizeof(bones.data) );
		math::vector<6, float>  weight;
		memcpy(weight.data, &weights_[i*weight_stride], sizeof(weight.data) );

		math::vector4f skinned_pos;
		math::vector4f skinned_norm;

		float total_weights = 0.0f;
		for(int w = 0; w < weight_stride; ++w)
		{
			if( bones.data[w] != (unsigned short)-1 && weight.data[w] > 0.0f )
			{
				unsigned short bone = bones.data[w];
				const math::matrix4f &mat = skeleton_->bones_[bone].skinning_matrix_;// * skeleton_->bind_shape_matrix_;

				skinned_pos  += math::transform(mat, pos)  * weight.data[w];
				if( !normals.empty() )
					skinned_norm += math::transform(mat, norm) * weight.data[w];

				total_weights += weight.data[w];
			}
		}

		/*if( total_weights != 1.0f )
		{
			float norm_weight = 1.0f / total_weights;
			skinned_pos *= norm_weight;
			skinned_norm *= norm_weight;
			//std::cout << "hahaahaha " << total_weights << std::endl;
		}*/

		memcpy(&skinned_positions_[i*pos_stride], skinned_pos.data , pos_stride * sizeof(float) );
		if( !normals.empty() )
			memcpy(&skinned_normals_[i*norm_stride] , skinned_norm.data, norm_stride * sizeof(float));
	}
}
//--------------------------------------------------------------------------------------------------