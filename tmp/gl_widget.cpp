#include "gl_widget.h"
#include <QMouseEvent>
#include <djah/math.hpp>
#include "../../collada_converter/sources/mesh_builder.hpp"
#include <djah/opengl.hpp>
#include <djah/system/gl.hpp>
#include <djah/3d/primitives.hpp>

using namespace djah;

//--------------------------------------------------------------------------------------------------
class d3d_object
{
public:
	d3d_object(submesh *sm, d3d::shader &shaderProg)
	{
		assert(sm != nullptr);

		auto itEnd = sm->attributeBuffers_.end();
		for( auto it = sm->attributeBuffers_.begin(); it != itEnd; ++it )
		{
			// Create new vertex buffer
			const unsigned int vbSize = it->data.size();
			opengl::vertex_buffer *newVB = new opengl::vertex_buffer(vbSize * sizeof(float), opengl::eBU_StaticDraw);
			newVB->write(&it->data[0], vbSize);

			// Add it to the list
			buffers_.push_back(newVB);

			// Generate vertex format
			const opengl::vertex_format &vertexFormat = generateVertexFormat(*it);

			// Add to the vertex array
			vertexArray_.addVertexBuffer(newVB, vertexFormat);
		}

		vertexArray_.setVertexCount( sm->vertex_count_ );
		vertexArray_.init(shaderProg.program());
	}

	~d3d_object()
	{
		auto itEnd = buffers_.end();
		for(auto it = buffers_.begin(); it != itEnd; ++it)
			delete (*it);
	}

	opengl::vertex_format generateVertexFormat(const submesh::attribute_buffer_t &buf)
	{
		opengl::vertex_format vf(opengl::vertex_format::ePT_Packed);

		std::string attrName = buf.semantic;// + "_";
		//attrName += char('0' + buf.set);

		vf << opengl::format::vertex_attrib_base(attrName, sizeof(float), buf.stride, GL_FLOAT, 0);

		return vf;
	}

