/*****************************************************************
 * $Id: gv_actioneditcut.h 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 4, 2008 8:29:45 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actioneditcut.h 7530
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONEDITCUT_H
#define GV_ACTIONEDITCUT_H

#include "gv_actioneditcopy.h"


/**
 * This action class can handle user events for cutting components 
 * to the clipboard.
 */
class GV_ActionEditCut : public GV_ActionEditCopy {
	
    Q_OBJECT
    
public:
    GV_ActionEditCut(GV_Drawing& dwg, GV_GraphicsView& gv);
    ~GV_ActionEditCut();

    static RB_String getName() {
        return "Edit Cut";
    }

    virtual RB_String name() {
        return GV_ActionEditCut::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
};

#endif
