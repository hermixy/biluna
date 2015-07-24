/*****************************************************************
 * $Id: acc_objectfactory.h 1517 2012-01-16 16:04:02Z rutger $
 * Created: Dec 18, 2009 8:34:25 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_OBJECTFACTORY_H
#define ACC_OBJECTFACTORY_H

#include <iostream>
#include <string>

#include "rb_objectatomic.h"
#include "rb_objectcontainer.h"
#include "rb_objectfactory.h"


#define ACC_OBJECTFACTORY ACC_ObjectFactory::getInstance()


/**
 * Instance (static) class which acts as an object factory.
 * This class is instantiated once and uses the interface
 * RB_ObjectFactoryInterface to interact with general Rb_ObjectAtomic,
 * RB_ObjectBase, RB_ObjectContainer, RB_ObjectIterator, RB_ObjectMember
 * functions. In the model use DB_OBJECTFACTORY->function() to create objects
 * or test whether it is an object of this model.
 */
class ACC_ObjectFactory : public RB_ObjectFactory {

public:
    virtual ~ACC_ObjectFactory();
    static ACC_ObjectFactory* getInstance();

    virtual RB_ObjectBase* newObject(const RB_String& id,
                                RB_ObjectBase* parent,
                                const RB_String& typeName = "",
                                bool addToParent = true);

    RB_String mainListName() const { return "ACC_CompanyList"; }

    bool isSystemSetting(const RB_String& name);
    bool isProjectSetting(const RB_String& name);

private:
    ACC_ObjectFactory();
    static ACC_ObjectFactory* mActiveObjectFactory;

};

#endif // ACC_OBJECTFACTORY_H