	void draw()
	{
		vertexArray_.draw(GL_TRIANGLES);
	}

private:
	std::vector<opengl::vertex_buffer*> buffers_;
	opengl::vertex_array vertexArray_;
};
//--------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------
gl_widget::gl_widget(QWidget *parent)
	: QGLWidget(parent)
	, pMeshBuilder_(nullptr)
	, pShader_(nullptr)
	, pTexture_(nullptr)
{
	setMouseTracking(true);
	startTimer(1000/60);
	setFocusPolicy( Qt::StrongFocus );
	setFocus();
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
gl_widget::~gl_widget()
{
	cleanUpMeshes();
	delete pShader_;
	delete pTexture_;
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void gl_widget::initializeGL()
{
	djah::system::load_extensions();

	pShader_ = new d3d::shader("viewer");

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}

//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
void gl_widget::resizeGL(int w, int h)
{
	glViewport(0, 0, (GLint)w, (GLint)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	perspectiveMat_ = math::make_perspective_projection(60.0f, (float)w/(float)h, 0.01f, 1000.0f);
	glMultMatrixf(&perspectiveMat_[0][0]);
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void gl_widget::setMesh(mesh_builder *pMeshBuilder)
{
	pMeshBuilder_ = pMeshBuilder;
	cleanUpMeshes();

	if( pMeshBuilder_ == nullptr )
		return;

	const unsigned int nbModels = pMeshBuilder_->getModelCount();
	for(unsigned int m = 0; m < nbModels; ++m)
	{
		model *mod = pMeshBuilder_->getModel(m);
		const unsigned int nbMeshes = mod->getMeshCount();
		for(unsigned int b = 0; b < nbMeshes; ++b)
		{
			submesh *msh = mod->getMesh(b);
			if(msh != nullptr)
			{
				d3d_object *newD3dObj = new d3d_object(msh, *pShader_);
				d3dObjects_.push_back( newD3dObj );
			}
		}
	}
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void gl_widget::setTexture(const std::string &fileName)
{
	delete pTexture_;
	pTexture_ = nullptr;

	QImage img1(fileName.c_str());
	if( img1.isNull() )
		return;

	QImage img = QGLWidget::convertToGLFormat(img1);
	if( img.isNull() )
		return;

	pTexture_ = new djah::opengl::texture(GL_RGB, img.width(), img.height());
	pTexture_->bind();
	pTexture_->setBestFiltering();
	pTexture_->setPixelBuffer(GL_RGBA, GL_UNSIGNED_BYTE, img.bits());
	pTexture_->unbind();
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
float gl_widget::snapToGrid()
{
	if( pMeshBuilder_ == nullptr )
		return 0.0f;

	float minY = std::numeric_limits<float>::max();

	const unsigned int nbModels = pMeshBuilder_->getModelCount();
	for(unsigned int m = 0; m < nbModels; ++m)
	{
		model *mod = pMeshBuilder_->getModel(m);
		math::vector3f ptMin = math::rotate(trans_.rotation(), mod->min_);
		math::vector3f ptMax = math::rotate(trans_.rotation(), mod->max_);

		ptMin.y *= trans_.scale().y;
		ptMax.y *= trans_.scale().y;

		minY = std::min(std::min(minY, ptMin.y), ptMax.y);
	}

	trans_.setTranslation( math::vector3f(0.0f, -minY, 0.0f) );

	return -minY;
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void gl_widget::cleanUpMeshes()
{
	auto itEnd = d3dObjects_.end();
	for( auto it = d3dObjects_.begin(); it != itEnd; ++it )
		delete (*it);

	d3dObjects_.clear();
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void gl_widget::timerEvent( QTimerEvent * event )
{
	Q_UNUSED(event);

	keyboard_.update();
	mouse_.update();

	if( this->hasFocus() )
	{
		djah::math::vector3f m;
		float rx = 0.0f;
		float ry = 0.0f;

		if( keyboard_.isKeyDown(djah::system::input::eKC_A) )
			m.x -= 1.0f;
		if( keyboard_.isKeyDown(djah::system::input::eKC_D) )
			m.x += 1.0f;
		if( keyboard_.isKeyDown(djah::system::input::eKC_Q) )
			m.y -= 1.0f;
		if( keyboard_.isKeyDown(djah::system::input::eKC_E) )
			m.y += 1.0f;
		if( keyboard_.isKeyDown(djah::system::input::eKC_W) )
			m.z -= 1.0f;
		if( keyboard_.isKeyDown(djah::system::input::eKC_S) )
			m.z += 1.0f;

		if( mouse_.leftButton().isDown() )
		{
			rx = static_cast<float>(mouse_.delta().y) * 0.7f;
			ry = static_cast<float>(mouse_.delta().x) * 0.7f;
		}

		if( m.lengthSq() > 0.0f )
		{
			const float speed = keyboard_.isKeyDown(djah::system::input::eKC_SHIFT) ? 0.5f : 0.1f;
			m.normalize() *= speed;
			cam_.move(m);
		}
		cam_.rotatex	(rx);
		cam_.rotatey	(ry);
	}
	
	updateGL();
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void drawAxis()
{
	glBegin(GL_LINES);
	{
		//X
		glColor3fv(math::vector3f::x_axis.data);
		glVertex3fv(math::vector3f::null_vector.data);
		glVertex3fv(math::vector3f::x_axis.data);
		//Y
		glColor3fv(math::vector3f::y_axis.data);
		glVertex3fv(math::vector3f::null_vector.data);
		glVertex3fv(math::vector3f::y_axis.data);
		//Z
		glColor3fv(math::vector3f::z_axis.data);
		glVertex3fv(math::vector3f::null_vector.data);
		glVertex3fv(math::vector3f::z_axis.data);
	}
	glEnd();
}
//--------------------------------------------------------------------------------------------------
void drawBoundingBox(const math::vector3f &bottomLeftBack, const math::vector3f &topRightFront)
{
	const math::vector3f bottomRightBack (topRightFront.x , bottomLeftBack.y, bottomLeftBack.z);
	const math::vector3f bottomLeftFront (bottomLeftBack.x, bottomLeftBack.y, topRightFront.z );
	const math::vector3f bottomRightFront(topRightFront.x , bottomLeftBack.y, topRightFront.z );

	const math::vector3f topRightBack(topRightFront.x , topRightFront.y, bottomLeftBack.z );
	const math::vector3f topLeftFront(bottomLeftBack.x, topRightFront.y, topRightFront.z );
	const math::vector3f topLeftBack (bottomLeftBack.x, topRightFront.y, bottomLeftBack.z);

	glBegin(GL_LINE_LOOP);
	{
		glVertex3fv(topLeftFront.data);
		glVertex3fv(topRightFront.data);
		glVertex3fv(bottomRightFront.data);
		glVertex3fv(bottomLeftFront.data);
	}
	glEnd();

	glBegin(GL_LINE_LOOP);
	{
		glVertex3fv(topLeftBack.data);
		glVertex3fv(topRightBack.data);
		glVertex3fv(bottomRightBack.data);
		glVertex3fv(bottomLeftBack.data);
	}
	glEnd();


	glBegin(GL_LINES);
	{
		glVertex3fv(topLeftFront.data);
		glVertex3fv(topLeftBack.data);

		glVertex3fv(topRightFront.data);
		glVertex3fv(topRightBack.data);

		glVertex3fv(bottomRightFront.data);
		glVertex3fv(bottomRightBack.data);

		glVertex3fv(bottomLeftFront.data);
		glVertex3fv(bottomLeftBack.data);
	}
	glEnd();
}
//--------------------------------------------------------------------------------------------------
void drawGrid(float w = 100, float h = 100, float cw = 1.0f, float ch = 1.0f)
{
	glPushMatrix();
	glBegin(GL_LINES);
	static const float e = -0.001f;
	static const float c = 0.3f;
	w *= cw;
	h *= ch;
	glColor3f(c,c,c);
	for(float x = -w/2; x < w/2+1; x+=cw)
	{
		glVertex3f((float)x*cw, e, (float)h/2*ch);
		glVertex3f((float)x*cw, e, (float)-h/2*ch);
	}
	for(float y = -h/2; y < h/2+1; y+=ch)
	{
		glVertex3f((float)w/2*cw, e, (float)y*ch);
		glVertex3f((float)-w/2*cw, e, (float)y*ch);
	}
	glEnd();
	glPopMatrix();
}
//--------------------------------------------------------------------------------------------------
void gl_widget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	const math::matrix4f &viewMat = math::make_look_at(cam_.eye(), cam_.center(), cam_.up());
	glMultMatrixf(&viewMat[0][0]);

	drawGrid(10,10,3,3);
	drawAxis();

	drawMesh(viewMat);
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void gl_widget::drawMesh(const math::matrix4f &viewMat)
{
	static const math::vector3f colors[] =
	{
		math::vector3f(1,0,0),
		math::vector3f(0,1,0),
		math::vector3f(0,0,1),
		math::vector3f(1,0,1),
		math::vector3f(1,1,0),
		math::vector3f(0,1,1),
		math::vector3f(1,1,1),
	};

	const int nbColors = sizeof(colors)/sizeof(colors[0]);
	int currentColor = 0;

	const math::matrix4f &matVP = viewMat * perspectiveMat_;
	const math::matrix4f &matTrans = trans_.toMatrix4();
	
	pShader_->program().begin();
	pShader_->program().sendUniform("in_World", matTrans);
	pShader_->program().sendUniform("in_WVP", matTrans * matVP);
	pShader_->program().sendUniform("in_UseTexture", pTexture_ != nullptr);

	if(pTexture_)
	{
		pTexture_->bind();
		pShader_->program().sendUniform("in_DiffuseSampler", 0);
	}

	auto itEnd = d3dObjects_.end();
	for( auto it = d3dObjects_.begin(); it != itEnd; ++it )
	{
		pShader_->program().sendUniform("in_Color", colors[currentColor++%nbColors]);
		(*it)->draw();
	}

	opengl::texture::unbind();

	pShader_->program().end();
}
//--------------------------------------------------------------------------------------------------
