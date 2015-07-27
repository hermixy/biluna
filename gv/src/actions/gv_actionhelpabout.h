/*****************************************************************
 * $Id: gv_actionhelpabout.h 1135 2010-04-19 21:19:17Z rutger $
 * Created: Jan 23, 2009 2:51:34 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONHELPABOUT_H
#define GV_ACTIONHELPABOUT_H

#include "gv_actionbase.h"


/**
 * This action triggers an help about dialog.
 */
class GV_ActionHelpAbout : public GV_ActionBase {
	
    Q_OBJECT
    
public:
    GV_ActionHelpAbout(GV_Drawing& dwg, GV_GraphicsView& gv);
    virtual ~GV_ActionHelpAbout() {}

    static RB_String getName() {
        return "Help About";
    }

    virtual RB_String name() {
        return GV_ActionHelpAbout::getName();
    }

    static RB_GuiAction* createGuiAction();
};

#endif  /*GV_ACTIONHELPABOUT_H*/
