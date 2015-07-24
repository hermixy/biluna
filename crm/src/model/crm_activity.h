/*****************************************************************
 * $Id: crm_activity.h 1545 2012-02-11 17:34:25Z rutger $
 * Created: Feb 12, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_ACTIVITY_H
#define CRM_ACTIVITY_H

#include "rb_objectcontainer.h"

/**
 * Class for activities such as bug, case, call,
 * correspondence, meeting, opportunity, project, solution
 */
class CRM_Activity : public RB_ObjectContainer {

public:
    CRM_Activity(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    CRM_Activity(CRM_Activity* obj);
    virtual ~CRM_Activity();

private:
    void createMembers();

};

#endif // CRM_ACTIVITY_H
