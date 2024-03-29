#include "viewer.h"

#include <QDebug>
#include <cmath>

Viewer::Viewer(QWidget* parent) :
    QGLWidget(parent)
{
    scale = 1;
}

void Viewer::setXRotation(int angle)
{
    normalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}

void Viewer::setYRotation(int angle)
{
    normalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void Viewer::setZRotation(int angle)
{
    normalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}

void Viewer::initializeGL()
{
    { // load textures
	textures[0] = bindTexture(QPixmap(":/textures/debug"),GL_TEXTURE_2D);
	textures[1] = bindTexture(QPixmap(":/textures/road2"),GL_TEXTURE_2D);
	textures[2] = bindTexture(QPixmap(":/textures/window"),GL_TEXTURE_2D);
	textures[3] = bindTexture(QPixmap(":/textures/grass"),GL_TEXTURE_2D);
	textures[4] = bindTexture(QPixmap(":/textures/roof"),GL_TEXTURE_2D);
	textures[5] = bindTexture(QPixmap(":/textures/sky"),GL_TEXTURE_2D);
	textures[6] = bindTexture(QPixmap(":/textures/particule"),GL_TEXTURE_2D);
    }

    qglClearColor(Qt::black);
    glLineWidth(2.);
    glShadeModel(GL_SMOOTH);

    xRot=0.0; yRot=0.0; zRot=0.0;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_AUTO_NORMAL);

    glColorMaterial ( GL_FRONT_AND_BACK, GL_EMISSION ) ;
    glEnable ( GL_COLOR_MATERIAL ) ;
    glEnable(GL_TEXTURE_2D);

    GLfloat global_ambient[] = { -1, -1, -1, 1 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

    glEnable(GL_LIGHT0);
    GLfloat ambient[] = {0.00, 0.0, 0.0, 1.0 };
    GLfloat diffuse[] = { 0.1, 0.1, 0.1, 1 };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    GLfloat position[] = { -10, -10, 0.5, 0.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, position);

}

void Viewer::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslated(0.0, 0.0, -10.0);
    glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotated(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotated(zRot / 16.0, 0.0, 0.0, 1.0);
    drawSky();
    glScaled(scale,scale,scale);
    glTranslated(0.0, 0.0, -1.0);
    glCallList(object);
}

void Viewer::resizeGL(int width, int height)
{
    const qreal side = qMin(width,height);
    const qreal xratio = width/side;
    const qreal yratio = height/side;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-xratio, +xratio, +yratio, -yratio, 2.0, 25.0);
    glMatrixMode(GL_MODELVIEW);
}

void Viewer::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void Viewer::keyPressEvent(QKeyEvent * event) {
    /// BOUH que c'est saaaale ! à virer du viewer !
    if(event->key() == Qt::Key_Space) {
        showBuilding(Building::random(Rect(-.5,-.5,1,1)));
	event->accept();
    }

    if (event->key() == Qt::Key_C) {
	object = glGenLists(1);
	glNewList(object, GL_COMPILE);
	for (int ii=-4; ii<5; ii++)
	    for (int jj=-4; jj<5; jj++) {
            prepareBuilding(Building::random(Rect(-.5+ii*1.2,-.5+jj*1.2,1,1)));
            drawEnvironment(Rect(-.5+ii*1.2,-.5+jj*1.2,1,1));
        }
	glEndList();
	updateGL();
	event->accept();
    }


}

void Viewer::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons().testFlag(Qt::LeftButton)) {
        setXRotation(xRot - 8 * dy);
        setYRotation(yRot + 8 * dx);
	event->accept();
	lastPos = event->pos();
    } else if (event->buttons().testFlag(Qt::RightButton)) {
        setXRotation(xRot - 8 * dy);
        setZRotation(zRot + 8 * dx);
	event->accept();
	lastPos = event->pos();
    }

}

void Viewer::showBuilding(const Building& building)
{
    object = glGenLists(1);
    glNewList(object, GL_COMPILE);
    prepareBuilding(building);
    drawEnvironment(building.bounding);
    glEndList();
    updateGL();
}

void Viewer::prepareBuilding(const Building& building) const
{
    foreach (const Piece& piece, building.pieces) 
        drawPiece(piece);
}


void Viewer::drawSky() const {
    qglColor(Qt::white);
    glBindTexture(GL_TEXTURE_2D,textures[5]);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    const float radius = 3;
    
    for (int ll=0; ll<31; ll++) {
	const float alpha_0 = ll/32.;
	const float theta_0 = M_PI/2.*alpha_0;
	const float alpha_1 = (ll+1)/32.;
	const float theta_1 = M_PI/2.*alpha_1;
	glBegin(GL_TRIANGLE_STRIP);
	for (int kk=0; kk<65; kk++) {
	    const float phi = -2*M_PI*kk/64;
	    glTexCoord2d(0.5+.48*cos(phi)*(1-alpha_0),0.5-.48*sin(phi)*(1-alpha_0));
	    glVertex3d(radius*cos(phi)*cos(theta_0),radius*sin(phi)*cos(theta_0),radius*sin(theta_0));
	    glTexCoord2d(0.5+.48*cos(phi)*(1-alpha_1),0.5-.48*sin(phi)*(1-alpha_1));
	    glVertex3d(radius*cos(phi)*cos(theta_1),radius*sin(phi)*cos(theta_1),radius*sin(theta_1));
	}
	glEnd();
    }
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}

