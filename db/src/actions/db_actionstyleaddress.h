/*****************************************************************
 * $Id: db_actionstyleaddress.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Jan 11, 2010 11:25:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONSTYLEADDRESS_H
#define DB_ACTIONSTYLEADDRESS_H

#include "rb_action.h"

/**
 * This action class can handle user events for providing contact information
 * for a document or part of a document, the text will shown in Italics.
 * Information provided by ADDRESS may include the names of the
 * document's maintainers, links to the maintainers' Web pages,
 * e-mail addresses for feedback, postal addresses, phone numbers, and so on.
 * The ADDRESS element is not appropriate for all postal and e-mail addresses
 */
class DB_EXPORT DB_ActionStyleAddress : public RB_Action {

    Q_OBJECT

public:
    DB_ActionStyleAddress();
    virtual ~DB_ActionStyleAddress() {}

    static RB_String getName() {
        return "Edit Select All";
    }

    virtual RB_String name() {
        return DB_ActionStyleAddress::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif
