#ifndef DJAH_OPENGL_RESOURCE_HPP
#define DJAH_OPENGL_RESOURCE_HPP

#include "../debug/assertion.hpp"

namespace djah { namespace opengl {

	//-------------------------------------------------------------------------------------------------
	class resource
	{
	public:
		unsigned int id()	   const { return id_; }
		bool		 isValid() const { return id_ != INVALID_ID && isValidResource(); }

	protected:
		virtual void aquire()  = 0;
		virtual void release() = 0;
		virtual bool isValidResource() const = 0;

		// 0 is an invalid id in OpenGL
		enum { INVALID_ID = 0 };

		// Meant to be used as a base class
		resource(unsigned int _id = INVALID_ID) : id_(_id) {}
		virtual ~resource() { DJAH_ASSERT(id_ == INVALID_ID); }

		// Non copyable
		resource(const resource &res);
		resource& operator =(const resource &res);

		// Resource's OpenGL id 
		unsigned int id_;
	};
	//-------------------------------------------------------------------------------------------------


	//-------------------------------------------------------------------------------------------------
	template<typename T>
	struct monitor
	{
		static void bind(unsigned int _id)
		{
			DJAH_ASSERT_MSG(_id == 0 || _id != boundId_, "Trying to bind an already bound resource");
			boundId_ = _id;
		}

		static void assert_if_not_bound(unsigned int _id)
		{
			DJAH_ASSERT_MSG(_id == boundId_, "Trying to call a function on an unbound resource");
		}

	private:
		static unsigned int boundId_;
	};
	//-------------------------------------------------------------------------------------------------
	template<typename T>
	unsigned int monitor<T>::boundId_ = 0;
	//-------------------------------------------------------------------------------------------------

} /*opengl*/ } /*djah*/

#endif /* DJAH_OPENGL_RESOURCE_HPP */
