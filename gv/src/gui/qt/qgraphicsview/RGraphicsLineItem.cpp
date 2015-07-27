#include "RGraphicsLineItem.h"

#include "RDebug.h"



RGraphicsLineItem::RGraphicsLineItem(const QPointF& p1, const QPointF& p2, QGraphicsItem* parent)
    : RGraphicsItem(parent) {

    line = new QGraphicsLineItem(p1.x(), p1.y(), p2.x(), p2.y(), this);
    line->setBoundingRegionGranularity(0.1);
}



RGraphicsLineItem::~RGraphicsLineItem() {
}
    
    
    
void RGraphicsLineItem::setHighlighted(bool on) {
    if (on) {
        pen.setColor(Qt::red);
    } else {
        pen.setColor(Qt::white);
    }
    line->setPen(pen);
}


    
void RGraphicsLineItem::setPen(const QPen& p) {
    line->setPen(p);
    pen = p;
}



QPen RGraphicsLineItem::getPen() {
    return pen;
}


QRectF RGraphicsLineItem::boundingRect() const {
    return QRectF(0, 0, 1, 1);
}
