#ifndef DJAH_OPENGL_BUFFER_INTERFACE_BUFFER_HPP
#define DJAH_OPENGL_BUFFER_INTERFACE_BUFFER_HPP

namespace djah { namespace opengl {

	namespace interface {

		class buffer
		{
		public:
			buffer() {}
			virtual ~buffer() {}

			virtual size_t read(void *_pDst, size_t _size) = 0;
			virtual size_t write(const void *_pSrc, size_t _size) = 0;
		};

	} /*interface*/

} /*opengl*/ } /*djah*/

#endif /* DJAH_OPENGL_BUFFER_INTERFACE_BUFFER_HPP */