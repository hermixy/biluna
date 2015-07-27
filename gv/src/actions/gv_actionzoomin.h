/*****************************************************************
 * $Id: gv_actionzoomin.h 1452 2011-09-10 08:19:50Z rutger $
 * Created: Oct 5, 2008 12:51:33 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionzoomin.h 10343
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONZOOMIN_H
#define GV_ACTIONZOOMIN_H

#include "gv_actionbase.h"


/**
 * This action triggers zoom increase.
 */
class GV_ActionZoomIn : public GV_ActionBase {

    Q_OBJECT

public:
    GV_ActionZoomIn(GV_Drawing& dwg, GV_GraphicsView& gv /* ,
                    RS2::ZoomDirection direction = RS2::In,
                    RS2::Axis axis = RS2::Both,
                    const RVector& center = RVector(false) */);
    ~GV_ActionZoomIn() {}

    static RB_String getName() {
        return "Zoom In";
    }

    virtual RB_String name() {
        return GV_ActionZoomIn::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();

    virtual void init(int status=0);
    virtual void trigger();

protected:
//    RS2::ZoomDirection direction;
//    RS2::Axis axis;
//    RVector center;
};

#endif
