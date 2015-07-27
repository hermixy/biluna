/*****************************************************************
 * $Id: gv_actioneditcutautoref.h 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 4, 2008 8:36:10 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actioneditcutautoref.h 7530
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONEDITCUTAUTOREF_H
#define GV_ACTIONEDITCUTAUTOREF_H

#include "gv_actioneditcopy.h"


/**
 * This action class can handle user events for cutting components 
 * to the clipboard.
 */
class GV_ActionEditCutAutoRef : public GV_ActionEditCopy {
	
    Q_OBJECT
    
public:
    GV_ActionEditCutAutoRef(GV_Drawing& dwg, GV_GraphicsView& gv);
    ~GV_ActionEditCutAutoRef();

    static RB_String getName() {
        return "Edit Cut Auto Ref";
    }

    virtual RB_String name() {
        return GV_ActionEditCutAutoRef::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
};

#endif
