#ifndef DJAH_VIDEO_DRIVERS_OGL_RESOURCE_HPP
#define DJAH_VIDEO_DRIVERS_OGL_RESOURCE_HPP

namespace djah { namespace video { namespace drivers { namespace ogl {

	class resource
	{
	public:

		unsigned int	id()		const { return id_; }
		bool			isValid()	const { return id_ != INVALID_ID; }

		virtual void	aquire()  = 0;
		virtual void	release() = 0;

	protected:

		// 0 is an invalid id in OpenGL
		enum { INVALID_ID = 0 };

		// Meant to be used as a base class
		resource(unsigned int id = INVALID_ID) : id_(id) {}
		virtual ~resource() {}

		// Non copyable
		resource(const resource &res);
		resource& operator =(const resource &res);

		// Resource's OpenGL id 
		unsigned int id_;
	};

} /*ogl*/ } /*drivers*/ } /*video*/ } /*djah*/

#endif /* DJAH_VIDEO_DRIVERS_OGL_RESOURCE_HPP */