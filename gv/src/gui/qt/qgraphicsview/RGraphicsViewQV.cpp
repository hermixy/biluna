#include "RGraphicsViewQV.h"

#include <QWheelEvent>
#include <QDragMoveEvent>
#include <QScrollBar>
#include <QGLWidget>
#include <QGraphicsLineItem>

#include "RDebug.h"
#include "RGraphicsSceneQV.h"
#include "RGrid.h"
#include "RSettings.h"


RGraphicsViewQV::RGraphicsViewQV(QWidget* parent)
    : QGraphicsView(parent), RGraphicsView() {

    doPaintOrigin = true;

    //setViewport(new QGLWidget());
    setBackgroundBrush(Qt::black);
    setInteractive(true);
    scale(1.0, -1.0);
    //setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    //setOptimizationFlags(QGraphicsView::DontSavePainterState|QGraphicsView::DontClipPainter);
    // _much_ faster for entity highlighting:
    //setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    //setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    //setFocusPolicy(Qt::WheelFocus);
    setMouseTracking(true);
    saveView();

    verticalScrollBarSize = verticalScrollBar()->sizeHint().width();
    horizontalScrollBarSize = horizontalScrollBar()->sizeHint().height();

    RDebug::debug("RGraphicsViewQV::RGraphicsView: verticalScrollBarSize: %d", verticalScrollBarSize);
    RDebug::debug("RGraphicsViewQV::RGraphicsView: horizontalScrollBarSize: %d", horizontalScrollBarSize);
}



RGraphicsViewQV::~RGraphicsViewQV() {
}
    
    

void RGraphicsViewQV::saveView() {
    previousView = transform();
}



void RGraphicsViewQV::restoreView() {
    setTransform(previousView);
}


/*
void RGraphicsViewQV::zoomIn(const QPointF& center) {
    zoom(center, 1.0/1.2);
}

void RGraphicsViewQV::zoomOut(const QPointF& center) {
    zoom(center, 1.2);
}
*/

void RGraphicsViewQV::zoom(const RVector& center, double factor) {
    RVector leftBottom = mapFromView(RVector(0,height()-horizontalScrollBarSize-4));
    RVector rightTop = mapFromView(RVector(width()-verticalScrollBarSize-4,0));

    leftBottom = center - (center-leftBottom)*factor;
    rightTop = center + (rightTop-center)*factor;
    QRectF rect(QPointF(leftBottom.x,leftBottom.y), QPointF(rightTop.x,rightTop.y));
    fitInView(rect, Qt::KeepAspectRatio);
}

void RGraphicsViewQV::pan(const RVector& delta) {
    QScrollBar *hBar = horizontalScrollBar();
    QScrollBar *vBar = verticalScrollBar();
    hBar->setValue(hBar->value() - delta.x);
    vBar->setValue(vBar->value() - delta.y);
}

    
void RGraphicsViewQV::unsetCursor() {
    viewport()->unsetCursor();
}

int RGraphicsViewQV::getWidth() {
    return width();
}

int RGraphicsViewQV::getHeight() {
    return height();
}

void RGraphicsViewQV::setCursor(Qt::CursorShape cursorShape) {
    viewport()->setCursor(cursorShape);
}

void RGraphicsViewQV::setCursor(const QCursor& cursor) {
    viewport()->setCursor(cursor);
}

void RGraphicsViewQV::paintGridPoint(const RVector& ucsPosition) {
    if (gridPainter==NULL) {
        RDebug::warning("RGraphicsViewQV::paintGridPoint: gridPainter is NULL");
        return;
    }
    gridPainter->drawPoint(QPointF(ucsPosition.x, ucsPosition.y));
}

void RGraphicsViewQV::paintMetaGridLine(const RLine& ucsPosition) {
    if (gridPainter==NULL) {
        RDebug::warning("RGraphicsViewQt::paintMetaGridLine: gridPainter is NULL");
        return;
    }

    gridPainter->drawLine(
        QPointF(ucsPosition.startPoint.x, ucsPosition.startPoint.y),
        QPointF(ucsPosition.endPoint.x, ucsPosition.endPoint.y)
    );
}

void RGraphicsViewQV::paintOrigin() {
    if (!doPaintOrigin) {
            return;
    }
    if (gridPainter==NULL) {
        RDebug::warning("RGraphicsViewQt::paintGridOrigin: gridPainter is NULL");
        return;
    }
    double r = mapDistanceFromView(20.0);
    gridPainter->drawLine(
        QPointF(-r, 0),
        QPointF(r,0)
    );
    gridPainter->drawLine(
        QPointF(0, -r),
        QPointF(0,r)
    );
}

/**
 * Regenerates the view from the underlying scene.
 */
void RGraphicsViewQV::regenerate(bool) {
    //invalidate();
    //repaint();
    update();
}

/**
 * Triggers a paintEvent based on a buffered offscreen bitmap (very fast).
 */
void RGraphicsViewQV::repaint() {
    update();
}

