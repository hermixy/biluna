/*****************************************************************
 * $Id: rb_dbvisitor.cpp 2165 2014-09-24 19:22:36Z rutger $
 * Created: Dec 7, 2011 3:48:59 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna db project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_dbvisitor.h"

#include "rb_database.h"
#include "rb_objectatomic.h"
#include "rb_objectcontainer.h"
#include "rb_string.h"


/**
 * Constructor
 * @param device device to be written
 */
RB_DbVisitor::RB_DbVisitor() {
    mObject = NULL;
    mIdFieldName = "parent";
    mCalledFromList = false;
    mUseParentId = false;
    mIsUpdateNewId = false;
}

/**
 * Visit object and execute Create Read Update database action,
 * preparation for the deletion only because children need to be deleted first
 * @param obj object
 */
void RB_DbVisitor::visitObject(RB_ObjectBase* obj) {
    mObject = obj;

    if (!mObject) {
        RB_DEBUG->error("RB_DbVisitor::visitObject() object is NULL ERROR");
        return;
    }

    switch (mAction) {
    case DbRead :
        dbRead();
        break;
    case DbUpdate :
        dbUpdate();
        break;
    case DbDelete :
        dbBeforeDelete();
        break;
    case DbNone :
        RB_DEBUG->error("RB_DbVisitor::visitObject() action not set ERROR");
        break;
    default :
        RB_DEBUG->error("RB_DbVisitor::visitObject() action not set ERROR");
        break;
    }
}

/**
 * Leave object, handles actual deletion of object(s) only
 * @param obj object
 */
void RB_DbVisitor::leaveObject(RB_ObjectBase* obj) {
    mObject = obj;

    if (!mObject) {
        RB_DEBUG->error("RB_DbVisitor::leaveObject() object is NULL ERROR");
        return;
    }

    switch (mAction) {
    case DbRead :
        // nothing
        break;
    case DbUpdate :
        // nothing
        break;
    case DbDelete :
        dbDelete();
        break;
    case DbNone :
        RB_DEBUG->error("RB_DbVisitor::leaveObject() action not set ERROR");
        break;
    default :
        RB_DEBUG->error("RB_DbVisitor::leaveObject() action not set ERROR");
        break;
    }
}

/**
 * Set object database visitor in read mode.
 */
void RB_DbVisitor::setDbRead() {
    mAction = DbRead;
}

/**
 * Set object database visitor in update mode.
 */
void RB_DbVisitor::setDbUpdate() {
    mAction = DbUpdate;
}

/**
 * Set object database visitor in delete mode.
 */
void RB_DbVisitor::setDbDelete() {
    mAction = DbDelete;
}

/**
 * Set database to be used
 * @param db database
 */
void RB_DbVisitor::setDatabase(QSqlDatabase db) {
    mDatabaseConnection = db.connectionName();
}

/**
 * Set flag whether object visitor is called from list
 * @param isFromList true if from list
 */
void RB_DbVisitor::setCalledFromList(bool isFromList) {
    mCalledFromList = isFromList;
}

/**
 * Set flag use parent id instead of ID of object for executing action
 * @param useParentId is true when parent ID is to be used
 */
void RB_DbVisitor::setUseParentId(bool useParentId) {
    mUseParentId = useParentId;
}

/**
 * Set fieldname of ID to be used for example 'parent' (is default)
 * or 'account_id'
 * @param idFieldName
 */
void RB_DbVisitor::setIdFieldName(const QString& idFieldName) {
    mIdFieldName = idFieldName;
}

/**
 * Set new ID for objects when executing update to database
 * Note: does not cascade down for the childrens parent ID.
 * @param isNewID true if new ID, default is false
 */
void RB_DbVisitor::setUpdateNewId(bool isNewId = true) {
    mIsUpdateNewId = isNewId;
}


