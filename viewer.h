#ifndef __VIEWER_HH__
#define __VIEWER_HH__

#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QGLWidget>
#include "data.h"

class Viewer : public QGLWidget
{
    Q_OBJECT
    public:
	Viewer(QWidget* parent=NULL);
    public slots:
	void showBuilding(const Building& building);
    signals:
	void xRotationChanged(int);
	void yRotationChanged(int);
	void zRotationChanged(int);
    protected:
	void setXRotation(int);
	void setYRotation(int);
	void setZRotation(int);

	void prepareBuilding(const Building& building) const;
	void drawPiece(const Piece& piece) const;
	void drawEnvironment(const Rect& rect) const;

	static void normalizeAngle(int&);

	void initializeGL();
	void paintGL();
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void wheelEvent(QWheelEvent* event);
	void keyPressEvent(QKeyEvent * event);

	void resizeGL(int width, int height);

	double xRot,yRot,zRot;
	double scale;
	QPoint lastPos;
	GLuint object;
	GLuint textures[6];

//	GLuint makeQtLogo() const;
//	void quad(double, double, double, double, double, double, double, double) const;
//	void extrude(double, double, double, double) const;
};

#endif
