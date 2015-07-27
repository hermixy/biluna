/*****************************************************************
 * $Id: gv_actionfileprint.h 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 2, 2008 8:27:11 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on gv_actionfileprint.h 4010
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONFILEPRINT_H
#define GV_ACTIONFILEPRINT_H

#include "gv_actionbase.h"


/**
 * This action class can handle user events to print the current document.
 */
class GV_ActionFilePrint : public GV_ActionBase {
	
    Q_OBJECT
    
public:
    GV_ActionFilePrint(GV_Drawing& dwg, GV_GraphicsView& gv);
    ~GV_ActionFilePrint() {}

    static RB_String getName() {
        return "File Print";
    }

    virtual RB_String name() {
        return GV_ActionFilePrint::getName();
    }

    static RB_GuiAction* createGuiAction();
};

#endif
