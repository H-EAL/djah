#include "djah/video/text.hpp"
#include "djah/video/font_engine.hpp"

namespace djah { namespace video {

	//----------------------------------------------------------------------------------------------
	text::text(const std::string &str, int x, int y, const properties &props)
		: std::string(str)
		, position_(x,y)
		, properties_(props)
	{
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void text::draw() const
	{
		font_engine::get().setFont(properties_.font_name, properties_.font_size);
		font_engine::get().setColor(properties_.font_color);
		font_engine::get().print(*this, position_.x, position_.y + properties_.font_size);
	}
	//----------------------------------------------------------------------------------------------

} /*video*/ } /*djah*/