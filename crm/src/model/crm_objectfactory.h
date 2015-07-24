/*****************************************************************
 * $Id: crm_objectfactory.h 1545 2012-02-11 17:34:25Z rutger $
 * Created: Feb 12, 2012 15:34:25 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_OBJECTFACTORY_H
#define CRM_OBJECTFACTORY_H

#include <iostream>
#include <string>

#include "rb_objectatomic.h"
#include "rb_objectcontainer.h"
#include "rb_objectfactory.h"


#define CRM_OBJECTFACTORY CRM_ObjectFactory::getInstance()


/**
 * Instance (static) class which acts as an object factory.
 * This class is instantiated once and uses the interface
 * RB_ObjectFactoryInterface to interact with general Rb_ObjectAtomic,
 * RB_ObjectBase, RB_ObjectContainer, RB_ObjectIterator, RB_ObjectMember
 * functions. In the model use DB_OBJECTFACTORY->function() to create objects
 * or test whether it is an object of this model.
 */
class CRM_ObjectFactory : public RB_ObjectFactory {

public:
    virtual ~CRM_ObjectFactory();
    static CRM_ObjectFactory* getInstance();

    virtual RB_ObjectBase* newObject(const RB_String& id,
                                RB_ObjectBase* parent,
                                const RB_String& typeName = "",
                                bool addToParent = true);

    RB_String mainListName() const { return "CRM_ProjectList"; }

    bool isSystemSetting(const RB_String& name);
    bool isProjectSetting(const RB_String& name);

private:
    CRM_ObjectFactory();
    static CRM_ObjectFactory* mActiveObjectFactory;

};

#endif // CRM_OBJECTFACTORY_H
