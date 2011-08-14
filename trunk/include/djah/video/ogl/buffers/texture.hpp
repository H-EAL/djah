#ifndef DJAH_VIDEO_OGL_TEXTURE_HPP
#define DJAH_VIDEO_OGL_TEXTURE_HPP

#include "../../../types.hpp"
#include "../resource.hpp"

namespace djah { namespace video { namespace ogl {

	enum E_TEXTURE_UNIT
	{
		ETU_0 = 0,
		ETU_1,
		ETU_2,
		ETU_3,
		ETU_4,
		ETU_5,
		ETU_6,
		ETU_7,
		ETU_8,
		ETU_9,
		ETU_10,
		ETU_11,
		ETU_12,
		ETU_13,
		ETU_14,
		ETU_15,
	};

	class texture
		: public resource
	{
	public:

		texture(int width, int height);
		virtual ~texture();

		void bind(int unit = 0) const;
		static void unbind();

		int width() const;
		int height() const;

		void setPixelBuffer(const byte *pixelBuffer, bool luminance);

	private:

		virtual void aquire();
		virtual void release();
		virtual bool isValidResource() const;

	protected:

		int width_;
		int height_;
	};

} /*ogl*/ } /*video*/ } /*djah*/

#endif /* DJAH_VIDEO_OGL_TEXTURE_HPP */
