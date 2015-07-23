/*****************************************************************
 * $Id: db_actionfilenewhtmlview.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Jan 13, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONFILENEWHTMLVIEW_H
#define DB_ACTIONFILENEWHTMLVIEW_H

#include "rb_action.h"


/**
 * This action class can handle user events to create a new HTML viewer.
 */
class DB_EXPORT DB_ActionFileNewHtmlView : public RB_Action {

    Q_OBJECT

public:
    DB_ActionFileNewHtmlView();
    virtual ~DB_ActionFileNewHtmlView() {}

    static RB_String getName() {
        return "File New HTML Viewer";
    }

    virtual RB_String name() {
        return DB_ActionFileNewHtmlView::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();
};

#endif
