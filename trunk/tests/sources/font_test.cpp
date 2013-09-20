#include "font_test.hpp"
#include "djah/types.hpp"
#include "resource_finder.hpp"
#include "djah/3d/primitives.hpp"
#include "djah/3d/font_engine.hpp"
#include "djah/resources/asset_finder.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

using namespace djah;
using namespace d3d;

static const char chars[] =
{
	'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
	'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
	'0','1','2','3','4','5','6','7','8','9',
	'`','-','=','[',']',';','\'',',','.','/','\\', ' ',
	'~','!','@','#','$','%','^','&','*','(',')','_','+','{','}','|','\"',':','?','>','<'
};

struct glyph_metrics_t
{
	geometry::rect_f region;
	math::vector2i   offset;
};

static glyph_metrics_t sGlyphMetrics[sizeof(chars)];
static glyph_metrics_t &glyph = sGlyphMetrics[0];

//--------------------------------------------------------------------------------------------------
FontTest::FontTest(djah::system::device_sptr pDevice, Camera &cam)
	: test_base(pDevice)
	, cam_(cam)
	, pVB_(nullptr)
	, pTexture_(nullptr)
	, textShader_("text")
	, colorShader_("colored")
{
	const float w = static_cast<float>(pDevice_->config().width);
	const float h = static_cast<float>(pDevice_->config().height);
	matOrthoProj_ = math::make_orthographic_projection(0.0f, w, h, 0.0f, -1.0f, 1.0f);

	initFont("fonts/mode_seven.ttf", 14);
	initPlane("The quick brown fox jumps over the lazy dog - 0123456789.");

	opengl::texture::unbind();
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void FontTest::initPlane(const std::string &str, const math::vector2f &position)
{
	/**/
	std::vector<float> planeVertices;
	planeVertices.reserve( str.size() * 16 );
	std::vector<u8> planeIndices;
	planeIndices.reserve(str.size() * 6);

	int i = 0;
	auto itEnd = str.end();
	float pen = 0.0f;
	for(auto it = str.begin(); it != itEnd; ++it)
	{
		int glyphIndex = (int)(std::find(chars, chars + sizeof(chars), (*it)) - chars);
		const glyph_metrics_t &glyph = sGlyphMetrics[glyphIndex];
		const math::vector2f &st0 = glyph.region.topLeft();
		const math::vector2f &st1 = glyph.region.bottomRight();
		const math::vector2f &off = math::cast<float>(glyph.offset);

		planeVertices.push_back(pen + off.x);
		planeVertices.push_back(0.0f + off.y);
		planeVertices.push_back(st0.x);
		planeVertices.push_back(st0.y);

		planeVertices.push_back(pen + off.x);
		planeVertices.push_back( -((sGlyphMetrics[glyphIndex].region.height() ) * pTexture_->height() - off.y) );
		planeVertices.push_back(st0.x);
		planeVertices.push_back(st1.y);

		planeVertices.push_back(pen+sGlyphMetrics[glyphIndex].region.width()*pTexture_->width() + off.x);
		planeVertices.push_back(-(sGlyphMetrics[glyphIndex].region.height() * pTexture_->height() - off.y));
		planeVertices.push_back(st1.x);
		planeVertices.push_back(st1.y);

		planeVertices.push_back(pen+sGlyphMetrics[glyphIndex].region.width()*pTexture_->width() + off.x);
		planeVertices.push_back(0.0f + off.y);
		planeVertices.push_back(st1.x);
		planeVertices.push_back(st0.y);

		pen += glyph.region.width()*pTexture_->width() + off.x;

		planeIndices.push_back(i);
		planeIndices.push_back(i+1);
		planeIndices.push_back(i+2);
		planeIndices.push_back(i);
		planeIndices.push_back(i+3);
		planeIndices.push_back(i+2);
		i += 4;
	}
	/**/
	/**
	// PLANE
	const float planeVertices[] =
	{
		0.0f,0.0f,  0.0f,0.0f,
		0.0f,1.0f,  0.0f,1.0f,
		1.0f,1.0f,  1.0f,1.0f,
		1.0f,0.0f,  1.0f,0.0f,
	};

	const u8 planeIndices[] =
	{
		0,1,2, 0,3,2
	};
	/**/

	opengl::vertex_format vertexFormat;
	vertexFormat.record()
		<< opengl::format::position<2,float>()
		<< opengl::format::tex_coord<2,float>();

	pIB_ = new opengl::index_buffer(planeIndices.size(), opengl::eBU_StaticDraw);
	//pIB_ = new opengl::index_buffer(sizeof(planeIndices), opengl::eBU_StaticDraw);
	pIB_->write(&planeIndices[0], planeIndices.size());
	//pIB_->write(planeIndices);

	pVB_ = new opengl::vertex_buffer(planeVertices.size() * sizeof(float), opengl::eBU_StaticDraw);
	//pVB_ = new opengl::vertex_buffer(sizeof(planeVertices), opengl::eBU_StaticDraw);
	pVB_->write(&planeVertices[0], planeVertices.size());
	//pVB_->write(planeVertices);

	vA_.addVertexBuffer(pVB_, vertexFormat);
	vA_.setIndexBuffer(pIB_);
	vA_.setVertexCount(pVB_->size() / vertexFormat.vertexSize());
	vA_.init(textShader_.program());
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void FontTest::initFont(const std::string &fontFile, int fontSize)
{
	filesystem::stream_ptr strm = filesystem::browser::get().openReadStream(fontFile);

	if( !strm )
		return;

	const size_t buffer_size = strm->size();
	FT_Byte *buffer = new FT_Byte[buffer_size];
	strm->read(buffer, buffer_size);

	FT_Face face;
	FT_New_Memory_Face(font_engine::get().ft_library_, buffer, buffer_size, 0, &face);
	FT_Set_Char_Size(face, 0, fontSize << 6, 0, 0);
	FT_Set_Pixel_Sizes(face, 0, fontSize);

	FT_GlyphSlot slot = face->glyph;

	int widthSum = 0;
	int maxHeight = 0;
	int lines = 1;
	const int maxLineWidth = 256;
	for(u8 c = 0; c < sizeof(chars); ++c)
	{
		if( FT_Load_Char(face, chars[c], FT_LOAD_RENDER) != 0 )
		{
			DJAH_GLOBAL_WARNING() << "Unable to process character '" << chars[c] << "'" << DJAH_END_LOG();
			continue;
		}

		int w = slot->bitmap.width;
		int h = slot->bitmap.rows;
		w = (w%4 == 0) ? w : w + (4-(w%4));
		h = (h%4 == 0) ? h : h + (4-(h%4));

		if( widthSum + w > maxLineWidth )
		{
			widthSum = 0;
			++lines;
		}
		else
		{
			widthSum += w;
		}
		maxHeight = std::max(h, maxHeight);
	}

	const int texWidth  = maxLineWidth;
	const int texHeight = /*_pow2*/(maxHeight * (lines));
	pTexture_ = new opengl::texture(GL_RED, texWidth, texHeight);
	pTexture_->bind();
	pTexture_->setNoFiltering();
	std::vector<u8> blank(texWidth*texHeight,0);
	pTexture_->setPixelBuffer(GL_RED, GL_UNSIGNED_BYTE, &blank[0]);
	
	int widthOffset = 0;
	lines = 0;

	for(u8 c = 0; c < sizeof(chars); ++c)
	{
		if( FT_Load_Char(face, chars[c], FT_LOAD_RENDER) != 0 )
		{
			DJAH_GLOBAL_WARNING() << "Unable to process character '" << chars[c] << "'" << DJAH_END_LOG();
			continue;
		}

		const int w = slot->bitmap.width;
		const int h = slot->bitmap.rows;
		const int width  = (w%4 == 0) ? w : w + (4-(w%4));
		const int height = (h%4 == 0) ? h : h + (4-(h%4));

		std::vector<u8> padded;
		u8 *bitmap = slot->bitmap.buffer;

		if( width != w || height != h )
		{
			padded.resize(width*height);
			for(int j = 0; j < height; ++j)
			{
				for(int i = 0; i < width; ++i)
				{
					padded[i+j*width] = (i>=w||j>=h) ? 0 : slot->bitmap.buffer[i+w*(h-j-1)];
				}
			}
			bitmap = &padded[0];
		}

		if( widthOffset + width >= maxLineWidth )
		{
			widthOffset = 0;
			++lines;
		}

		sGlyphMetrics[c].region = geometry::rect_f
		(
			math::vector2f((float)widthOffset, (float)lines * maxHeight), (float)w, (float)h
		);

		sGlyphMetrics[c].region.topLeft().x /= (float)(pTexture_->width());
		sGlyphMetrics[c].region.topLeft().y /= (float)(pTexture_->height());
		sGlyphMetrics[c].region.bottomRight().x /= (float)(pTexture_->width());
		sGlyphMetrics[c].region.bottomRight().y /= (float)(pTexture_->height());

		sGlyphMetrics[c].offset.x = (slot->advance.x >> 6) - w;
		sGlyphMetrics[c].offset.y = ((slot->metrics.height - slot->metrics.horiBearingY) >> 6);

		pTexture_->updatePixelBuffer(widthOffset, lines * maxHeight, width, height, GL_RED, GL_UNSIGNED_BYTE, bitmap);

		widthOffset += width;
	}
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
FontTest::~FontTest()
{
	delete pIB_;
	delete pVB_;
	delete pTexture_;
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void FontTest::onInit()
{
	opengl::frame_buffer::bind_default_frame_buffer();
	glEnable(GL_BLEND);
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void FontTest::onExit()
{
	glDisable(GL_BLEND);
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void FontTest::update(float dt)
{
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void FontTest::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	static const math::matrix4f matWorld = math::make_translation(5.0f,15.0f,0.0f);

	textShader_.program().begin();
	textShader_.program().sendUniform("in_WVP", matWorld * matOrthoProj_);
	pTexture_->bind();
	vA_.draw();
	pTexture_->unbind();
	textShader_.program().end();
}
//--------------------------------------------------------------------------------------------------