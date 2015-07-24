/*****************************************************************
 * $Id: srm_objectfactory.h 1893 2013-02-21 21:41:33Z rutger $
 * Created: Feb 12, 2012 15:34:25 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_OBJECTFACTORY_H
#define SRM_OBJECTFACTORY_H

#include <iostream>
#include <string>

#include "rb_objectatomic.h"
#include "rb_objectcontainer.h"
#include "rb_objectfactory.h"


#define SRM_OBJECTFACTORY SRM_ObjectFactory::getInstance()


/**
 * Instance (static) class which acts as an object factory.
 * This class is instantiated once and uses the interface
 * RB_ObjectFactoryInterface to interact with general Rb_ObjectAtomic,
 * RB_ObjectBase, RB_ObjectContainer, RB_ObjectIterator, RB_ObjectMember
 * functions. In the model use DB_OBJECTFACTORY->function() to create objects
 * or test whether it is an object of this model.
 */
class SRM_ObjectFactory : public RB_ObjectFactory {

public:
    virtual ~SRM_ObjectFactory();
    static SRM_ObjectFactory* getInstance();

    virtual RB_ObjectBase* newObject(const RB_String& id,
                                RB_ObjectBase* parent,
                                const RB_String& typeName = "",
                                bool addToParent = true);

    RB_String mainListName() const { return "SRM_ProjectList"; }

    bool isSystemSetting(const RB_String& name);
    bool isProjectSetting(const RB_String& name);

private:
    SRM_ObjectFactory();
    static SRM_ObjectFactory* mActiveObjectFactory;

};

#endif // SRM_OBJECTFACTORY_H
