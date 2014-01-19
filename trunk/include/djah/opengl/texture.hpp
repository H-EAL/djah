#ifndef DJAH_OPENGL_TEXTURE_HPP
#define DJAH_OPENGL_TEXTURE_HPP

#include <set>
#include "djah/types.hpp"
#include "djah/opengl/resource.hpp"

namespace djah { namespace opengl {

	class texture
		: public resource
	{
		DJAH_OPENGL_RESOURCE(texture);
		template<int Unit> friend class texture_unit;

	public:
		texture(int _internalFormat, int w, int h, bool generateMipMap = false);
		virtual ~texture();

		int  width()			const	{ return width_; }
		int  height()			const	{ return height_; }
		int  internalFormat()	const	{ return internalFormat_; }
		bool hasMipmapping()	const	{ return hasMipmapping_; }
		bool isBoundToAnyUnit()	const	{ return !boundTextureUnits_.empty(); }
		bool isBoundToUnit(unsigned int textureUnit) const { return boundTextureUnits_.find(textureUnit) != boundTextureUnits_.end(); }

		void setPixelBuffer(int pixelFormat, int pixelDataType, const byte *pixelBuffer);
		void updatePixelBuffer(int xOffset, int yOffset, int w, int h, int pixelFormat, int pixelDataType, const byte *subPixelBuffer);

	private:
		virtual void aquire();
		virtual void release();
		virtual bool isValidResource() const;

		void bindToUnit(unsigned int textureUnit) const;
		void unbindFromUnit(unsigned int textureUnit) const;

	private:
		int  internalFormat_;
		int  width_;
		int  height_;
		bool hasMipmapping_;
		mutable std::set<unsigned int> boundTextureUnits_;
	};

} /*opengl*/ } /*djah*/

#endif /* DJAH_OPENGL_TEXTURE_HPP */
