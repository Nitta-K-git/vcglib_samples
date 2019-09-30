#include "widget.h"
//#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <wrap/qt/device_to_logical.h>
#include <wrap/gl/pick.h>
#include <gl/GLU.h>
#include <QApplication>
#include <wrap/qt/trackball.h>
#include <wrap/gl/pick.h>

Widget::Widget(QWidget *parent)
	: QGLWidget(parent)
{
	//	vcg::tri::Octahedron(mesh);
	vcg::tri::Dodecahedron(mesh);
	//	vcg::tri::Dodecahedron(mesh);
	
	//	vcg::tri::UpdateNormal<CMeshO>::PerVertex(mesh);
	vcg::tri::UpdateNormal<CMeshO>::PerFace(mesh);
	vcg::tri::UpdateBounding<CMeshO>::Box(mesh);
	attrVertex = vcg::tri::Allocator<CMeshO>::GetPerMeshAttribute<QVector<QVector3D>>(mesh,"vertex");
	attrVertexNormal = vcg::tri::Allocator<CMeshO>::GetPerMeshAttribute<QVector<QVector3D>>(mesh,"normal");
	
	// init attrVertex and vertex index array
	vertices = &attrVertex();
	normals = &attrVertexNormal();
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
	
	fov = fovDefault();
	clipRatioFar = 5;
	clipRatioNear = clipRatioNearDefault();
	nearPlane = .2f;
	farPlane = 5.f;
	activeDefaultTrackball=true;
	
}
Widget::~Widget(){
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
	//	GLenum err = glewInit();
	//	if (err != GLEW_OK ) {
	//		assert(0);
	//	}
}
void Widget::resetTrackBall(){
	makeCurrent();
	trackball.Reset();
	float newScale= 3.0f/mesh.bbox.Diag();
	trackball.track.sca = newScale;
	trackball.track.tra.Import(-mesh.bbox.Center());
	clipRatioNear = clipRatioNearDefault();
	fov=fovDefault();
	update();
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
	bb.Add(Matrix44m::Construct(mt), mesh.bbox); // 座標変換後のボックス領域
	float fov = 60;
	float cameraDist = viewRatio()/*const*/ / tanf(vcg::math::ToRad(fov*.5f)); // fovで決まる(初期値60.f)
	
	// fovは5ぴったりならorthoモードになる(分かりにくい使い方)
	if(fov<=5) cameraDist = 8.0f; // small hack for orthographic projection where camera distance is rather meaningless...
	
	nearPlane = cameraDist*clipRatioNear;
	farPlane = cameraDist + max(viewRatio(),float(-bb.min[2]));
	
	if(fov==5)
		glOrtho( -viewRatio()*fAspect, viewRatio()*fAspect, -viewRatio(), viewRatio(),  nearPlane, farPlane);
	else
		gluPerspective(fov, fAspect, nearPlane, farPlane);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, cameraDist, 0, 0, 0, 0, 1, 0);
}
void Widget::drawMesh(){
	//	world.setToIdentity();
	
	////	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, indecies->constData());
	//	glDrawArrays(GL_TRIANGLES, 0, vertices->size());
	////	glDrawElements(GL_TRIANGLES, indecies->size(), GL_UNSIGNED_INT, nullptr);
	////	glDrawElements(GL_POINTS, indecies->size(), GL_UNSIGNED_INT, nullptr); // draw only vertex points
	////	glDrawElements(GL_LINE_STRIP, indecies->size(), GL_UNSIGNED_INT, nullptr); // draw triangle lines(GL_LINE_LOOP is same result)
	
	////	glDrawElements(GL_TRIANGLES, indecies->size(), GL_UNSIGNED_INT, indecies->constData());
	
	
	
	glPushAttrib(GL_ENABLE_BIT|GL_VIEWPORT_BIT| GL_CURRENT_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_LIGHTING);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth(1.f);
	glDepthRange (0.0, 0.999);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	//	glMultMatrix(tr);
	//	if (TV.size() > 0)
	//	{
	//		glEnableClientState (GL_VERTEX_ARRAY);
	//		glEnableClientState (GL_COLOR_ARRAY);
	//		glVertexPointer(3,vcg::GL_TYPE_NM<Scalarm>::SCALAR(),sizeof(PointPC),&(TV.begin()[0].first));
	//		glColorPointer(4,GL_UNSIGNED_BYTE,sizeof(PointPC),&(TV.begin()[0].second));
	//		glDrawArrays(GL_TRIANGLES,0,TV.size());
	//		glDisableClientState (GL_COLOR_ARRAY);
	//		glDisableClientState (GL_VERTEX_ARRAY);
	//	}
	{
		//		glEnableClientState (GL_VERTEX_ARRAY);
		//		glEnableClientState (GL_COLOR_ARRAY);
		//		glVertexPointer(3,vcg::GL_TYPE_NM<Scalarm>::SCALAR(),sizeof(PointPC),&(TV.begin()[0].first));
		//		glColorPointer(4,GL_UNSIGNED_BYTE,sizeof(PointPC),&(TV.begin()[0].second));
		//		glDrawArrays(GL_TRIANGLES, 0, vertices->size());
		
		//		glDisableClientState (GL_COLOR_ARRAY);
		//		glDisableClientState (GL_VERTEX_ARRAY);
		
		glEnable(GL_LIGHTING);
		glBegin(GL_TRIANGLES);
//		for(auto &&p : *vertices){
//			glVertex3f(p.x(), p.y(), p.z());
//		}
		for(auto &&f : mesh.face){
			for(int i=0; i<f.VN(); ++i){
				auto p = f.P(i);
				glVertex3f(p.X(), p.Y(), p.Z());
				glNormal3f(f.N().X(), f.N().Y(), f.N().Z());
			}
		}
		glEnd();
		glDisable(GL_LIGHTING);
	}
	glPopMatrix();
	glPopAttrib();
	
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
	
	if (!(isDefaultTrackBall()))
	{
		glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
		glColor3f(1,1,0);
		glDisable(GL_LIGHTING);
		const unsigned int lineNum=3;
		glBegin(GL_LINES);
		for(unsigned int i=0;i<=lineNum;++i)
			for(unsigned int j=0;j<=lineNum;++j) {
				glVertex3f(-1.0f+i*2.0f/lineNum,-1.0f+j*2.0f/lineNum,-2);
				glVertex3f(-1.0f+i*2.0f/lineNum,-1.0f+j*2.0f/lineNum, 2);
			}
		glEnd();
		glPopAttrib();
	}
	glPopMatrix();
	if(!isDefaultTrackBall()) trackball_light.DrawPostApply();
}
void Widget::paintGL(){
	makeCurrent();
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
	
	// drawMeshする
	drawMesh();
	
	pick();
	
	glPopMatrix(); // We restore the state to immediately after the trackball (and before the bbox scaling/translating)
	
	//	if(trackBallVisible && !takeSnapTile && !(iEdit && !suspendedEditor))
	trackball.DrawPostApply();
	
	glPopMatrix(); // We restore the state to immediately before the trackball
	glFlush();
	glFinish();
}
void Widget::resizeGL(int w, int h){
	//	proj.setToIdentity();
	//	proj.perspective(45.0f, GLfloat(w) / h, 0.01f, 100.0f);
}
void Widget::pick(){
	vector<CMeshO::FacePointer>::iterator fpi;
	// Starting Sel
	vector<CMeshO::FacePointer> NewSelFace;
	vcg::Point2f mid = (start + cur) / 2;
	vcg::Point2f wid = vcg::Abs(start - cur);

	glPushMatrix();
//		glMultMatrix(mesh.Tr);
	{
		//m.cm.selvert.clear();
		vector<CMeshO::VertexPointer> NewSelVert;
		vector<CMeshO::VertexPointer>::iterator vpi;

		vcg::GLPickTri<CMeshO>::PickVert(mid[0], mid[1], mesh, NewSelVert, wid[0], wid[1]);
		glPopMatrix();
		vcg::tri::UpdateSelection<CMeshO>::VertexClear(mesh);

		for (vpi = NewSelVert.begin(); vpi != NewSelVert.end(); ++vpi){
			qDebug("pick: %d", vcg::tri::Index(mesh, *vpi));
			(*vpi)->SetS();
		}
	}
}
void Widget::mousePressEvent(QMouseEvent *e){
	makeCurrent();
	e->accept();
	if (!this->hasFocus()){
		this->setFocus();
	}
	
	if ((e->modifiers() & Qt::ShiftModifier) && (e->modifiers() & Qt::ControlModifier) && (e->button() == Qt::LeftButton))
		activeDefaultTrackball = false;
	else
		activeDefaultTrackball = true;
	
	if(isDefaultTrackBall()){
		if (QApplication::keyboardModifiers() & Qt::Key_Control) trackball.ButtonDown(QT2VCG(Qt::NoButton, Qt::ControlModifier));
		else trackball.ButtonUp(QT2VCG(Qt::NoButton, Qt::ControlModifier));
		if (QApplication::keyboardModifiers() & Qt::Key_Shift) trackball.ButtonDown(QT2VCG(Qt::NoButton, Qt::ShiftModifier));
		else trackball.ButtonUp(QT2VCG(Qt::NoButton, Qt::ShiftModifier));
		if (QApplication::keyboardModifiers() & Qt::Key_Alt) trackball.ButtonDown(QT2VCG(Qt::NoButton, Qt::AltModifier));
		else trackball.ButtonUp(QT2VCG(Qt::NoButton, Qt::AltModifier));
		
		trackball.MouseDown(QT2VCG_X(this, e), QT2VCG_Y(this, e), QT2VCG(e->button(), e->modifiers()));
	}else
		trackball_light.MouseDown(QT2VCG_X(this, e), QT2VCG_Y(this, e), QT2VCG(e->button(), Qt::NoModifier));
	update();
	
	start = QTLogicalToOpenGL(this, e->pos());
	cur = QTLogicalToOpenGL(this, e->pos()+QPoint(20,20));
}

