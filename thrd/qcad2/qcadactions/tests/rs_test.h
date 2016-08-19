#ifndef RS_TEST1
#define RS_TEST1

#include <cppunit/extensions/HelperMacros.h>
#include "rs_debug.h"
#include "rs_bitmapgraphicview.h"
#include "rs_graphic.h"
#include "rs_img.h"

/**
 * Base class for all tests.
 */
class RS_Test : public CppUnit::TestFixture {
public:
    void setUp() {
        bitmapView = new RS_BitmapGraphicView(250, 250);
        bitmapView->setFactor(1.0);
        bitmapView->setOffset(0,0);
        container = new RS_Graphic();
        bitmapView->setContainer(container);
    }

    void tearDown() {
        if (bitmapView!=NULL) {
            delete bitmapView;
            bitmapView = NULL;
        }
        if (container!=NULL) {
            delete container;
            container = NULL;
        }
    }
    
    void doMousePress(int x, int y) {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_Test::doMousePress");
        RS_MouseEvent* e = new RS_MouseEvent(
            QEvent::MouseButtonPress, 
            QPoint(x,bitmapView->getHeight() - y),
            Qt::LeftButton,
            Qt::LeftButton,
            Qt::NoModifier);
        bitmapView->mousePressEvent(e);
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_Test::doMousePress: OK");
    }
    
    void doMouseRelease(int x, int y) {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_Test::doMouseRelease");
        RS_MouseEvent* e = new RS_MouseEvent(
            QEvent::MouseButtonRelease, 
            QPoint(x,bitmapView->getHeight() - y),
            Qt::LeftButton,
            Qt::LeftButton,
            Qt::NoModifier);
        bitmapView->mouseReleaseEvent(e);
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_Test::doMouseRelease: OK");
    }

    void doMouseClick(int x, int y) {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_Test::doMouseClick");
        doMousePress(x, y);
        doMouseRelease(x, y);
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_Test::doMouseClick: OK");
    }
    
    void doRightClick() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_Test::doRightClick");
        RS_MouseEvent* e1 = new RS_MouseEvent(
            QEvent::MouseButtonPress, 
            QPoint(0,0),
            Qt::RightButton,
            Qt::RightButton,
            Qt::NoModifier);
        bitmapView->mousePressEvent(e1);
        RS_MouseEvent* e2 = new RS_MouseEvent(
            QEvent::MouseButtonRelease, 
            QPoint(0,0),
            Qt::RightButton,
            Qt::RightButton,
            Qt::NoModifier);
        bitmapView->mouseReleaseEvent(e2);
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_Test::doRightClick: OK");
    }
    
    void doMouseMove(int x, int y) {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_Test::doMouseMove");
        RS_MouseEvent* e = new RS_MouseEvent(
            QEvent::MouseMove, 
            QPoint(x,bitmapView->getHeight() - y),
            Qt::NoButton,
            Qt::NoButton,
            Qt::NoModifier);
        bitmapView->mouseMoveEvent(e);
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_Test::doMouseMove: OK");
    }

protected:
    RS_BitmapGraphicView* bitmapView;
    RS_Graphic* container;
};

#endif
