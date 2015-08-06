/*****************************************************************
 * $Id: rb_modelfactory.cpp 2222 2015-02-26 19:38:49Z rutger $
 * Created: Nov 12, 2009 9:42 AM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_modelfactory.h"

#include "db_objectfactory.h"
#include "db_sqlcommonfunctions.h"
#include "rb_database.h"
#include "rb_debug.h"

RB_ObjectContainer* RB_ModelFactory::mPermissionList = NULL;
int RB_ModelFactory::mUserCount = 0;


/**
 * Constructor
 */
RB_ModelFactory::RB_ModelFactory(RB_MainWindow* mw) {
    RB_DEBUG->print("RB_ModelFactory::RB_ModelFactory()");
    // always default database connection
    mDatabaseConnection = RB_DATABASE->database().connectionName();
    mMainWindow = mw;
    mObjectFactory = NULL;
    mRoot = NULL; // not connected to root of DB_ModelFactory
    mModelIsModified = false;

    // mPermissionList in DB_ModelFactory
}

/**
 * Destructor
 */
RB_ModelFactory::~RB_ModelFactory() {
    // Delete remaining models
    // key: first, value: second
    std::map<int, RB_MmProxy*>::iterator iter;
    iter = mModelList.begin();
    while (iter != mModelList.end()) {
        if ((*iter).second != NULL) {
            delete (*iter).second;
            (*iter).second = NULL;
        }
        ++iter;
    }

    // empty list (vector), this is usually not overdone because otherwise
    // map contains invalid pointers
    mModelList.clear();

    // delete root object which is not connected to root of DB_ModelFactory
    delete mRoot;
    // delete object factory
    delete mObjectFactory;

    RB_DEBUG->print("RB_ModelFactory::~RB_ModelFactory() OK");
}

/**
 * @returns root object of this model factory
 */
RB_ObjectBase* RB_ModelFactory::getRoot() {
    return mRoot;
}

/**
 * Set or recreate root object. This function can only be called after
 * the object factory has (once) been set.
 */
void RB_ModelFactory::setRoot(RB_ObjectBase* root) {
    if (mRoot && mObjectFactory) {
        delete mRoot;
    }

    if (root) {
        mRoot = root;
    } else if (mObjectFactory) {
        mRoot = mObjectFactory->newSingleObject("XXX_Project");
    } else {
        RB_DEBUG->print(RB_Debug::D_ERROR, "RB_ModelFactory::setRoot() "
                        "objectFactory not set ERROR");
    }
}

/**
 * Get root id
 * @return root id
 */
RB_String RB_ModelFactory::getRootId() const {

    if (!mRoot) {
        RB_DEBUG->print(RB_Debug::D_ERROR, "RB_ModelFactory::getRootId() "
                        "root not set ERROR");
        return "";
    }

    return mRoot->getId();
}

/**
 * Set root id, used when connected to database
 * @param id
 */
void RB_ModelFactory::setRootId(const RB_String& id) {
    if (!mRoot) {
        RB_DEBUG->print(RB_Debug::D_ERROR, "RB_ModelFactory::setRootId() "
                        "root not set ERROR");
    }

    mRoot->setId(id);
}

/**
 * Remove and delete model from factory, called by model themselves
 * @param type model type
 * @return true if success
 */
bool RB_ModelFactory::removeModel(int type) {
    // RB_DEBUG->print("RB_ModelFactory::removeModel()");

    bool success = false;
    RB_MmProxy* model = NULL;
    std::map<int, RB_MmProxy*>::iterator iter;
    iter = mModelList.find(type);

    if (iter != mModelList.end()) {
        // model 1- exists or 2- has been removed/deleted and set to NULL
        model = (*iter).second;

        if (model) {
            // do not delete here, this function is called
            // by the destructor of the model
            (*iter).second = NULL;
            success = true;
            RB_DEBUG->print("RB_ModelFactory::removeModel() %i",
                            model->getModelType());
        }
    }

    return success;
}

/**
 * Get database (actual connection) object
 */
QSqlDatabase RB_ModelFactory::getDatabase() {
    return RB_DATABASE->database(mDatabaseConnection);
}

/**
 * Set database
 * @param dbConnection actual connection string
 */
void RB_ModelFactory::setDatabase(const QSqlDatabase &db) {
    mDatabaseConnection = db.connectionName();
}

/**
 * Set object factory and setRoot()
 * @param f object factory
 */
void RB_ModelFactory::setObjectFactory(RB_ObjectFactory* f) {
    mObjectFactory = f;
    setRoot();
}

