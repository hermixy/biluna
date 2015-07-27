/*****************************************************************
 * $Id: gv_coordinateevent.h 1452 2011-09-10 08:19:50Z rutger $
 * Created: Oct 7, 2008 8:11:30 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_coordinateevent.h 4008
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_COORDINATEEVENT_H
#define GV_COORDINATEEVENT_H

#include "RVector.h"
#include <QPointF>

/**
 * Coordinate Events.
 * TODO: combine getCoordinate() and pos()
 */
class GV_CoordinateEvent {
public:
    GV_CoordinateEvent() {
        this->mCurrentPosition = RVector();
    }

    /**
     * @param pos coordinate
     * @param abs true: absolute coordinate, false: relative coordinate
     */
    GV_CoordinateEvent(const RVector& pos) {
        this->mCurrentPosition = pos;
    }

    /**
     * @return the position of the event in real graphic measures.
     */
    RVector getCoordinate() {
        return mCurrentPosition;
    }

    /**
     * @return true if the position of the coordinate event is valid.
     */
    bool isValid() {
        return mCurrentPosition.valid;
    }

    QPointF pos() {
        mPoint.setX(mCurrentPosition.x);
        mPoint.setY(mCurrentPosition.y);
        return mPoint;
    }
    void setPos(const QPointF& pos) {
        mCurrentPosition.x = pos.x();
        mCurrentPosition.y = pos.y();
    }
    QPointF lastPos() { return mLastPosition; }
    void setLastPos(const QPointF& pos) { mLastPosition = pos; }

protected:
    RVector mCurrentPosition;
    QPointF mPoint;
    QPointF mLastPosition;
};

#endif
