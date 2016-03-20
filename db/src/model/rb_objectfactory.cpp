/*****************************************************************
 * $Id: rb_objectfactory.cpp 2144 2014-05-01 18:22:24Z rutger $
 * Created: Aug 12, 2009 12:34:25 AM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_objectfactory.h"

#include "rb_objectatomic.h"
#include "rb_objectcontainer.h"
#include "rb_system.h"


/**
 * Constructor. In implementation use the following after the constructor:
 * \li createModelStructure("XXX_Project");
 * \li setName("XXX_ObjectFactory");
 */
RB_ObjectFactory::RB_ObjectFactory() {
    mFactoryIsInitiated = false;
    mCreateNewId = false;
    mObjC = new RB_ObjectContainer("", NULL, "RB_ObjectFactory::RB_ObjectFactory()", this);
    mRoot = NULL;
}

/**
 * Destructor, deletes all other object factories
 */
RB_ObjectFactory::~RB_ObjectFactory() {
    delete mObjC;
    mObjC = NULL;
    delete mRoot;
    mRoot = NULL;
    mObjectNameList.clear();

    // delete all other object factories, now done by models
//    std::vector<RB_ObjectFactory*>::iterator iter;
//    iter = mFactoryList.begin();
//    while (iter != mFactoryList.end()) {
//        RB_ObjectFactory* f = *iter;
//        if(f != this) {
//            delete f;
//        }
//        ++iter;
//    }

    RB_DEBUG->print(QString("RB_ObjectFactory::~RB_ObjectFactory() %1 OK")
                    .arg(this->getName()));
}

/**
 * Create new single object with new id and NULL as parent
 * @param objectName object name
 */
RB_ObjectBase* RB_ObjectFactory::newSingleObject(const RB_String& objectName) {
    if (objectName.endsWith("List", Qt::CaseInsensitive)) {
        return new RB_ObjectContainer("", NULL, objectName, this);
    } else if (objectName.endsWith("_Project", Qt::CaseInsensitive)) {
        return newObject(RB_Uuid::createUuid().toString(), NULL, "", true);
    }

    RB_String str = objectName;
    tableNameToObjectName(str);

    RB_String name = str + "List";
    mObjC->setName(name);

    RB_ObjectBase* obj = newObject(RB_Uuid::createUuid().toString(), mObjC, "", true);
    if (!obj) {
        RB_DEBUG->error(RB_String("RB_ObjectFactory::newSingleObject() "
                                  "%1 does not exist ERROR").arg(objectName));
        return NULL;
    }
    mObjC->clear(); // otherwise obj part of mObjC container
    obj->setParent(NULL);

    return obj;
}

/**
 * Create new parent object with new id and NULL as parent
 * @param childObjectName object name of child
 */
RB_ObjectBase* RB_ObjectFactory::newParentObject(const RB_String& childObjectName) {
    RB_ObjectBase* obj = mRoot->getChildObject(childObjectName);
    if (obj && obj->getParent()) {
        RB_ObjectBase* pObj = obj->getParent()->getParent();
        if (pObj) {
            return newSingleObject(pObj->getName());
        }
    }

    // XXX_Project objects do not have parent, but the model sometimes
    // needs a root or placeholder for the ID or from other perspective
//    if (childObjectName.endsWith("Project", Qt::CaseInsensitive)) {
        return new RB_ObjectContainer("", NULL,
                        "RB_ObjectFactory::newParentObject()", this);
//    }
//    RB_DEBUG->print(RB_Debug::D_ERROR,
//                    "RB_ObjectFactory::newParentObject() "
//                    "no parent ERROR");
//    return NULL;
}

/**
 * Copy object with same id and reference to parent without the children.
 * Function is used by RB_ObjectBase::copy() and clone()
 * @param object to be copied
 */
RB_ObjectBase* RB_ObjectFactory::shallowCopy(RB_ObjectBase* object) {
    // RB_DEBUG->print("RB_ObjectFactory::shallowCopy()");
    return deepCopy(object, RB2::ResolveNone);
}


