/*****************************************************************
 * $Id: gv_actionsetsnapmodefree.h 1267 2010-08-02 16:14:33Z rutger $
 * Created: Oct 23, 2008 2:22:31 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionsetsnapmodefree.h 7068
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONSETSNAPMODEFREE_H
#define GV_ACTIONSETSNAPMODEFREE_H

#include "gv_actionsetsnapmode.h"


/**
 * Sets the current snap mode to free snap.
 */
class GV_ActionSetSnapModeFree : public GV_ActionSetSnapMode {

    Q_OBJECT

public:
    GV_ActionSetSnapModeFree(GV_Drawing& dwg, GV_GraphicsView& gv);
    ~GV_ActionSetSnapModeFree() {}

    static RB_String getName() {
        return "Set Snap Mode Free";
    }

    virtual RB_String name() {
        return GV_ActionSetSnapModeFree::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();

protected:
    static RB_GuiAction* action;
};

#endif //GV_ACTIONSETSNAPMODEFREE_H
