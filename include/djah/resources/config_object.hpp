#ifndef DJAH_RESOURCES_CONFIG_OBJECT_HPP
#define DJAH_RESOURCES_CONFIG_OBJECT_HPP

#include "djah/core/macros.hpp"
#include "djah/resources/data_object.hpp"

/*
#define declare_config_var(TYPE, NAME, DEFAULT)\
	public: TYPE NAME; \
	private: void config_load_config_vars_ ##NAME() { NAME = pDO_->get<decltype(NAME)>(#NAME, DEFAULT);	}\
	private: void config_save_config_vars_ ##NAME() { pDO_->set(#NAME, NAME);			}
*/

#define load_config_var(VAR) loadConfigVar(VAR, #VAR)

#define list_config_vars_1(A1) load_config_var(A1)
#define list_config_vars_2(A1,A2) load_config_var(A1); list_config_vars_1(A2)
#define list_config_vars_3(A1,A2,A3) load_config_var(A1); list_config_vars_2(A2,A3)
#define list_config_vars_4(A1,A2,A3,A4) load_config_var(A1); list_config_vars_3(A2,A3,A4)
#define list_config_vars_5(A1,A2,A3,A4,A5) load_config_var(A1); list_config_vars_4(A2,A3,A4,A5)
#define list_config_vars_6(A1,A2,A3,A4,A5,A6) load_config_var(A1); list_config_vars_5(A2,A3,A4,A5,A6)
#define list_config_vars_7(A1,A2,A3,A4,A5,A6,A7) load_config_var(A1); list_config_vars_6(A2,A3,A4,A5,A6,A7)
#define list_config_vars_8(A1,A2,A3,A4,A5,A6,A7,A8) load_config_var(A1); list_config_vars_7(A2,A3,A4,A5,A6,A7,A8)
#define list_config_vars_9(A1,A2,A3,A4,A5,A6,A7,A8,A9) load_config_var(A1); list_config_vars_8(A2,A3,A4,A5,A6,A7,A9)

#define list_config_vars(...) VA_ARGS_MACRO(list_config_vars_, __VA_ARGS__)

namespace djah { namespace resources {

	template<typename ObjectToConfig>
	class config_object
		: public ObjectToConfig
	{
	public:
		config_object(const std::string &url)
			: successfullyLoaded_(true)
		{
			static asset_finder<TYPELIST(data_object<>), false> s_asset_finder;
			static bool s_initialized = false;
			if( !s_initialized )
			{
				s_asset_finder.registerLoader(std::make_shared<loader<data_object<>>>(), "config");
				s_initialized = true;
			}

			pDataObject_ = s_asset_finder.get<data_object<>>(url);

			if( !pDataObject_ )
			{
				DJAH_GLOBAL_ERROR() << "Missing data object file " << url << ", using default values" << DJAH_END_LOG();
				successfullyLoaded_ = false;
				pDataObject_ = std::make_shared<data_object<>>(url);
			}
		}

		std::string toString()
		{
			return pDataObject_->toString();
		}
		
	protected:
		template<typename T>
		void loadConfigVar(T &var, const char *varName)
		{
			if( successfullyLoaded_ )
			{
				var = pDataObject_->get<T>(varName, var);
			}
			else
			{
				pDataObject_->add(attribute<T>(varName, var));
			}
		}

	private:
		data_object<>::data_object_sptr pDataObject_;
		bool successfullyLoaded_;
	};

	template<typename T>
	std::shared_ptr<T> open_config(const std::string &url)
	{
		return std::make_shared<T>(url);
	}

} /*resources*/ } /*djah*/

#endif /* DJAH_RESOURCES_CONFIG_OBJECT_HPP */