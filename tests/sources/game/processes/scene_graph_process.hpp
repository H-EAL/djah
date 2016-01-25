#ifndef GAME_PROCESSES_SCENE_GRAPH_PROCESS_HPP
#define GAME_PROCESSES_SCENE_GRAPH_PROCESS_HPP

#include <map>
#include <string>
#include "djah/core/typelist.hpp"
#include "djah/gameplay/component_process.hpp"
#include "game/components_registry.hpp"

using game::components::transform;
using game::components::scene_node;
using djah::gameplay::component;

namespace game { namespace processes {

    typedef TYPELIST(transform, scene_node) SceneGraphProcessComponents;

    class scene_graph_process
        : public djah::gameplay::component_process<components::DefaultComponentTypes, SceneGraphProcessComponents>
    {
        typedef djah::gameplay::component_process<components::DefaultComponentTypes, SceneGraphProcessComponents> super;

    public:
        scene_graph_process()
        {
        }
        
        virtual bool add(game_object_t *pEntity)
        {
            if (pEntity && pEntity->isUsingList<SceneGraphProcessComponents>())
            {
                sceneGraph_.add(pEntity);
                
                return true;
            }

            return false;
        }        

        virtual void execute(float dt)
        {
            for (auto &layer : sceneGraph_.graph_)
            {
                // parallel for
                for (auto &entity : layer)
                {
                    executeFor(*entity, dt);
                }
            }
        }

    protected:
        virtual void executeFor(game_object_t &entity, float dt)
        {
            const component<transform>  &transformComp = entity.get<transform>();
            component<scene_node>       &sceneNodeComp = entity.get<scene_node>();

            //sceneNodeComp->in_World = djah::math::make_translation(transformComp->position);
            sceneNodeComp->in_World = transformComp->toMatrix4();
            if (sceneNodeComp->parentIndex != -1)
            {
                check(sceneNodeComp->depth > 0);
                sceneNodeComp->in_World *= sceneGraph_[sceneNodeComp->depth - 1][sceneNodeComp->parentIndex]->get<scene_node>()->in_World;
            }
        }

    private:

        class scene_graph
        {
        public:
            typedef std::vector<game_object_t*> layer_t;
            typedef std::vector<layer_t> graph_t;

        public:
            scene_graph()
                : graph_(10)
            {

            }

            void add(game_object_t *pEntity)
            {
                const component<scene_node> &sceneNodeComp = pEntity->get<scene_node>();
                graph_[sceneNodeComp->depth].push_back(pEntity);
            }

            layer_t& operator [](unsigned int i)
            {
                check(i < graph_.size());
                return graph_[i];
            }

            graph_t graph_;
        };

        scene_graph sceneGraph_;
    };

} /*services*/ } /*game*/

#endif /* GAME_PROCESSES_SCENE_GRAPH_PROCESS_HPP */