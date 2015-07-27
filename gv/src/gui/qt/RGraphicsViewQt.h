#ifndef RGRAPHICSVIEWQT_H
#define RGRAPHICSVIEWQT_H

#include <QtCore>
#include <QFrame>
#include <QFocusFrame>
#include <QWidget>
#include <QPinchGesture>

#include "RGraphicsView.h"
#include "RPainterPath.h"

class RAction;
class RDocument;
class RDocumentInterface;
class RGraphicsSceneQt;
class RLine;



/**
 * \brief Qt based 2d graphics view.
 *
 * This graphics view displays a rectangular area of a
 * RGraphicsSceneQt.
 *
 * \ingroup qcadguiqt
 * \scriptable
 * \generateScriptShell
 */
class RGraphicsViewQt : public QWidget, public RGraphicsView {

    Q_OBJECT

public:
    RGraphicsViewQt(QWidget* parent=NULL);
    virtual ~RGraphicsViewQt();
    
    virtual void setScene(RGraphicsSceneQt* scene);

    virtual void regenerate(bool force=false);
    virtual void repaint();

    virtual bool hasFocus() {
        return QWidget::hasFocus();
    }

    void saveViewport();
    void restoreViewport();

    virtual RVector mapFromView(const RVector& v, double z=0.0);
    virtual RVector mapToView(const RVector& v);
    virtual double mapDistanceFromView(double d);
    virtual double mapDistanceToView(double d);
    
    double getCurrentScale();

    virtual int getWidth();
    virtual int getHeight();
    
    virtual void setCursor(Qt::CursorShape cursorShape);
    virtual void setCursor(const QCursor& cursor);

    virtual void paintGridPoint(const RVector& ucsPosition);
    virtual void paintMetaGridLine(const RLine& ucsPosition);

    void paintOrigin();

    int getNumBuffers() {
        return numBuffers;
    }

    virtual void viewportChangeEvent();

    void setPaintOrigin(bool val);

    void setAntialiasing(bool val);
    bool getAntialiasing();

signals:
    void viewportChanged();
    void drop(QDropEvent* event);
    void dragEnter(QDragEnterEvent* event);

protected:
    virtual QSize sizeHint() const;

    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void wheelEvent(QWheelEvent* event);
    virtual void paintEvent(QPaintEvent*);
    virtual void resizeEvent(QResizeEvent* event);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void keyReleaseEvent(QKeyEvent* event);
    virtual void dragEnterEvent(QDragEnterEvent* event);
    virtual void dropEvent(QDropEvent* event);

    virtual void paintGrid(QPaintDevice& device, const QRect& rect = QRect());
    virtual void paintCursor(QPaintDevice& device);
    virtual void paintRelativeZero(QPaintDevice& device);
    virtual void paintDocument(const QRect& rect = QRect());

    QPainter* initPainter(QPaintDevice& device, bool erase, bool screen = false, const QRect& rect = QRect());

    void invalidate(bool force=false);
    void updateGraphicsBuffer();
    void updateTransformation();

private:
    // drawing class for concurrent drawing (QtConcurrent)
    struct Draw {
        Draw(QPainter** painter, RGraphicsViewQt* view, QMutex* mutex,
             QSet<RObject::Id>& selectedIds, int bgColorValue);

        Draw(QPainter* painter, RGraphicsViewQt* view, int bgColorValue);

        void operator()(int id);

        void setSelected(bool on);

        QPainter** painter;
        RGraphicsViewQt* view;
        QMutex* mutex;
        bool isSelected;
        QSet<RObject::Id>* selectedIds;
        int bgColorValue;
    };

protected:
    QImage graphicsBuffer;

private:
    QPainter** painter;
    QImage* threadBuffer;
    QMutex* mutex;
    int numBuffers;

    RGraphicsSceneQt* sceneQt;
    double currentScale;
    QSize lastSize;
    RVector lastOffset;
    double lastFactor;
    bool graphicsBufferNeedsUpdate;
    QTransform transform;
    QTransform previousView;
    //! remember last mouse button state for wacom bug workaround
    Qt::MouseButtons lastButtonState;

    QPainter* gridPainter;
    bool doPaintOrigin;
    bool antialiasing;
};

Q_DECLARE_METATYPE(RGraphicsViewQt*)

#endif
