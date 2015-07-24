/*****************************************************************
 * $Id: sail_objectfactory.h 1723 2012-07-27 14:42:24Z rutger $
 * Created: Jul 26, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SAIL_OBJECTFACTORY_H
#define SAIL_OBJECTFACTORY_H

#include <iostream>
#include <string>

#include "rb_objectatomic.h"
#include "rb_objectcontainer.h"
#include "rb_objectfactory.h"


#define SAIL_OBJECTFACTORY SAIL_ObjectFactory::getInstance()


/**
 * Instance (static) class which acts as an object factory.
 * This class is instantiated once and uses the interface
 * RB_ObjectFactoryInterface to interact with general Rb_ObjectAtomic,
 * RB_ObjectBase, RB_ObjectContainer, RB_ObjectIterator, RB_ObjectMember
 * functions. In the model use SAIL_OBJECTFACTORY->function() to create objects
 * or test whether it is an object of this model.
 */
class SAIL_ObjectFactory : public RB_ObjectFactory {

public:
    virtual ~SAIL_ObjectFactory();
    static SAIL_ObjectFactory* getInstance();

    virtual RB_ObjectBase* newObject(const RB_String& id,
                                RB_ObjectBase* parent,
                                const RB_String& typeName = "",
                                bool addToParent = true);

    RB_String mainListName() const { return "SAIL_ProjectList"; }

    bool isSystemSetting(const RB_String& name);
    bool isProjectSetting(const RB_String& name);

private:
    SAIL_ObjectFactory();
    static SAIL_ObjectFactory* mActiveObjectFactory;

};

#endif // SAIL_OBJECTFACTORY_H
