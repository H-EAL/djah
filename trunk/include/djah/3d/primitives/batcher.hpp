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
#include "djah/opengl.hpp"

namespace djah { namespace d3d { namespace primitives {

	class batcher
	{
	public:
		typedef u32 prim_id_t;

		batcher();
		~batcher();

		prim_id_t add(const math::transformation_f &t, const math::vector3f &color);
		void remove(prim_id_t primId);
		int count() const;

		void clean();
		void resetTransformationsBuffer();
		void draw(const math::matrix4f &matViewProjection);
		void init(const std::vector<triangle> &triangles);

		//------------------------------------------------------------------------------------------
		template<typename PrimType>
		void init(int level1 = 0, int level2 = 0)
		{
			init( PrimType().construct(level1, level2) );
		}
		//------------------------------------------------------------------------------------------

	private:
		opengl::vertex_buffer *pPrimitiveVertexBuffer_;
		opengl::vertex_buffer *pTransformationsBuffer_;
		opengl::vertex_format  vertexFormat_;
		opengl::vertex_format  transformationFormat_;
		opengl::vertex_array   vertexArray_;
		shader				   batchShader_;

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