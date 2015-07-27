/*****************************************************************
 * $Id: gv_actionformatitemcolor.h 1202 2010-06-14 20:59:27Z rutger $
 * Created: June 12, 2010 11:00:51 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONFORMATITEMCOLOR_H
#define GV_ACTIONFORMATITEMCOLOR_H

#include "gv_actionbase.h"


/**
 * This action class can handle user events to set the item color property
 */
class GV_ActionFormatItemColor : public GV_ActionBase {
	
    Q_OBJECT
    
public:
    GV_ActionFormatItemColor(GV_Drawing& dwg, GV_GraphicsView& gv);
    virtual ~GV_ActionFormatItemColor();

    static RB_String getName() {
        return "Set Item Color";
    }

    virtual RB_String name() {
        return GV_ActionFormatItemColor::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    
    virtual void init(int status = 0);
    virtual void trigger();
    
protected:
    
};

#endif