void Viewer::drawEnvironment(const Rect& rect) const {
    {
	glBindTexture(GL_TEXTURE_2D,textures[3]);
	glBegin(GL_QUADS);
	glTexCoord2d(0,0); glVertex3d(rect.left(), rect.bottom(), 0.);
	glTexCoord2d(rect.width(),0); glVertex3d(rect.right(), rect.bottom(), 0.);
	glTexCoord2d(rect.width(),rect.height()); glVertex3d(rect.right(), rect.top(), 0.);
	glTexCoord2d(0,rect.height()); glVertex3d(rect.left(), rect.top(), 0.);
	glEnd();
    }

    {
	glBindTexture(GL_TEXTURE_2D,textures[1]);
	glBegin(GL_QUADS);
	glTexCoord2d(0,0); glVertex3d(rect.left()-.1, rect.bottom()+.1, -1e-3);
	glTexCoord2d(rect.width(),0); glVertex3d(rect.right()+.1, rect.bottom()+.1, -1e-3);
	glTexCoord2d(rect.width(),rect.height()); glVertex3d(rect.right()+.1, rect.top()-.1, -1e-3);
	glTexCoord2d(0,rect.height()); glVertex3d(rect.left()-.1, rect.top()-.1, -1e-3);
	glEnd();
    }
}

void Viewer::wheelEvent(QWheelEvent* event)
{
    if (event->delta()>0) scale*=1.1;
    if (event->delta()<0) scale/=1.1;
    updateGL();
}

void Viewer::drawPiece(const Piece& piece) const
{

    { // draw faces
	// Sol
	glBindTexture(GL_TEXTURE_2D,0);
	qglColor(Qt::green);
	glBegin(GL_QUADS);
	glVertex3d(piece.base.left(), piece.base.top(), 0);
	glVertex3d(piece.base.right(), piece.base.top(), 0);
	glVertex3d(piece.base.right(), piece.base.bottom(), 0);
	glVertex3d(piece.base.left(), piece.base.bottom(), 0);
	glEnd();
	qglColor(Qt::white);

	// Plafond
	glBindTexture(GL_TEXTURE_2D,textures[4]);
	glBegin(GL_QUADS);
	glTexCoord2d(piece.base.width(),piece.base.height()); glVertex3d(piece.base.right(), piece.base.top(), piece.height);
	glTexCoord2d(0,piece.base.height()); glVertex3d(piece.base.left(), piece.base.top(), piece.height);
	glTexCoord2d(0,0); glVertex3d(piece.base.left(), piece.base.bottom(), piece.height);
	glTexCoord2d(piece.base.width(),0); glVertex3d(piece.base.right(), piece.base.bottom(), piece.height);
	glEnd();

	// Derrière
	glBindTexture(GL_TEXTURE_2D,textures[2]);
	glBegin(GL_QUADS);
	glTexCoord2d(piece.base.width()/2.,-piece.height/2.); glVertex3d(piece.base.left(), piece.base.top(), 0);
	glTexCoord2d(piece.base.width()/2.,0); glVertex3d(piece.base.left(), piece.base.top(), piece.height);
	glTexCoord2d(0,0); glVertex3d(piece.base.right(), piece.base.top(), piece.height);
	glTexCoord2d(0,-piece.height/2.); glVertex3d(piece.base.right(), piece.base.top(), 0);
	glEnd();

	// Droite
	glBegin(GL_QUADS);
	glTexCoord2d(piece.base.height()/2.,-piece.height/2.); glVertex3d(piece.base.right(), piece.base.top(), 0);
	glTexCoord2d(piece.base.height()/2.,0); glVertex3d(piece.base.right(), piece.base.top(), piece.height);
	glTexCoord2d(0,0); glVertex3d(piece.base.right(), piece.base.bottom(), piece.height);
	glTexCoord2d(0,-piece.height/2.); glVertex3d(piece.base.right(), piece.base.bottom(), 0);
	glEnd();

	// Devant
	glBegin(GL_QUADS);
	glTexCoord2d(piece.base.width()/2.,-piece.height/2.); glVertex3d(piece.base.right(), piece.base.bottom(), 0);
	glTexCoord2d(piece.base.width()/2.,0); glVertex3d(piece.base.right(), piece.base.bottom(), piece.height);
	glTexCoord2d(0,0); glVertex3d(piece.base.left(), piece.base.bottom(), piece.height);
	glTexCoord2d(0,-piece.height/2.); glVertex3d(piece.base.left(), piece.base.bottom(), 0);
	glEnd();

	// Gauche
	glBegin(GL_QUADS);
	glTexCoord2d(piece.base.height()/2.,-piece.height/2.); glVertex3d(piece.base.left(), piece.base.bottom(), 0);
	glTexCoord2d(piece.base.height()/2.,0); glVertex3d(piece.base.left(), piece.base.bottom(), piece.height);
	glTexCoord2d(0,0); glVertex3d(piece.base.left(), piece.base.top(), piece.height);
	glTexCoord2d(0,-piece.height/2.); glVertex3d(piece.base.left(), piece.base.top(), 0);
	glEnd();
    }

/*    { // draw edges
	qglColor(Qt::red);
	glBindTexture(GL_TEXTURE_2D,0);
	glBegin(GL_LINE_STRIP);
	glVertex3d(piece.base.left(), piece.base.top(), 0);
	glVertex3d(piece.base.right(), piece.base.top(), 0);
	glVertex3d(piece.base.right(), piece.base.bottom(), 0);
	glVertex3d(piece.base.left(), piece.base.bottom(), 0);
	glVertex3d(piece.base.left(), piece.base.top(), 0);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3d(piece.base.left(), piece.base.top(), piece.height);
	glVertex3d(piece.base.right(), piece.base.top(), piece.height);
	glVertex3d(piece.base.right(), piece.base.bottom(), piece.height);
	glVertex3d(piece.base.left(), piece.base.bottom(), piece.height);
	glVertex3d(piece.base.left(), piece.base.top(), piece.height);
	glEnd();

	glBegin(GL_LINES);
	glVertex3d(piece.base.left(), piece.base.top(), 0);
	glVertex3d(piece.base.left(), piece.base.top(), piece.height);
	glVertex3d(piece.base.right(), piece.base.top(), 0);
	glVertex3d(piece.base.right(), piece.base.top(), piece.height);
	glVertex3d(piece.base.left(), piece.base.bottom(), 0);
	glVertex3d(piece.base.left(), piece.base.bottom(), piece.height);
	glVertex3d(piece.base.right(), piece.base.bottom(), 0);
	glVertex3d(piece.base.right(), piece.base.bottom(), piece.height);
	glEnd();

	qglColor(Qt::white);
    }*/
}


