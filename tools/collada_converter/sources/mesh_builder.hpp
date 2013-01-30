#ifndef MESH_BUILDER_HPP
#define MESH_BUILDER_HPP

#include <djah/math.hpp>
#include "collada/fwd_decl.hpp"
#include "collada/utils.hpp"
#include <map>

using namespace djah;


//--------------------------------------------------------------------------------------------------
enum E_VERTEX_ATTRIBUTE
{
	EVA_POSITION,
	EVA_NORMAL,
	EVA_TEX_COORD0,
	EVA_TEX_COORD1,
	EVA_TEX_COORD2,
	EVA_TEX_COORD3,
	EVA_TEX_TANGENT0,
	EVA_TEX_TANGENT1,
	EVA_TEX_BINORMAL0,
	EVA_TEX_BINORMAL1,
	EVA_BONES_INFLUENCES,
	EVA_WEIGHTS,
	EVA_COUNT
};
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
class key_frame
{
public:
	key_frame(float time = 0.0f) : time_(time)/**/, transformation_(math::matrix4f::identity)/**/ {}

	float			time_;
	math::matrix4f	transformation_;
};
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
class bone
{
public:
	bone() 
		: inv_bind_matrix_(math::matrix4f::identity)
		, joint_matrix_(math::matrix4f::identity)
		, world_matrix_(math::matrix4f::identity)
	{}
	~bone() {}

	void setupAnimation(const collada::library::animations &anim_lib);

	std::string		id_;
	std::string		sid_;
	std::string		name_;
	math::matrix4f	joint_matrix_;
	math::matrix4f	inv_bind_matrix_;
	math::matrix4f	world_matrix_;
	math::matrix4f	skinning_matrix_;
	
	typedef std::map<float, key_frame> key_frame_list_t;
	key_frame_list_t key_frames_;

	math::vector3f start_;
	math::vector3f end_;
};
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
class skeleton
{
public:
	skeleton(const collada::proxy &obj, const collada::library::skin &skn);
	~skeleton();

	bool isValid() const { return is_valid_; }

	static const unsigned int max_influences_ = 6;
	struct skin_attribute
	{
		unsigned short bones_ids[max_influences_];
		float weights[max_influences_];
	};
	const skin_attribute& getSkinAttribute(unsigned int vertex_index) const;

//private:
	bool setupBones(const collada::library::skin &skn);
	bool setupWeights(const collada::library::skin &skn);
	bool setupBonesHierarchy(const collada::library::visual_scenes &scenes, bool bbb = false, float t = 0.0f);
	void initBone(collada::library::node *current_node, const math::matrix4f &parent_mat, bool bbb = false, float t = 0.0f);
	int  getBoneIdById(const std::string &bone_id) const;
	int  getBoneIdBySid(const std::string &bone_sid) const;
	int  getBoneIdByName(const std::string &bone_name) const;

public:
	typedef std::vector<bone> bone_list_t;
	std::vector<skin_attribute> skin_attributes_;
	bone_list_t					bones_;
	bool						is_valid_;
	math::matrix4f				bind_shape_matrix_;
};
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
class submesh
{
public:
	struct attribute_buffer_t
	{
		attribute_buffer_t(collada::library::input *ipt = nullptr, collada::library::source *src = nullptr, unsigned int count = 0) : pInput(ipt), pSource(src) { data.reserve(count); }

		collada::library::input		*pInput;
		collada::library::source	*pSource;
		std::string					semantic;
		unsigned int				set;
		std::vector<float>			data;
		unsigned int				stride;
	};
	typedef std::vector< attribute_buffer_t > attrib_buffer_list_t;

	attrib_buffer_list_t attributeBuffers_;

public:
	submesh(const collada::library::mesh &m, const collada::library::triangles &tris, skeleton *skel);
	~submesh();

	void cleanUp();
	unsigned int setupAttributeBuffers(const collada::library::mesh &m, const collada::library::triangles &tris);
	void populateAttributeBuffers(unsigned int indicesPerVertex, const collada::library::triangles &tris, skeleton *skel);
	void computeVertexSize();
	void computeBoundingBox();

	bool hasAttribute(collada::E_SOURCE_SEMANTIC semantic) const;
	const attribute_buffer_t& getAttributeBufferBySemantic(collada::E_SOURCE_SEMANTIC semantic) const;

	int getWeightStride()		const { return strides_[0]; }
	int getInfluenceStride()	const { return strides_[1]; }

	bool hasAnimation() const { return skeleton_ != nullptr; }

	void setupSkeleton() {} // TODO
	void setupBindPose();

	std::vector<float>			skinned_positions_;
	std::vector<float>			skinned_normals_;
	std::vector<float>			weights_;
	std::vector<unsigned short>	influences_;
	size_t						vertex_size_;
	int							vertex_count_;

	math::vector3f min_, max_;

	skeleton					*skeleton_;
	int							strides_[2];
};
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
class model
{
public:
	model(const collada::proxy &obj, const collada::library::geometry &geom);
	~model();

	submesh* getMesh(unsigned int i) const { return meshes_[i]; }
	size_t	getMeshCount() const { return meshes_.size(); }

private:
	typedef std::vector<submesh*> mesh_list_t;
	mesh_list_t	meshes_;
	skeleton	*skeleton_;

public:
	math::vector3f min_, max_;
};
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
class mesh_builder
{
public:
	mesh_builder(const collada::proxy &obj);
	~mesh_builder();

	model*  getModel(unsigned int i) const { return models_[i]; }
	size_t	getModelCount() const { return models_.size(); }

private:
	typedef std::vector<model*>	model_list_t;
	model_list_t models_;
};
//--------------------------------------------------------------------------------------------------

#endif /* MESH_BUILDER_HPP */