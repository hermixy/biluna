#ifndef RGRAPHICSLINEITEM_H
#define RGRAPHICSLINEITEM_H

#include "RGraphicsItem.h"

#include <QGraphicsLineItem>



/**
 * 2d graphics view based on Qt's graphics view. 
 */
class RGraphicsLineItem : public RGraphicsItem {
public:
    RGraphicsLineItem(const QPointF& p1, const QPointF& p2, QGraphicsItem* parent=NULL);
    virtual ~RGraphicsLineItem();
    
    virtual void setHighlighted(bool on);
    
    void setPen(const QPen& p);
    QPen getPen();
    
    QRectF boundingRect() const;
    
protected:
    QPen pen;
    QGraphicsLineItem* line;
};

#endif
