/*****************************************************************
 * $Id: db_actioneditcut.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Jan 5, 2010 5:48:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONEDITCUT_H
#define DB_ACTIONEDITCUT_H

#include "rb_action.h"

/**
 * This action class can handle user events for cutting a selection
 * to the clipboard.
 */
class DB_EXPORT DB_ActionEditCut : public RB_Action {

    Q_OBJECT

public:
    DB_ActionEditCut();
    virtual ~DB_ActionEditCut() {}

    static RB_String getName() {
        return "Edit Cut";
    }

    virtual RB_String name() {
        return DB_ActionEditCut::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

//public:
//    RS_ActionEditCut(RS_EntityContainer& container,
//                      RS_GraphicView& graphicView);
//    ~RS_ActionEditCut();
//
//    static RS_String getName() {
//        return "Edit Cut";
//    }
//
//    virtual RS_String name() {
//        return RS_ActionEditCut::getName();
//    }
//
//    static RS_GuiAction* createGuiAction();
//    static RS_ActionInterface* factory();
};

#endif
