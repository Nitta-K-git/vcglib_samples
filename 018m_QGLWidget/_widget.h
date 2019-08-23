#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <QGLWidget>
#include <QGLShaderProgram>

#include <QMatrix4x4>
#include <QVector>
#include <QVector3D>

#include <common/ml_mesh_type.h>
#include <vcg/complex/algorithms/create/platonic.h>
//#include <vcg/container/simple_temporary_data.h>
#include <vcg/complex/complex.h>
#include <wrap/gui/trackball.h>

#include <QtDebug>

class Widget : public QGLWidget
{
	Q_OBJECT
public:
	Widget(QWidget *parent = 0);
	~Widget();
	
public slots:
	void setXRotation(int angle);
	void setYRotation(int angle);
	void setZRotation(int angle);
	
signals:
	void xRotationChanged(int angle);
	void yRotationChanged(int angle);
	void zRotationChanged(int angle);
	
protected:
	void initializeGL() override;
//	void paintGL() override;
	void resizeGL(int width, int height) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void paintEvent(QPaintEvent *event) override;
private:
	int xRot;
	int yRot;
	int zRot;
	QPoint lastPos;
	
	enum{
		vertexLoc = 0,
		normalLoc = 1,
		colorLoc = 2, 
	};
	QGLShaderProgram *program, *program_pick;
	QMatrix4x4 proj;
	QMatrix4x4 camera;
	QMatrix4x4 world;
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
	void DrawXORRect();
	void pick();
	
	void initAttribute();
	void drawObject();
	
	// meshlabベースの関数
	float fov;
    float clipRatioFar;
    float clipRatioNear;
    float nearPlane;
    float farPlane;
	
	void setView();
	void drawLight();
	float getCameraDistance();
	inline float viewRatio() const { return 1.75f; }
	vcg::Trackball trackball, trackball_light;
};
#endif // WIDGET_H
