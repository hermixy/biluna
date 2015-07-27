/*****************************************************************
 * $Id: gv_rotationlever.h 1380 2011-03-07 20:47:51Z rutger $
 * Created: Apr 2, 2008 9:01:42 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ROTATIONLEVER_H
#define GV_ROTATIONLEVER_H

#include <QGraphicsPolygonItem>
#include <QGraphicsSceneMouseEvent>
#include "rb_string.h"

class GV_Component;


/**
 * Lever to rotate a graphics item
 */
class GV_RotationLever : public QGraphicsPolygonItem {

public:
    GV_RotationLever(QGraphicsItem* parent);
	GV_RotationLever(GV_RotationLever* lever);
	virtual ~GV_RotationLever();

    void draw();
	
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent* e);
    void mousePressEvent(QGraphicsSceneMouseEvent* e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* e);

private:
    //! Context prefix such as GV and RPT
    RB_String mContextPrefix;

};

#endif /*GV_ROTATIONLEVER_H*/
