#include "widget.h"
//#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <wrap/qt/device_to_logical.h>
#include <wrap/gl/pick.h>

Widget::Widget(QWidget *parent)
	: QGLWidget(parent), 
	  xRot(0),
	  yRot(0),
	  zRot(0)
{
	vcg::tri::Octahedron(mesh);
//	vcg::tri::Dodecahedron(mesh);
	
//	vcg::tri::UpdateNormal<CMeshO>::PerVertex(mesh);
	vcg::tri::UpdateNormal<CMeshO>::PerFace(mesh);
	attrVertex = vcg::tri::Allocator<CMeshO>::GetPerMeshAttribute<QVector<QVector3D>>(mesh,"vertex");
	attrVertexNormal = vcg::tri::Allocator<CMeshO>::GetPerMeshAttribute<QVector<QVector3D>>(mesh,"normal");
	
	// init attrVertex and vertex index array
	vertices = &attrVertex();
	normals = &attrVertexNormal();
//	vcg::SimpleTempData<typename CMeshO::VertContainer, GLuint> vert_indices(mesh.vert);
//	GLuint i=0;
//	CMeshO::VertexIterator vi;
//	for(i=0,vi=mesh.vert.begin(); vi!=mesh.vert.end(); ++vi,++i){
//		QVector3D p(vi->P()[0], vi->P()[1], vi->P()[2]);
////		vertices->push_back(p);
////		colors.push_back(QVector3D(1,0,0));
//		vert_indices[vi] = i; // vertex point to vertex index
		
////		// init attrVertexNormal
////		QVector3D n(vi->N()[0], vi->N()[1], vi->N()[2]);
////		normals->push_back(n);
//	}
	
	//init attrIndex
	CMeshO::FaceIterator fi;
	for(fi=mesh.face.begin(); fi!=mesh.face.end(); ++fi){
		CMeshO::FacePointer pf = &*fi;
		for(int k=0; k<pf->VN(); ++k){
			CVertexO *pv = pf->cV(k);
//			GLuint vidx = vert_indices[pv];
			QVector3D p(pv->P()[0], pv->P()[1], pv->P()[2]);
			vertices->push_back(p);
			// init color
			colors.push_back(QVector3D(1,0,0));
			// init attrVertexNormal
			QVector3D n(fi->N()[0], fi->N()[1], fi->N()[2]); // use face normal
			normals->push_back(n);
		}
	}
}
Widget::~Widget(){
}

static void qNormalizeAngle(int &angle)
{
	while (angle < 0)
		angle += 360 * 16;
	while (angle > 360 * 16)
		angle -= 360 * 16;
}

void Widget::setXRotation(int angle){
	qNormalizeAngle(angle);
	if (angle != xRot) {
		xRot = angle;
		emit xRotationChanged(angle);
		update();
	}
}

void Widget::setYRotation(int angle){
	qNormalizeAngle(angle);
	if (angle != yRot) {
		yRot = angle;
		emit yRotationChanged(angle);
		update();
	}
}

void Widget::setZRotation(int angle){
	qNormalizeAngle(angle);
	if (angle != zRot) {
		zRot = angle;
		emit zRotationChanged(angle);
		update();
	}
}

void Widget::initializeGL(){
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	world.setToIdentity();
	proj.setToIdentity();
	camera.setToIdentity();
	camera.translate(0,0,-5);
}
void Widget::paintGL(){
//	QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
	
//	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
//	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE); // gl_PointSize in vertex shader
//	glEnable(GL_POINT_SPRITE);	// enable gl_PointCoord in fragment shader -> can not use current gl version

	world.setToIdentity();
	
//	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, indecies->constData());
	glDrawArrays(GL_TRIANGLES, 0, vertices->size());
//	glDrawElements(GL_TRIANGLES, indecies->size(), GL_UNSIGNED_INT, nullptr);
//	glDrawElements(GL_POINTS, indecies->size(), GL_UNSIGNED_INT, nullptr); // draw only vertex points
//	glDrawElements(GL_LINE_STRIP, indecies->size(), GL_UNSIGNED_INT, nullptr); // draw triangle lines(GL_LINE_LOOP is same result)
	
//	glDrawElements(GL_TRIANGLES, indecies->size(), GL_UNSIGNED_INT, indecies->constData());
	pick();
	
//	float ff;
//	glGetFloatv(GL_PROJECTION_MATRIX, &ff);
	
	if(isDragging){
		DrawXORRect();
	}
}
void Widget::resizeGL(int w, int h){
	proj.setToIdentity();
	proj.perspective(45.0f, GLfloat(w) / h, 0.01f, 100.0f);
}

