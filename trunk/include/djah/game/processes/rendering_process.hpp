#ifndef DJAH_GAME_PROCESSES_RENDERING_PROCESS_HPP
#define DJAH_GAME_PROCESSES_RENDERING_PROCESS_HPP

#include <map>
#include <string>
#include "djah/core/typelist.hpp"
#include "djah/3d/shader.hpp"
#include "djah/gameplay/component_process.hpp"
#include "djah/game/components/transform.hpp"
#include "djah/game/components/visual_mesh.hpp"
#include "djah/game/components/texture.hpp"

using djah::game::components::transform;
using djah::game::components::visual_mesh;
using djah::game::components::texture;
using djah::game::components::uv_modifier;
using djah::gameplay::component;

namespace djah { namespace game { namespace processes {

	typedef TYPELIST(transform, visual_mesh, texture) RenderingProcessComponents;

	template<typename ComponentTypeList>
	class rendering_process
		: public gameplay::component_process<ComponentTypeList, RenderingProcessComponents>
	{
		typedef gameplay::component_process<ComponentTypeList, RenderingProcessComponents> super;

	public:
		rendering_process()
			: simpleShader("lit_textured")
		{
			simpleShader.program().begin();
			simpleShader.program().sendUniform("in_World", math::matrix4f::identity);
			simpleShader.program().sendUniform("in_DiffuseSampler", 0);
			simpleShader.program().sendUniform("in_NormalSampler", 1);
			simpleShader.program().sendUniform("in_SpecularSampler", 2);
			simpleShader.program().end();
		}

		virtual bool add(game_object_t *pEntity)
		{
			if( super::add(pEntity) )
			{
				const component<visual_mesh> &visualMeshComp = pEntity->get<visual_mesh>();
				visualMeshComp->spMesh->init(simpleShader.program());
				return true;
			}

			return false;
		}

		void refresh(game_object_t *pEntity)
		{
			const component<visual_mesh> &visualMeshComp = pEntity->get<visual_mesh>();
			visualMeshComp->spMesh->init(simpleShader.program());
		}

		void setMatrixInfos(const math::matrix4f &vp, const math::vector3f &cameraPosition)
		{
			in_VP = vp;
			cameraPosition_ = cameraPosition;
		}

		void setSampler(const std::shared_ptr<opengl::sampler> &spSampler)
		{
			spSampler_ = spSampler;
		}

		virtual void execute(float dt)
		{
			simpleShader.program().begin();
			//simpleShader.program().sendUniform("in_EyeWorldPos", cameraPosition_);//math::resize<3>(math::apply_transform(in_World, math::point3_to_point4(cameraPosition_))));
			super::execute(dt);
			simpleShader.program().end();
		}

	protected:
		virtual void executeFor(game_object_t &entity, float dt)
		{
			const component<transform>   &transformComp	 = entity.get<transform>();
			const component<visual_mesh> &visualMeshComp = entity.get<visual_mesh>();
			const component<texture>	 &textureComp	 = entity.get<texture>();

			if( entity.isUsing<uv_modifier>() )
			{
				component<uv_modifier> uvModifierComp = entity.get<uv_modifier>();
				uvModifierComp->position += uvModifierComp->velocity;
				simpleShader.program().sendUniform("in_UVScale", uvModifierComp->scale);
				simpleShader.program().sendUniform("in_UVOffset", uvModifierComp->offset + uvModifierComp->position);
			}
			else
			{
				simpleShader.program().sendUniform("in_UVScale", math::vector2f(1.0f, 1.0f));
				simpleShader.program().sendUniform("in_UVOffset", math::vector2f(0.0f, 0.0f));
			}

			const auto &in_World = transformComp->toMatrix4();

			opengl::texture_unit<0>::activate_and_bind(textureComp->spTexture, spSampler_);
			simpleShader.program().sendUniform("in_EyeWorldPos", math::resize<3>(math::apply_transform(in_World, math::point3_to_point4(cameraPosition_))));
			simpleShader.program().sendUniform("in_World", in_World);
			simpleShader.program().sendUniform("in_WVP", in_World * in_VP);
			visualMeshComp->spMesh->draw();
		}

	private:
		d3d::shader simpleShader;
		math::matrix4f in_VP;
		math::vector3f cameraPosition_;
		std::shared_ptr<opengl::sampler> spSampler_;
	};

} /*services*/ } /*game*/ } /*djah*/

#endif /* DJAH_GAME_PROCESSES_RENDERING_PROCESS_HPP */