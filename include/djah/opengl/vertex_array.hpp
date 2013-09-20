#ifndef DJAH_OPENGL_VERTEX_ARRAY_HPP
#define DJAH_OPENGL_VERTEX_ARRAY_HPP

#include <map>
#include <stack>
#include "djah/system/gl.hpp"
#include "djah/opengl/resource.hpp"

namespace djah { namespace opengl {

	// Forward declarations
	class vertex_buffer;
	class index_buffer;
	class shader_program;
	class vertex_format;

	class vertex_array
		: public resource
	{
		DJAH_OPENGL_RESOURCE(vertex_array);

	public:
		vertex_array();
		virtual ~vertex_array();

		void addVertexBuffer(vertex_buffer *pVB, const vertex_format &format);
		void removeVertexBuffer(vertex_buffer *pVB);
		void setIndexBuffer(index_buffer *pIb);
		void setVertexCount(int vertexCount);
		void init(const shader_program &sp);
		void draw(int primitiveType = GL_TRIANGLES) const;
		void drawInstanced(int instanceCount, int primitiveType = GL_TRIANGLES) const;

	private:
		virtual void aquire();
		virtual void release();
		virtual bool isValidResource() const;

		void enableVertexBuffer(vertex_buffer *pVB, const vertex_format &format, const shader_program &sp, std::stack<int> &attributesStack);

		void bind() const;
		static void unbind();

	private:
		typedef std::map<vertex_buffer*, vertex_format> vb_list_t;

		vb_list_t		vertexBuffers_;
		index_buffer	*pIndexBuffer_;
		int				vertexCount_;
		bool			initialized_;
	};

} /*opengl*/ } /*djah*/

#endif /* DJAH_OPENGL_VERTEX_ARRAY_HPP */
