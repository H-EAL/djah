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
	template<typename T>
	struct value_of;
	//----------------------------------------------------------------------------------------------
	template<>
	struct value_of<int>
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
	struct value_of<float>
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
	struct value_of<std::string>
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
		attrib_map_t attributes_;
	};
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	class capabilities
		: public utils::gen_scatter_hierarchy<TYPELIST(int, float, std::string), caps_holder>
	{
	public:
		void init();
		bool has_extension(const std::string &extension);

		template<typename T>
		T valueOf(int glEnum)
		{
			auto it = caps_holder<T>::attributes_.find(glEnum);
			if( it == caps_holder<T>::attributes_.end() )
			{
				T val = value_of<T>::get(glEnum);
				it = caps_holder<T>::attributes_.insert(
					caps_holder<T>::attrib_map_t::value_type(glEnum, val) ).first;
			}

			check( it != caps_holder<T>::attributes_.end() );

			return it->second;
		}

	private:
		typedef std::vector<std::string> string_list_t;
		string_list_t extensions_;
	};

} /*opengl*/ } /*djah*/

#endif /* DJAH_OPENGL_CAPABILITIES_HPP */
