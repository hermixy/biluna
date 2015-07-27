/*****************************************************************
 * $Id: gv_graphicsview.h 1263 2010-07-30 14:07:45Z rutger $
 * Created: Mar 24, 2008 11:11:05 AM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_GRAPHICSVIEW_H
#define GV_GRAPHICSVIEW_H

#include <QGraphicsView>


class GV_GraphicsView : public QGraphicsView {

    Q_OBJECT

public:
    GV_GraphicsView(QWidget* parent = 0);
    virtual ~GV_GraphicsView();
    virtual void setMouseCursor(Qt::CursorShape c);
    virtual void setScene(QGraphicsScene* drawing);
    virtual int getHeight();
    virtual int getWidth();
    virtual void zoomIn();
    virtual void zoomOut();

public slots:
    void slotSliderValueChanged(int value);

protected:
    virtual void wheelEvent(QWheelEvent *event);
    virtual void scaleView(double sf);

    virtual void dragEnterEvent(QDragEnterEvent* e);
    virtual void dragMoveEvent(QDragMoveEvent* e);
    virtual void dropEvent(QDropEvent* e);

    virtual void resizeEvent(QResizeEvent* event);

private:
    bool mBeingScaled;


};

#endif /*GV_GRAPHICSVIEW_H*/
