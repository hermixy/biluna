/*****************************************************************
 * $Id: gv_actionhelpmanual.h 1135 2010-04-19 21:19:17Z rutger $
 * Created: Jan 23, 2009 3:24:23 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONHELPMANUAL_H
#define GV_ACTIONHELPMANUAL_H

#include "gv_actionbase.h"


/**
 * This action triggers the online help manual dialog.
 */
class GV_ActionHelpManual : public GV_ActionBase {
	
    Q_OBJECT
    
public:
    GV_ActionHelpManual(GV_Drawing& dwg, GV_GraphicsView& gv);
    virtual ~GV_ActionHelpManual() {}

    static RB_String getName() {
        return "Help Online Manual";
    }

    virtual RB_String name() {
        return GV_ActionHelpManual::getName();
    }

    static RB_GuiAction* createGuiAction();
};

#endif /*GV_ACTIONHELPMANUAL_H*/
