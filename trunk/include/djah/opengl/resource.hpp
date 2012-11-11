#ifndef DJAH_OPENGL_RESOURCE_HPP
#define DJAH_OPENGL_RESOURCE_HPP

#include <string>
#include "../debug/assertion.hpp"

#define DJAH_OPENGL_RESOURCE(R) public: static const char* type_name() { return #R; } private:

namespace djah { namespace opengl {

	//-------------------------------------------------------------------------------------------------
	class resource
	{
	public:
		static const char* type_name() { return "unknown opengl resource type"; }

	public:
		unsigned int       id()	     const { return id_; }
		bool		       isValid() const { return id_ != INVALID_ID && isValidResource(); }
		const std::string& name()    const { return name_; }

	protected:
		virtual void aquire()  = 0;
		virtual void release() = 0;
		virtual bool isValidResource() const = 0;

		// 0 is an invalid id in OpenGL
		enum { INVALID_ID = 0 };

		// Meant to be used as a base class
		resource(const std::string &_name = "", unsigned int _id = INVALID_ID) : id_(_id), name_(_name) {}
		virtual ~resource() { DJAH_ASSERT(id_ == INVALID_ID); }

		// Non copyable
		resource(const resource &res);
		resource& operator =(const resource &res);

		// Resource's OpenGL id 
		unsigned int id_;
		// Resource name for debug purpose
		std::string  name_;
	};
	//-------------------------------------------------------------------------------------------------


	//-------------------------------------------------------------------------------------------------
	template<typename T>
	struct monitor
	{
		static void bind(unsigned int _id)
		{
			DJAH_ASSERT_MSG(_id == 0 || _id != boundId_, "Trying to bind an already bound %s (%d)", T::type_name(), _id);
			boundId_ = _id;
		}

		static void assert_if_not_bound(unsigned int _id)
		{
			DJAH_ASSERT_MSG(_id == boundId_, "Trying to call a function on an unbound %s (%d)", T::type_name(), _id);
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
