#ifndef __VIEWER_HH__
#define __VIEWER_HH__

#include <QMouseEvent>
#include <QGLWidget>

class Viewer : public QGLWidget
{
    Q_OBJECT
    public:
	Viewer(QWidget* parent=NULL);
    signals:
	void xRotationChanged(int);
	void yRotationChanged(int);
	void zRotationChanged(int);
    protected:

	void setXRotation(int);
	void setYRotation(int);
	void setZRotation(int);

	GLuint makeQtLogo() const;
	void quad(double, double, double, double, double, double, double, double) const;
	void extrude(double, double, double, double) const;
	static void normalizeAngle(int&);

	void initializeGL();
	void paintGL();
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);

	void resizeGL(int width, int height);

	double xRot,yRot,zRot;
	QPoint lastPos;
	GLuint object;
};

#endif