/**
 * Copy object 'level' deep children with same id and reference to parent.
 * Function is used by RB_ObjectBase::copy() and clone()
 * @param object to be copied
 * @param level level of children to be copied
 * @return copy of object
 */
RB_ObjectBase* RB_ObjectFactory::deepCopy(RB_ObjectBase* object,
                                          RB2::ResolveLevel level) {
    // RB_DEBUG->print("RB_ObjectFactory::deepCopy()");
    RB_ObjectBase* copyObject = resolveCopy(object, level);
    copyObject->setParent(NULL); // sets object and all childrens parent
    copyObject->setCloned(true); // sets object and all children cloned
    return copyObject;
}

/**
 * Recursive part of deepCopy()
 * \todo rewrite with visitor
 * @param object to be copied
 * @param level level of children to be copied
 * @return copy of object of that level
 */
RB_ObjectBase* RB_ObjectFactory::resolveCopy(RB_ObjectBase* object,
                                                      RB2::ResolveLevel level) {
    RB_ObjectBase* copyObject = NULL;
    RB_String objName = object->getName();

    if (objName.endsWith("List")) {
        // Copy container called once if first object is container, after that
        // only objects are passed.
        copyObject = new RB_ObjectContainer("", NULL, object->getName(), this);

        RB_ObjectIterator* iter = object->createIterator();
        for (iter->first(); !iter->isDone(); iter->next()) {
            RB_ObjectBase* childCopyObj = resolveCopy(iter->currentObject(),
                                                     resolveNext(level));
            // childCopyObj->setParent(copyObject); not required
            copyObject->addObject(childCopyObj);
        }
        delete iter;

        copyObject->setOriginal(object);
        return copyObject;
    } else {
        // Copy object, in case of copy id will be reset later, parent will be reset later
        copyObject = newSingleObject(object->getName());
    }

    // copy members
    if (copyObject) {
        *copyObject = *object;
        copyObject->setOriginal(object);
        if (createNewId()) {
            copyObject->setId(RB_Uuid::createUuid().toString());
        } else {
            copyObject->setId(object->getId());
        }
    }

    if (level == RB2::ResolveNone) return copyObject;

    // Find containers and copy children in case of deep copy
    RB_ObjectBase* objC = NULL;
    RB_ObjectBase* copyContainer = NULL;

    RB_ObjectIterator* iter = object->createIterator();
    if (!iter) return copyObject;

    for (iter->first(); !iter->isDone(); iter->next()) {
        objC = iter->currentObject();
        if (objC) {
            copyContainer = copyObject->getContainer(objC->getName());
            if (copyContainer) {
                copyContainer->setOriginal(objC);
                // Children in container
                RB_ObjectIterator* iterC = objC->createIterator();
                for (iterC->first(); !iterC->isDone(); iterC->next()) {
                    RB_ObjectBase* copyChild = resolveCopy(iterC->currentObject(),
                                                             resolveNext(level));
                    // copyChild->setParent(copyContainer); set parent done later
                    copyContainer->addObject(copyChild);
                }
                delete iterC;
            }
        }
    }

    delete iter;
    return copyObject;
}

/**
 * @return true if new ID's need to be created for the copy action
 */
bool RB_ObjectFactory::createNewId() {
    return mCreateNewId;
}

/**
 * @param createId true if creation of new id is required when copied
 */
void RB_ObjectFactory::setCreateNewId(bool createId) {
    mCreateNewId = createId;
}

/**
 * Get part or whole model structure based on the object name
 * @param objName object name as root of the requested model structure (part)
 */
RB_ObjectBase* RB_ObjectFactory::getModelStructure(const RB_String& objName) {
    if (!isFactoryInitiated() || !isObject(objName) || !mRoot) return NULL;

    if (mRoot && mRoot->getName().endsWith("Project", Qt::CaseInsensitive)
                && objName.endsWith("Project", Qt::CaseInsensitive)) {
        return mRoot;
    } else {
        return mRoot->getChildObject(objName);
    }

    return NULL;
}

