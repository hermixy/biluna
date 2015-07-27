/*****************************************************************
 * $Id: gv_actionfilesaveas.h 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 2, 2008 8:37:55 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionfilesaveas.h 10343
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONFILESAVEAS_H
#define GV_ACTIONFILESAVEAS_H

#include "gv_actionbase.h"


/**
 * This action class can handle user events to export files
 * in various formats.
 */
class GV_ActionFileSaveAs : public GV_ActionBase {
	
    Q_OBJECT
    
public:
    GV_ActionFileSaveAs(GV_Drawing& dwg, GV_GraphicsView& gv);
    ~GV_ActionFileSaveAs() {}

    static RB_String getName() {
        return "File Save As";
    }

    virtual RB_String name() {
        return GV_ActionFileSaveAs::getName();
    }

    static RB_GuiAction* createGuiAction();


};

#endif
