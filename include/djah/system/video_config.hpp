#ifndef DJAH_SYSTEM_VIDEO_CONFIG_HPP
#define DJAH_SYSTEM_VIDEO_CONFIG_HPP

#include <string>
#include <memory>
#include "djah/core/macros.hpp"
#include "djah/dataobject/global_registry.hpp"


#define declare_config_var(TYPE, NAME, DEFAULT)\
	public: TYPE NAME; \
	private: void config_load_config_vars_ ##NAME() { NAME = pDO_->get<TYPE>(#NAME, DEFAULT);	}\
	private: void config_save_config_vars_ ##NAME() { pDO_->set<TYPE>(#NAME, NAME);			}

#define list_config_vars_1(A1) config_load_config_vars_ ##A1();
#define list_config_vars_2(A1,A2) config_load_config_vars_ ##A1(); list_config_vars_1(A2)
#define list_config_vars_3(A1,A2,A3) config_load_config_vars_ ##A1(); list_config_vars_2(A2,A3)
#define list_config_vars_4(A1,A2,A3,A4) config_load_config_vars_ ##A1(); list_config_vars_3(A2,A3,A4)
#define list_config_vars_5(A1,A2,A3,A4,A5) config_load_config_vars_ ##A1(); list_config_vars_4(A2,A3,A4,A5)
#define list_config_vars_6(A1,A2,A3,A4,A5,A6) config_load_config_vars_ ##A1(); list_config_vars_5(A2,A3,A4,A5,A6)
#define list_config_vars_7(A1,A2,A3,A4,A5,A6,A7) config_load_config_vars_ ##A1(); list_config_vars_6(A2,A3,A4,A5,A6,A7)
#define list_config_vars_8(A1,A2,A3,A4,A5,A6,A7,A8) config_load_config_vars_ ##A1(); list_config_vars_7(A2,A3,A4,A5,A6,A7,A8)
#define list_config_vars_9(A1,A2,A3,A4,A5,A6,A7,A8,A9) config_load_config_vars_ ##A1(); list_config_vars_8(A2,A3,A4,A5,A6,A7,A9)

#define list_config_vars(...) VA_ARGS_MACRO(list_config_vars_, __VA_ARGS__)


namespace djah { namespace system {

	//----------------------------------------------------------------------------------------------
	class configurable
	{
	public:
		configurable(const std::string &configName = "")
		{
			if( !configName.empty() )
			{
				pDO_ = djah::dataobject::default_registry::get().getDO(configName);
			}
			else
			{
				pDO_ = std::make_shared<djah::dataobject::default_registry::data_object_t>("");
			}
		}

	protected:
		djah::dataobject::default_registry::data_object_ptr pDO_;
	};

	template<typename T>
	std::shared_ptr<T> open_config(const std::string &configName)
	{
		return std::make_shared<T>(configName);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	enum eOpenglVersion
	{
		OpenGL_Version_None		= -1,
		OpenGL_Version_Default	= 0,

		OpenGL_Version_1_0		= (1 << 8) | 0,
		OpenGL_Version_1_1		= (1 << 8) | 1,
		OpenGL_Version_1_2		= (1 << 8) | 2,
		OpenGL_Version_1_3		= (1 << 8) | 3,
		OpenGL_Version_1_4		= (1 << 8) | 4,
		OpenGL_Version_1_5		= (1 << 8) | 5,

		OpenGL_Version_2_0		= (2 << 8) | 0,
		OpenGL_Version_2_1		= (2 << 8) | 1,

		OpenGL_Version_3_0		= (3 << 8) | 0,
		OpenGL_Version_3_1		= (3 << 8) | 1,
		OpenGL_Version_3_2		= (3 << 8) | 2,
		OpenGL_Version_3_3		= (3 << 8) | 3,

		OpenGL_Version_4_0		= (4 << 8) | 0,
		OpenGL_Version_4_1		= (4 << 8) | 1,
		OpenGL_Version_4_2		= (4 << 8) | 2,
		OpenGL_Version_4_3		= (4 << 8) | 3,

		OpenGL_ES_Version_1_0	= (1 << 16) | (1 << 8) | 0,
		OpenGL_ES_Version_1_1	= (1 << 16) | (1 << 8) | 1,
		OpenGL_ES_Version_2_0	= (1 << 16) | (2 << 8) | 0,
		OpenGL_ES_Version_2_1	= (1 << 16) | (2 << 8) | 1,
	};
	//----------------------------------------------------------------------------------------------
	class driver_config
		: public configurable
	{
	public:
		driver_config(const std::string &configName) : configurable(configName)
		{
			list_config_vars
			(
				majorVersion,		
				minorVersion,		
				isOpenglES,	
				enableDebug,	
				enableCompatibilityProfile,
				vsync
			);
		}

		driver_config(eOpenglVersion version = OpenGL_Version_Default, bool _enableDebug = false,
					  bool _enableCompatibilityProfile = true, bool _vsync = true);

		driver_config(int _majorVersion, int _minorVersion, bool _isOpenglES,
					  bool _enableDebug, bool _enableCompatibilityProfile, bool _vsync);

		declare_config_var(int,  majorVersion,				0);
		declare_config_var(int,  minorVersion,				0);
		declare_config_var(bool, isOpenglES,					false);
		declare_config_var(bool, enableDebug,				false);
		declare_config_var(bool, enableCompatibilityProfile,	false);
		declare_config_var(bool,	vsync,						true);

	public:
		static int s_default_major_version;
		static int s_default_minor_version;
	};
	typedef std::shared_ptr<driver_config> driver_config_sptr;
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	class device_config
		: public configurable
	{
	public:
		device_config(const std::string &configName) : configurable(configName)
		{
			list_config_vars
			(
				width,		
				height,		
				colorBits,	
				depthBits,	
				stencilBits,
				fullscreen,
				title
			);
		}

		device_config(int _width = 640, int _height = 480, int _colorBits = 32, int _depthBit = 24,
					  int _stencilBits = 0, bool _fullscreen = false,
					  const std::string &_title = "Djah's Heavenly Window");

		declare_config_var(int,			width,			640);
		declare_config_var(int,			height,			480);
		declare_config_var(int,			colorBits,		32);
		declare_config_var(int,			depthBits,		24);
		declare_config_var(int,			stencilBits,	0);
		declare_config_var(bool,		fullscreen,		false);
		declare_config_var(std::string,	title,			"Djah's Heavenly Window");
	};
	typedef std::shared_ptr<device_config> device_config_sptr;
	//----------------------------------------------------------------------------------------------

} /*system*/ } /*djah*/

#endif /* DJAH_SYSTEM_VIDEO_CONFIG_HPP */