RB_ObjectFactory* RB_ModelFactory::getObjectFactory(const RB_String& /*objName*/) {
    return NULL;
}

void RB_ModelFactory::submit() {
    if (!getDatabase().isOpen()) return;

    RB_MmProxy* model = NULL;
    std::map<int, RB_MmProxy*>::iterator iter;
    iter = mModelList.begin();

    if (iter != mModelList.end()) {
        model = (*iter).second;

        if (model) {
            model->submit();
        }

        ++iter;
    }
}

void RB_ModelFactory::submitAll() {
    if (!getDatabase().isOpen()) return;

    RB_MmProxy* model = NULL;
    std::map<int, RB_MmProxy*>::iterator iter;
    iter = mModelList.begin();

    if (iter != mModelList.end()) {
        model = (*iter).second;

        if (model) {
            model->submitAll();
        }

        ++iter;
    }
}

void RB_ModelFactory::revert() {
    if (!getDatabase().isOpen()) return;

    RB_MmProxy* model = NULL;
    std::map<int, RB_MmProxy*>::iterator iter;
    iter = mModelList.begin();

    if (iter != mModelList.end()) {
        model = (*iter).second;

        if (model) {
            model->revert();
        }

        ++iter;
    }
}

void RB_ModelFactory::revertAll() {
    if (!getDatabase().isOpen()) return;

    RB_MmProxy* model = NULL;
    std::map<int, RB_MmProxy*>::iterator iter;
    iter = mModelList.begin();

    if (iter != mModelList.end()) {
        model = (*iter).second;

        if (model) {
            model->revertAll();
        }

        ++iter;
    }
}

/**
 * @returns true if one of the models is modified
 */
bool RB_ModelFactory::isModelModified() {
    bool modelIsModified = false;

    RB_MmProxy* model = NULL;
    std::map<int, RB_MmProxy*>::iterator iter;
    iter = mModelList.begin();

    while (iter != mModelList.end() && !modelIsModified) {
        model = (*iter).second;
        if (model) {
            modelIsModified = model->isModelModified();
        }
        ++iter;
    }

    return modelIsModified;
}

/**
 * Set all models modified to false, parameter is not used
 * @param modified
 */
void RB_ModelFactory::setModelIsModified(bool /* modified */) {
    RB_MmProxy* model = NULL;
    std::map<int, RB_MmProxy*>::iterator iter;
    iter = mModelList.begin();

    if (iter != mModelList.end()) {
        model = (*iter).second;
        if (model) {
            model->setModelIsModified(false);
        }
        ++iter;
    }
}

/**
 * Get new table model
 * @param db database
 * @param objF object factory
 * @param type model type
 * @param listName listname which is the objectcontainer name = tablename+List
 * @param shared true if this is a shared model and will be registered
 * with this factory. A parent model is always a shared/unique model.
 * @return model
 */
RB_MmProxy* RB_ModelFactory::getTableModel(const QSqlDatabase &db,
        RB_ObjectFactory* objF,
        int type,
        const RB_String& listName,
        bool shared) {
    // Create proxy model
    RB_MmProxy* model = new RB_MmProxy(NULL);
    // Create source model
    model->setSourceModel(new RB_MmSource(NULL, db, 0));
    // Set object factory
    model->setObjectFactory(objF);
    // Set model factory, which is called in destructor of model
    model->setModelFactory(this);
    // Set model type, which is used to manage the models
    model->setModelType(type);
    // Add to the unique list and for deletion by factory
    if (shared) mModelList[type] = model;
    // Extra flag for deletion check by factory
    model->setShared(shared);
    // Set model list and parent manager
    model->setTableModel(listName, getParentModel(type));
    return model;
    // after this set root to create root object with id, creates filter: tableName.parent='id'
    // optional setFilter() if a complex filter is to be use instead of based on parent
    // and select()
}

/**
 * Get new tree model
 * @param db database
 * @param objF object factory
 * @param type model type
 * @param listName listname which is the objectcontainer name = tablename+List
 * @param shared true if this is a shared model and will be registered
 * with this factory. A parent model is always a shared model.
 * @return model
 */
