#include "RGraphicsSceneQV.h"

#include <QPainter>
#include <QWidget>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneEvent>

#include "RVector.h"
#include "RDebug.h"
#include "RGraphicsViewQV.h"
#include "RGraphicsLineItem.h"
#include "RLine.h"

RGraphicsSceneQV::RGraphicsSceneQV(RDocumentInterface& di, QObject* parent)
    : QGraphicsScene(parent), RGraphicsScene(di) {
    
    setProjectionRenderingHint(RS::Top);
    setItemIndexMethod(QGraphicsScene::BspTreeIndex);
    setSceneRect(-50000,-50000,100000,100000);

    highlightedItem = NULL;
}

RGraphicsSceneQV::~RGraphicsSceneQV() {
}

void RGraphicsSceneQV::exportLine(const RLine& line, double /*offset*/) {
    if (getEntity()==NULL) {
        RDebug::warning("RGraphicsSceneQV::exportLine: entity is NULL");
        return;
    }

    RGraphicsLineItem* l = new RGraphicsLineItem(QPointF(line.startPoint.x, line.startPoint.y),
                                                 QPointF(line.endPoint.x, line.endPoint.y), NULL);
    l->setPen(currentPen);
    addItem(l);

    //RDebug::debug("RGraphicsSceneQt::exportLine: %d", getEntity()->getId());

    // remove existing painter paths for this entity from spatial index (in exportEntity?):

    // add new painter path with current entity ID:
    /*
    RPainterPath p;
    p.setZLevel(0);
    p.setPen(getCurrentPen());
    p.moveTo(line.startPoint.x, line.startPoint.y);
    p.lineTo(line.endPoint.x, line.endPoint.y);

    painterPaths.insert(getEntity()->getId(), p);
    */
}

void RGraphicsSceneQV::exportLineSegment(const RLine& /*line*/){
}

void RGraphicsSceneQV::exportTriangle(const RTriangle& /*triangle*/) {
    // TODO: implement
}

void RGraphicsSceneQV::highlightEntity(REntity& /*entity*/) {
    /*
    // get painter paths for closest entity:
    QList<RPainterPath> painterPaths = getPainterPaths(entity.getId());
    for (int i=0; i<painterPaths.size(); ++i) {
        QPen pen = painterPaths[i].getPen();
        //pen.setColor(QColor(255,0,0,128));
        pen.setColor(pen.color().darker());
        painterPaths[i].setPen(pen);
    }
    addToPreview(painterPaths);
    */
}

void RGraphicsSceneQV::highlightReferencePoint(const RVector& /*position*/) {
    /*
    // TODO: implement
    QList<RGraphicsViewGl*> glViews = getGlViews();
    QList<RGraphicsViewGl*>::iterator it;
    for (it=glViews.begin(); it!=glViews.end(); it++) {
        RVector screenPosition = (*it)->mapToView(position);

        RDebug::debug("RGraphicsSceneGl::highlightReferencePoint: "
            "screenPosition: %f/%f/%f",
            screenPosition.x, screenPosition.y, screenPosition.z);

        beginPreview();
        (*it)->callReferencePoint(screenPosition, true);
        endPreview();
    }
    */
}


void RGraphicsSceneQV::exportLine(const RVector& p1, const RVector& p2) {
    /*
    pen.setWidth(1);
    pen.setCosmetic(true);
    pen.setStyle(Qt::DashLine);
    pen.setCapStyle(Qt::RoundCap);
    */

    RGraphicsLineItem* line = new RGraphicsLineItem(QPointF(p1.x, p1.y), QPointF(p2.x, p2.y), NULL);
    line->setPen(currentPen);
    addItem(line);
    
    /*
    QGraphicsLineItem* line2 = new QGraphicsLineItem(p1.x+100, p1.y, p2.x+100, p2.y, NULL);
    line2->setPen(currentPen);
    addItem(line2);
    */
}



void RGraphicsSceneQV::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    if (event==NULL) {
        return;
    }

    RGraphicsViewQV* view = getGraphicsView(event);
    if (view==NULL) {
        return;
    }

    double range = view->mapDistanceFromView(10);

    QPointF pos = event->scenePos();
    QPointF relPos(0,0);
    emit mouseMoved(pos, pos-relPos);

    //RDebug::debug("RGraphicsSceneQV::mouseMoveEvent: getting items...");

    QList<QGraphicsItem*> closeItems = 
        items(
            QRectF(
                pos-QPointF(range/2,range/2), 
                pos+QPointF(range/2,range/2)
            ),
            Qt::IntersectsItemShape
        );

    if (closeItems.count()>0 && closeItems.count()<32) {
        RDebug::debug("RGraphicsSceneQV::mouseMoveEvent: "
            "found %d items", closeItems.count());
        //QGraphicsItem* item = closeItems.at(0);
        highlightItem(closeItems);
    }
    else {
        highlightItem(QList<QGraphicsItem*>());
    }
}


/**
 * Mouse wheel event. Zooms in/out or scrolls when
 * shift or ctrl is pressed.
 */
void RGraphicsSceneQV::wheelEvent(QGraphicsSceneWheelEvent* event) {
    if (event==NULL) {
        return;
    }
}

void RGraphicsSceneQV::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (event==NULL) {
        return;
    }
}

void RGraphicsSceneQV::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    if (event==NULL) {
        return;
    }
}



/**
 * @return The graphics view from which the given event originated.
 */
RGraphicsViewQV* RGraphicsSceneQV::getGraphicsView(QGraphicsSceneEvent* event) {
    if (event==NULL) {
        return NULL;
    }

    return dynamic_cast<RGraphicsViewQV*>(event->widget()->parentWidget());
}



void RGraphicsSceneQV::highlightItem(QList<QGraphicsItem*> items) {
    // ignore container items for finding the closest item:
    for (int i=0; i<items.count(); i++) {
        RGraphicsItem* item = dynamic_cast<RGraphicsItem*>(items.at(i));
        if (item==NULL) {
            highlightItem(items.at(i));
            return;
        }
    }
}



void RGraphicsSceneQV::highlightItem(QGraphicsItem* item) {
    /*
    if (highlightedItem==item) {
        return;
    }
    */

    // find a RGraphicsItem instance in ancestors:
    QGraphicsItem* ancestor = item;
    RGraphicsItem* gitem = NULL;
    while (ancestor!=NULL) {
        gitem = dynamic_cast<RGraphicsItem*>(ancestor);
        if (gitem!=NULL) {
            break;
        }
        ancestor = ancestor->parentItem();
    }

    if (highlightedItem!=NULL) {
        RDebug::debug("RGraphicsSceneQV::mouseMoveEvent: dehighlighting item");
        highlightedItem->setHighlighted(false);
        //highlightedItem->update(highlightedItem->boundingRect());
        highlightedItem = NULL;
    }

    if (gitem!=NULL) {
        RDebug::debug("RGraphicsSceneQV::mouseMoveEvent: highlighting item");
        highlightedItem = gitem;
        highlightedItem->setHighlighted(true);
        //RDebug::debug("RGraphicsSceneQV::mouseMoveEvent: updating item");
        //highlightedItem->update(highlightedItem->boundingRect());
        //RDebug::debug("RGraphicsSceneQV::mouseMoveEvent: updating item: OK");
    }
}
