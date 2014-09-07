#ifndef GAME_PROCESSES_RENDERING_PROCESS_HPP
#define GAME_PROCESSES_RENDERING_PROCESS_HPP

#include <map>
#include <string>
#include "djah/core/typelist.hpp"
#include "djah/3d/shader.hpp"
#include "djah/gameplay/component_process.hpp"
#include "game/components_registry.hpp"

using game::components::transform;
using game::components::visual_mesh;
using game::components::texture;
using game::components::uv_modifier;
using djah::gameplay::component;

namespace game { namespace processes {

    typedef TYPELIST(transform, visual_mesh, texture) RenderingProcessComponents;

    class rendering_process
        : public djah::gameplay::component_process<components::DefaultComponentTypes, RenderingProcessComponents>
    {
        typedef djah::gameplay::component_process<components::DefaultComponentTypes, RenderingProcessComponents> super;

    public:
        rendering_process()
            : simpleShader("lit_textured")
        {
            simpleShader.program().begin();
            simpleShader.program().sendUniform("in_World", djah::math::matrix4f::identity);
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

        void setMatrixInfos(const djah::math::matrix4f &vp, const djah::math::vector3f &cameraPosition)
        {
            in_VP = vp;
            cameraPosition_ = cameraPosition;
        }

        void setSamplers(const std::shared_ptr<djah::opengl::sampler> &spSamplerLC, const std::shared_ptr<djah::opengl::sampler> &spSamplerHC)
        {
            spSamplerLC_ = spSamplerLC;
            spSamplerHC_ = spSamplerHC;
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

                TextureFilter::Type textureFilter = uvModifierComp->textureFilter;
                switch(textureFilter)
                {
                case TextureFilter::Bilinear:
                    djah::opengl::texture_unit<0>::activate_and_bind(textureComp->spTexture, spSamplerLC_);
                    break;
                case TextureFilter::Trilinear:
                    djah::opengl::texture_unit<0>::activate_and_bind(textureComp->spTexture, spSamplerHC_);
                    break;
                case TextureFilter::Anisotropic:
                    djah::opengl::texture_unit<0>::activate_and_bind(textureComp->spTexture, spSamplerHC_);
                    break;
                }
            }
            else
            {
                djah::opengl::texture_unit<0>::activate_and_bind(textureComp->spTexture, spSamplerHC_);
                simpleShader.program().sendUniform("in_UVScale", djah::math::vector2f(1.0f, 1.0f));
                simpleShader.program().sendUniform("in_UVOffset", djah::math::vector2f(0.0f, 0.0f));
            }

            const auto &in_World = transformComp->toMatrix4();

            simpleShader.program().sendUniform("in_EyeWorldPos", djah::math::resize<3>(djah::math::apply_transform(in_World, djah::math::point3_to_point4(cameraPosition_))));
            simpleShader.program().sendUniform("in_World", in_World);
            simpleShader.program().sendUniform("in_WVP", in_World * in_VP);
            visualMeshComp->spMesh->draw();
        }

    private:
        djah::d3d::shader simpleShader;
        djah::math::matrix4f in_VP;
        djah::math::vector3f cameraPosition_;
        std::shared_ptr<djah::opengl::sampler> spSamplerLC_;
        std::shared_ptr<djah::opengl::sampler> spSamplerHC_;
    };

} /*services*/ } /*game*/

#endif /* GAME_PROCESSES_RENDERING_PROCESS_HPP */