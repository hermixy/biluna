/*****************************************************************
 * $Id: db_modelfactory.cpp 2248 2015-06-21 09:13:00Z rutger $
 * Created: Dec 6, 2009 11:29 AM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_modelfactory.h"

#include "db_objectfactory.h"
#include "db_sqlcommonfunctions.h"
#include "rb_database.h"

DB_ModelFactory* DB_ModelFactory::mActiveFactory = NULL;


/**
 * Constructor
 */
DB_ModelFactory::DB_ModelFactory(RB_MainWindow* mw) : RB_ModelFactory(mw) {
    RB_DEBUG->print("DB_ModelFactory::DB_ModelFactory()");
    this->setObjectFactory(DB_OBJECTFACTORY);
}

/**
 * Destructor
 */
DB_ModelFactory::~DB_ModelFactory() {
    closeAllFactories();
    disconnectFromDatabase();

    mActiveFactory = NULL;
    RB_DEBUG->print("DB_ModelFactory::~DB_ModelFactory() OK");
}

/**
 * Create instance of factory
 */
DB_ModelFactory* DB_ModelFactory::getInstance(RB_MainWindow* mw) {
    if (!mActiveFactory) {
        mActiveFactory = new DB_ModelFactory(mw);
    }
    return mActiveFactory;
}

/**
 * Set this object root id and of all models factories,
 * currently at first connection and when disconnected from database
 * refer db_actionsystemdatabasedisconnect.cpp
 * @param id
 */
void DB_ModelFactory::setAllRootId(const RB_String& id) {
    setRootId(id);

    std::vector<RB_ModelFactory*>::iterator iter;
    iter = mFactoryList.begin();
    while (iter != mFactoryList.end()) {
        if(*iter) {
            RB_ModelFactory* mf = dynamic_cast<RB_ModelFactory*>(*iter);
            mf->setRootId(id);
        }
        ++iter;
    }
}

/**
 * @param type model type
 * @param shared true if model is to be created as a shared/single model
 * @return modelManager, also looks in other registered modelFactories
 */