/**
 * Read object data from database
 * if mCalledFromList is true, objects have already been read
 * with dbReadList, only the child objects in the list containers
 * need to be read
 * If mUseParentId is true, use parent ID instead of object ID in query
 */
bool RB_DbVisitor::dbRead() {
    if (mCalledFromList && !mObject->isList()) {
        // List object updates have been handled by dbReadList()
        return true;
    } else if (mObject->isList()) {
        return dbReadList();
    }

    RB_String sqlStr = "SELECT * FROM " + mObject->getName().toLower();

    if (!mUseParentId) {
        sqlStr += " WHERE id='" + mObject->getId() + "';";
    } else {
        sqlStr += " WHERE parent='" + mObject->getValue("parent").toString() + "';";
    }

    QSqlQuery q(RB_DATABASE->database(mDatabaseConnection));
    q.setForwardOnly(true); // performance only

    if (!q.exec(sqlStr)) {
        RB_DEBUG->error("RB_DbVisitor::dbRead() ERROR");
        RB_DEBUG->print(q.lastError().text());
        RB_DEBUG->print(q.lastQuery());
        return false;
    }

//    RB_DEBUG->print(q.executedQuery());

    // only one object with id, parent, name already set
    if (q.first()) {
        QSqlRecord rec = q.record();
        RB_String fieldName;
        int count = rec.count();

        for (int i = 0; i < count; ++i) {
            fieldName = rec.fieldName(i);
            mObject->setValue(fieldName, rec.value(i));
        }

        mObject->setFlag(RB2::FlagFromDatabase);
        mObject->deleteFlag(RB2::FlagIsDirty);
    } else {
        // no record available
        mObject->setValue("id", "");
        mObject->deleteFlag(RB2::FlagFromDatabase);
        mObject->deleteFlag(RB2::FlagIsDirty);
    }

    return true;

}

/**
 * Read objects from database in list
 * 1. based on ID of parent object
 * 2. if parent is NULL, mIdFieldName (or default='parent') will be used
 * with 'parent' ID of this container
 * 3. if parent is NULL and mIdFieldName is empty (should not happen), parent
 * will be used with actual ID 'id' of this container
 * \TODO this last option needs further consideration
 * @return true on success
 */
bool RB_DbVisitor::dbReadList() {
    if (!mObject->isList()) {
        return dbRead();
    }

    mCalledFromList = true;

    RB_String tableName = mObject->getName().remove("List");
    RB_String sqlStr = "SELECT * FROM " + tableName.toLower();

    if (mObject->getParent()) {
        sqlStr += " WHERE parent='" + mObject->getParent()->getId() + "';";
    } else if (!mIdFieldName.isEmpty()) {
        // RB_ObjectContainer list was send without parent to read from a table
        sqlStr += " WHERE " + mIdFieldName + "='" + mObject->getValue("parent").toString() + "';";
    } else if (mIdFieldName.isEmpty()) {
        // List to be read with value of ID of container, should not happen(!)
        sqlStr += " WHERE parent='" + mObject->getValue("id").toString() + "';";
    }

    QSqlQuery q(RB_DATABASE->database(mDatabaseConnection));
    q.setForwardOnly(true);

    if (!q.exec(sqlStr)) {
        RB_DEBUG->error("RB_DbVisitor::dbReadList() ERROR");
        RB_DEBUG->print(q.lastError().text());
        RB_DEBUG->print(q.lastQuery());
        return false;
    }

//    RB_DEBUG->print(q.executedQuery());

    RB_String fieldName;
//    RB_String value;
    QSqlRecord rec;

    while (q.next()) {
        rec = q.record();
        RB_ObjectBase* childObj;

        if (mObject->getFactory()) {
            // factory available
            childObj = mObject->getFactory()->newObject("", mObject, "", true);

            for (int i = 0; i < rec.count(); ++i) {
                fieldName = rec.fieldName(i);

                if (fieldName.toLower() == "id") {
                    childObj->setId(rec.value(i).toString());
                } else /* if (fieldName.toLower() != "parent"
                           && fieldName.toLower() != "name")*/ {
                    // parent ID and name is set by factory
                    RB_ObjectMember* mem = childObj->getMember(fieldName);
                    if (mem) {
                        mem->setValue(rec.value(i));
                    }
                }
            }
        } else {
            // no factory
            RB_ObjectContainer* objC = dynamic_cast<RB_ObjectContainer*>(mObject);
            if (!objC) {
                RB_DEBUG->print("RB_DbVisitor::dbReadList() object container ERROR");
                return false;
            }

            childObj = objC->newObject();

            for (int i = 0; i < rec.count(); ++i) {
                fieldName = rec.fieldName(i);

                if (fieldName.toLower() == "id") {
                    childObj->setId(rec.value(i).toString());
                } else if (fieldName.toLower() == "parent"
                           || fieldName.toLower() == "name"
                           || fieldName.toLower() == "status"
                           || fieldName.toLower() == "created"
                           || fieldName.toLower() == "changed"
                           || fieldName.toLower() == "muser") {
                    childObj->setValue(fieldName.toLower(), rec.value(i));
                } else {
                    childObj->addMember(fieldName.toLower(), "-", rec.value(i));
                }
            }
        }

        childObj->setFlag(RB2::FlagFromDatabase);
        childObj->deleteFlag(RB2::FlagIsDirty);
    }

    return true;
}

/**
 * Update database or insert object data.
 * NOTE: A tree (database) model creates a dummy root of the same class as
 * the tree objects. This root object has the same ID as the actual parent.
 * The parent ID is set to "". This dummy root object is not stored in the
 * database based on the "" parent ID. Therefore make sure that objects
 * that need to be stored to the database should always have a parent ID.
 * The root XXX_Project objects have standard "0" as parent ID and can be
 * stored to the database. Objects for which the flag FlagIsDeleted is set,
 * will call dbDelete() and for the children.
 * @returns true on success
 */
bool RB_DbVisitor::dbUpdate() {
    if (mCalledFromList && !mObject->isList()) {
        // List object updates have been handled by dbUpdateList()
        return true;
    } else if (mObject->isList()) {
        return dbUpdateList();
    }

    if (mObject->getFlag(RB2::FlagFromDatabase)
            && mObject->getFlag(RB2::FlagIsDeleted)) {
        return dbDelete();
    }

    if (mObject->getValue("created").toString() == "0000-00-00T00:00:00") {
        mObject->setValue("created", QDateTime::currentDateTime().toString(Qt::ISODate));
    }
    mObject->setValue("changed", QDateTime::currentDateTime().toString(Qt::ISODate));

    QSqlQuery q(RB_DATABASE->database(mDatabaseConnection));

    if(!mObject->getFlag(RB2::FlagFromDatabase) && !mObject->isList()) {
        prepareInsertQuery(q, mObject);
        int memberCount = mObject->countMember();

        for (int i = 0; i < memberCount; ++i) {
            RB_ObjectMember* mem = mObject->getMember(i);
            if (mem->getName() == "id") {
                if (!mIsUpdateNewId) {
                    q.addBindValue(mem->getValue(), QSql::In);
                } else {
                    q.addBindValue(RB_Uuid::createUuid().toString(), QSql::In);
                }
            } else {
                q.addBindValue(mem->getValue(), QSql::In);
            }
        }
    } else if (mObject->getFlag(RB2::FlagFromDatabase) && !mObject->isList()
               && mObject->getFlag(RB2::FlagIsDirty)) {
        prepareUpdateQuery(q, mObject);
        int memberCount = mObject->countMember();

        for (int i = 1; i < memberCount; ++i) { // not ID
            // parent has to have the correct ID
            RB_ObjectMember* mem = mObject->getMember(i);
            q.addBindValue(mem->getValue(), QSql::In);
        }

        // WHERE id=?
        q.addBindValue(mObject->getMember(0)->getValue(), QSql::In);
    }

    if (mObject->getFlag(RB2::FlagIsDirty) && !mObject->isList()
            && (!mObject->getValue("parent").toString().isEmpty())) { // refer to method description
        if (q.exec()) {
//            RB_DEBUG->print(q.executedQuery());
            mObject->setFlag(RB2::FlagFromDatabase);
            mObject->deleteFlag(RB2::FlagIsDirty);
        } else {
            RB_DEBUG->error("RB_DbVisitor::dbUpdate()1 ERROR");
            RB_DEBUG->print(q.lastError().text());
            RB_DEBUG->print(q.lastQuery());
            return false;
        }
    } else if (mObject->getValue("parent").toString() == "") {
        // Debug only
        RB_DEBUG->error("RB_DbVisitor::dbUpdate()11 parent is empty ERROR");
    }

    return true;
}

