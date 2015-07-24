/*****************************************************************
 * $Id: acc_project.h 1380 2011-03-07 20:47:51Z rutger $
 * Created: Dec 18, 2009 9:04:25 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_PROJECT_H
#define ACC_PROJECT_H

#include "rb_objectcontainer.h"

/**
 * Root class for the ACC model, is table companies in webERP
 */
class ACC_Project : public RB_ObjectContainer {

public:
    ACC_Project(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_Project(ACC_Project* project);
    virtual ~ACC_Project();

private:
    void createMembers();

};

#endif // ACC_PROJECT_H