RB_MmProxy* RB_ModelFactory::getTreeModel(const QSqlDatabase &db,
        RB_ObjectFactory* objF,
        int type,
        const RB_String& listName,
        bool shared) {
    // Create proxy model, NOTE: for tree model do not pass the database
    RB_MmProxy* model = new RB_MmProxy(NULL);
    // Create source model, NOTE: no database
    model->setSourceModel(new RB_MmSource(NULL, QSqlDatabase(), RB2::HIDDENCOLUMNS));
    // Set object factory
    model->setObjectFactory(objF);
    // Set model factory, which is called in destructor of model
    model->setModelFactory(this);
    // Set model type, which is used to manage the models
    model->setModelType(type);
    // Add to the unique list and for deletion by factory
    if (shared) mModelList[type] = model;
    // Extra flag for deletion check by factory
    model->setShared(shared);
    // For tree model pass database here, NOTE: include database
    model->setTreeModel(listName, getParentModel(type), db);
    return model;
    // after this set root to create root object with id, creates filter: tableName.parent='id'
    // optional setFilter() if a complex filter is to be use instead of based on parent
}

/**
 * Get new in-memory model
 * @param root root object
 * @param objF object factory
 * @param type model type
 * @param listName listname which is the objectcontainer name = tablename+List
 * @param shared true if this is a shared model and will be registered
 * with this factory. A parent model is always a shared model.
 * @return model
 */
RB_MmProxy* RB_ModelFactory::getInMemoryTableModel(
        RB_ObjectBase* root,
        RB_ObjectFactory* objF,
        int type,
        const RB_String& listName,
        bool shared) {
    // Create proxy model, NOTE: for int-memory model do not pass the database
    RB_MmProxy* model = new RB_MmProxy(NULL);
    // Create source model, NOTE: no database
    model->setSourceModel(new RB_MmSource(NULL, QSqlDatabase(), 0));
    // Set object factory
    model->setObjectFactory(objF);
    // Set model factory, which is called in destructor of model
    model->setModelFactory(this);
    // Set model type, which is used to manage the models
    model->setModelType(type);
    // Add to the unique list and for deletion by factory
    if (shared) mModelList[type] = model;
    // Extra flag for deletion check by factory
    model->setShared(shared);
    // Set this model as in-memory model
    model->setInMemoryModel(true);
    // For tree model pass database here, NOTE: include database
    model->setTableModel(listName, getParentModel(type));
    // TODO: HACK: delete root set by setTableModel()
    RB_ObjectBase* obj = model->getRoot();
    delete obj;
    // Set root object
    model->setRoot(root);
    return model;
    // optional setFilter() if a complex filter is to be use instead of based on parent
}

/**
 * @returns hidden column count (hidden columns are id, parent, name, ...)
 */
int RB_ModelFactory::hiddenColumnCount() {
    return RB_MmSource::HiddenColumns;
}

/**
 * Emit state signals of this modelfactory,
 * such as is database open
 */
void RB_ModelFactory::emitState() {
    if (RB_DATABASE->database(mDatabaseConnection).isOpen()) {
        emit databaseIsSet(RB2::ValidTrue);
        emit databaseIsNotSet(RB2::ValidFalse);

        if (getRootId() != "") {
            emit rootIsSet(RB2::ValidTrue);
        }
    } else {
        emit databaseIsSet(RB2::ValidFalse);
        emit databaseIsNotSet(RB2::ValidTrue);
        emit rootIsSet(RB2::ValidFalse);
    }
}

/**
 * Add default rows to database.
 * To be overriden by implementation
 * @returns true on success (true is default)
 */
bool RB_ModelFactory::addDefaultRows() {
    return true;
}

void RB_ModelFactory::setUserPermission() {
    // set user count
    DB_SqlCommonFunctions f;
    mUserCount = f.getUserCount();

    // set user list
    f.getPermissionList(mPermissionList);

    // create per actual project with the user permission
    RB_ObjectContainer* newPermissionList = new RB_ObjectContainer("", NULL,
                                             "DB_SystemUserPermissionList",
                                             DB_OBJECTFACTORY);
    RB_String projectId = "";
    RB_ObjectIterator* tmpIter = newPermissionList->createIterator();
    RB_ObjectIterator* iter = mPermissionList->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();
        projectId = obj->getValue("persproject_idx").toString();
        RB_ObjectBase* projectObj = newPermissionList->getObject(
                    "persproject_idx", projectId);

        if (projectObj) {
            // project already created and permission needs to be updated
            RB_ModelFactory::handlePermissionSetting(projectObj, obj);
        } else {
            // project not yet part of permission list
            RB_ObjectBase* newProjectObj = obj->clone(RB2::ResolveNone);
            newProjectObj->setParent(newPermissionList);
            newPermissionList->addObject(newProjectObj);
        }

    }

    delete tmpIter;
    delete iter;

    delete mPermissionList;
    mPermissionList = newPermissionList;

    // consolidate project
}

