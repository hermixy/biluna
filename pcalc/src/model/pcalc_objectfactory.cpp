/*****************************************************************
 * $Id: pcalc_objectfactory.cpp 2232 2015-04-23 16:04:08Z rutger $
 * Created: Apr 14, 2010 8:34:25 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "pcalc_objectfactory.h"

#include "db_objectfactory.h"
#include "pcalc_classgenerator.h"
#include "pcalc_en1591_assembly.h"
#include "pcalc_en1591_boltnutwasher.h"
#include "pcalc_en1591_flange.h"
#include "pcalc_en1591_gasket.h"
#include "pcalc_en1591_loadcase.h"
#include "pcalc_en1591_shell.h"
#include "pcalc_generatorformula.h"
#include "peng_project.h"
#include "rb_debug.h"
#include "rb_uuid.h"
#include "std_en13555egeg.h"
#include "std_en13555gasket.h"
#include "std_en13555manufacturer.h"
#include "std_en13555pqrdeltaegc.h"
#include "std_en13555qminl.h"
#include "std_en13555qsmaxalphag.h"
#include "std_en13555qsminl.h"
#include "std_creeptable.h"
#include "std_elasmodul.h"
#include "std_elasmodultable.h"
#include "std_extpresstable.h"
#include "std_material.h"
#include "std_rmmin.h"
#include "std_rp10t10kh.h"
#include "std_rp10t100kh.h"
#include "std_rpt10kh.h"
#include "std_rpt100kh.h"
#include "std_rpt200kh.h"
#include "std_thermexp.h"
#include "std_thermexptable.h"


PCALC_ObjectFactory* PCALC_ObjectFactory::mActiveObjectFactory = 0;


/**
 * Constructor
 */