/**
 * Update or insert objects from list into database
 * @param db database connection
 * @param level resolve level
 * @return true on success
 */
bool RB_DbVisitor::dbUpdateList() {
    if (!mObject->isList()) {
        return dbUpdate();
    }

    mCalledFromList = true;

    bool isFirstUpdateObject = true;
    bool isFirstInsertObject = true;
    int memberCount = 0;
    bool success = true;

    QSqlQuery qUpdate(RB_DATABASE->database(mDatabaseConnection));
    QSqlQuery qInsert(RB_DATABASE->database(mDatabaseConnection));
    RB_ObjectIterator* iter = mObject->createIterator();

    for (iter->first(); !iter->isDone() && success; iter->next()) {
        // This is a list container otherwise no objects
        RB_ObjectBase* obj = iter->currentObject();

        if (obj->getFlag(RB2::FlagFromDatabase)
                && obj->getFlag(RB2::FlagIsDeleted)) {
            RB_DbVisitor* vis = new RB_DbVisitor();
            vis->setDatabase(RB_DATABASE->database(mDatabaseConnection));
            vis->setDbDelete();
            obj->acceptVisitor(*vis);
            delete vis;
            continue;
        }

        if(!obj->getFlag(RB2::FlagFromDatabase) && !obj->isList()) {
            // can be an object from outside
            if (obj->getValue("created").toString() == "0000-00-00T00:00:00") {
                obj->setValue("created", QDateTime::currentDateTime().toString(Qt::ISODate));
                obj->setValue("changed", QDateTime::currentDateTime().toString(Qt::ISODate));
            }

            if (isFirstInsertObject) {
                prepareInsertQuery(qInsert, obj);
                memberCount = obj->countMember();
                isFirstInsertObject = false;
            }

            for (int i = 0; i < memberCount; ++i) {
                RB_ObjectMember* mem = obj->getMember(i);
                if (mem->getName() == "id") {
                    if (!mIsUpdateNewId) {
                        qInsert.addBindValue(mem->getValue(), QSql::In);
                    } else {
                        qInsert.addBindValue(RB_Uuid::createUuid().toString(), QSql::In);
                    }
// used in acc_dbvisitor:
//                } else if (mem->getName() == "parent") {
//                    RB_ObjectBase* grParent = obj->getGrandParent();

//                    // make sure that children have correct parent ID
//                    if (grParent) {
//                        qInsert.addBindValue(grParent->getId(), QSql::In);
//                    }
                } else {
                    qInsert.addBindValue(mem->getValue(), QSql::In);
                }
            }

            if (qInsert.exec()) {
//                RB_DEBUG->print(qInsert.executedQuery());
                obj->setFlag(RB2::FlagFromDatabase);
                obj->deleteFlag(RB2::FlagIsDirty);
            } else {
                RB_DEBUG->error("RB_DbVisitor::dbUpdate()1 ERROR");
                RB_DEBUG->print(qInsert.lastError().text());
                RB_DEBUG->print(qInsert.lastQuery());
                return false;
            }
        } else if (obj->getFlag(RB2::FlagFromDatabase) && !obj->isList()
                   && obj->getFlag(RB2::FlagIsDirty)) {
            // changed date and flag was already set

            if (isFirstUpdateObject) {
                prepareUpdateQuery(qUpdate, obj);
                memberCount = obj->countMember();
                isFirstUpdateObject = false;
            }

            for (int i = 1; i < memberCount; ++i) { // not ID
                RB_ObjectMember* mem = obj->getMember(i);
                qUpdate.addBindValue(mem->getValue(), QSql::In);
            }
            // ID is last in where statement
            qUpdate.addBindValue(obj->getMember(0)->getValue(), QSql::In);

            if (qUpdate.exec()) {
                RB_DEBUG->print(qUpdate.executedQuery());
                obj->setFlag(RB2::FlagFromDatabase);
                obj->deleteFlag(RB2::FlagIsDirty);
            } else {
                RB_DEBUG->error("RB_DbVisitor::dbUpdate()1 ERROR");
                RB_DEBUG->print(qUpdate.lastError().text());
                RB_DEBUG->print(qUpdate.lastQuery());
                return false;
            }
        }
    }

    delete iter;
    return success;
}

