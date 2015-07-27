/*****************************************************************
 * $Id: gv_actionfilesave.h 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 2, 2008 8:34:43 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionfilesave.h 10343
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONFILESAVE_H
#define GV_ACTIONFILESAVE_H

#include "gv_actionbase.h"


/**
 * This action class can handle user events to save the current file.
 */
class GV_ActionFileSave : public GV_ActionBase {
    Q_OBJECT
public:
    GV_ActionFileSave(GV_Drawing& dwg, GV_GraphicsView& gv);
    ~GV_ActionFileSave() {}

    static RB_String getName() {
        return "File Save";
    }

    virtual RB_String name() {
        return GV_ActionFileSave::getName();
    }

    static RB_GuiAction* createGuiAction();

};

#endif
