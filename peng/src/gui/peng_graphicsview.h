/*****************************************************************
 * $Id: peng_graphicsview.h 1826 2012-12-08 00:31:33Z rutger $
 * Created: Mar 24, 2008 11:11:05 AM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_GRAPHICSVIEW_H
#define PENG_GRAPHICSVIEW_H

#include <QGraphicsView>


class PENG_GraphicsView : public QGraphicsView {

    Q_OBJECT

public:
    PENG_GraphicsView(QWidget* parent = 0);
    virtual ~PENG_GraphicsView();
	virtual void setMouseCursor(Qt::CursorShape c);
    virtual void setScene(QGraphicsScene* drawing);
    virtual int getHeight();
    virtual int getWidth();

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

#endif /*PENG_GRAPHICSVIEW_H*/
