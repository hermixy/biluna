/*****************************************************************
 * $Id: gv_actionviewdraft.h 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 5, 2008 12:27:46 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionviewdraft.h ?
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONVIEWDRAFT_H
#define GV_ACTIONVIEWDRAFT_H

#include "gv_actionbase.h"


/**
 * This action toggles the draft mode.
 */
class GV_ActionViewDraft : public GV_ActionBase {

	Q_OBJECT

public:
	GV_ActionViewDraft(GV_Drawing& dwg, GV_GraphicsView& gv);
    ~GV_ActionViewDraft() {}

    static RB_String getName() {
        return "View Draft";
    }

    virtual RB_String name() {
        return GV_ActionViewDraft::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();

	virtual void init(int status=0);
	virtual void trigger();
    
private:
    static RB_GuiAction* action;
};

#endif