/**
 * Create a model structure with a root object based on an object type (name)
 * with all containers/list and per container one object. Structure can be the
 * whole model or a part.
 * Example of use: the whole model structure is used to generate the SQL
 * statements for creating the necessary tables in a database. The model
 * structure can also be used to assess the integrity of data models such
 * as XML files.
 * TODO: delete possibility to create part of model, this function only to be used to make full model
 * @param objName name of the object to be root of the model structure (-part)
 */
RB_ObjectBase* RB_ObjectFactory::createModelStructure(const RB_String& objName) {
    RB_DEBUG->print(QString("RB_ObjectFactory::createModelStructure() %1")
                    .arg(objName));
    if (isFactoryInitiated() && !isObject(objName)) return NULL;
    RB_ObjectBase* obj = NULL;
    RB_ObjectContainer* tmpRoot = NULL;

    if (objName.endsWith("Project")) {
        // Root object of a model for example LDT_Project
        tmpRoot = dynamic_cast<RB_ObjectContainer*>(
                newObject(RB_Uuid::createUuid().toString(), NULL, "", true));
        if (!tmpRoot) return NULL;
        buildModelStructure(tmpRoot);
        if (!isFactoryInitiated()) {
            mRoot = tmpRoot;
            mFactoryIsInitiated = true; // object list is set
            tmpRoot = NULL;             // do not return the root of the object list
//            mObjectNameList.push_back(mRoot->getName());
        }
    } else if (objName.endsWith("List")) {
        // Container object, id, parent, name, objectFactory
        tmpRoot = new RB_ObjectContainer("", NULL, objName, this);
        // Child object, id, parent, type (type is only for equipment)
        obj = newObject(RB_Uuid::createUuid().toString(), tmpRoot, "", true);
        buildModelStructure(obj);
    } else {
        tmpRoot = dynamic_cast<RB_ObjectContainer*>(newSingleObject(objName));
        if (!tmpRoot) return NULL;
        buildModelStructure(tmpRoot);
    }
    RB_DEBUG->print("RB_ObjectFactory::createModelStructure() OK");
    return tmpRoot;
}

/**
 * Recursive function to create the children objects for all object lists
 * @param obj current object for which to create the children
 */
void RB_ObjectFactory::buildModelStructure(RB_ObjectBase* obj) {
    RB_ObjectContainer* objC = dynamic_cast<RB_ObjectContainer*>(obj);
    if (!objC) {
        RB_ObjectAtomic* objA = dynamic_cast<RB_ObjectAtomic*>(obj);
        if (!isFactoryInitiated() && objA) {
            mObjectNameList.push_back(objA->getName());
        }
        return; // end of branch
    } else if (!isFactoryInitiated()) {
        mObjectNameList.push_back(objC->getName());
    }

    RB_ObjectBase* tmpObj = NULL;
    RB_ObjectContainer* tmpObjC = NULL;
    RB_String objName = "";

    RB_ObjectIterator* iter = objC->createIterator();
    for (iter->first(); !iter->isDone(); iter->next()) {
        objName = iter->currentObject()->getName();

        // second statement is to prevent tree model endless looping
        if (isFactoryInitiated() || (!isFactoryInitiated() && !isObject(objName))) {
            if (objName.endsWith("List")) {
                tmpObjC = dynamic_cast<RB_ObjectContainer*>(iter->currentObject());
                tmpObj = newObject(RB_Uuid::createUuid().toString(), tmpObjC, "", true);
                if (!isFactoryInitiated()) {
//                    mObjectNameList.push_back(obj->getName());
                    mObjectNameList.push_back(tmpObjC->getName());
                }
                buildModelStructure(tmpObj);
            } else {
                RB_ObjectIterator* iterC = objC->createIterator();
                for (iterC->first(); !iterC->isDone(); iterC->next()) {
                    buildModelStructure(iterC->currentObject());
                }
            }
        }
    }
}

