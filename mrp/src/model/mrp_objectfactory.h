/*****************************************************************
 * $Id: sail_objectfactory.h 1723 2012-07-27 14:42:24Z rutger $
 * Created: Oct 24, 2014 21:17:25 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna MRP project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef MRP_OBJECTFACTORY_H
#define MRP_OBJECTFACTORY_H

#include <iostream>
#include <string>

#include "rb_objectatomic.h"
#include "rb_objectcontainer.h"
#include "rb_objectfactory.h"


#define MRP_OBJECTFACTORY MRP_ObjectFactory::getInstance()


/**
 * Instance (static) class which acts as an object factory.
 * This class is instantiated once and uses the interface
 * RB_ObjectFactoryInterface to interact with general Rb_ObjectAtomic,
 * RB_ObjectBase, RB_ObjectContainer, RB_ObjectIterator, RB_ObjectMember
 * functions. In the model use MRP_OBJECTFACTORY->function() to create objects
 * or test whether it is an object of this model.
 */
class MRP_ObjectFactory : public RB_ObjectFactory {

public:
    virtual ~MRP_ObjectFactory();
    static MRP_ObjectFactory* getInstance();

    virtual RB_ObjectBase* newObject(const RB_String& id,
                                RB_ObjectBase* parent,
                                const RB_String& typeName = "",
                                bool addToParent = true);

    RB_String mainListName() const { return "ACC_ProjectList"; }

    bool isSystemSetting(const RB_String& name);
    bool isProjectSetting(const RB_String& name);

private:
    MRP_ObjectFactory();
    static MRP_ObjectFactory* mActiveObjectFactory;

};

#endif // MRP_OBJECTFACTORY_H
