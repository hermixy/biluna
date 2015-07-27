#ifndef RGRAPHICSITEM_H
#define RGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QPen>



/**
 * 2d graphics item based on Qt's graphics view module. 
 */
class RGraphicsItem : public QGraphicsItem {
public:
    RGraphicsItem(QGraphicsItem* parent=NULL);
    virtual ~RGraphicsItem();

    virtual void setHighlighted(bool on) = 0;
    
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
};

#endif
