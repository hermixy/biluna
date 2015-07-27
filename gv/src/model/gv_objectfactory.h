/*****************************************************************
 * $Id: gv_objectfactory.h 1450 2011-08-29 17:40:53Z rutger $
 * Created: Apr 23, 2008 10:04:58 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_OBJECTFACTORY_H
#define GV_OBJECTFACTORY_H

#include "rb_objectatomic.h"
#include "rb_objectcontainer.h"
#include "rb_objectfactory.h"


#define GV_OBJECTFACTORY GV_ObjectFactory::getInstance()

/**
 * Instance (static) class which acts as an object factory.
 * This class is instantiated once and uses the interface
 * RB_ObjectFactoryInterface to interact with general Rb_ObjectAtomic,
 * RB_ObjectBase, RB_ObjectContainer, RB_ObjectIterator, RB_ObjectMember
 * functions. In the model use GV_OBJECTFACTORY->function() to create objects
 * or test whether it is an object of this model.
 */
class GV_ObjectFactory : public RB_ObjectFactory {
	
public:
    virtual ~GV_ObjectFactory();
    static GV_ObjectFactory* getInstance();
	
    virtual RB_ObjectBase* newObject(const RB_String& id,
                                RB_ObjectBase* parent,
                                const RB_String& typeName = "",
                                bool addToParent = true);
	
    RB_String mainListName() const { return "GV_DrawingList"; }

private:
    GV_ObjectFactory();
    static GV_ObjectFactory* mActiveObjectFactory;

};

#endif /*GV_OBJECTFACTORY_H*/
