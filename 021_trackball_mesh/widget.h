#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

//#include <QOpenGLWidget>
//#include <QOpenGLShader>
//#include <QOpenGLVertexArrayObject>
//#include <QOpenGLBuffer>
//#include <QOpenGLFunctions>

#include <QGLWidget>
//#include <QGLShaderProgram>

#include <QMatrix4x4>
#include <QVector>
#include <QVector3D>

#include <common/ml_mesh_type.h>
#include <vcg/complex/algorithms/create/platonic.h>
//#include <vcg/container/simple_temporary_data.h>
#include <vcg/complex/complex.h>

#include <QtDebug>
#include <wrap/gui/trackball.h>

class Widget : public QGLWidget //, protected QOpenGLFunctions
{
	Q_OBJECT
public:
	Widget(QWidget *parent = 0);
	~Widget();
	
	void resetTrackBall();
	
	vcg::Trackball trackball;
	vcg::Trackball trackball_light;
	
protected:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int width, int height) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void wheelEvent(QWheelEvent*e) override;
	void keyPressEvent (QKeyEvent *e) override;
	void keyReleaseEvent(QKeyEvent *e) override;
private:
	enum{
		vertexLoc = 0,
		normalLoc = 1,
		colorLoc = 2, 
	};
	QVector<QVector3D> *vertices;
	QVector<QVector3D> *normals;
	QVector<QVector3D> colors;
	QVector<QVector3D> pick_rect;
	
	int projMatLocation;
	int moveMatLocation;
	int normalMatrixLocation;
	int lightPosLocation;
	int projLocPick;
	int moveLocPick;
	int vertLocPick;
	
	CMeshO mesh;
	CMeshO::PerMeshAttributeHandle<QVector<QVector3D>> attrVertex;
	CMeshO::PerMeshAttributeHandle<QVector<QVector3D>> attrVertexNormal;
	
	// cursor picking
	vcg::Point2f start;
	vcg::Point2f cur;
	vcg::Point2f prev;
	bool isDragging;
	
	void initAttribute();
	
	void setView();
	void drawLight();
	// view setting variables
	float fov;
	float clipRatioFar;
	float clipRatioNear;
	float nearPlane;
	float farPlane;
	bool activeDefaultTrackball; // keep track on active trackball
	
	inline float viewRatio() const { return 1.75f; }
	inline float clipRatioNearDefault() const { return 0.1f; }
	inline float fovDefault() const { return 60.f; }
	bool isDefaultTrackBall()   {return activeDefaultTrackball;}
	void drawMesh();
};
#endif // WIDGET_H
