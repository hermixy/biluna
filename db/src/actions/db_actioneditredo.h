/*****************************************************************
 * $Id: db_actioneditredo.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Jan 5, 2010 5:48:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONEDITREDO_H
#define DB_ACTIONEDITREDO_H

#include "rb_action.h"

/**
 * This action class can handle user events for redo.
 */
class DB_EXPORT DB_ActionEditRedo : public RB_Action{

    Q_OBJECT

public:
    DB_ActionEditRedo();
    virtual ~DB_ActionEditRedo() {}

    static RB_String getName() {
        return "Edit Redo";
    }

    virtual RB_String name() {
        return DB_ActionEditRedo::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();



//public:
//    RS_ActionEditRedo(RS_EntityContainer& container,
//                      RS_GraphicView& graphicView);
//    ~RS_ActionEditRedo();
//
//    static RS_String getName() {
//        return "Edit Redo";
//    }
//
//    virtual RS_String name() {
//        return RS_ActionEditRedo::getName();
//    }
//
//    static RS_GuiAction* createGuiAction();
//    static RS_ActionInterface* factory();
};

#endif