void Widget::mouseMoveEvent(QMouseEvent *e){
	makeCurrent();
	if (isDefaultTrackBall()){
		trackball.MouseMove(QT2VCG_X(this,e), QT2VCG_Y(this,e));
	}
	else trackball_light.MouseMove(QT2VCG_X(this,e), QT2VCG_Y(this,e));
	update();
}

void Widget::mouseReleaseEvent(QMouseEvent *e){
	makeCurrent();
	//clearFocus();
	activeDefaultTrackball=true;
	if (isDefaultTrackBall()) trackball.MouseUp(QT2VCG_X(this,e), QT2VCG_Y(this,e), QT2VCG(e->button(), e->modifiers() ) );
	else trackball_light.MouseUp(QT2VCG_X(this,e), QT2VCG_Y(this,e), QT2VCG(e->button(),e->modifiers()) );
	// setCursorTrack(trackball.current_mode);
	
	update();
}
void Widget::wheelEvent(QWheelEvent *e){
	makeCurrent();
	setFocus();
	const int WHEEL_STEP = 120;
	float notch = e->delta()/ float(WHEEL_STEP);
	switch(e->modifiers()){
	case Qt::ControlModifier: 
	{
		clipRatioNear = vcg::math::Clamp(clipRatioNear*powf(1.1f, notch),0.01f,500.0f); 
		break;
	}
	case Qt::ShiftModifier: 
	{
		fov = vcg::math::Clamp(fov+1.2f*notch,5.0f,90.0f); 
		break;
	}
	case Qt::AltModifier: // point sizeの調整
	{
		//				glas.pointSize = vcg::math::Clamp(glas.pointSize*powf(1.2f, notch), MLPerViewGLOptions::minPointSize(), MLPerViewGLOptions::maxPointSize());
		//                MLSceneGLSharedDataContext* cont = mvc()->sharedDataContext();
		//                if (cont != NULL)
		//                {
		//					foreach(MeshModel * mp, this->md()->meshList)
		//					{
		//						MLRenderingData dt;
		//						cont->getRenderInfoPerMeshView(mp->id(), context(), dt);
		//						MLPerViewGLOptions opt;
		//						dt.get(opt);
		//						opt._perpoint_pointsize = glas.pointSize;
		//						opt._perpoint_pointsmooth_enabled = glas.pointSmooth;
		//						opt._perpoint_pointattenuation_enabled = glas.pointDistanceAttenuation;
		//						cont->setGLOptions(mp->id(), context(), opt);
		//					}
		//					if (mw() != NULL)
		//						mw()->updateLayerDialog();
		//                }
		break;
	}
	default:
		trackball.MouseWheel( e->delta()/ float(WHEEL_STEP));
		break;
	}
	update();
}
void Widget::keyReleaseEvent(QKeyEvent *e){
	makeCurrent();
	e->ignore();
	if(e->key()==Qt::Key_Control) trackball.ButtonUp(QT2VCG(Qt::NoButton, Qt::ControlModifier ) );
	if(e->key()==Qt::Key_Shift) trackball.ButtonUp(QT2VCG(Qt::NoButton, Qt::ShiftModifier ) );
	if(e->key()==Qt::Key_Alt) trackball.ButtonUp(QT2VCG(Qt::NoButton, Qt::AltModifier ) );
}
void Widget::keyPressEvent (QKeyEvent *e){
	makeCurrent();
	e->ignore();
	if(e->key()==Qt::Key_Control) trackball.ButtonDown(QT2VCG(Qt::NoButton, Qt::ControlModifier ) );
	if(e->key()==Qt::Key_Shift) trackball.ButtonDown(QT2VCG(Qt::NoButton, Qt::ShiftModifier ) );
	if(e->key()==Qt::Key_Alt) trackball.ButtonDown(QT2VCG(Qt::NoButton, Qt::AltModifier ) );
	if(e->key()==Qt::Key_J){
		resetTrackBall();
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
