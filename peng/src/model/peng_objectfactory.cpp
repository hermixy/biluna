/*****************************************************************
 * $Id: peng_objectfactory.cpp 2233 2015-04-29 19:10:59Z rutger $
 * Created: Apr 14, 2010 8:34:25 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_objectfactory.h"

#include "db_objectfactory.h"
#include "peng_cone.h"
#include "peng_equipment.h"
#include "peng_fluid.h"
#include "peng_fluidstability.h"
#include "peng_hazardgroup.h"
#include "peng_insulationcc.h"
#include "peng_insulationhci.h"
#include "peng_insulationhco.h"
#include "peng_insulationpp.h"
#include "peng_line.h"
#include "peng_lineclassif.h"
#include "peng_linecustom.h"
#include "peng_lineengindes.h"
#include "peng_lineprocess.h"
#include "peng_paint.h"
#include "peng_pedclassif.h"
#include "peng_pipeclass.h"
#include "peng_pipeclasspt.h"
#include "peng_pipeclasssize.h"
#include "peng_project.h"
#include "peng_setting.h"
#include "peng_type.h"
#include "peng_typemember.h"
#include "peng_typesection.h"
#include "peng_typesubsection.h"
#include "peng_weldolet.h"
#include "rb_debug.h"
#include "rb_uuid.h"

PENG_ObjectFactory* PENG_ObjectFactory::mActiveObjectFactory = 0;


/**
 * Constructor
 */