void RB_ModelFactory::handlePermissionSetting(RB_ObjectBase* projectObj,
                                              RB_ObjectBase* permObj) {
    //    userstart
    //    userend
    //    groupstatus_id live locked hidden
    //    grouppermission_id none r ru crud crudx
    //    groupstart
    //    groupend
    //    persproject_idx {Uuid}tablename - project number
    //    persprojectstatus_id live locked hidden test
    //    persprojectstart
    //    persprojectend

        // continue here ...
    // if project userstart < permission userstart : permission userstart
    if (projectObj->getValue("userstart").toString()
            < permObj->getValue("userstart").toString()) {
        projectObj->setValue("userstart", permObj->getValue("userstart"));
    }

    // if project userend > permission userend : permission userend
    if (projectObj->getValue("userend").toString()
            > permObj->getValue("userend").toString()) {
        projectObj->setValue("userend", permObj->getValue("userend"));
    }

    // TODO: Remove? Makes the permission flexible but also complicated?
    // largest of groupstatus_id live=open locked=r only hidden=none
    if (projectObj->getValue("groupstatus_id").toInt()
            < permObj->getValue("groupstatus_id").toInt()) {
        projectObj->setValue("groupstatus_id", permObj->getValue("groupstatus_id"));
    }

    // largest of grouppermission_id r ru crud crudx
    if (projectObj->getValue("grouppermission_id").toInt()
            < permObj->getValue("grouppermission_id").toInt()) {
        projectObj->setValue("grouppermission_id", permObj->getValue("grouppermission_id"));
    }

    // if project groupstart < permission groupstart : project groupstart
    if (projectObj->getValue("groupstart").toString()
            < permObj->getValue("groupstart").toString()) {
        projectObj->setValue("groupstart", permObj->getValue("groupstart"));
    }

    // if project groupend > permission groupend : project groupend
    if (projectObj->getValue("groupend").toString()
            > permObj->getValue("groupend").toString()) {
        projectObj->setValue("groupend", permObj->getValue("groupend"));
    }

    // persproject_idx do nothing

    // smallest of persprojectstatus_id (should all be the same)
    if (projectObj->getValue("groupstatus_id").toInt()
            > permObj->getValue("groupstatus_id").toInt()) {
        projectObj->setValue("groupstatus_id", permObj->getValue("groupstatus_id"));
    }

    // if project persprojectstart < permission persprojectstart : permission persprojectstart (should all be the same)
    if (projectObj->getValue("persprojectstart").toString()
            < permObj->getValue("persprojectstart").toString()) {
        projectObj->setValue("persprojectstart", permObj->getValue("persprojectstart"));
    }

    // if project persprojectend > permission persprojectend : permission persprojectend (should all be the same)
    if (projectObj->getValue("persprojectend").toString()
            > permObj->getValue("persprojectend").toString()) {
        projectObj->setValue("persprojectend", permObj->getValue("persprojectend"));
    }

    RB_DEBUG->print(projectObj->toString());
}

int RB_ModelFactory::getPerspectivePermission(const RB_String& perspectiveCode) {
    RB2::PermissionType perm = RB2::PermissionNone;
    // at least an administrator and a user should be part of the permission list
    if (mUserCount < 2) {
        return perm;
    }

    RB_ObjectIterator* iter = mPermissionList->createIterator();

    for (iter->first(); !iter->isDone() && perm == RB2::PermissionNone;
         iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();
        RB_String str = obj->getDValue("persproject_idx").toString();
        // PENG_Project - NEW001
        RB_StringList strList = str.split("_");
        RB_String perspCode = strList.at(0);

        if (perspCode.compare(perspectiveCode, Qt::CaseInsensitive) == 0) {
            perm = RB2::PermissionDefault;
        }
    }

    delete iter;
    return perm;
}

int RB_ModelFactory::getCrudxPermission(const RB_String& modelName,
                                        const RB_String& projectId) {
    RB_StringList modelList = modelName.split("_", QString::SkipEmptyParts);

    if (modelList.size() < 2) {
        RB_DEBUG->error("RB_ModelFactory::getCrudxPermission() method name ERROR");
        return (int)RB2::PermissionNone;
    }

    // Check perspective
    int permissionGranted = 0; // userHasPerspectivePermission(modelList.at(0));

    // TODO: class level, method level
    // ...

    return permissionGranted;
}

/**
 * Slot to receive central the signal that a model has been updated
 * with submitAll(). Emits the signal modelUpdated(tableName).
 * @param tableName table name of the model that have been updated
 */
void RB_ModelFactory::slotModelUpdated(const QString& tableName) {
    emit modelUpdated(tableName);
}
