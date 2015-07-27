#ifndef RGRAPHICSVIEWGL_H
#define RGRAPHICSVIEWGL_H

#include <QWidget>

#include "RGraphicsView.h"

class QGLPixelBuffer;
class RAction;
class RDocument;
class RDocumentInterface;
class RGraphicsSceneGl;
class RLine;
    
    

/**
 * \brief OpenGL 2d or 3d graphics view.
 *
 * An OpenGL graphics view displays a rectangular area of an
 * \ref RGraphicsSceneGl "OpenGL graphics scene".
 *
 * \ingroup gui_qt
 *
 * \scriptable
 */
class RGraphicsViewGl : public QWidget, public RGraphicsView {
    Q_OBJECT

public:
    RGraphicsViewGl(QWidget* parent = NULL);
    virtual ~RGraphicsViewGl();

    virtual void setScene(RGraphicsSceneGl* scene);

    virtual void regenerate(bool force=false);
    virtual void repaint();

    void makeMainCurrent();
    void makePreviewCurrent();
    void invalidate();
    
    virtual bool hasFocus() {
        return QWidget::hasFocus();
    }

    virtual RVector mapFromView(const RVector& v, double z=0.0);
    virtual RVector mapToView(const RVector& v);
    virtual double mapDistanceFromView(double d);
    virtual double mapDistanceToView(double d);

    virtual int getWidth();
    virtual int getHeight();
    
    virtual void setCursor(Qt::CursorShape cursorShape);
    virtual void setCursor(const QCursor& cursor);
    
    void callReferencePoint(const RVector& screenPosition, bool highlighted=false);

    virtual void paintGridPoint(const RVector& ucsPosition);
    
public slots:
    void slotReset();

protected:
    virtual void focusInEvent(QFocusEvent* event);
    virtual void focusOutEvent(QFocusEvent* event);
    virtual void resizeEvent(QResizeEvent* event);
    virtual void paintEvent(QPaintEvent* event);

    void updateMainBuffer();
    void paintMainBuffer();
    void paintPreviewBuffer();
    void callGrid();
    void callZero();
    void callCursor();
    void callReferencePoints();

    virtual void setupViewport();
    virtual void setupScreenViewport();

    virtual void keyPressEvent(QKeyEvent* event);
    virtual void keyReleaseEvent(QKeyEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void wheelEvent(QWheelEvent* e);
    virtual void tabletEvent(QTabletEvent* e);

protected:
    QImage buffer;
    QGLPixelBuffer* mainBuffer;
    QGLPixelBuffer* previewBuffer;
    RGraphicsSceneGl* sceneGl;
    bool graphicsBufferNeedsUpdate;
    //! remember last mouse button state for wacom bug workaround
    Qt::MouseButtons lastButtonState;
};

Q_DECLARE_METATYPE(RGraphicsViewGl*)

#endif
