/*****************************************************************
 * $Id: scan_objectfactory.cpp 1419 2011-05-16 13:43:55Z rutger $
 * Created: 2011-04-17 rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "scan_objectfactory.h"

#include "db_objectfactory.h"
#include "scan_answer.h"
#include "scan_answerresult.h"
#include "scan_block.h"
#include "scan_project.h"
#include "scan_question.h"
#include "scan_questionresult.h"
#include "scan_scan.h"
#include "scan_scanresult.h"
#include "rb_debug.h"
#include "rb_uuid.h"

SCAN_ObjectFactory* SCAN_ObjectFactory::mActiveObjectFactory = 0;


/**
 * Constructor
 */
SCAN_ObjectFactory::SCAN_ObjectFactory() {
    RB_DEBUG->print("SCAN_ObjectFactory::SCAN_ObjectFactory()");
    DB_OBJECTFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
SCAN_ObjectFactory::~SCAN_ObjectFactory() {
    DB_OBJECTFACTORY->unregisterFactory(this);
    mActiveObjectFactory = NULL;
    RB_DEBUG->print("SCAN_ObjectFactory::~SCAN_ObjectFactory() OK");
}

/**
 * Create or get instance
 * @return factory
 */
SCAN_ObjectFactory* SCAN_ObjectFactory::getInstance() {
    RB_DEBUG->print("SCAN_ObjectFactory::getInstance()");

    if (!mActiveObjectFactory) {
        mActiveObjectFactory = new SCAN_ObjectFactory();
        // initiate factory
        mActiveObjectFactory->createModelStructure("SCAN_Project");
        mActiveObjectFactory->setName("SCAN_ObjectFactory");
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
RB_ObjectBase* SCAN_ObjectFactory::newObject(const RB_String& id,
                                            RB_ObjectBase* parent,
                                            const RB_String& /* typeName */,
                                            bool addToParent) {
    RB_String uuid = id;
    RB_String str = "";

    if (parent != NULL) {
        str = parent->getName();
    } else {
        str = "SCAN_"; // root
    }

    RB_ObjectContainer* list = NULL;
    RB_ObjectBase* obj = NULL;

    if (str == "SCAN_") { // root
        obj = new SCAN_Project(uuid, NULL, "SCAN_Project", this);

        uuid = ""; // RB_Uuid::createUuid().toString(); No Uuid for ..Lists
        list = new RB_ObjectContainer (uuid, obj, "SCAN_ScanList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "SCAN_ScanResultList", this);
        obj->addObject(list);
    } else if (str == "SCAN_AnswerList") {
        obj = new SCAN_Answer(uuid, parent, "SCAN_Answer", this);
    } else if (str == "SCAN_AnswerResultList") {
        obj = new SCAN_AnswerResult(uuid, parent, "SCAN_AnswerResult", this);
    } else if (str == "SCAN_BlockList") {
        obj = new SCAN_Block(uuid, parent, "SCAN_Block", this);

        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "SCAN_BlockList", this);
        obj->addObject(list);
    } else if (str == "SCAN_QuestionList") {
        obj = new SCAN_Question(uuid, parent, "SCAN_Question", this);

        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "SCAN_AnswerList", this);
        obj->addObject(list);
    } else if (str == "SCAN_QuestionResultList") {
        obj = new SCAN_QuestionResult(uuid, parent, "SCAN_QuestionResult", this);

        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "SCAN_AnswerResultList", this);
        obj->addObject(list);
    } else if (str == "SCAN_ScanList") {
        obj = new SCAN_Scan(uuid, parent, "SCAN_Scan", this);

        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "SCAN_QuestionList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "SCAN_BlockList", this);
        obj->addObject(list);
    } else if (str == "SCAN_ScanResultList") {
        obj = new SCAN_ScanResult(uuid, parent, "SCAN_ScanResult", this);

        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "SCAN_QuestionResultList", this);
        obj->addObject(list);
    }

    // Add object to parent and set object parent
    if (parent && addToParent) parent->addObject(obj);
    return obj;
}
