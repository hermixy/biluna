/*****************************************************************
 * $Id: db_objectfactory.h 2248 2015-06-21 09:13:00Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna LDT project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_OBJECTFACTORY_H
#define DB_OBJECTFACTORY_H

#include <iostream>
#include <string>

#include "rb_objectatomic.h"
#include "rb_objectcontainer.h"
#include "rb_objectfactory.h"


#define DB_OBJECTFACTORY DB_ObjectFactory::getInstance()


/**
 * Instance (static) class which acts as an object factory. 
 * This class is instantiated once and uses the interface 
 * RB_ObjectFactory to interact with general Rb_ObjectAtomic,
 * RB_ObjectBase, RB_ObjectContainer, RB_ObjectIterator, RB_ObjectMember 
 * functions. In the model use DB_OBJECTFACTORY->function() to create objects
 * or test whether it is an object of this model.
 */
class DB_EXPORT DB_ObjectFactory : public RB_ObjectFactory {
	
public:
    virtual ~DB_ObjectFactory();
    static DB_ObjectFactory* getInstance();
	
    virtual RB_ObjectBase* newObject(const RB_String& id,
                                     RB_ObjectBase* parent,
                                     const RB_String& typeName = "",
                                     bool addToParent = true);
	
    RB_String mainListName() const { return "DB_MapList"; }

    virtual RB_ObjectFactory* getFactory(const RB_String& perspective);
    virtual std::vector<RB_ObjectFactory*> getFactoryList();
    virtual void registerFactory(RB_ObjectFactory* factory);
    virtual void unregisterFactory(RB_ObjectFactory* factory);

private:
    DB_ObjectFactory();
    //! Unique instance of this object
    static DB_ObjectFactory* mActiveObjectFactory;
    //! List of all currently created object factories
    std::vector<RB_ObjectFactory*> mFactoryList;
};

#endif /*DB_OBJECTFACTORY_H*/
