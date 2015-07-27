#ifndef RGRAPHICSVIEWQV_H
#define RGRAPHICSVIEWQV_H

#include <QGraphicsView>

#include "RAction.h"
#include "RDocument.h"
#include "RDocumentInterface.h"
#include "RGraphicsView.h"


class RGraphicsSceneQV;

/**
 * 2d graphics view based on Qt's graphics view. 
 * \scriptable
 */
class RGraphicsViewQV : public QGraphicsView, public RGraphicsView {
    Q_OBJECT

public:
    RGraphicsViewQV(QWidget* parent=0);
    ~RGraphicsViewQV();

    /*
    void zoomIn(const QPointF& center);
    void zoomOut(const QPointF& center);
    */
    void zoom(const RVector& center, double factor);
    void pan(const RVector& delta);

    void unsetCursor();
    void saveView();
    void restoreView();

    virtual void setScene(RGraphicsSceneQV* scene);

    virtual void regenerate(bool force=false);
    virtual void repaint();

    virtual bool hasFocus() {
        return QWidget::hasFocus();
    }

    virtual void removeFocus() {
        // TODO: implement
    }

    virtual int getWidth();
    virtual int getHeight();

    virtual void setCursor(Qt::CursorShape cursorShape);
    virtual void setCursor(const QCursor& cursor);

    virtual void paintGridPoint(const RVector& ucsPosition);
    virtual void paintMetaGridLine(const RLine& ucsPosition);

    virtual void paintOrigin();

    virtual double mapDistanceFromView(double d);
    virtual double mapDistanceToView(double d);
    virtual RVector mapFromView(const RVector& v, double z=0.0);
    virtual RVector mapToView(const RVector& v);

    virtual void viewportChangeEvent();

signals:
    void viewportChanged();

protected:
    virtual void drawBackground(QPainter* painter, const QRectF& rect);
    virtual void drawForeground(QPainter* painter, const QRectF& rect);
    virtual void drawItems(QPainter* painter, int numItems,
                             QGraphicsItem* items[],
                             const QStyleOptionGraphicsItem options[]);

private:
    QTransform previousView;
    int verticalScrollBarSize;
    int horizontalScrollBarSize;
    RGraphicsSceneQV* sceneQV;

    QPainter* gridPainter;
    bool doPaintOrigin;

};

Q_DECLARE_METATYPE(RGraphicsViewQV*)

#endif
