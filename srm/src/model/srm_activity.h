/*****************************************************************
 * $Id: srm_activity.h 1893 2013-02-21 21:41:33Z rutger $
 * Created: Feb 12, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_ACTIVITY_H
#define SRM_ACTIVITY_H

#include "rb_objectcontainer.h"

/**
 * Class for activities such as bug, case, call,
 * correspondence, meeting, opportunity, project, solution
 */
class SRM_Activity : public RB_ObjectContainer {

public:
    SRM_Activity(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    SRM_Activity(SRM_Activity* obj);
    virtual ~SRM_Activity();

private:
    void createMembers();

};

#endif // SRM_ACTIVITY_H
