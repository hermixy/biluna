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
    mStoredId = "";

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
 * Get stored/saved id, for example directly after previous selection
 * @return stored id
 */
RB_String RB_ModelFactory::getStoredId() const {
    return mStoredId;
}

/**
 * Set stored/saved id, for example directly after previous selection
 * @return stored id
 */
void RB_ModelFactory::setStoredId(const QString& id) {
    mStoredId = id;
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
            RB_DEBUG->info(QString("RB_ModelFactory::removeModel() %1")
                           .arg(model->getModelType()));
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

RB_ObjectFactory* RB_ModelFactory::getObjectFactory(const RB_String& objName) {
    if (objName.isEmpty()) {
        return mObjectFactory;
    }

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

/**
 * Slot to receive central the signal that a model has been updated
 * with submitAll(). Emits the signal modelUpdated(tableName).
 * @param tableName table name of the model that have been updated
 */
void RB_ModelFactory::slotModelUpdated(const QString& tableName) {
    emit modelUpdated(tableName);
}

bool RB_ModelFactory::isValidId(const QString& id) {
    if (id.isEmpty() || id == "0" || id.size() < 38) {
        RB_DEBUG->info("Not a valid ID = " + id + " INFO");
        return false;
    }

    return true;
}
