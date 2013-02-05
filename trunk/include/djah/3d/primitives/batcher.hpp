#ifndef DJAH_3D_PRIMITIVES_BATCHER_HPP
#define DJAH_3D_PRIMITIVES_BATCHER_HPP

#include <stack>
#include <vector>
#include "djah/types.hpp"
#include "djah/math/transformation.hpp"
#include "djah/math/vector3.hpp"
#include "djah/math/matrix4.hpp"
#include "djah/3d/primitives/triangle.hpp"
#include "djah/3d/shader.hpp"
#include "djah/resources/mesh.hpp"
#include "djah/opengl.hpp"

namespace djah { namespace d3d { namespace primitives {

	class batcher
	{
	public:
		typedef u32 prim_id_t;

		batcher();
		~batcher();

		prim_id_t add(const math::transformation_f &t, const math::vector3f &color);
		void set(prim_id_t primId, float h);
		void remove(prim_id_t primId);
		int count() const;

		void clean();
		void resetTransformationsBuffer();
		void init(const std::vector<triangle> &triangles);
		void init(const resources::mesh_ptr &pMesh);

		//------------------------------------------------------------------------------------------
		template<typename PrimType>
		void init(int level1 = 0, int level2 = 0)
		{
			init( PrimType().construct(level1, level2) );
		}
		//------------------------------------------------------------------------------------------

		void draw();
		opengl::shader_program& program() { return batchShader_.program(); }

	private:
		resources::mesh_ptr	   pMesh_;
		opengl::vertex_buffer *pPrimitiveVertexBuffer_;
		opengl::vertex_buffer *pTransformationsBuffer_;
		opengl::vertex_format  vertexFormat_;
		opengl::vertex_format  transformationFormat_;
		opengl::vertex_array   vertexArray_;
		shader				   batchShader_;
		shader				   pickShader_;

		struct trans
		{
			math::transformation_f t;
			math::vector3f       c;
		};

		std::vector<trans>	transformations_;
		std::set<int>		availableSlots_;
		bool				dirtyFlag_;
	};

} /*primitives*/ } /*d3d*/ } /*djah*/

#endif /* DJAH_3D_PRIMITIVES_BATCHER_HPP */