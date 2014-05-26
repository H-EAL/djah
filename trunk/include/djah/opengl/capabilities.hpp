#ifndef DJAH_OPENGL_CAPABILITIES_HPP
#define DJAH_OPENGL_CAPABILITIES_HPP

#include <map>
#include <vector>
#include <string>
#include "djah/core/typelist.hpp"
#include "djah/core/hierarchy_generation.hpp"
#include "djah/debug/assertion.hpp"
#include "djah/system/gl.hpp"

namespace djah { namespace opengl {

	//----------------------------------------------------------------------------------------------
	/*struct subset_specs
	{
		u32 isOpenGLES:1;
		u32 majorVersion:4;
		u32 minorVersion:4;
		const char *extensions[];
	};*/

	//----------------------------------------------------------------------------------------------
	template<typename T>
	struct ValueOf;
	//----------------------------------------------------------------------------------------------
	template<>
	struct ValueOf<int>
	{
		static int get(int glEnum)
		{
			int val = 0;
			glGetIntegerv(glEnum, &val);
			return val;
		}
	};
	//----------------------------------------------------------------------------------------------
	template<>
	struct ValueOf<float>
	{
		static float get(int glEnum)
		{
			float val = 0.0f;
			glGetFloatv(glEnum, &val);
			return val;
		}
	};
	//----------------------------------------------------------------------------------------------
	template<>
	struct ValueOf<std::string>
	{
		static std::string get(int glEnum)
		{
			const GLubyte *buf = glGetString(glEnum);
			return buf ? reinterpret_cast<const char*>(buf) : "";
		}
	};
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename T>
	struct caps_holder
	{
		typedef std::map<int, T>  attrib_map_t;
		static attrib_map_t attributes_;
	};
	//----------------------------------------------------------------------------------------------
	template<typename T>
	typename caps_holder<T>::attrib_map_t caps_holder<T>::attributes_;
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	class capabilities
		: public utils::gen_scatter_hierarchy<TYPELIST(int, float, std::string), caps_holder>
	{
	public:
		void init();
		bool has_extension(const std::string &extension);

		template<typename T>
		static T value_of(int glEnum, bool updateCache = false)
		{
			T val;

			auto it = caps_holder<T>::attributes_.end();

			if( !updateCache )
			{
				it = caps_holder<T>::attributes_.find(glEnum);
			}

			if( it != caps_holder<T>::attributes_.end() )
			{
				val = it->second;
			}
			else
			{
				val = ValueOf<T>::get(glEnum);
				caps_holder<T>::attributes_[glEnum] = val;
			}

			return val;
		}

	private:
		typedef std::vector<std::string> string_list_t;
		string_list_t extensions_;
	};

} /*opengl*/ } /*djah*/

#endif /* DJAH_OPENGL_CAPABILITIES_HPP */
