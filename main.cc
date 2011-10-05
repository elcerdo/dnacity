#include <QApplication>
#include "viewer.h"

int main(int argc,char * argv[])
{
    QApplication app(argc,argv);

    Viewer widget;
    widget.resize(800,600);
    widget.show();

    return app.exec();
}



