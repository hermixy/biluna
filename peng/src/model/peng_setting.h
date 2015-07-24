/*****************************************************************
 * $Id: peng_setting.h 2155 2014-08-04 16:50:28Z rutger $
 * Created: Aug 4, 2014 13:04:25 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_SETTING_H
#define PENG_SETTING_H

#include "rb_objectcontainer.h"

/**
 * Setting model data
 */
class PENG_Setting : public RB_ObjectContainer {

public:
    PENG_Setting(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    PENG_Setting(PENG_Setting* project);
    virtual ~PENG_Setting();

private:
    void createMembers();

};

#endif // PENG_SETTING_H