/**
 * Before delete, run through children of object in database
 * @return true on success
 */
bool RB_DbVisitor::dbBeforeDelete() {
    if (!mObject->isList() && !mObject->getFlag(RB2::FlagFromDatabase)) {
        return false;
    }

    if (mObject->isList()) {
        return dbDeleteList();
    } else if (mObject->isContainer() && !mObject->isList()) {
        // Object is a container with possible lists, delete lists
        RB_ObjectBase* objC = NULL;
        RB_ObjectIterator* iter = mObject->createIterator();

        for (iter->first(); !iter->isDone(); iter->next()) {
            objC = iter->currentObject();

            // use list of corresponding list of model structure and set parent id
            objC = mObject->getFactory()->getModelStructure(objC->getName());
            objC->getParent()->setId(mObject->getId());

            // Set resolve level temporary to all
            RB2::ResolveLevel level = getResolveLevel();
            setResolveLevel(RB2::ResolveAll);
            objC->acceptVisitor(*this);
            setResolveLevel(level);
        }

        delete iter;
    }

    return true;
}

/**
 * Actual deletion of object from database
 * TODO: no flags are set, because maybe not necessary after deletion of objects
 */
bool RB_DbVisitor::dbDelete() {
    if (!mObject->isList() && !mObject->getFlag(RB2::FlagFromDatabase)) {
        return false;
    }

    if (mObject->isList()) {
        return dbDeleteList();
    }

    // Object is atomic or container and not list, delete object
    RB_String sqlStr = "DELETE FROM " + mObject->getName().toLower();
    sqlStr += " WHERE id='" + mObject->getId() + "';";
    QSqlQuery q(RB_DATABASE->database(mDatabaseConnection));
    q.setForwardOnly(true); // performance only

    bool success = q.exec(sqlStr);

    if (!success) {
        RB_DEBUG->error("RB_DbVisitor::dbDelete() ERROR");
        RB_DEBUG->print(q.lastError().text());
        RB_DEBUG->print(q.lastQuery());
    }

//    RB_DEBUG->print(q.executedQuery());
    return success;
}

/**
 * Delete in database objects from list and their children. The list is
 * coming from the objectfactory modelstructure and has the corresponding
 * parent id.
 * TODO: no flags are set, because maybe not necessary after deletion of objects
 * @return true on success
 */
