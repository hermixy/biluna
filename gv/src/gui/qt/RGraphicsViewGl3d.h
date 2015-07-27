#ifndef RGRAPHICSVIEWGL3D_H
#define RGRAPHICSVIEWGL3D_H

#include <QGLWidget>

#include "RGraphicsViewGl.h"

class RGraphicsSceneGl;
    
    

/**
 * \brief OpenGL based 3d graphics view.
 *
 * A 3d graphics view displays a rectangular area of a graphics scene.
 *
 * \ingroup qcadguiqt
 */
class RGraphicsViewGl3d : public RGraphicsViewGl {
    Q_OBJECT

public:
    RGraphicsViewGl3d(QWidget* parent=NULL);
    ~RGraphicsViewGl3d();
    
    //static RGraphicsViewGl3d* create(RGraphicsSceneGl& scene, QWidget* parent=NULL);

    // these functions are disabled here since they are not available for 3d views or don't
    // make sense for 3d views.
    virtual void pan(const RVector& /*delta*/) {}
    virtual void zoom(const RVector& /*center*/, double /*factor*/) {}
    virtual void zoomTo(const RBox& /*box*/) {}
    virtual void centerTo(const RBox& /*box*/) {}
    
    //virtual RVector mapFromView(const RVector& v, double z=0.0);

public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void setZoom(float z);
    void setOffset(const RVector& o);
    void setCenterOfRotation(const RVector& c);

    void slotReset();

protected:
    virtual void setupViewport();

    void normalizeAngle(int* angle);
    
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void wheelEvent(QWheelEvent* event);

private:
    QPoint lastPos;
    bool navigation;

    RVector centerOfRotation;
    int xRot;
    int yRot;
    int zRot;
};

#endif
