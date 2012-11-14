#ifndef DJAH_OPENGL_TEXTURE_HPP
#define DJAH_OPENGL_TEXTURE_HPP

#include "../types.hpp"
#include "resource.hpp"

namespace djah { namespace opengl {

	class texture
		: public resource
	{
		DJAH_OPENGL_RESOURCE(texture);

	public:
		enum eBilinearMode
		{
			eBM_None      = 0,
			eBM_Near      = 1 << 0,
			eBM_Far       = 1 << 1,
			eBM_NearFar   = eBM_Near | eBM_Far
		};

		enum eMimappingMode
		{
			eMM_None,
			eMM_Standard,
			eMM_Trilinear,
		};

	public:
		texture(int _internalFormat, int w, int h, bool generatedMipMap = false);
		virtual ~texture();

		void bind() const;
		static void unbind();

		int width() const			{ return width_; }
		int height() const			{ return height_; }
		int internalFormat() const	{ return internalFormat_; }

		void setPixelBuffer(int pixelFormat, int pixelDataType, const byte *pixelBuffer);
		void updatePixelBuffer(int xOffset, int yOffset, int w, int h, int pixelFormat, int pixelDataType, const byte *subPixelBuffer);
		void setWrapMode(int wrapMode);
		void setFiltering(eBilinearMode bilinearMode, eMimappingMode mipmappingMode);
		void setNoFiltering();
		void setBestFiltering();

		static void set_active_unit(u8 unit);
		static u8   active_unit();

	private:
		virtual void aquire();
		virtual void release();
		virtual bool isValidResource() const;

	private:
		int  internalFormat_;
		int  width_;
		int  height_;
		bool hasMipmapping_;

		static u8 sTextureUnit_;
	};

} /*opengl*/ } /*djah*/

#endif /* DJAH_OPENGL_TEXTURE_HPP */