bool RB_DbVisitor::dbDeleteList() {
    if (!mObject->isList()) {
        return dbDelete();
    }

    RB_ObjectBase* obj = NULL; // object name used for id and table name
    RB_String parentId;
    if (mObject->getParent()) {
        parentId = mObject->getParent()->getId();
    } else {
        // RB_ObjectContainer list was send without parent to delete from a table
        parentId = mObject->getValue("parent").toString();
    }

    QSqlQuery q(RB_DATABASE->database(mDatabaseConnection));
    q.setForwardOnly(true); // performance only
    RB_String qStr = "";
    RB_ObjectIterator* iter = mObject->createIterator();

    // actually only one object in model structure
    for (iter->first(); !iter->isDone(); iter->next()) {
        obj = iter->currentObject();

        // Set resolve level temporary to all, idFieldName and delete other tables
        RB2::ResolveLevel level = getResolveLevel();
        setResolveLevel(RB2::ResolveAll);
        QString idFieldName = mIdFieldName;
        mIdFieldName = "parent";
        obj->acceptVisitor(*this);
        setResolveLevel(level);
        setIdFieldName(idFieldName);

        // Remove all from the table object parent=id
        qStr = "DELETE FROM " + obj->getName().toLower()
                         + " WHERE " + idFieldName + "='" + parentId + "'";
        if (!q.exec(qStr)) {
            RB_DEBUG->error("RB_DbVisitor::dbDeleteList() ERROR");
            RB_DEBUG->print(q.lastError().text());
            RB_DEBUG->print(q.lastQuery());
            return false;
        }

//        RB_DEBUG->print(q.executedQuery());
    }

    delete iter;
    return true;
}

void RB_DbVisitor::setReadQuery(QSqlQuery& /*q*/, RB_ObjectBase* /*obj*/,
                                const QString& /*fieldName*/, const QString&/* id*/) {

}

void RB_DbVisitor::prepareInsertQuery(QSqlQuery& q, RB_ObjectBase* obj) {
    // INSERT INTO table_name (column1, column2, column3,...)
    // VALUES (value1, value2, value3,...)
    //     or better
    // QSqlQuery query;
    // query.prepare("INSERT INTO person (id, forename, surname) "
    //               "VALUES (?, ?, ?)");
    // query.addBindValue(1001);
    // query.addBindValue("Bart");
    // query.addBindValue("Simpson");
    // query.exec();

    RB_String insertStr = "INSERT INTO `" + obj->getName().toLower() + "` (`";
    RB_String valueStr = "`) VALUES (";
    bool isFirstRow = true;
    int memberCount = obj->countMember();

    for (int i = 0; i < memberCount; ++i) {
        if (!isFirstRow) {
            insertStr += "`, `";
            valueStr += ", ";
        }

        insertStr += obj->getMember(i)->getName().toLower();
        valueStr += "?";
        isFirstRow = false;
    }

    q.prepare(insertStr + valueStr + ")");
}

void RB_DbVisitor::prepareUpdateQuery(QSqlQuery& q, RB_ObjectBase* obj) {
    // UPDATE table_name
    // SET column1=value, column2=value2,...
    // WHERE some_column=some_value
    //     or better
    // QSqlQuery query;
    // query.prepare("UPDATE person "
    //               "SET forename=?, surname=? "
    //               "WHERE id=?");
    // query.addBindValue("Bart");
    // query.addBindValue("Simpson");
    // query.exec();

    bool isFirstRow = true;
    RB_String sqlStr = "UPDATE `" + obj->getName().toLower() + "` SET `";
    int memberCount = obj->countMember();

    for (int i = 1; i < memberCount; ++i) { // not ID
        if (!isFirstRow) {
            sqlStr += ", `";
        }

        // parent has to have the correct ID
        sqlStr += obj->getMember(i)->getName().toLower() + "`=?";
        isFirstRow = false;
    }

    // Use ID
    sqlStr += " WHERE `id`=?";

    q.prepare(sqlStr);
}

void RB_DbVisitor::setDeleteQuery(QSqlQuery& /*q*/, RB_ObjectBase* /*obj*/,
                                  const QString& /*fieldName*/, const QString& /*id*/) {

}
