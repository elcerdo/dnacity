#include <QApplication>
#include "viewer.h"
#include "data.h"

int main(int argc,char * argv[])
{
    QApplication app(argc,argv);

    Building building =  Building::random(Rect(-.5,-.5,1,1));

    Viewer widget;
    widget.resize(800,600);
    widget.show();

    widget.showBuilding(building);

    return app.exec();
}





