#ifndef __VIEWER_HH__
#define __VIEWER_HH__

#include <QGLWidget>

class Viewer : public QGLWidget
{
    Q_OBJECT
    public:
	Viewer(QWidget* parent=NULL);
};

#endif
