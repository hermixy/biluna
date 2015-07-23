/*****************************************************************
 * $Id: db_actioneditcopy.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Jan 5, 2010 5:48:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONEDITCOPY_H
#define DB_ACTIONEDITCOPY_H

#include "rb_action.h"

/**
 * This action class can handle user events for copying selections
 * to the clipboard.
 */
class DB_EXPORT DB_ActionEditCopy : public RB_Action {

    Q_OBJECT

public:
    DB_ActionEditCopy();
    virtual ~DB_ActionEditCopy() {}

    static RB_String getName() {
        return "Edit Copy";
    }

    virtual RB_String name() {
        return DB_ActionEditCopy::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

//    public:
//    /**
//     * Action States.
//     */
//    enum Status {
//        SetReferencePoint    /**< Setting the reference point. */
//    };
//
//public:
//    RS_ActionEditCopy(RS_EntityContainer& container,
//                      RS_GraphicView& graphicView,
//                      bool copy,
//                      bool autoRef);
//    ~RS_ActionEditCopy();
//
//    static RS_String getName() {
//        return "Edit Copy";
//    }
//
//    virtual RS_String name() {
//        return RS_ActionEditCopy::getName();
//    }
//
//    static RS_GuiAction* createGuiAction();
//    static RS_ActionInterface* factory();
//
//    virtual void init(int status=0);
//
//    virtual void trigger();
//
//    virtual void mouseMoveEvent(RS_MouseEvent* e);
//    virtual void mouseReleaseEvent(RS_MouseEvent* e);
//
//    virtual void coordinateEvent(RS_CoordinateEvent* e);
//
//    virtual void updateMouseButtonHints();
//    virtual void updateMouseCursor();
//    virtual void updateToolBar();
//
//protected:
//    /** Copy (true) or cut (false) */
//    bool copy;
//    /** Auto reference point on copy / cut (true) or
//        set reference point manually (false) */
//    bool autoRef;
//
//    RS_Vector referencePoint;
};

#endif
