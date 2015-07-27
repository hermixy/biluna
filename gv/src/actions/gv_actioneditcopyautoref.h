/*****************************************************************
 * $Id: gv_actioneditcopyautoref.h 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 4, 2008 8:18:32 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actioneditcopyautoref.h 7530
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONEDITCOPYAUTOREF_H
#define GV_ACTIONEDITCOPYAUTOREF_H

#include "gv_actioneditcopy.h"


/**
 * This action class can handle user events for copying components 
 * to the clipboard.
 */
class GV_ActionEditCopyAutoRef : public GV_ActionEditCopy {
	
    Q_OBJECT
    
public:
    GV_ActionEditCopyAutoRef(GV_Drawing& dwg, GV_GraphicsView& gv);
    ~GV_ActionEditCopyAutoRef();

    static RB_String getName() {
        return "Edit Copy Auto Ref";
    }

    virtual RB_String name() {
        return GV_ActionEditCopyAutoRef::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
};

#endif
