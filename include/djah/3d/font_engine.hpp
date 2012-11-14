#ifndef DJAH_VIDEO_FONT_ENGINE_HPP
#define DJAH_VIDEO_FONT_ENGINE_HPP

#include <map>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "../types.hpp"
#include "../math/vector2.hpp"
#include "../math/vector3.hpp"
#include "../core/singleton.hpp"

namespace djah { namespace d3d {

	typedef math::vector3f color;

	namespace ogl {
		class texture;
	};

	class font_engine
		: public utils::singleton<font_engine>
	{
		DJAH_MAKE_SINGLETON(font_engine);

	public:
		void setFontsPath(const std::string &fontsPath);
		void setFont(const std::string &fontName, u8 size);
		void setColor(const color &c);

		void print(const std::string &text, int x = 0, int y = 0) const;

	protected:
		font_engine();
		virtual ~font_engine();

	private:
		struct font_traits
		{
			std::string name;
			u8 size;
			bool operator ==(const font_traits &rhs) const
			{
				return (name == rhs.name) && (size == rhs.size);
			}
			bool operator <(const font_traits &rhs) const
			{
				return (name < rhs.name) && (size < rhs.size);
			}
		};

		struct font_data* newFont(const font_traits &traits);

		typedef std::map<font_traits, struct font_data*> font_map_t;

		font_map_t font_map_;
		const struct font_data *current_font_;

		color color_;
	public:
		FT_Library ft_library_;
		std::string fonts_path_;
	};

} /*d3d*/ } /*djah*/

#endif /* DJAH_VIDEO_FONT_ENGINE_HPP */