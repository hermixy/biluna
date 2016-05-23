/*****************************************************************
 * $Id: db_testdlg.h 1092 2010-02-10 09:52:33Z rutger $
 * Created: Oct 26, 2009 8:34:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_TESTDLG_H
#define DB_TESTDLG_H

#include "rb_objectatomic.h"


/**
 * Class for testing dialog object based on DB_Test parent
 */
class DB_TestDlg : public RB_ObjectAtomic {

public:
    DB_TestDlg(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    DB_TestDlg(DB_TestDlg* testDlg);
    virtual ~DB_TestDlg();

private:
    void createMembers();
};

#endif // DB_TESTDLG_H