void Viewer::normalizeAngle(int& angle)
{
    while (angle < 0)
	angle += 360 * 16;
    while (angle > 360 * 16)
	angle -= 360 * 16;
}



/***
 *


GLuint Viewer::makeQtLogo() const
{
    GLuint list = glGenLists(1);
    glNewList(list, GL_COMPILE);

    glBegin(GL_QUADS);

    GLdouble x1 = +0.06;
    GLdouble y1 = -0.14;
    GLdouble x2 = +0.14;
    GLdouble y2 = -0.06;
    GLdouble x3 = +0.08;
    GLdouble y3 = +0.00;
    GLdouble x4 = +0.30;
    GLdouble y4 = +0.22;

    quad(x1, y1, x2, y2, y2, x2, y1, x1);
    quad(x3, y3, x4, y4, y4, x4, y3, x3);

    extrude(x1, y1, x2, y2);
    extrude(x2, y2, y2, x2);
    extrude(y2, x2, y1, x1);
    extrude(y1, x1, x1, y1);
    extrude(x3, y3, x4, y4);
    extrude(x4, y4, y4, x4);
    extrude(y4, x4, y3, x3);

    const double Pi = 3.14159265358979323846;
    const int NumSectors = 200;

    for (int i = 0; i < NumSectors; ++i) {
	double angle1 = (i * 2 * Pi) / NumSectors;
	GLdouble x5 = 0.30 * sin(angle1);
	GLdouble y5 = 0.30 * cos(angle1);
	GLdouble x6 = 0.20 * sin(angle1);
	GLdouble y6 = 0.20 * cos(angle1);

	double angle2 = ((i + 1) * 2 * Pi) / NumSectors;
	GLdouble x7 = 0.20 * sin(angle2);
	GLdouble y7 = 0.20 * cos(angle2);
	GLdouble x8 = 0.30 * sin(angle2);
	GLdouble y8 = 0.30 * cos(angle2);

	quad(x5, y5, x6, y6, x7, y7, x8, y8);

	extrude(x6, y6, x7, y7);
	extrude(x8, y8, x5, y5);
    }

    glEnd();

    glEndList();
    return list;
}



void Viewer::quad(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2,
	GLdouble x3, GLdouble y3, GLdouble x4, GLdouble y4) const
{
    qglColor(Qt::green);

    glVertex3d(x1, y1, -0.05);
    glVertex3d(x2, y2, -0.05);
    glVertex3d(x3, y3, -0.05);
    glVertex3d(x4, y4, -0.05);

    glVertex3d(x4, y4, +0.05);
    glVertex3d(x3, y3, +0.05);
    glVertex3d(x2, y2, +0.05);
    glVertex3d(x1, y1, +0.05);
}

void Viewer::extrude(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2) const
{
    qglColor(Qt::yellow);

    glVertex3d(x1, y1, +0.05);
    glVertex3d(x2, y2, +0.05);
    glVertex3d(x2, y2, -0.05);
    glVertex3d(x1, y1, -0.05);
}

*/
