/*****************************************************************
 * $Id: gv_actiondeleteitem.h 1135 2010-04-19 21:19:17Z rutger $
 * Created: Feb 11, 2009 9:13:39 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONDELETEITEM_H
#define GV_ACTIONDELETEITEM_H

#include "gv_actionbase.h"
#include "gv_line.h"
#include "rb_list.h"

#include <qaction.h>


/**
 * This action class deletes a graphics item such as line and symbol
 */
class GV_ActionDeleteItem : public GV_ActionBase {
	
    Q_OBJECT

public:
	GV_ActionDeleteItem(GV_Drawing& dwg, GV_GraphicsView& gv);
	virtual ~GV_ActionDeleteItem();

	static RB_String getName() {
        return "Delete Item";
    }

    virtual RB_String name() {
        return GV_ActionDeleteItem::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();

    virtual void init(int status = 0);
    virtual void trigger();
};

#endif /*GV_ACTIONDELETEITEM_H*/
