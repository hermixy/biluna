#ifndef RGRAPHICSSCENEQV_H
#define RGRAPHICSSCENEQV_H

#include <QGraphicsScene>
#include <QStack>

#include "RGraphicsScene.h"
#include "RLine.h"
#include "RPoint.h"
#include "RCircle.h"
#include "RArc.h"

class RDocumentInterface;
class RGraphicsItem;
class RGraphicsViewQV;


/**
 * Graphics scene. This class implements the exporter interface to
 * export entities into a scene.
 * \scriptable
 */
class RGraphicsSceneQV : public QGraphicsScene, public RGraphicsScene {

    Q_OBJECT

public:
    RGraphicsSceneQV(RDocumentInterface& di, QObject* parent=0);
    ~RGraphicsSceneQV();
    
    virtual void exportLine(const RVector& p1, const RVector& p2);
    virtual void exportPoint(const RPoint& point) {
    }
    virtual void exportCircle(const RCircle& circle) {
    }
    virtual void exportArc(const RArc& arc, double /*offset*/= RNANDOUBLE) {
    }
    virtual void exportArcSegment(const RArc& arc) {
    }

    void highlightItem(QList<QGraphicsItem*> items);
    void highlightItem(QGraphicsItem* item);

    virtual void exportLine(const RLine& line, double offset = RNANDOUBLE);
    virtual void exportLineSegment(const RLine& line);
    virtual void exportTriangle(const RTriangle& triangle);

    virtual void highlightEntity(REntity& entity);
    virtual void highlightReferencePoint(const RVector& position);

signals:
    void mouseMoved(const QPointF& pos, const QPointF& relPos);

protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    virtual void wheelEvent(QGraphicsSceneWheelEvent* event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    RGraphicsViewQV* getGraphicsView(QGraphicsSceneEvent* event);

    RGraphicsItem* highlightedItem;
};

Q_DECLARE_METATYPE(RGraphicsSceneQV*)

#endif
