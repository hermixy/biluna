/*****************************************************************
 * $Id: gv_actionhelpwebsite.h 1135 2010-04-19 21:19:17Z rutger $
 * Created: Jan 23, 2009 3:32:07 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONHELPWEBSITE_H
#define GV_ACTIONHELPWEBSITE_H

#include "gv_actionbase.h"

/**
 * This action triggers the dialog with the website of the application.
 */
class GV_ActionHelpWebsite : public GV_ActionBase {
	
    Q_OBJECT
    
public:
    GV_ActionHelpWebsite(GV_Drawing& dwg, GV_GraphicsView& gv);
    virtual ~GV_ActionHelpWebsite() {}

    static RB_String getName() {
        return "Visit the Website";
    }

    virtual RB_String name() {
        return GV_ActionHelpWebsite::getName();
    }

    static RB_GuiAction* createGuiAction();
};

#endif /*GV_ACTIONHELPWEBSITE_H*/
