/*****************************************************************
 * $Id: std_objectfactory.h 0001 2016-01-06 00:31:33Z rutger $
 * Created: Jan 06, 2016 8:34:25 PM - rutger
 *
 * Copyright (C) 2016 Biluna. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_OBJECTFACTORY_H
#define STD_OBJECTFACTORY_H

#include "rb_objectatomic.h"
#include "rb_objectcontainer.h"
#include "rb_objectfactory.h"

#define STD_OBJECTFACTORY STD_ObjectFactory::getInstance()


/**
 * Instance (static) class which acts as an object factory.
 * This class is instantiated once and uses the interface
 * RB_ObjectFactory to interact with general Rb_ObjectAtomic,
 * RB_ObjectBase, RB_ObjectContainer, RB_ObjectIterator, RB_ObjectMember
 * functions. In the model use STD_OBJECTFACTORY->function() to create objects
 * or test whether it is an object of this model.
 */
class STD_ObjectFactory : public RB_ObjectFactory {

public:
    virtual ~STD_ObjectFactory();
    static STD_ObjectFactory* getInstance();

    virtual RB_ObjectBase* newObject(const RB_String& id,
                                RB_ObjectBase* parent,
                                const RB_String& typeName = "",
                                bool addToParent = true);

    RB_String mainListName() const { return "STD_ProjectList"; } // TODO: remove

private:
    STD_ObjectFactory();
    static STD_ObjectFactory* mActiveObjectFactory;

};

#endif // STD_OBJECTFACTORY_H