void RGraphicsViewQV::setScene(RGraphicsSceneQV* scene) {
    sceneQV = scene;
    RGraphicsView::setScene(scene);
    QGraphicsView::setScene(scene);
}

void RGraphicsViewQV::drawBackground(QPainter* painter, const QRectF& rect) {
    if (painter==NULL) {
        return;
    }

    QGraphicsView::drawBackground(painter, rect);
//    gridPainter = painter;
    grid->update(*this);
//    grid->paint(*this);
//    grid->paintMetaGrid(*this);
//    paintOrigin();



    QRect r = rect.toRect();
    if (rect.isNull()) {
        r = QRect(0,0,width(),height());
    }

    RVector c1 = mapFromView(RVector(r.left()-1,r.top()-1));
    RVector c2 = mapFromView(RVector(r.right()+1,r.bottom()+1));
    QRectF rf(c1.x, c1.y, c2.x-c1.x, c2.y-c1.y);

    /*gridPainter = initPainter(device, false);
        QPainter* RGraphicsViewQt::initPainter(QPaintDevice& device, bool erase, bool screen, const QRect& rect) {
        QPainter* painter = new QPainter(&device);
        if (antialiasing) {
            painter->setRenderHint(QPainter::Antialiasing);
        }
        if (erase) {
            QRect r = rect;
            if (rect.isNull()) {
                r = QRect(0,0,lastSize.width(),lastSize.height());
            }
            painter->setCompositionMode(QPainter::CompositionMode_Clear);
            //painter->eraseRect(0, 0, lastSize.width(), lastSize.height());
            painter->eraseRect(r);
            painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
        }
        //painter->setRenderHint(QPainter::NonCosmeticDefaultPen, false);

        if (!screen) {
            painter->setWorldTransform(transform);
        }
        return painter;
    }
      */

    gridPainter = painter;

    if (!rect.isNull()) {
        gridPainter->setClipRect(rect /*rf*/);
    }

    //    gridPainter->setPen(
    //                QPen(RSettings::getColor(RSettings::OriginColor, RColor(Qt::red))));
        gridPainter->setPen(QPen(RColor(Qt::red)));
        paintOrigin();

        if (grid!=NULL) {
    //        gridPainter->setPen(
    //                    QPen(RSettings::getColor(RSettings::MetaGridColor,
    //                                             RColor(Qt::gray)),
    //                         0, Qt::DotLine));
            gridPainter->setPen(QPen(RColor(Qt::gray), 0, Qt::DotLine));
            grid->paintMetaGrid(*this);

    //        gridPainter->setPen(
    //                    QPen(RSettings::getColor(RSettings::GridColor,
    //                                             RColor(Qt::gray))));
            gridPainter->setPen(QPen(RColor(Qt::gray)));
            grid->paint(*this);
        }
}



void RGraphicsViewQV::drawForeground(QPainter* painter, const QRectF& /*rect*/) {
    if (painter==NULL) {
        return;
    }

    //RDebug::debug("RGraphicsViewQV::drawForeground: "
    //    "%f/%f - %f/%f", rect.left(), rect.top(), rect.width(), rect.height());

    //grid.paintGrid(painter, rect);
}



void RGraphicsViewQV::drawItems(
    QPainter* painter, 
    int numItems,
    QGraphicsItem* items[],
    const QStyleOptionGraphicsItem /*options*/[]) {

    double oneUnitInPixels = mapDistanceToView(1.0);
            
    for (int i=0; i<numItems; ++i) {
        // Draw the item
        //painter->save();
        //painter->setMatrix(items[i]->sceneMatrix(), true);
        QGraphicsLineItem* l = qgraphicsitem_cast<QGraphicsLineItem*>(items[i]);
        if (l!=NULL) {
            double penWidth = oneUnitInPixels * l->pen().widthF();
            QPen p(l->pen());
            p.setWidth((int)penWidth);
            painter->setPen(p);
            painter->drawLine(l->line());
        }
        //items[i]->paint(painter, &options[i], widget);
        //painter->restore();
    }
}
    
    

double RGraphicsViewQV::mapDistanceToView(double d) {
    QPoint point1 = mapFromScene(QPointF(0,0));
    QPoint point2 = mapFromScene(QPointF(d,0));
    return point2.x() - point1.x();
}



double RGraphicsViewQV::mapDistanceFromView(double d) {
    QPointF point1 = mapToScene(QPoint(0,0));
    QPointF point2 = mapToScene(QPoint(100,0));
    return d * (point2.x() - point1.x()) / 100.0;

}

RVector RGraphicsViewQV::mapToView(const RVector& v) {
    QPoint p = mapFromScene(QPointF(v.x,v.y));
    return RVector(p.x(),p.y());
}

RVector RGraphicsViewQV::mapFromView(const RVector& v, double /*z*/) {
    QPointF p = mapToScene(QPoint(v.x,v.y));
    return RVector(p.x(),p.y());
}

void RGraphicsViewQV::viewportChangeEvent() {
    RGraphicsView::viewportChangeEvent();
    emit viewportChanged();
}