PENG_ObjectFactory::PENG_ObjectFactory() {
    RB_DEBUG->print("PENG_ObjectFactory::PENG_ObjectFactory()");
    DB_OBJECTFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
PENG_ObjectFactory::~PENG_ObjectFactory() {
    DB_OBJECTFACTORY->unregisterFactory(this);
    mActiveObjectFactory = NULL;
    RB_DEBUG->print("PENG_ObjectFactory::~PENG_ObjectFactory() OK");
}

/**
 * Create or get instance
 * @return factory
 */
PENG_ObjectFactory* PENG_ObjectFactory::getInstance() {
    RB_DEBUG->print("PENG_ObjectFactory::getInstance()");

    if (!mActiveObjectFactory) {
        mActiveObjectFactory = new PENG_ObjectFactory();
        // initiate factory
        mActiveObjectFactory->createModelStructure("PENG_Project");
        mActiveObjectFactory->setName("PENG_ObjectFactory");
    }
    return mActiveObjectFactory;
}

/**
 * Create new object connected to and part of parent.
 * @param id identification of object (Uuid)
 * @param parent parent list (container) of the object, if NULL the root object
 *        will be returned
 * @param name name of the parent list (container)
 * @return object, connected to parent
 */
RB_ObjectBase* PENG_ObjectFactory::newObject(const RB_String& id,
                                            RB_ObjectBase* parent,
                                            const RB_String& /* typeName */,
                                            bool addToParent) {
    RB_String uuid = id;
    RB_String str = "";

    if (parent != NULL) {
        str = parent->getName();
    } else {
        str = "PENG_"; // root
    }

    RB_ObjectContainer* list = NULL;
    RB_ObjectBase* obj = NULL;

    if (str == "PENG_") { // root
        obj = new PENG_Project(uuid, NULL, "PENG_Project", this);

        uuid = ""; // RB_Uuid::createUuid().toString(); No Uuid for ..Lists
        list = new RB_ObjectContainer (uuid, obj, "PENG_ConeList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "PENG_EquipmentList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "PENG_FluidList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "PENG_FluidStabilityList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "PENG_HazardGroupList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "PENG_InsulationCCList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "PENG_InsulationHCIList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "PENG_InsulationHCOList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "PENG_InsulationPPList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "PENG_LineList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "PENG_PaintList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "PENG_PipeClassList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "PENG_TypeList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "PENG_SettingList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "PENG_WeldoletList", this);
        obj->addObject(list);
    } else if (str == "PENG_ConeList") {
        obj = new PENG_Cone(uuid, parent, "PENG_Cone", this);
    } else if (str == "PENG_EquipmentList") {
        obj = new PENG_Equipment(uuid, parent, "PENG_Equipment", this);

        uuid = ""; // RB_Uuid::createUuid().toString(); No Uuid for ..Lists
        list = new RB_ObjectContainer (uuid, obj, "PENG_PedClassifList", this);
        obj->addObject(list);
    } else if (str == "PENG_FluidList") {
        obj = new PENG_Fluid(uuid, parent, "PENG_Fluid", this);
    } else if (str == "PENG_FluidStabilityList") {
        obj = new PENG_FluidStability(uuid, parent, "PENG_FluidStability", this);
    } else if (str == "PENG_HazardGroupList") {
        obj = new PENG_HazardGroup(uuid, parent, "PENG_HazardGroup", this);
    } else if (str == "PENG_InsulationCCList") {
        obj = new PENG_InsulationCC(uuid, parent, "PENG_InsulationCC", this);
    } else if (str == "PENG_InsulationHCIList") {
        obj = new PENG_InsulationHCI(uuid, parent, "PENG_InsulationHCI", this);
    } else if (str == "PENG_InsulationHCOList") {
        obj = new PENG_InsulationHCO(uuid, parent, "PENG_InsulationHCO", this);
    } else if (str == "PENG_InsulationPPList") {
        obj = new PENG_InsulationPP(uuid, parent, "PENG_InsulationPP", this);
    } else if (str == "PENG_LineList") {
        obj = new PENG_Line(uuid, parent, "PENG_Line", this);

        uuid = "";
        list = new RB_ObjectContainer (uuid, obj, "PENG_PedClassifList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "PENG_LineClassifList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "PENG_LineCustomList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "PENG_LineEnginDesList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "PENG_LineProcessList", this);
        obj->addObject(list);
    } else if (str == "PENG_LineClassifList") {
        obj = new PENG_LineClassif(uuid, parent, "PENG_LineClassif", this);
    } else if (str == "PENG_LineCustomList") {
        obj = new PENG_LineCustom(uuid, parent, "PENG_LineCustom", this);
    } else if (str == "PENG_LineEnginDesList") {
        obj = new PENG_LineEnginDes(uuid, parent, "PENG_LineEnginDes", this);
    } else if (str == "PENG_LineProcessList") {
        obj = new PENG_LineProcess(uuid, parent, "PENG_LineProcess", this);
    } else if (str == "PENG_PaintList") {
        obj = new PENG_Paint(uuid, parent, "PENG_Paint", this);
    } else if (str == "PENG_PedClassifList") {
        obj = new PENG_PedClassif(uuid, parent, "PENG_PedClassif", this);
    } else if (str == "PENG_PipeClassList") {
        obj = new PENG_PipeClass(uuid, parent, "PENG_PipeClass", this);

        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "PENG_PipeClassPTList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "PENG_PipeClassSizeList", this);
        obj->addObject(list);
    } else if (str == "PENG_PipeClassPTList") {
        obj = new PENG_PipeClassPT(uuid, parent, "PENG_PipeClassPT", this);
    } else if (str == "PENG_PipeClassSizeList") {
        obj = new PENG_PipeClassSize(uuid, parent, "PENG_PipeClassSize", this);
    } else if (str == "PENG_SettingList") {
        obj = new PENG_Setting(uuid, parent, "PENG_Setting", this);
    } else if (str == "PENG_TypeList") {
        obj = new PENG_Type(uuid, parent, "PENG_Type", this);

        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "PENG_TypeMemberList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "PENG_TypeSectionList", this);
        obj->addObject(list);
    } else if (str == "PENG_TypeSectionList") {
        obj = new PENG_TypeSection(uuid, parent, "PENG_TypeSection", this);

        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "PENG_TypeMemberList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "PENG_TypeSubsectionList", this);
        obj->addObject(list);
    } else if (str == "PENG_TypeSubsectionList") {
        obj = new PENG_TypeSubsection(uuid, parent, "PENG_TypeSubsection", this);

        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "PENG_TypeMemberList", this);
        obj->addObject(list);
    } else if (str == "PENG_TypeMemberList") {
        obj = new PENG_TypeMember(uuid, parent, "PENG_TypeMember", this);
    } else if (str == "PENG_WeldoletList") {
        obj = new PENG_Weldolet(uuid, parent, "PENG_Weldolet", this);
    }

    // Add object to parent and set object parent
    if (parent && addToParent) parent->addObject(obj);
    return obj;
}
