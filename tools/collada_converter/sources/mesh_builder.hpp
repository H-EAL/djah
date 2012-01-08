#ifndef MESH_BUILDER_HPP
#define MESH_BUILDER_HPP

#include <djah/math.hpp>
#include "collada/fwd_decl.hpp"
#include <map>

using namespace djah;


//--------------------------------------------------------------------------------------------------
enum E_VERTEX_ATTRIBUTE
{
	EVA_POSITION,
	EVA_NORMAL,
	EVA_TEX_COORD,
	EVA_TEX_TANGENT,
	EVA_TEX_BINORMAL,
	EVA_BONES_INFLUENCES,
	EVA_WEIGHTS,
	EVA_COUNT
};
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
class key_frame
{
public:
	key_frame(float time = 0.0f) : time_(time)/**/, transformation_(math::matrix4f::mat_identity)/**/ {}

	float			time_;
	math::matrix4f	transformation_;
};
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
class bone
{
public:
	bone() 
		: inv_bind_matrix_(math::matrix4f::mat_identity)
		, joint_matrix_(math::matrix4f::mat_identity)
		, world_matrix_(math::matrix4f::mat_identity)
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
class mesh_t
{
public:
	mesh_t(const collada::library::mesh &m, const collada::library::triangles &tris, skeleton *skel);
	~mesh_t();

	int getPositionStride()		const { return strides_[EVA_POSITION]; }
	int getNormalStride()		const { return strides_[EVA_NORMAL]; }
	int getTexCoordStride()		const { return strides_[EVA_TEX_COORD]; }
	int getTexTangentStride()	const { return strides_[EVA_TEX_TANGENT]; }
	int getTexBinormalStride()	const { return strides_[EVA_TEX_BINORMAL]; }
	int getWeightStride()		const { return strides_[EVA_WEIGHTS]; }
	int getInfluenceStride()	const { return strides_[EVA_BONES_INFLUENCES]; }

	bool hasAnimation() const { return skeleton_ != 0; }

	void setupSkeleton() {} // TODO
	void setupBindPose();

	std::vector<float>			positions_;
	std::vector<float>			normals_;
	std::vector<float>			tex_coords_;
	std::vector<float>			tex_tangents_;
	std::vector<float>			tex_binormals_;
	std::vector<float>			skinned_positions_;
	std::vector<float>			skinned_normals_;
	std::vector<float>			weights_;
	std::vector<unsigned short>	influences_;
	size_t						vertex_size_;
	int							vertex_count_;

	skeleton					*skeleton_;
	int							strides_[EVA_COUNT];
};
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
class model
{
public:
	model(const collada::proxy &obj, const collada::library::geometry &geom);
	~model();

	mesh_t* getMesh(int i) const { return meshes_[i]; }
	size_t	getMeshCount() const { return meshes_.size(); }

private:
	typedef std::vector<mesh_t*> mesh_list_t;
	mesh_list_t	meshes_;
	skeleton	*skeleton_;
};
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
class mesh_builder
{
public:
	mesh_builder(const collada::proxy &obj);
	~mesh_builder();

	model*  getModel(int i) const { return models_[i]; }
	size_t	getModelCount() const { return models_.size(); }

private:
	typedef std::vector<model*>	model_list_t;
	model_list_t models_;
};
//--------------------------------------------------------------------------------------------------

#endif /* MESH_BUILDER_HPP */