#include <QDragMoveEvent>
#include <QScrollBar>

#include "RGraphicsViewGl3d.h"
#include "RDebug.h"
#include "RDocumentInterface.h"
#include "REntity.h"
#include "RGraphicsSceneGl.h"
#include "RMouseEvent.h"



RGraphicsViewGl3d::RGraphicsViewGl3d(QWidget* parent)
    : RGraphicsViewGl(parent), navigation(false) {
    
    slotReset();
}



RGraphicsViewGl3d::~RGraphicsViewGl3d() {
}



void RGraphicsViewGl3d::normalizeAngle(int* angle) {
    while (*angle < 0) {
        *angle += 360 * 16;
    }
    while (*angle > 360 * 16) {
        *angle -= 360 * 16;
    }
}
    
    

void RGraphicsViewGl3d::mouseMoveEvent(QMouseEvent* event) {
    if (event==NULL || scene==NULL) {
        return;
    }
    
    RMouseEvent e(*event, *scene, *this);
    scene->handleMouseMoveEvent(e);

    /*
    if (e.isConsumed()) {
        return;
    }
    */
    
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    //if (abs(dx) + abs(dy) > 15) {
        if (event->buttons() & Qt::RightButton) {
            navigation = true;
            setXRotation(xRot + 4 * dy);
            setZRotation(zRot + 4 * dx);
        } else if (event->buttons() & Qt::RightButton) {
            navigation = true;
            setXRotation(xRot + 4 * dy);
            setYRotation(yRot + 4 * dx);
        } else if (event->buttons() & Qt::MidButton) {
            navigation = true;
            double f = 500.0/fabs(factor);
            setOffset(offset + RVector(dx, -dy)/f);
        }
        
        lastPos = event->pos();
    //}
}



void RGraphicsViewGl3d::mousePressEvent(QMouseEvent* event) {
    if (event==NULL || scene==NULL) {
        return;
    }

    RMouseEvent e(*event, *scene, *this);
    scene->handleMousePressEvent(e);
    
    /*
    if (e.isConsumed()) {
        return;
    }
    */
    
    lastPos = event->pos();
    navigation = false;
}



void RGraphicsViewGl3d::mouseReleaseEvent(QMouseEvent* event) {
    if (event==NULL || scene==NULL) {
        return;
    }

    if (!navigation) {
        RMouseEvent e(*event, *scene, *this);
        scene->handleMouseReleaseEvent(e);
    }
}



void RGraphicsViewGl3d::wheelEvent(QWheelEvent* event) {
    if (event==NULL || scene==NULL) {
        return;
    }

    double step = ::fabs(factor)*0.1;
    if (step<1) {
        step = 1;
    }

    scene->clearPreview();

    // zoom in / out:
    if (event->delta()<0) {
        setZoom(factor - step);
    }
    else {
        setZoom(factor + step);
    }

    //event->accept();
    //RGraphicsViewGl::wheelEvent(event);
}



void RGraphicsViewGl3d::setXRotation(int angle) {
    RDebug::debug("RGraphicsViewGl3d::setXRotation: %d", angle);
    normalizeAngle(&angle);
    if (angle != xRot) {
        xRot = angle;
        regenerate();
    }
}



void RGraphicsViewGl3d::setYRotation(int angle) {
    RDebug::debug("RGraphicsViewGl3d::setYRotation: %d", angle);
    normalizeAngle(&angle);
    if (angle != yRot) {
        yRot = angle;
        regenerate();
    }
}



void RGraphicsViewGl3d::setZRotation(int angle) {
    RDebug::debug("RGraphicsViewGl3d::setZRotation: %d", angle);
    normalizeAngle(&angle);
    if (angle != zRot) {
        zRot = angle;
        regenerate();
    }
}



void RGraphicsViewGl3d::setZoom(float z) {
    RDebug::debug("RGraphicsViewGl3d::setZoom: %f", z);
    if (z!=factor) {
        factor = z;
        regenerate();
    }
}



void RGraphicsViewGl3d::setOffset(const RVector& o) {
    RDebug::debug("RGraphicsViewGl3d::setOffset: %f/%f/%f", o.x, o.y, o.z);
    if (o!=offset) {
        offset = o;
        regenerate();
    }
}



void RGraphicsViewGl3d::setCenterOfRotation(const RVector& c) {
    if (c!=centerOfRotation) {
        centerOfRotation = c;
        RDebug::debug("RGraphicsViewGl3d::setCenterOfRotation: "
            "%f/%f/%f", c.x, c.y, c.z);
        //updateGL();
    }
}



/**
 * Resets the view to a typical 3d view with the X/Y plane visible and 
 * the Z axis almost vertical.
 */
void RGraphicsViewGl3d::slotReset() {
    offset = RVector(-25,-25);
    factor = -75.0;

    xRot = 4500;
    yRot = 0;
    zRot = 0;
    regenerate();
}



/**
 * Sets up the OpenGL viewport and transformations for the current
 * OpenGL context. This can be the OpenGL view or the pbuffer for
 * the preview.
 */
void RGraphicsViewGl3d::setupViewport() {
    glShadeModel(GL_SMOOTH);
    glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glViewport(0, 0, getWidth(), getHeight());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
//    gluPerspective(45.0f, (GLfloat)getWidth()/(GLfloat)getHeight(), 0.1f, 100000.0f);
    glMatrixMode(GL_MODELVIEW);
    
    // clear the screen and the depth buffer:
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // reset transformations:
    glLoadIdentity();

    //if (glassHouse) {
    //    glDisable(GL_DEPTH_TEST);
    //}
    //else {
        glEnable(GL_DEPTH_TEST);
    //}
    
    //if (culling) {
    //    glEnable(GL_CULL_FACE);
    //}
    //else {
        glDisable(GL_CULL_FACE);
    //}

    glTranslatef(offset.x, offset.y, factor);

    glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotated(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotated(zRot / 16.0, 0.0, 0.0, 1.0);
    
    glTranslatef(
        -centerOfRotation.x,
        -centerOfRotation.y,
        -centerOfRotation.z);
}



/*
RVector RGraphicsViewGl3d::mapFromView(const RVector& v, double / *z* /) {
    // We're producing a ray here that goes from the mouse position at the
    // front clipping plane to the mouse position at the back clipping plane.
    // The intersection of that ray with the x/y plane is the coordinate that
    // interests us for now.

    RVector mouseFront = RGraphicsViewGl::mapFromView(v);
    
    double modelview[16];
    double projection[16];
    int viewport[4];

    double z = 1.0;
    
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    RVector mouseBack;
    gluUnProject(
        v.x, viewport[3]-v.y, z, 
        modelview, projection, viewport, 
        &mouseBack.x, &mouseBack.y, &mouseBack.z
    );

    RVector v2 = mouseBack - mouseFront;

    double f = - mouseFront.z / v2.z;

    RVector ret = mouseFront + v2 * f;
    
    RDebug::debug("RGraphicsViewGl3d::mapFromView: "
        "ret: %f/%f/%f", 
        ret.x, ret.y, ret.z);

    return ret;
}
*/
