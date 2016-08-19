#include <stdio.h>

#include <QApplication>
#include <QTime>

#include "rs_debug.h"
#include "rs_entitycontainer.h"
#include "rs_bitmapgraphicview.h"
#include "rs_line.h"

int main(int argc, char** argv) {

    //RS_DEBUG->setLevel(RS_Debug::D_DEBUGGING);
    RS_DEBUG->setLevel(RS_Debug::D_TESTING);
        
    QApplication app(argc, argv);

    RS_EntityContainer ec;
    for (int i=0; i<100000; ++i) {
        ec.addEntity(new RS_Line(&ec, 
            RS_LineData(RS_Vector(0,0), RS_Vector(1000,1000))));
    }

    RS_BitmapGraphicView gv(1000,1000);
    gv.setContainer(&ec);
    gv.zoomAuto(false);
    QTime t1 = QTime::currentTime();
    gv.drawEntity(&ec, true);
    QTime t2 = QTime::currentTime();

    RS_DEBUG->print(RS_Debug::D_TESTING, "t: %d msec", t1.msecsTo(t2));

    return 0;
}
