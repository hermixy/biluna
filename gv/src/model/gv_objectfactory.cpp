/*****************************************************************
 * $Id: gv_objectfactory.cpp 1450 2011-08-29 17:40:53Z rutger $
 * Created: Apr 23, 2008 10:04:50 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_objectfactory.h"

#include "db_objectfactory.h"
//#include "gv_component.h"
#include "gv_drawing.h"
//#include "gv_equipment.h"
#include "gv_line.h"
#include "gv_node.h"
//#include "gv_pipeline.h"
#include "gv_port.h"
#include "gv_project.h"
#include "gv_rotationlever.h"
#include "gv_symbol.h"
#include "gv_text.h"
//#include "gv.h"


GV_ObjectFactory* GV_ObjectFactory::mActiveObjectFactory = 0;

/**
 * Constructor
 */
GV_ObjectFactory::GV_ObjectFactory() {
    RB_DEBUG->print("GV_ObjectFactory::GV_ObjectFactory()");
    DB_OBJECTFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
GV_ObjectFactory::~GV_ObjectFactory() {
    mActiveObjectFactory = NULL;
    RB_DEBUG->print("ACC_ObjectFactory::~ACC_ObjectFactory() OK");
}


/**
 * Create or get instance
 * @return factory
 */
GV_ObjectFactory* GV_ObjectFactory::getInstance() {
    RB_DEBUG->print("GV_ObjectFactory::getInstance()");

    if (!mActiveObjectFactory) {
        mActiveObjectFactory = new GV_ObjectFactory();
        // initiate factory
        mActiveObjectFactory->createModelStructure("GV_Project");
        mActiveObjectFactory->setName("GV_ObjectFactory");
    }
    return mActiveObjectFactory;
}

/**
 * Create new object connected to and part of parent.
 * @param id identification of object (Uuid)
 * @param parent parent list (container) of the object, if NULL the root object
 *        will be returned
 * @param typeName name of the parent list (container) for symbol- and linetype
 * @return object, connected to parent
 */
RB_ObjectBase* GV_ObjectFactory::newObject(const RB_String& id,
                                            RB_ObjectBase* parent,
                                            const RB_String& /* typeName */,
                                            bool addToParent) {
    RB_DEBUG->print("GV_ObjectFactory::newObject()");

	RB_String uuid = id;
	if (uuid == "") {
		uuid = RB_Uuid::createUuid().toString();
	}
	
	RB_String str = "";
	if (parent != NULL) {
		str = parent->getName();
	} else {
		str = "GV_";
	}
	
    RB_ObjectContainer* list = NULL;
    RB_ObjectBase* obj = NULL;

    if (str == "GV_") {
        obj = new GV_Project(uuid, NULL, "GV_Project", this);

        // Note: always main container list first, for object model managers
        uuid = ""; // RB_Uuid::createUuid().toString(); No Uuid for ..Lists
        list = new RB_ObjectContainer(uuid, obj, "GV_DrawingList", this);
        obj->addObject(list);

    } else if (str == "GV_DrawingList") {
        obj = new GV_Drawing(uuid, parent, "GV_Drawing", this);
		
        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "GV_SymbolList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "GV_LineList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "GV_TextList", this);
        obj->addObject(list);
        // drawing global node list
        list = new RB_ObjectContainer(uuid, obj, "GV_NodeList", this);
        obj->addObject(list);

    } else if (str == "GV_LineList") {
        obj = new GV_Line(uuid, parent, "GV_Line", this);

        list = new RB_ObjectContainer(uuid, obj, "GV_PortList", this);
        obj->addObject(list);
        obj->init(); // extra to create graphicitem handles

    } else if (str == "GV_NodeList") {
        obj = new GV_Node(uuid, parent, "GV_Node", this);

    } else if (str == "GV_PortList") {
        GV_Component* portParent = dynamic_cast<GV_Component*>(parent->getParent());

        if (portParent) {
            obj = new GV_Port(uuid, parent, "GV_Port", this, portParent->getGraphicsItem());
            portParent->addPort(obj); // to set mPort0 and mPort1
            return obj; // no parent->addObject, is already done with portParent->addPort(obj)
        } else {
            // in case of copy or clone of the object, the graphicsitem is not known
            obj = new GV_Port(uuid, parent, "GV_Port", this, NULL);
        }
	} else if (str == "GV_SymbolList") {
        obj = new GV_Symbol(uuid, parent, "GV_Symbol", this);

        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "GV_PortList", this);
        obj->addObject(list);

        list = new RB_ObjectContainer(uuid, obj, "GV_SymbolList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "GV_LineList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "GV_TextList", this);
        obj->addObject(list);
        // symbol local node list
        list = new RB_ObjectContainer(uuid, obj, "GV_NodeList", this);
        obj->addObject(list);
        obj->init(); // extra to create graphicitem handles

    } else if (str == "GV_TextList") {
        obj = new GV_Text(uuid, parent, "GV_Text", this);

        list = new RB_ObjectContainer(uuid, obj, "GV_PortList", this);
        obj->addObject(list);
        obj->init(); // extra to create graphicitem handles
	}
	
    // Add object to parent and set object parent
    if (parent && addToParent) parent->addObject(obj);
    return obj;
}