/**
 * @return true if model is completely initiated
 */
bool RB_ObjectFactory::isFactoryInitiated() {
    return mFactoryIsInitiated;
}

/**
 * @returns true if object name is an object of this factory
 */
bool RB_ObjectFactory::isObject(const RB_String& objectName) {
    if (objectName.endsWith("Project", Qt::CaseInsensitive)) {
        return true;
    }

    bool result = false;
    std::vector<RB_String>::iterator iter;
    iter = mObjectNameList.begin();
    while (iter != mObjectNameList.end() && !result) {
        if ((*iter).toLower() == objectName.toLower()) {
            result = true;
        }
        ++iter;
    }

    return result;
}

void RB_ObjectFactory::tableNameToObjectName(RB_String& name) {
    bool result = false;
    RB_String str = "";
    std::vector<RB_String>::iterator iter;
    iter = mObjectNameList.begin();
    while (iter != mObjectNameList.end() && !result) {
        str = *iter;
        if (str.toLower() == name.toLower()) {
            name = str;
        }
        ++iter;
    }
}

/**
 * TODO: use function in RB_ObjectBase
 */
RB2::ResolveLevel RB_ObjectFactory::resolveNext(RB2::ResolveLevel level) {
    switch (level) {
        case RB2::ResolveNone:
            return RB2::ResolveNone;
            break;
        case RB2::ResolveOne:
            return RB2::ResolveNone;
            break;
        case RB2::ResolveTwo:
            return RB2::ResolveOne;
            break;
        case RB2::ResolveThree:
            return RB2::ResolveTwo;
            break;
        case RB2::ResolveFour:
            return RB2::ResolveThree;
            break;
        case RB2::ResolveFive:
            return RB2::ResolveFour;
            break;
        case RB2::ResolveSix:
            return RB2::ResolveFive;
            break;
        case RB2::ResolveSeven:
            return RB2::ResolveSix;
            break;
        case RB2::ResolveEight:
            return RB2::ResolveSeven;
            break;
        case RB2::ResolveNine:
            return RB2::ResolveEight;
            break;
        case RB2::ResolveTen:
            return RB2::ResolveNine;
            break;
        case RB2::ResolveAll:
            return RB2::ResolveAll;
            break;
        default:
            return RB2::ResolveNone;
            break;
    }
    return RB2::ResolveNone;
}

/**
 * @returns name of object factory which is the same as the class name
 */
RB_String RB_ObjectFactory::getName() const {
    return mName;
}

/**
 * @returns perspective name of object factory which is the same
 * as the part in the class name before the underscore e.g.:
 * name is ACC_ObjectFactory, perspective is ACC, always capitals
 */
RB_String RB_ObjectFactory::getPerspective() const {
    if (!mName.isEmpty()) {
        RB_StringList strL = mName.split("_");
        return strL.at(0).toUpper();
    }
    return "";
}

/**
 * @returns version of object factory, here the application version is returned
 */
RB_String RB_ObjectFactory::getVersion() const {
    return RB_SYSTEM->getAppVersion();
}


/**
 * Is field or object name a system setting,
 * to be implemented by factory instance
 * @param name field or object name
 * @returns false
 */
bool RB_ObjectFactory::isSystemSetting(const RB_String& /*name*/) {
    return false;
}

/**
 * Is field or object name a project setting
 * to be implemented by factory instance
 * @param name field or object name
 * @returns false
 */
bool RB_ObjectFactory::isProjectSetting(const RB_String& /*name*/) {
    return false;
}

/**
 * Set name of this object factory which is the same as the class name
 * @param factorName name of this factory
 */
void RB_ObjectFactory::setName(const RB_String& factoryName) {
    mName = factoryName;
}