RB_MmProxy* DB_ModelFactory::getModel(int type, bool shared) {
    RB_MmProxy* model = NULL;

    if (shared) {
        std::map<int, RB_MmProxy*>::iterator iter;
        iter = mModelList.find(type);
        if (iter != mModelList.end()) {
            // shared model 1- exists or 2- has been removed/deleted and set to NULL
            model = (*iter).second;
        }
        // Model already created
        if (model) return model;

        // TODO: Look in other modelFactories, future or not?
        //        std::vector<RB_ModelFactory*>::iterator iterF;
        //        iterF = mFactoryList.begin();
        //        while (iterF != mFactoryList.end()) {
        //            if(*iterF) {
        //                RB_ModelFactory* f
        //                        = dynamic_cast<RB_ModelFactory*>(*iterF);
        //                model = f->getModel(type, shared);
        //                if (model) return model;
        //            }
        //            ++iterF;
        //        }
    }

    QSqlDatabase db = RB_DATABASE->database(mDatabaseConnection);

    switch (type) {
    case ModelActivity:
        model = getTableModel(db, mObjectFactory, type, "DB_ActivityList", shared);
        model->setSourceSortOrder(RB2::SortOrderDescending, "mend");
        model->setDisplayRole(model->fieldIndex("mvalue"), RB2::AlignFinancialAmount);
        break;
    case ModelPermissionGroup:
        model = getTableModel(db, mObjectFactory, type, "DB_PermissionGroupList", shared);
        break;
    case ModelPermissionPerspectiveProject: {
        // Custom in-memory model, mObjectFactory not used, root set later in dialog
        RB_ObjectContainer* root = new RB_ObjectContainer("", NULL, "DB_PermissionPerspectiveProjectRoot");
        RB_ObjectContainer* list = new RB_ObjectContainer("", root, "DB_PermissionPerspectiveProjectList",
                                                          DB_OBJECTFACTORY);
        root->addObject(list);

        DB_SqlCommonFunctions oper;
        oper.setPerspectiveProjects(list);
        model = getInMemoryTableModel(root, mObjectFactory,
                                      type, "DB_PermissionPerspectiveProjectList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "name");
        break;
    }
    case ModelPermissionProject:
        model = getTableModel(db, mObjectFactory, type, "DB_PermissionProjectList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "persproject_idx");
        break;
    case ModelProject:
        // DB_ProjectList actually does not exist but required to create model
        model = getTableModel(db, mObjectFactory, type, "DB_ProjectList", shared);
        break;
    case ModelProjectEdit:
        model = getTableModel(db, mObjectFactory, type, "DB_ProjectList", shared);
        break;
    case ModelSelectMapping:
        model = getTableModel(db, mObjectFactory, type, "DB_MapList", shared);
        // model->setRoot(mRoot->getId());
        break;
    case ModelSimpleReport:
        model = getTableModel(db, mObjectFactory, type, "DB_SimpleReportList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "title");
        break;
    case ModelSystemGroup:
        model = getTableModel(db, mObjectFactory, type, "DB_SystemGroupList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "code");
        break;
    case ModelSystemUser:
        model = getTableModel(db, mObjectFactory, type, "DB_SystemUserList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "lastname");
        break;
    case ModelSystemUserGroup:
        model = getTableModel(db, mObjectFactory, type, "DB_SystemUserGroupList", shared);
        break;
    case ModelVersion:
        model = getTableModel(db, mObjectFactory, type, "DB_VersionList", shared);
        model->setSourceSortOrder(RB2::SortOrderDescending, "perspectivecode");
        break;
    default:
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "DB_ModelFactory::getModel()"
                        "non-existing model ERROR");
        break;
    }

    return model;
}

/**
 * Get parent model
 * @param type child model type
 * @return parent model or NULL if not exists
 */
RB_MmProxy* DB_ModelFactory::getParentModel(int type) {
    RB_MmProxy* model = NULL;
    std::map<int, RB_MmProxy*>::iterator iter;

    switch (type) {
    case ModelActivity:
        iter = mModelList.find(ModelNone);
        break;
    case ModelPermissionGroup:
        iter = mModelList.find(ModelPermissionProject);
        break;
    case ModelPermissionPerspectiveProject:
        iter = mModelList.find(ModelNone);
        break;
    case ModelPermissionProject:
        iter = mModelList.find(ModelNone);
        break;
    case ModelProject:
        iter = mModelList.find(ModelNone);
        break;
    case ModelProjectEdit:
        iter = mModelList.find(ModelNone);
        break;
    case ModelSelectMapping:
        iter = mModelList.find(ModelNone);
        break;
    case ModelSimpleReport:
        iter = mModelList.find(ModelNone);
        break;
    case ModelSystemGroup:
        iter = mModelList.find(ModelNone);
        break;
    case ModelSystemUser:
        iter = mModelList.find(ModelNone);
        break;
    case ModelSystemUserGroup:
        iter = mModelList.find(ModelSystemUser);
        break;
    case ModelVersion:
        iter = mModelList.find(ModelNone);
        break;
    default:
        iter = mModelList.find(ModelNone);
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "DB_ModelFactory::getParentModel()"
                        "non-existing (parent) model ERROR");
        break;
    }
    if (iter != mModelList.end()) {
        model = (*iter).second;
    }
    return model;
}

/**
 * @return factory list
 */
std::vector<RB_ModelFactory*> DB_ModelFactory::getFactoryList() {
    return mFactoryList;
}

/**
 * @param objName object name
 * @returns factory if found otherwise NULL
 */
RB_ObjectFactory* DB_ModelFactory::getObjectFactory(const RB_String& objName) {
    if (objName.isEmpty()) {
        return mObjectFactory;
    }

    RB_ObjectFactory* factory = NULL;
    RB_StringList strL = objName.split("_");
    RB_String perspective = strL.at(0);

    // Check if relevant perspective is loaded
    std::vector<RB_ObjectFactory*> fl = DB_OBJECTFACTORY->getFactoryList();
    std::vector<RB_ObjectFactory*>::iterator iter;
    iter = fl.begin();

    while (iter != fl.end()) {
        RB_ObjectFactory* f = *iter;
        strL = f->getName().split("_");

        if (perspective.toLower() == strL.at(0).toLower()) {
            factory = f;
        }
        ++iter;
    }

    return factory;
}

/**
 * Set database
 * @param dbConnection actual connection name
 */
void DB_ModelFactory::setDatabase(const QSqlDatabase& db) {
    RB_ModelFactory::setDatabase(db);

    std::vector<RB_ModelFactory*>::iterator iter;
    iter = mFactoryList.begin();
    while (iter != mFactoryList.end()) {
        if(*iter) {
            RB_ModelFactory* f
                    = dynamic_cast<RB_ModelFactory*>(*iter);
            f->setDatabase(db);
        } else {
            RB_DEBUG->print(RB_Debug::D_ERROR,
                            "DB_ModelFactory::setDatabase() "
                            "*iter == NULL ERROR");
        }
        ++iter;
    }
}

/**
 * Register factory
 * @param f Objectfactory
 */
void DB_ModelFactory::registerFactory(RB_ModelFactory* f) {
    bool exists = false;

    std::vector<RB_ModelFactory*>::iterator iter;
    iter = mFactoryList.begin();
    while (iter != mFactoryList.end() && !exists) {
        if(f == *iter) {
            exists = true;
            RB_DEBUG->print(RB_Debug::D_ERROR,
                            "DB_ModelFactory::registerFactory() "
                            "objectfactory already registered ERROR");
        }
        ++iter;
    }

    if (!exists) {
        mFactoryList.push_back(f);
    }
}

/**
 * Unregister factory
 * @param f Objectfactory
 */
void DB_ModelFactory::unregisterFactory(RB_ModelFactory* f) {
    bool exists = false;

    std::vector<RB_ModelFactory*>::iterator iter;
    iter = mFactoryList.begin();
    while (!exists && iter != mFactoryList.end()) {
        if (f == *iter) {
            mFactoryList.erase(iter);
            exists = true;
        } else {
            ++iter;
        }
    }

    if (!exists) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "DB_ModelFactory::unregisterFactory() "
                        "objectfactory was not registered ERROR");
    }
}

/**
 * Close and delete all factories except this (DB) factory
 */
void DB_ModelFactory::closeAllFactories() {
    /* NOTE: Below does not work because factory deletion changes the std::vector
    std::vector<RB_ModelFactory*>::iterator iter;
    iter = mFactoryList.begin();
    while (iter != mFactoryList.end()) {
        if(*iter) {
            RB_ModelFactory* f
                    = dynamic_cast<RB_ModelFactory*>(*iter);
            f->setDatabase(QSqlDatabase()); // invalid database
            delete f;
        }
        ++iter;
    }

    mFactoryList.clear(); // overdone
    */

    while (!mFactoryList.empty()) {
        RB_ModelFactory* f
                = dynamic_cast<RB_ModelFactory*>(mFactoryList.back());
        delete f; // unregisters factory and removes, therefor no pop_back()
    }
}

/**
 * Disconnect all connnections from database
 */
void DB_ModelFactory::disconnectFromDatabase() {
    RB_DEBUG->print("DB_ModelFactory::disconnectFromDatabase()");
    // Set invalid database first to all models
    setDatabase(QSqlDatabase());

    // Close all database connections
    QStringList strL = RB_DATABASE->connectionNames();
    for (int i = 0; i < strL.size(); ++i) {
        RB_DEBUG->print(strL.at(i));
        RB_DATABASE->database(strL.at(i)).close();
        RB_DATABASE->removeDatabase(strL.at(i));
    }

    delete RB_DATABASE;
    RB_DEBUG->print("DB_ModelFactory::disconnectFromDatabase() OK");
}

/**
 * Create tables in database if not exist for perspective
 * @param perspective
 * @return error message, is empty if no errors
 */
RB_String DB_ModelFactory::createTables(const RB_String& perspective) {
    mMessage = "";

    if (perspective == "None" || perspective == "") {
        return tr("No perspective selected");
    } else {
        mTableList = DB_MODELFACTORY->getDatabase().tables(QSql::AllTables);

        RB_ObjectBase* modelRoot = getObjectFactoryRoot(perspective);
        bool success = false;

        RB_String sqlStr;
        beginTransaction(sqlStr);
        success = createTables(sqlStr, modelRoot);
        success = success ? createDefaultRows(sqlStr, modelRoot) : false;
        commitTransaction(sqlStr);
        success = success ? executeQuery(sqlStr) : false;

        if (success) {
            return tr("Database tables created.");
        }
    }

    mMessage += "DB_ModelFactory::createTables() ERROR";
    return mMessage;
}

/**
 * Create the SQL string for the perspective, does not execute the SQL
 * @param sqlStr SQL string to be set
 * @param perspective perspective
 */
void DB_ModelFactory::createSql(RB_String& sqlStr, const RB_String& perspective) {
    RB_DEBUG->print("DB_ModelFactory::createSql(RB_String&, RB_String&)");

    if (perspective == "None" || perspective == "") {
        sqlStr = tr("No perspective selected");
    } else {
        RB_ObjectBase* modelRoot = getObjectFactoryRoot(perspective);
        createSql(sqlStr, modelRoot);
    }
}

/**
 * Create the SQL string for the table to be updated, does not execute the SQL
 * @param sqlStr SQL string to be set
 * @param tableName table name
 */
void DB_ModelFactory::createUpdateTableSql(RB_String& sqlStr,
                                           const RB_String& tableName) {
    if (tableName.isEmpty()) {
        sqlStr = tr("Table name empty or no table selected.");
        return;
    }

    RB_ObjectFactory* factory = getObjectFactory(tableName);

    if (!factory) {
        sqlStr = tr("Relevant perspective is not loaded.");
        return;
    }

    // Create temporary table object
    RB_String objName = tableName;
    factory->tableNameToObjectName(objName);
    RB_ObjectBase* obj = factory->newSingleObject(objName);

    if (!obj) {
        sqlStr = tr("Object name does not exist.");
        return;
    }

    sqlStr.clear();

    beginTransaction(sqlStr);
    createSql(sqlStr, obj, true); // CREATE TEMPORARY TABLE `" + tmpTableName

    sqlStr += "INSERT INTO `" + tableName + "_tmp` SELECT\n";
    int count = obj->memberCount();
    for (int i = 0; i < count; ++i) {
        if (i < count - 1) {
            sqlStr += " `" + obj->getMember(i)->getName().toLower() + "`,\n";
        } else {
            sqlStr += " `" + obj->getMember(i)->getName().toLower() + "`\n";
        }
    }

    sqlStr += "FROM `" + tableName + "`;\n\n";

    // DROP TABLE will automatically drop the indices too
    sqlStr += "DROP TABLE `" + tableName + "`;\n\n";

    createSql(sqlStr, obj);
    sqlStr += "INSERT INTO `" + tableName + "` SELECT\n";

    for (int i = 0; i < count; ++i) {
        if (i < count - 1) {
            sqlStr += " `" + obj->getMember(i)->getName().toLower() + "`,\n";
        } else {
            sqlStr += " `" + obj->getMember(i)->getName().toLower() + "`\n";
        }
    }

    sqlStr += "FROM `" + tableName + "_tmp`;\n\n";
    // To make sure. will also be deleted when database closed
    sqlStr += "DROP TABLE `" + tableName + "_tmp`;\n\n";

    commitTransaction(sqlStr);

    delete obj;
    obj = NULL;
}

/**
 * Create SQL of objects in object factory, does not execute the SQL
 * @param sqlStr string to be set with SQL
 * @param obj root object
 */
void DB_ModelFactory::createSql(RB_String& sqlStr, RB_ObjectBase* obj, bool isTempTable) {
    RB_DEBUG->print("DB_ModelFactory::createSql(RB_String&, RB_ObjectBase*)");

    if (obj && obj->isContainer()) {
        RB_ObjectContainer* objC = (RB_ObjectContainer*)obj;

        if (objC->getName().endsWith("List")) {
            for (int i = 0; i < objC->objectCount(); ++i) {
                createSql(sqlStr, objC->getObject(i), isTempTable);
            }
        } else {
            createTableSql(sqlStr, objC, isTempTable);

            for (int i = 0; i < objC->objectCount(); ++i) {
                createSql(sqlStr, objC->getObject(i), isTempTable);
            }
        }
    } else if (obj) {
        createTableSql(sqlStr, obj, isTempTable);
    } else {
        sqlStr.append("DB_ModelFactory::createSql() object is NULL ERROR");
    }

}

/**
 * Check whether tables exist, if not update the database
 * with the new tables. Called from select project action.
 * @param perspective perspective
 * @returns true on success
 */
bool DB_ModelFactory::createMissingTables(const RB_String& perspective,
                                          int major, int medior, int minor,
                                          int build) {
    DB_SqlCommonFunctions f;
    RB_ObjectBase* root = getObjectFactoryRoot(perspective + "_ObjectFactory"); // TODO: change to enumerator

    if (!isTableExisting(root)) {
        // create all tables
        createTables(perspective + "_ObjectFactory"); // TODO: change to enumerator
        if (mMessage.isEmpty()) {
            f.insertPerspectiveVersion(perspective, major, medior, minor, build);
        } else {
            return false;
        }
    } else {
        if (f.isPerspectiveVersionExisting(perspective, major, medior, minor, build)) {
            // No change required if version already exists
            return true;
        }

        RB_String sqlStr;
        beginTransaction(sqlStr);
        createNonExistingTables(sqlStr, root);
        commitTransaction(sqlStr);
        bool success = executeQuery(sqlStr);

        if (success) {
            f.insertPerspectiveVersion(perspective, major, medior, minor, build);
        }

        return success;
    }

    return true;
}

/**
 * Check existing tables against model for differences.
 * Differences are listed in the result
 * @param perspective tables of perspective to be checkes
 * @param result result of the check
 * @return true if check OK
 */
bool DB_ModelFactory::checkExistingTables(const RB_String& perspective,
                                          RB_String& result) {
    QSqlDatabase db = getDatabase();
    if (!db.isOpen()) {
        result = "Database is not open.";
        return false;
    }

    result = "";

    RB_String factoryName = perspective; // .toUpper() + "_ObjectFactory";
    RB_ObjectBase* modelRoot = NULL;
    std::vector<RB_ObjectFactory*> fl = DB_OBJECTFACTORY->getFactoryList();
    std::vector<RB_ObjectFactory*>::iterator iter;
    iter = fl.begin();

    while (iter != fl.end()) {
        RB_ObjectFactory* f = *iter;

        if(factoryName == f->getName()) {
            modelRoot = f->getModelStructure("XXX_Project");
            return checkTables(modelRoot, result);
        }
        ++iter;
    }

    if (perspective == "None") {
        result = "No perspective selected";
        return false;
    } else if (!modelRoot) {
        RB_DEBUG->error("DB_ModelFactory::checkExistingTables() ERROR");
        result = "System ERROR";
        return false;
    }

    return result == "";
}

/**
 * @returns true when objName is a valid object from the loaded perspectives
 */
bool DB_ModelFactory::isObject(const RB_String& objName) {
    RB_ObjectFactory* factory = getObjectFactory(objName);

    if (!factory) {
        return false;
    }

    return factory->isObject(objName);
}

void DB_ModelFactory::emitState() {
    std::vector<RB_ModelFactory*>::iterator iter;
    iter = mFactoryList.begin();

    while (iter != mFactoryList.end()) {
        (*iter)->emitState();
        ++iter;
    }

    RB_ModelFactory::emitState();
}

/**
 * Create SQL for table creation for example:
    CREATE TABLE IF NOT EXISTS db_group
        (`id` VARCHAR(40) NOT NULL PRIMARY KEY,
        `parent` VARCHAR(40) NOT NULL,
        `name` VARCHAR(40) NOT NULL DEFAULT 'DB_Group',
        `status` INT DEFAULT '0',
        `created` VARCHAR(20) DEFAULT '0000-00-00T00:00:00',
        `changed` VARCHAR(20)  DEFAULT '0000-00-00T00:00:00',
        `muser` VARCHAR(20)  DEFAULT 'system',
        `groupname` VARCHAR(40) NULL DEFAULT 'group_name',
        `description` VARCHAR(255) NULL DEFAULT '',
        `remark` VARCHAR(125) NULL DEFAULT '')
 * @param sqlStr string to be set with SQL
 * @param obj object with all members/columns
 */
void DB_ModelFactory::createTableSql(RB_String& sqlStr, RB_ObjectBase* obj,
                                     bool isTempTable) {
    RB_DEBUG->print("DB_ModelFactory::createTableSql()");

    RB_String dtStr = QDateTime::currentDateTime().toString(Qt::ISODate);
    obj->setValue("created", dtStr);
    obj->setValue("changed", dtStr);

    RB_String sqlCreateIndex = "";
    RB_String memName = "";
    RB2::MemberType memType = RB2::MemberNone;

    if (!isTempTable) {
        sqlStr += "CREATE TABLE IF NOT EXISTS `";
        sqlStr += obj->getName().toLower() + "` (";
    } else {
        sqlStr += "CREATE TEMPORARY TABLE `";
        sqlStr += obj->getName().toLower() + "_tmp` (";
    }

    int mCount = obj->memberCount();

    for (int i = 0; i < mCount; ++i) {
        RB_ObjectMember* mem = obj->getMember(i);
        memName = mem->getName();
        memType = mem->getType();
        if (i == 0) {
            sqlStr += "\n `" + memName + "`";
        } else {
            sqlStr += ",\n `" + memName + "`";
        }

        if (memName == "id" || memName == "parent") {
            sqlStr += " VARCHAR(40) NOT NULL";
        } else if (memName == "name") {
            sqlStr += " VARCHAR(40) NOT NULL DEFAULT \'" + obj->getName() + "\'";
        } else if (memName == "status") {
            sqlStr += " INT NOT NULL DEFAULT 0";
        } else if (memName == "created" || memName == "changed") {
            sqlStr += " VARCHAR(20) NOT NULL DEFAULT '1900-01-01T00:00:00'";
        } else if (memName == "muser" ) {
            sqlStr += " VARCHAR(20) NOT NULL DEFAULT 'unknown'";
        } else {
            switch (memType) {
                case RB2::MemberByteArray:
                    sqlStr += " BLOB";
                    break;
                case RB2::MemberChar10:
                    sqlStr += " VARCHAR(10)";
                    if (!mem->getValue().toString().isEmpty()) {
                        sqlStr += " DEFAULT \'" + mem->getValue().toString() + "\'";
                    }
                    break;
                case RB2::MemberChar20:
                    sqlStr += " VARCHAR(20)";
                    if (!mem->getValue().toString().isEmpty()) {
                        sqlStr += " DEFAULT \'" + mem->getValue().toString() + "\'";
                    }
                    break;
                case RB2::MemberChar40:
                    sqlStr += " VARCHAR(40)";
                    if (!mem->getValue().toString().isEmpty()) {
                        sqlStr += " DEFAULT \'" + mem->getValue().toString() + "\'";
                    }
                    break;
                case RB2::MemberChar125:
                    sqlStr += " VARCHAR(125)";
                    if (!mem->getValue().toString().isEmpty()) {
                        sqlStr += " DEFAULT \'" + mem->getValue().toString() + "\'";
                    }
                    break;
                case RB2::MemberChar165:
                    sqlStr += " VARCHAR(165)";
                    if (!mem->getValue().toString().isEmpty()) {
                        sqlStr += " DEFAULT \'" + mem->getValue().toString() + "\'";
                    }
                    break;
                case RB2::MemberChar255:
                    sqlStr += " VARCHAR(255)";
                    if (!mem->getValue().toString().isEmpty()) {
                        sqlStr += " DEFAULT \'" + mem->getValue().toString() + "\'";
                    }
                    break;
                case RB2::MemberChar2500:
                    sqlStr += " VARCHAR(2500)";
                    if (!mem->getValue().toString().isEmpty()) {
                        sqlStr += " DEFAULT \'" + mem->getValue().toString() + "\'";
                    }
                    break;
                case RB2::MemberDateTime:
                    sqlStr += " VARCHAR(20)";
                    if (!mem->getValue().toString().isEmpty()) {
                        sqlStr += " DEFAULT \'" + mem->getValue().toString() + "\'";
                    }
                    break;
                case RB2::MemberDouble:
                    sqlStr += " DOUBLE";
                    if (!mem->getValue().toString().isEmpty()) {
                        sqlStr += " DEFAULT " + mem->getValue().toString();
                    }
                    break;
                case RB2::MemberInteger:
                    sqlStr += " INT";
                    if (!mem->getValue().toString().isEmpty()) {
                        sqlStr += " DEFAULT " + mem->getValue().toString();
                    }
                    break;
                case RB2::MemberString: // TINYTEXT 256, TEXT 64000, MEDIUMTEXT etc.
                    sqlStr += " TEXT";
                    // MySQL does not allow default value for blob or text
                    // if (!mem->getValue().toString().isEmpty()) {
                    //    sqlStr += " DEFAULT \'" + mem->getValue().toString() + "\'";
                    // }
                    break;
                case RB2::MemberUInteger:
                    sqlStr += " UNSIGNED INT";
                    if (!mem->getValue().toString().isEmpty()) {
                        sqlStr += " DEFAULT " + mem->getValue().toString();
                    }
                    break;
                default:
                    break;
            }
        }

        // CREATE INDEX acc_accountcontrol_controlnumber_index ON acc_accountcontrol (controlnumber)
        if (!isTempTable && (memName.endsWith("_id") || memName.endsWith("_idx")
                              || memName.endsWith("_ix") || memName == "parent")) {
            RB_String indexName = obj->getName().toLower() + "_" + memName + "_index";
            sqlCreateIndex += "CREATE INDEX ";
            sqlCreateIndex += indexName + " ON ";
            sqlCreateIndex += obj->getName().toLower() + "(" + memName + ");\n";
        }
    }

    if (DB_MODELFACTORY->getDatabase().driverName() == "QMYSQL") {
        sqlStr += ",\n PRIMARY KEY (`id`)\n) ENGINE=MyISAM DEFAULT CHARSET=utf8;\n";
    } else {
        sqlStr += ",\n PRIMARY KEY (`id`));\n";
    }

    sqlStr += sqlCreateIndex + "\n";
}

/**
 * Create tables in database, create SQL and execute
 * @param obj object(tree) for which the tables are created
 */
bool DB_ModelFactory::createTables(RB_String& sqlStr, RB_ObjectBase* obj) {
    RB_DEBUG->print("DB_ModelFactory::createTables()");

    if (obj && obj->isContainer()) {
        RB_ObjectContainer* objC = (RB_ObjectContainer*)obj;

        if (objC->getName().endsWith("List")) {
            for (int i = 0; i < objC->objectCount(); ++i) {
                createTables(sqlStr, objC->getObject(i));
            }
        } else {
            if (!mTableList.contains(objC->getName().toLower())) {
                createTableSql(sqlStr, objC);
            }

            for (int i = 0; i < objC->objectCount(); ++i) {
                createTables(sqlStr, objC->getObject(i));
            }
        }
    } else if (obj) {
        if (!mTableList.contains(obj->getName().toLower())) {
            createTableSql(sqlStr, obj);
        }
    } else {
        sqlStr.append("DB_ModelFactory::createTables() object is NULL ERROR");
        return false;
    }

    return true;
}

/**
 * Create default rows in database, to make sure ID '0' exists
 * @param obj object(tree) for which the rows are created
 */
bool DB_ModelFactory::createDefaultRows(RB_String& sqlStr, RB_ObjectBase* obj) {
    if (obj && obj->isContainer()) {
        RB_ObjectContainer* objC = (RB_ObjectContainer*)obj;

        if (objC->isList()) {
            for (int i = 0; i < objC->objectCount(); ++i) {
                createDefaultRows(sqlStr, objC->getObject(i));
            }
        } else {
            if (!mTableList.contains(objC->getName().toLower())) {
                createDefaultRowSql(sqlStr, objC);
            }

            for (int i = 0; i < objC->objectCount(); ++i) {
                createDefaultRows(sqlStr, objC->getObject(i));
            }
        }
    } else if (obj) {
        if (!mTableList.contains(obj->getName().toLower())) {
            createDefaultRowSql(sqlStr, obj);
        }
    } else {
        RB_DEBUG->error("DB_ModelFactory::createDefaultRows() ERROR");
        return false;
    }

    return true;
}

/**
 * Add default row with id='0' to new created table. DB_Project is
 * an exception, the id is a valid Uuid.
 * @parem sqlStr SQL statement to be set with add default row
 * @param obj object with table name
 */
void DB_ModelFactory::createDefaultRowSql(RB_String& sqlStr, RB_ObjectBase* obj) {
    sqlStr += "INSERT INTO " + obj->getName().toLower() + " (`id`, `parent`";
    RB_String valueStr;

    if (obj->getName().toLower() == "db_project") {
        valueStr = ") VALUES ('" + obj->getId() + "', 'none'";
    } else {
        valueStr = ") VALUES ('0', 'none'";
    }

    int memberCount = obj->memberCount();
    RB_ObjectMember* mem = NULL;


    for (int i = 2; i < memberCount; ++i) {
        sqlStr += ", ";
        valueStr += ", ";

        mem = obj->getMember(i);
        sqlStr += "`" + mem->getName().toLower() + "`";

        if (mem->getType() == RB2::MemberDouble
                || mem->getType() == RB2::MemberInteger) {
            valueStr += mem->getValue().toString();
        } else {
            valueStr += "'" + mem->getValue().toString() + "'";
        }
    }

    sqlStr += valueStr + ");\n";

    // Future, to prevent duplicates which should never happen
//    if (DB_MODELFACTORY->getDatabase().driverName() == "QDB2") {
//        sqlStr += "";
//        MERGE INTO T
//        USING TABLE(VALUES(1, 2, 3)) AS S(pk, c1, c2)
//        ON T.pk = S.pk
//        WHEN NOT MATCHED THEN INSERT VALUES (S.pk, S.c1, S.c2)
//        Can be extended to INSERT from a SELECT, by changing the USING part
//    } else if (DB_MODELFACTORY->getDatabase().driverName() == "QMYSQL") {
//        sqlStr += "INSERT IGNORE INTO " + obj->getName().toLower() + " VALUES (";
//    } else if (DB_MODELFACTORY->getDatabase().driverName() == "QOCI") {
//        sqlStr += "";
//        MERGE INTO your_table yt
//        USING (
//           SELECT 42 as the_pk_value,
//                  'some_value' as some_column
//           FROM dual
//        ) t on (yt.pk = t.the_pke_value)
//        WHEN NOT MATCHED THEN
//           INSERT (pk, the_column)
//           VALUES (t.the_pk_value, t.some_column);
//    } else if (DB_MODELFACTORY->getDatabase().driverName() == "QPSQL") {
//        sqlStr += "";
//        WHERE NOT EXISTS (SELECT 1 FROM tbl WHERE right_col = left_col)
//    } else if (DB_MODELFACTORY->getDatabase().driverName() == "QSQLITE") {
//        sqlStr += "INSERT OR IGNORE INTO " + obj->getName().toLower()
//                + " (id, parent, name, created, changed, status) "
//                + " VALUES (";
//    } else {
//        sqlStr += "";
//    }
}

/**
 * Check tables in database, list difference
 * TODO: other checks than only name to be added
 * TODO: check for existing tables that are not used anymore
 * based on perspective abbreviation
 * @param obj object(tree)
 * @param result result of the check
 */
bool DB_ModelFactory::checkTables(RB_ObjectBase* obj, RB_String& result) {
    RB_DEBUG->print("DB_ModelFactory::checkTables()");

    if (obj && obj->isContainer()) {
        RB_ObjectContainer* objC = (RB_ObjectContainer*)obj;
        bool difference = false;

        if (objC->getName().endsWith("List")) {
            for (int i = 0; i < objC->objectCount(); ++i) {
                checkTables(objC->getObject(i), result);
            }
        } else {
            QSqlRecord rec = getDatabase().record(objC->getName().toLower());

            if (rec.count() < 1) {
                result += "Table " + objC->getName().toLower() + " does not exist\n";
                difference = true;
            } else {
                result += "Table " + objC->getName().toLower() + " check:\n";
                RB_ObjectMember* mem = NULL;
                int memCount = rec.count();

                for (int i = 0; i < memCount; ++i) {
                    QSqlField fld = rec.field(i);
                    mem = objC->getMember(fld.name());

                    if (!mem) {
                        result += "  existing column " + fld.name() + " not used anymore\n";
                        difference = true;
                    }

// other checks:
//                    model->setData(model->index(i, 0), fld.name());
//                    model->setData(model->index(i, 1), fld.typeID() == -1
//                            ? QString(QVariant::typeToName(fld.type()))
//                            : QString("%1 (%2)").arg(QVariant::typeToName(fld.type())).arg(fld.typeID()));
//                    model->setData(model->index(i, 2), fld.length());
//                    model->setData(model->index(i, 3), fld.precision());
//                    model->setData(model->index(i, 4), fld.requiredStatus() == -1 ? QVariant("?")
//                            : QVariant(bool(fld.requiredStatus())));
//                    model->setData(model->index(i, 5), fld.isAutoValue());
//                    model->setData(model->index(i, 6), fld.defaultValue());
                }

                memCount = objC->memberCount();

                for (int i = 0; i < memCount; ++i) {
                    mem = objC->getMember(i);

                    if (!rec.contains(mem->getName())) {
                        result += "  column " + mem->getName() + " does not exist\n";
                        difference = true;
                    }
                }
            }

            if (!difference) {
                result += "  OK\n";
            }

            for (int i = 0; i < objC->objectCount(); ++i) {
                checkTables(objC->getObject(i), result);
            }
        }
    } else if (obj) {
        QSqlRecord rec = getDatabase().record(obj->getName().toLower());
        bool difference = false;

        if (rec.count() < 1) {
            result += "Table " + obj->getName().toLower() + " does not exist\n";
            difference = true;
        } else {
            result += "Table " + obj->getName().toLower() + " check:\n";
            RB_ObjectMember* mem = NULL;
            difference = false;
            int memCount = rec.count();

            for (int i = 0; i < memCount; ++i) {
                QSqlField fld = rec.field(i);
                mem = obj->getMember(fld.name());

                if (!mem) {
                    result += "  existing column " + fld.name() + " not used anymore\n";
                    difference = true;
                }
            }

            memCount = obj->memberCount();

            for (int i = 0; i < memCount; ++i) {
                mem = obj->getMember(i);

                if (!rec.contains(mem->getName())) {
                    result += "  column " + mem->getName() + " does not exist\n";
                    difference = true;
                }
            }
        }

        if (!difference) {
            result += "  OK\n";
        }
    }

    return true;
}

/**
 * Re-entrant method for the creation of SQL statements for missing tables
 * @brief DB_ModelFactory::createNonExistingTables
 * @param sqlStr creation of table statement added if not exists
 * @param obj current table object
 */
void DB_ModelFactory::createNonExistingTables(QString &sqlStr, RB_ObjectBase *obj) {
    // RB_DEBUG->print("DB_ModelFactory::createNonExistingTables()");

    if (obj && obj->isContainer()) {
        RB_ObjectContainer* objC = (RB_ObjectContainer*)obj;

        if (objC->getName().endsWith("List")) {
            for (int i = 0; i < objC->objectCount(); ++i) {
                createNonExistingTables(sqlStr, objC->getObject(i));
            }
        } else {
            if (!isTableExisting(objC)) {
                createTableSql(sqlStr, obj);
                createDefaultRowSql(sqlStr, obj);
            } else {
                for (int i = 0; i < objC->objectCount(); ++i) {
                    createNonExistingTables(sqlStr, objC->getObject(i));
                }
            }
        }
    } else if (obj) {
        if (!isTableExisting(obj)) {
            createTableSql(sqlStr, obj);
            createDefaultRowSql(sqlStr, obj);
        }
    }
}

RB_ObjectBase *DB_ModelFactory::getObjectFactoryRoot(const RB_String& objectFactoryName) {
    RB_ObjectBase* objFactRoot = NULL;
    std::vector<RB_ObjectFactory*> fl = DB_OBJECTFACTORY->getFactoryList();
    std::vector<RB_ObjectFactory*>::iterator iter;
    iter = fl.begin();

    while (iter != fl.end() && objFactRoot == NULL) {
        RB_ObjectFactory* f = *iter;

        if(objectFactoryName == f->getName()) {
            objFactRoot = f->getModelStructure("XXX_Project");
        }
        ++iter;
    }

    return objFactRoot;
}

bool DB_ModelFactory::isTableExisting(RB_ObjectBase* obj) {
    QSqlRecord rec = getDatabase().record(obj->getName().toLower());

    if (rec.count() < 1) {
        return false;
    }

    return true;
}

void DB_ModelFactory::beginTransaction(QString& sqlStr) {
    sqlStr.clear();

    if (DB_MODELFACTORY->getDatabase().driverName() == "QDB2") {
        sqlStr += "BEGIN TRANSACTION;\n\n";
    } else if (DB_MODELFACTORY->getDatabase().driverName() == "QMYSQL") {
        sqlStr += "START TRANSACTION;\n\n";
    } else if (DB_MODELFACTORY->getDatabase().driverName() == "QOCI") {
        sqlStr += "SET TRANSACTION;\n\n";
    } else if (DB_MODELFACTORY->getDatabase().driverName() == "QPSQL") {
        sqlStr += "BEGIN;\n\n";
    } else if (DB_MODELFACTORY->getDatabase().driverName() == "QSQLITE") {
        sqlStr += "BEGIN TRANSACTION;\n\n";
    } else {
        sqlStr += "BEGIN TRANSACTION;\n\n";
    }
}

void DB_ModelFactory::commitTransaction(QString& sqlStr) {
    if (DB_MODELFACTORY->getDatabase().driverName() == "QDB2") {
        sqlStr += "COMMIT TRANSACTION;\n";
    } else if (DB_MODELFACTORY->getDatabase().driverName() == "QMYSQL") {
        sqlStr += "COMMIT;\n";
    } else if (DB_MODELFACTORY->getDatabase().driverName() == "QOCI") {
        sqlStr += "COMMIT;\n";
    } else if (DB_MODELFACTORY->getDatabase().driverName() == "QPSQL") {
        sqlStr += "COMMIT;\n";
    } else if (DB_MODELFACTORY->getDatabase().driverName() == "QSQLITE") {
        sqlStr += "COMMIT;\n";
    } else {
        sqlStr += "COMMIT;\n";
    }
}

bool DB_ModelFactory::executeQuery(const QString& sqlStr) {
    QSqlQuery query(getDatabase());
    query.setForwardOnly(true); // performance only

    // remove \t \r \n etc.
    RB_String sqlStrSimplified = sqlStr.simplified();
    // handle multiple line sql
    QStringList sqlList = sqlStrSimplified.split(";", QString::SkipEmptyParts);
    int sqlStrCount = sqlList.count();

    for (int i = 0; i < sqlStrCount; ++i) {
        QString sql = sqlList.at(i);
        sql = sql.trimmed() + ";";

        if (!query.exec(sql)) {
            RB_DEBUG->error("DB_ModelFactory::executeQuery() ERROR");
            RB_DEBUG->print(sqlStr);
            mMessage += "DB_ModelFactory::executeQuery() ERROR, ";
            mMessage += query.lastError().text();
            query.exec("ROLLBACK;");
            return false;
        }
    }

    return true;
}

