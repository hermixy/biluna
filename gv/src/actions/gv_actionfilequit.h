/*****************************************************************
 * $Id: gv_actionfilequit.h 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 2, 2008 8:31:29 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionfilequit.h 4010
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONFILEQUIT_H
#define GV_ACTIONFILEQUIT_H

#include "gv_actionbase.h"


/**
 * This action class can handle user events to quit the application.
 */
class GV_ActionFileQuit : public GV_ActionBase {
    Q_OBJECT
public:
    GV_ActionFileQuit(GV_Drawing& dwg, GV_GraphicsView& gv);
    ~GV_ActionFileQuit() {}

    static RB_String getName() {
        return "File Quit";
    }

    virtual RB_String name() {
        return GV_ActionFileQuit::getName();
    }

    static RB_GuiAction* createGuiAction();
};

#endif