void Widget::mousePressEvent(QMouseEvent *event){
//    lastPos = event->pos();
	start = QTLogicalToOpenGL(this, event->pos());
	cur = start;
//	qDebug("start: %f, %f", start.X(), start.Y());
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
//    int dx = event->x() - lastPos.x();
//    int dy = event->y() - lastPos.y();

//    if (event->buttons() & Qt::LeftButton) {
//        setXRotation(xRot - 8 * dy);
//        setYRotation(yRot - 8 * dx);
//    } else if (event->buttons() & Qt::RightButton) {
//        setXRotation(xRot - 8 * dy);
//        setZRotation(zRot - 8 * dx);
//    }
//    lastPos = event->pos();
	
	prev = cur;
	cur = QTLogicalToOpenGL(this, event->pos());
	isDragging = true;
	this->update();
}

void Widget::mouseReleaseEvent(QMouseEvent *event){
	prev = cur;
	cur = QTLogicalToOpenGL(this, event->pos());
	isDragging = false;
}
void Widget::DrawXORRect(){
}

void Widget::pick(){
//	QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
	vector<CMeshO::FacePointer>::iterator fpi;
	// Starting Sel
	vector<CMeshO::FacePointer> NewSelFace;
	vcg::Point2f mid = (start + cur) / 2;
	vcg::Point2f wid = vcg::Abs(start - cur);
	
	QMatrix4x4 Mq = proj*camera*world;
	Eigen::Matrix<CMeshO::ScalarType,4,4> M;
	for(int i=0; i<4; ++i){
		for(int j=0; j<4; ++j){
			M(i,j) = Mq(i,j); // more better way https://stackoverflow.com/questions/36733601/converting-eigen-4x4-matrix-to-qmatrix4x4
		}
	}
	int hit = vcg::GLPickTri<CMeshO>::PickVisibleFace(mid[0], mid[1], mesh, NewSelFace, wid[0], wid[1]);
//	qDebug("hit:%d", hit);
	vcg::tri::UpdateSelection<CMeshO>::FaceClear(mesh);
	for (fpi = NewSelFace.begin(); fpi != NewSelFace.end(); ++fpi){
		(*fpi)->SetS();
//		qDebug("select");
	}
	if(hit>0){
		initAttribute();
	}
}
void Widget::initAttribute(){
//	vcg::tri::UpdateNormal<CMeshO>::PerFace(mesh);
	attrVertex = vcg::tri::Allocator<CMeshO>::GetPerMeshAttribute<QVector<QVector3D>>(mesh,"vertex");
	attrVertexNormal = vcg::tri::Allocator<CMeshO>::GetPerMeshAttribute<QVector<QVector3D>>(mesh,"normal");
	
	// init attrVertex and vertex index array
	vertices = &attrVertex();
	normals = &attrVertexNormal();
	
	vertices->clear();
	normals->clear();
	colors.clear();
	
	//init attrIndex
	CMeshO::FaceIterator fi;
	for(fi=mesh.face.begin(); fi!=mesh.face.end(); ++fi){
		CMeshO::FacePointer pf = &*fi;
		for(int k=0; k<pf->VN(); ++k){
			CVertexO *pv = pf->cV(k);
			QVector3D p(pv->P()[0], pv->P()[1], pv->P()[2]);
			vertices->push_back(p);
			// init color
			QVector3D col;
			if(fi->IsS()){
				col = QVector3D(1,1,0);
			}else{
				col = QVector3D(1,0,0);
			}
			colors.push_back(col);
			// init attrVertexNormal
			QVector3D n(fi->N()[0], fi->N()[1], fi->N()[2]); // use face normal
			normals->push_back(n);
		}
	}
}
