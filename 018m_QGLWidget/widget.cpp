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
	program = new QGLShaderProgram;
	program->addShaderFromSourceCode(
				QGLShader::Vertex, 
				"attribute vec4 vertex;\n"
				"attribute vec3 normal;\n"
				"attribute vec3 color;\n"
				"varying vec3 vert;\n" // pass to fragment
				"varying vec3 vertNormal;\n"
				"varying vec3 vertColor;\n"
				"uniform mat4 projMat;\n"
				"uniform mat4 moveMat;\n"
				"uniform mat3 normalMatrix;\n" // world matrix normal
				"void main(void)\n"
				"{\n"
				"   vert = vertex.xyz;\n"
				"   vertNormal = normalMatrix * normal;\n"
				"   vertColor = color.rgb;\n"
				"   gl_Position = projMat * moveMat * vertex;\n"
				"   gl_PointSize = 300.0 / gl_Position.w;\n"
				"}"
				);
	program->addShaderFromSourceCode(
				QGLShader::Fragment, 
				"varying highp vec3 vert;\n"
				"varying highp vec3 vertNormal;\n"
				"varying highp vec3 vertColor;\n"
				"uniform highp vec3 lightPos;\n"
				"void main(void)\n"
				"{\n"
//				"   gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0);\n"
				"   highp vec3 L = normalize(lightPos - vert);\n"
				"   highp float NL = max(dot(normalize(vertNormal), L), 0.0);\n"
//				"   highp vec3 color = vec3(0.39, 1.0, 0.0);\n"
				"   highp vec3 color = vertColor;\n"
				"   highp vec3 col = clamp(color * 0.2 + color * 0.8 * NL, 0.0, 1.0);\n"
				"   gl_FragColor = vec4(col, 1.0);\n"
				"}"
				);
	program->bindAttributeLocation("vertex", vertexLoc);
	program->bindAttributeLocation("normal", normalLoc);
	program->bindAttributeLocation("color", colorLoc);
	
	program->link();
	program->bind();
	
	projMatLocation = program->uniformLocation("projMat");
	moveMatLocation = program->uniformLocation("moveMat");
	lightPosLocation = program->uniformLocation("lightPos");
	normalMatrixLocation = program->uniformLocation("normalMatrix");
//	QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	world.setToIdentity();
	proj.setToIdentity();
	camera.setToIdentity();
	camera.translate(0,0,-5);

	program_pick = new QGLShaderProgram;
	program_pick->addShaderFromSourceCode(
				QGLShader::Vertex, 
				"attribute vec4 vertex;\n"
				"uniform mat4 projMat;\n"
				"uniform mat4 moveMat;\n"
				"void main(void)\n"
				"{\n"
				"   gl_Position = projMat * moveMat * vertex;\n"
				"}"
				);
	program_pick->addShaderFromSourceCode(QGLShader::Fragment, "void main(void){gl_FragColor = vec4(1.0, 0.0, 1.0, 1.0);}");
//	program_pick->bindAttributeLocation("vertex", vertexLoc);
	program_pick->link();
	program_pick->bind();
	projLocPick = program_pick->uniformLocation("projMat");
	moveLocPick = program_pick->uniformLocation("moveMat");
	vertLocPick = program_pick->attributeLocation("vertex");
	
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
	world.rotate(180.0f - (xRot / 16.0f), 1, 0, 0);
	world.rotate(yRot / 16.0f, 0, 1, 0);
	world.rotate(zRot / 16.0f, 0, 0, 1);
	
	program->bind();
		
	program->setUniformValue(lightPosLocation, QVector3D(70, 70, 70));
	program->enableAttributeArray(normalMatrixLocation);
	program->setUniformValue(normalMatrixLocation, world.normalMatrix());
	program->enableAttributeArray(vertexLoc);
	program->setAttributeArray(vertexLoc, vertices->constData());
	program->enableAttributeArray(normalLoc);
	program->setAttributeArray(normalLoc, normals->constData());
	program->enableAttributeArray(colorLoc);
	program->setAttributeArray(colorLoc, colors.constData());
	program->setUniformValue(projMatLocation, proj);
	program->setUniformValue(moveMatLocation, camera*world);
	
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
