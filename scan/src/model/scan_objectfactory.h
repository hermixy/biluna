/*****************************************************************
 * $Id: scan_objectfactory.h 1401 2011-04-27 20:58:49Z rutger $
 * Created: 2011-04-17 rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SCAN_OBJECTFACTORY_H
#define SCAN_OBJECTFACTORY_H

#include "rb_objectatomic.h"
#include "rb_objectcontainer.h"
#include "rb_objectfactory.h"


#define SCAN_OBJECTFACTORY SCAN_ObjectFactory::getInstance()


/**
 * Instance (static) class which acts as an object factory.
 * This class is instantiated once and uses the interface
 * RB_ObjectFactory to interact with general Rb_ObjectAtomic,
 * RB_ObjectBase, RB_ObjectContainer, RB_ObjectIterator, RB_ObjectMember
 * functions. In the model use SCAN_OBJECTFACTORY->function() to create objects
 * or test whether it is an object of this model.
 */
class SCAN_ObjectFactory : public RB_ObjectFactory {

public:
    virtual ~SCAN_ObjectFactory();
    static SCAN_ObjectFactory* getInstance();

    virtual RB_ObjectBase* newObject(const RB_String& id,
                                RB_ObjectBase* parent,
                                const RB_String& typeName = "",
                                bool addToParent = true);

    RB_String mainListName() const { return "SCAN_ScanList"; }

private:
    SCAN_ObjectFactory();
    static SCAN_ObjectFactory* mActiveObjectFactory;

};

#endif // SCAN_OBJECTFACTORY_H
