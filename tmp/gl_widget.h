#ifndef GL_WIDGET_H
#define GL_WIDGET_H

#include <QtOpenGL/QGLWidget>
#include <djah/math.hpp>
#include <djah/3d/shader.hpp>
#include <djah/3d/primitives/batcher.hpp>
#include <djah/system/input/keyboard.hpp>
#include <djah/system/input/mouse.hpp>

class mesh_builder;
class d3d_object;


class Camera
{
public:

	Camera()
	{
		h = 1.8f;
		trans_.setRotation( djah::math::quatf(0.0f,0.0f,0.0f,1.0f) );
	}

	void move(const djah::math::vector3f &newPosition)
	{
		movex(newPosition.x);
		movey(newPosition.y);
		movez(newPosition.z);
	}

	void movex(float xmmod)
	{
		if( xmmod != 0.0f )
		{
			trans_.translate( djah::math::rotate(trans_.rotation(), djah::math::vector3f(xmmod, 0.0f, 0.0f)) );
		}
	}

	void movey(float ymmod)
	{
		h += ymmod;
		if( ymmod != 0.0f )
			trans_.translate( djah::math::vector3f(0.0f, -ymmod, 0.0f) );
	}

	void movez(float zmmod)
	{
		if( zmmod != 0.0f )
			trans_.translate( djah::math::rotate(trans_.rotation(), djah::math::vector3f(0.0f, 0.0f, zmmod)) );
	}

	void rotatex(float xrmod)
	{
		if( xrmod != 0.0f )
		{
			const djah::math::quatf &nrot = djah::math::make_quaternion(djah::math::deg_to_rad(xrmod), djah::math::vector3f(-1.0f, 0.0f, 0.0f));
			trans_.rotate( nrot );
		}
	}

	void rotatey(float yrmod)
	{
		if( yrmod != 0.0f )
		{
			const djah::math::quatf &nrot = djah::math::make_quaternion(djah::math::deg_to_rad(yrmod), djah::math::vector3f(0.0f, -1.0f, 0.0f));
			trans_.setRotation( nrot * trans_.rotation() );
		}
	}

	djah::math::vector3f eye() const
	{
		djah::math::vector3f e = trans_.translation();
		e.y = h;
		return e;
	}

	djah::math::vector3f center() const
	{
		return eye() + djah::math::rotate( trans_.rotation(), djah::math::vector3f(0.0f, 0.0f, -1.0f) );
	}

	djah::math::vector3f up() const
	{
		return  djah::math::rotate( trans_.rotation(), djah::math::vector3f(0.0f, 1.0f, 0.0f) );
	}

	djah::math::transformation_f trans_;
	float h;
};



class gl_widget
	: public QGLWidget
{
	Q_OBJECT;

public:
	gl_widget(QWidget *parent);
	virtual ~gl_widget();

	// QGLWidget overrides
	virtual void initializeGL();
	virtual void resizeGL(int w, int h);
	virtual void paintGL();

	// Events
	virtual void timerEvent( QTimerEvent * event );

	// Mesh
	void setMesh(mesh_builder *pMeshBuilder);
	void setTexture(const std::string &fileName);
	djah::math::transformation_f& transformation() { return trans_; }
	float snapToGrid();

private:
	void drawMesh(const djah::math::matrix4f &viewMat);
	void cleanUpMeshes();

private:
	std::vector<d3d_object*> d3dObjects_;
	mesh_builder *pMeshBuilder_;
	djah::math::transformation_f trans_;
	djah::d3d::shader *pShader_;
	djah::math::matrix4f perspectiveMat_;
	djah::system::input::keyboard keyboard_;
	djah::system::input::mouse mouse_;
	djah::opengl::texture *pTexture_;
	Camera cam_;
};

#endif /* GL_WIDGET_H */