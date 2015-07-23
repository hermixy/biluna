/*****************************************************************
 * $Id: db_actionfilepdf.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONFILEPDF_H
#define DB_ACTIONFILEPDF_H

#include "rb_action.h"

class RB_Dialog;


/**
 * This action class can handle user events to save the current file as PDF.
 */
class DB_EXPORT DB_ActionFilePdf : public RB_Action {

    Q_OBJECT

public:
    DB_ActionFilePdf();
    virtual ~DB_ActionFilePdf() {}

    static RB_String getName() {
        return "File Pdf";
    }

    virtual RB_String name() {
        return DB_ActionFilePdf::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();
    virtual void printDialog(RB_Dialog* dialog);

    void getSaveFileName(RB_String& fn, const RB_String& filters = RB_String());
};

#endif
