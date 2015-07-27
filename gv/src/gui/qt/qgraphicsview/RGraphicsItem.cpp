#include "RGraphicsItem.h"

#include "RDebug.h"


RGraphicsItem::RGraphicsItem(QGraphicsItem* parent)
    : QGraphicsItem(parent) {

    setBoundingRegionGranularity(1.0);
}



RGraphicsItem::~RGraphicsItem() {
}



QRectF RGraphicsItem::boundingRect() const {
    QRectF ret;

    for (int i=0; i<childItems().count(); i++) {
        if (i==0) {
            ret = childItems().at(i)->boundingRect();
        }
        else {
            ret = ret.united(childItems().at(i)->boundingRect());
        }
    }

    return ret;
}



void RGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    for (int i=0; i<childItems().count(); ++i) {
        childItems().at(i)->paint(painter, option, widget);
    }
}

