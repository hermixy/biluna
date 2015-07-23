/*****************************************************************
 * $Id: db_actioneditundo.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Jan 5, 2010 5:48:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONEDITUNDO_H
#define DB_ACTIONEDITUNDO_H

#include "rb_action.h"

/**
 * This action class can handle user events for undo.
 */
class DB_EXPORT DB_ActionEditUndo : public RB_Action {

    Q_OBJECT

public:
    DB_ActionEditUndo();
    virtual ~DB_ActionEditUndo() {}

    static RB_String getName() {
        return "Edit Undo";
    }

    virtual RB_String name() {
        return DB_ActionEditUndo::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

//public:
//    RS_ActionEditUndo(RS_EntityContainer& container,
//                      RS_GraphicView& graphicView,
//                      bool undo);
//    ~RS_ActionEditUndo();
//
//    static RS_String getName() {
//        return "Edit Undo";
//    }
//
//    virtual RS_String name() {
//        return RS_ActionEditUndo::getName();
//    }
//
//    static RS_GuiAction* createGuiAction();
//    static RS_ActionInterface* factory();
//
//    virtual void init(int status=0);
//    virtual void trigger();
//
//protected:
//    /** Undo (true) or redo (false) */
//    bool undo;
};

#endif
