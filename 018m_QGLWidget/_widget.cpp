#include "widget.h"
#include <QMouseEvent>
#include <wrap/qt/device_to_logical.h>
#include <wrap/gl/pick.h>
#include <vcg/math/matrix44.h>
#include <vcg/space/deprecated_point3.h>
#include <gl/GLU.h>

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
	makeCurrent();
    glShadeModel(GL_SMOOTH);
    glPixelStorei(GL_PACK_ROW_LENGTH, 0);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    static float diffuseColor[]={1.0,1.0,1.0,1.0};
    glEnable(GL_LIGHT0);
    glDisable(GL_LIGHT1);
    glLightfv(GL_LIGHT1,GL_DIFFUSE,diffuseColor);
    trackball.center=vcg::Point3f(0, 0, 0);
    trackball.radius= 1;

    trackball_light.center=vcg::Point3f(0, 0, 0);
    trackball_light.radius= 1;
//    GLenum err = glewInit();
//    if (err != GLEW_OK ) {
//        assert(0);
//    }
	//doneCurrent();
}
//void Widget::paintGL(){
void Widget::paintEvent(QPaintEvent */*event*/){
	QPainter painter(this);
    painter.beginNativePainting();
	makeCurrent();
	glClearColor(1.0,1.0,1.0,0.0);
	glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    setView();  // Set Modelview and Projection matrix

	drawLight();

    glPushMatrix();
    // Finally apply the Trackball for the model
    trackball.GetView();
    trackball.Apply();

    glPushMatrix();
	// draw
	drawObject();
	glPopMatrix(); // We restore the state to immediately after the trackball (and before the bbox scaling/translating)

	trackball.DrawPostApply();
	
	glPopMatrix(); // We restore the state to immediately before the trackball

	glFlush();
	glFinish();
    painter.endNativePainting();
}
void Widget::resizeGL(int w, int h){
	proj.setToIdentity();
	proj.perspective(45.0f, GLfloat(w) / h, 0.01f, 100.0f);
}

void Widget::mousePressEvent(QMouseEvent *event){
//    lastPos = event->pos();
	start = QTLogicalToOpenGL(this, event->pos());
	cur = start;
	qDebug("start: %f, %f", start.X(), start.Y());
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
	program_pick->bind();
//	QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
	pick_rect.clear();
	pick_rect.push_back(QVector3D(start.X(), start.Y(), 0));
	pick_rect.push_back(QVector3D(cur.X(), start.Y(), 0));
	pick_rect.push_back(QVector3D(cur.X(), cur.Y(), 0));
	pick_rect.push_back(QVector3D(start.X(), cur.Y(), 0));
	
//	qDebug("rect: %f, %f, %f, %f", start.X(), start.Y(), cur.X(), cur.Y());
	
	QMatrix4x4 proj_rect, I;
	proj_rect.ortho(0, QTDeviceWidth(this), 0, QTDeviceHeight(this), -1, 1);
	I.setToIdentity();
	
	program_pick->enableAttributeArray(vertLocPick);
	program_pick->setAttributeArray(vertLocPick, pick_rect.constData()); // set vertex data. 
	program_pick->setUniformValue(projLocPick, proj_rect);
	program_pick->setUniformValue(moveLocPick, I);
	
	glDrawArrays(GL_LINE_LOOP, 0, pick_rect.size());
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

void Widget::setView(){
	makeCurrent();
    glViewport(0,0, (GLsizei) QTLogicalToDevice(this,width()),(GLsizei) QTLogicalToDevice(this,height()));

    GLfloat fAspect = (GLfloat)width()/height();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    vcg::Matrix44f mtTr; mtTr.SetTranslate( trackball.center); // trackballの中心へ移動
    vcg::Matrix44f mtSc; mtSc.SetScale(4.0f,4.0f,4.0f); // 表示倍率
    vcg::Matrix44f mt = mtSc * mtTr * trackball.Matrix() *(-mtTr); // 描画用変換行列
    //    Matrix44f mt =  trackball.Matrix();

    Box3m bb;
    bb.Add(Matrix44m::Construct(mt),mesh.bbox); // 座標変換後のボックス領域
    float cameraDist = viewRatio()/*const*/ / tanf(vcg::math::ToRad(fov*.5f)); // fovで決まる(初期値60.f) //this->getCameraDistance();

	// fovは5ぴったりならorthoモードになる(分かりにくい使い方)
    if(fov<=5) cameraDist = 8.0f; // small hack for orthographic projection where camera distance is rather meaningless...
	

    nearPlane = cameraDist*clipRatioNear;
    farPlane = cameraDist + max(viewRatio(),float(-bb.min[2]));

	gluPerspective(fov, fAspect, nearPlane, farPlane);
	
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, cameraDist, 0, 0, 0, 0, 1, 0);
}

float Widget::getCameraDistance(){
    float cameraDist = viewRatio()/*const*/ / tanf(vcg::math::ToRad(fov*.5f)); // fovで決まる(初期値60.f)
    return cameraDist;
}
void Widget::drawLight(){
	makeCurrent();
    // ============== LIGHT TRACKBALL ==============
    // Apply the trackball for the light direction
    glPushMatrix();
    trackball_light.GetView();
    trackball_light.Apply();

    static float lightPosF[]={0.0,0.0,1.0,0.0};
    glLightfv(GL_LIGHT0,GL_POSITION,lightPosF);
    static float lightPosB[]={0.0,0.0,-1.0,0.0};
    glLightfv(GL_LIGHT1,GL_POSITION,lightPosB);

//    if (!(isDefaultTrackBall()))
//    {
//        glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
//        glColor3f(1,1,0);
//        glDisable(GL_LIGHTING);
//        const unsigned int lineNum=3;
//        glBegin(GL_LINES);
//        for(unsigned int i=0;i<=lineNum;++i)
//            for(unsigned int j=0;j<=lineNum;++j) {
//                glVertex3f(-1.0f+i*2.0/lineNum,-1.0f+j*2.0/lineNum,-2);
//                glVertex3f(-1.0f+i*2.0/lineNum,-1.0f+j*2.0/lineNum, 2);
//            }
//        glEnd();
//        glPopAttrib();
//    }
    glPopMatrix();
//    if(!isDefaultTrackBall()) trackball_light.DrawPostApply();
}
void Widget::drawObject(){
	CMeshO::FaceIterator fi;
	for(fi=mesh.face.begin(); fi!=mesh.face.end(); ++fi){
		if(!fi->IsD()){
			CMeshO::FacePointer pf = &*fi;
			glNormal3f(fi->N()[0], fi->N()[1], fi->N()[2]);
			glBegin(GL_LINE_LOOP);
			for(int k=0; k<pf->VN(); ++k){
				CVertexO *vp = pf->cV(k);
				if(vp->IsS()){
					glColor3f(1,0,0);
				}else{
					QColor c = Qt::red;
//					glColor4i(c.red(), c.blue(), c.green(), c.alpha());
					glColor3f(c.red(), c.green(), c.blue());
//					qDebug() << c;
				}
				glVertex3f(vp->P()[0], vp->P()[1], vp->P()[2]);
			}
			glEnd();
		}
	}
	
}