PCALC_ObjectFactory::PCALC_ObjectFactory() {
    RB_DEBUG->print("PCALC_ObjectFactory::PCALC_ObjectFactory()");
    DB_OBJECTFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
PCALC_ObjectFactory::~PCALC_ObjectFactory() {
    DB_OBJECTFACTORY->unregisterFactory(this);
    mActiveObjectFactory = NULL;
    RB_DEBUG->print("PCALC_ObjectFactory::~PCALC_ObjectFactory() OK");
}

/**
 * Create or get instance
 * @return factory
 */
PCALC_ObjectFactory* PCALC_ObjectFactory::getInstance() {
    RB_DEBUG->print("PCALC_ObjectFactory::getInstance()");

    if (!mActiveObjectFactory) {
        mActiveObjectFactory = new PCALC_ObjectFactory();
        // initiate factory
        mActiveObjectFactory->createModelStructure("PCALC_Project");
        mActiveObjectFactory->setName("PCALC_ObjectFactory");
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
RB_ObjectBase* PCALC_ObjectFactory::newObject(const RB_String& id,
                                            RB_ObjectBase* parent,
                                            const RB_String& /* typeName */,
                                            bool addToParent) {
    RB_String uuid = id;
    RB_String str = "";

    if (parent != NULL) {
        str = parent->getName();
    } else {
        str = "PCALC_"; // root
    }

    RB_ObjectContainer* list = NULL;
    RB_ObjectBase* obj = NULL;

    if (str == "PCALC_") { // root
        obj = new PENG_Project(uuid, NULL, "PENG_Project", this);

        list = new RB_ObjectContainer (uuid, obj, "PCALC_EN1591_AssemblyList", this);
        obj->addObject(list);
        // actually top level objects, here only to generate objects
        list = new RB_ObjectContainer (uuid, obj, "PCALC_ClassGeneratorList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "STD_CreepTableList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "STD_ElasModulTableList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "STD_EN13555ManufacturerList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "STD_ExtPressTableList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "STD_MaterialList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "STD_ThermExpTableList", this);
        obj->addObject(list);
    } else if (str == "PCALC_ClassGeneratorList") {
        obj = new PCALC_ClassGenerator(uuid, parent, "PCALC_ClassGenerator", this);

        uuid = "";
        list = new RB_ObjectContainer (uuid, obj, "PCALC_GeneratorFormulaList", this);
    } else if (str == "PCALC_EN1591_AssemblyList") {
        obj = new PCALC_EN1591_Assembly(uuid, parent, "PCALC_EN1591_Assembly", this);

        uuid = "";
        list = new RB_ObjectContainer (uuid, obj, "PCALC_EN1591_BoltNutWasherList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "PCALC_EN1591_FlangeList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "PCALC_EN1591_GasketList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "PCALC_EN1591_LoadCaseList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "PCALC_EN1591_ShellList", this);
        obj->addObject(list);
    } else if (str == "PCALC_EN1591_BoltNutWasherList") {
        obj = new PCALC_EN1591_BoltNutWasher(uuid, parent, "PCALC_EN1591_BoltNutWasher", this);
    } else if (str == "PCALC_EN1591_FlangeList") {
        obj = new PCALC_EN1591_Flange(uuid, parent, "PCALC_EN1591_Flange", this);
    } else if (str == "PCALC_EN1591_GasketList") {
        obj = new PCALC_EN1591_Gasket(uuid, parent, "PCALC_EN1591_Gasket", this);
    } else if (str == "PCALC_EN1591_LoadCaseList") {
        obj = new PCALC_EN1591_LoadCase(uuid, parent, "PCALC_EN1591_LoadCase", this);
    } else if (str == "PCALC_EN1591_ShellList") {
        obj = new PCALC_EN1591_Shell(uuid, parent, "PCALC_EN1591_Shell", this);
    } else if (str == "PCALC_GeneratorFormulaList") {
        obj = new PCALC_GeneratorFormula(uuid, parent, "PCALC_GeneratorFormula", this);
    } else if (str == "STD_CreepTableList") {
        obj = new STD_CreepTable(uuid, parent, "STD_CreepTable", this);

        uuid = "";
        list = new RB_ObjectContainer (uuid, obj, "STD_Rp10T10khList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "STD_Rp10T100khList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "STD_RpT10khList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "STD_RpT100khList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "STD_RpT200khList", this);
        obj->addObject(list);
    } else if (str == "STD_ElasModulList") {
        obj = new STD_ElasModul(uuid, parent, "STD_ElasModul", this);
    } else if (str == "STD_ElasModulTableList") {
        obj = new STD_ElasModulTable(uuid, parent, "STD_ElasModulTable", this);

        uuid = "";
        list = new RB_ObjectContainer (uuid, obj, "STD_ElasModulList", this);
        obj->addObject(list);
    } else if (str == "STD_EN13555EGeGList") {
        obj = new STD_EN13555EGeG(uuid, parent, "STD_EN13555EGeG", this);
    } else if (str == "STD_EN13555GasketList") {
        obj = new STD_EN13555Gasket(uuid, parent, "STD_EN13555Gasket", this);

        uuid = "";
        list = new RB_ObjectContainer (uuid, obj, "STD_EN13555EGeGList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "STD_EN13555PqrDeltaeGCList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "STD_EN13555QminLList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "STD_EN13555QsmaxAlphaGList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "STD_EN13555QsminLList", this);
        obj->addObject(list);
    } else if (str == "STD_EN13555ManufacturerList") {
        obj = new STD_EN13555Manufacturer(uuid, parent, "STD_EN13555Manufacturer", this);

        uuid = "";
        list = new RB_ObjectContainer (uuid, obj, "STD_EN13555GasketList", this);
        obj->addObject(list);
    } else if (str == "STD_EN13555PqrDeltaeGCList") {
        obj = new STD_EN13555PqrDeltaeGC(uuid, parent, "STD_EN13555PqrDeltaeGC", this);
    } else if (str == "STD_EN13555QminLList") {
        obj = new STD_EN13555QminL(uuid, parent, "STD_EN13555QminL", this);
    } else if (str == "STD_EN13555QsmaxAlphaGList") {
        obj = new STD_EN13555QsmaxAlphaG(uuid, parent, "STD_EN13555QsmaxAlphaG", this);
    } else if (str == "STD_EN13555QsminLList") {
        obj = new STD_EN13555QsminL(uuid, parent, "STD_EN13555QsminL", this);
    } else if (str == "STD_ExtPressTableList") {
        obj = new STD_ExtPressTable(uuid, parent, "STD_ExtPressTable", this);

    // TODO: external pressure tables and values
//        uuid = "";
//        list = new RB_ObjectContainer (uuid, obj, "STD_Rp02List", this);
//        obj->addObject(list);
    } else if (str == "STD_MaterialList") {
        obj = new STD_Material(uuid, parent, "STD_Material", this);

        uuid = "";
        list = new RB_ObjectContainer (uuid, obj, "STD_Rp02List", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "STD_Rp10List", this);
        obj->addObject(list);
    } else if (str == "STD_Rp10T10khList") {
        obj = new STD_Rp10T10kh(uuid, parent, "STD_Rp10T10kh", this);
    } else if (str == "STD_Rp10T100khList") {
        obj = new STD_Rp10T100kh(uuid, parent, "STD_Rp10T100kh", this);
    } else if (str == "STD_RpT10khList") {
        obj = new STD_RpT10kh(uuid, parent, "STD_RpT10kh", this);
    } else if (str == "STD_RpT100khList") {
        obj = new STD_RpT100kh(uuid, parent, "STD_RpT100kh", this);
    } else if (str == "STD_RpT200khList") {
        obj = new STD_RpT200kh(uuid, parent, "STD_RpT200kh", this);
    } else if (str == "STD_ThermExpList") {
        obj = new STD_ThermExp(uuid, parent, "STD_ThermExp", this);
    } else if (str == "STD_ThermExpTableList") {
        obj = new STD_ThermExpTable(uuid, parent, "STD_ThermExpTable", this);

        uuid = "";
        list = new RB_ObjectContainer (uuid, obj, "STD_ThermExpList", this);
        obj->addObject(list);
    }

    // Add object to parent and set object parent
    if (parent && addToParent) parent->addObject(obj);
    return obj;
}
