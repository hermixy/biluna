/*****************************************************************
 * $Id: pcalc_objectfactory.h 1826 2012-12-08 00:31:33Z rutger $
 * Created: Apr 14, 2010 8:34:25 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PCALC_OBJECTFACTORY_H
#define PCALC_OBJECTFACTORY_H

//#include <iostream>
//#include <string>

#include "rb_objectatomic.h"
#include "rb_objectcontainer.h"
#include "rb_objectfactory.h"


#define PCALC_OBJECTFACTORY PCALC_ObjectFactory::getInstance()


/**
 * Instance (static) class which acts as an object factory.
 * This class is instantiated once and uses the interface
 * RB_ObjectFactory to interact with general Rb_ObjectAtomic,
 * RB_ObjectBase, RB_ObjectContainer, RB_ObjectIterator, RB_ObjectMember
 * functions. In the model use PCALC_OBJECTFACTORY->function() to create objects
 * or test whether it is an object of this model.
 */
class PCALC_ObjectFactory : public RB_ObjectFactory {

public:
    virtual ~PCALC_ObjectFactory();
    static PCALC_ObjectFactory* getInstance();

    virtual RB_ObjectBase* newObject(const RB_String& id,
                                RB_ObjectBase* parent,
                                const RB_String& typeName = "",
                                bool addToParent = true);

    RB_String mainListName() const { return "PCALC_EquipmentList"; }

private:
    PCALC_ObjectFactory();
    static PCALC_ObjectFactory* mActiveObjectFactory;

};

#endif // PCALC_OBJECTFACTORY_H
