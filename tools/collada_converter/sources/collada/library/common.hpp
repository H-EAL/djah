#ifndef COLLADA_LIBRARY_COMMON_HPP
#define COLLADA_LIBRARY_COMMON_HPP

#include <string>
#include <vector>
#include <sstream>
#include <tinyxml.h>
#include "../fwd_decl.hpp"

namespace collada { namespace library {

	//----------------------------------------------------------------------------------------------
	template<class Container>
	void delete_content(Container &container)
	{
		typename Container::iterator it;
		typename Container::iterator it_end = container.end();
		for(it = container.begin(); it != it_end; ++it)
			delete *it;
		container.clear();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename T>
	class t_array
	{
	public:
		t_array() : data_(0) {}
		void deserialize(const TiXmlElement &element)
		{
			if(element.Attribute("id"))
				id_ = element.Attribute("id");
			int tmp = 0;
			element.Attribute("count", &tmp);
			count_ = static_cast<unsigned int>(tmp);

			if( count_ )
			{
				data_ = new T[count_];
				std::stringstream ss(element.GetText());
				for(unsigned int i = 0; i < count_ && !ss.eof(); ++i)
					ss >> data_[i];
			}
		}
		~t_array()
		{
			delete [] data_;
		}

		std::string	 id_;
		unsigned int count_;
		T			 *data_;
	};
	typedef t_array<float>			float_array;
	typedef t_array<std::string>	name_array;
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	class param
	{
	public:
		void deserialize(const TiXmlElement &element);

		std::string name_;
		std::string type_;
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	class accessor
	{
	public:
		void deserialize(const TiXmlElement &element);
		~accessor();

		unsigned int count_;
		unsigned int offset_;
		std::string  source_;
		unsigned int stride_;
		param_list_t params_;
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	class technique_common
	{
	public:
		technique_common() : accessor_(0) {}
		void deserialize(const TiXmlElement &element);
		~technique_common();

		accessor *accessor_;
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	class source
	{
	public:
		source() : float_array_(0), name_array_(0), technique_common_(0) {}
		void deserialize(const TiXmlElement &element);
		~source();

		std::string		 id_;
		std::string		 name_;
		float_array		 *float_array_;
		name_array		 *name_array_;
		technique_common *technique_common_;
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	class input
	{
	public:
		void deserialize(const TiXmlElement &element);

		std::string  semantic_;
		std::string  source_;
		unsigned int offset_;
		unsigned int set_;
	};
	//----------------------------------------------------------------------------------------------


} /*library*/ } /*collada*/

#endif /* COLLADA_LIBRARY_COMMON_HPP */