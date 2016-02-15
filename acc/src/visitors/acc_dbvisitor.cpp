/*****************************************************************
 * $Id: acc_dbvisitor.cpp 2166 2014-09-25 13:23:18Z rutger $
 * Created: Jan 16, 2012 12:53:43 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_dbvisitor.h"

#include "acc_modelfactory.h"
#include "acc_objectfactory.h"
#include "acc_sqlcommonfunctions.h"
#include "db_modelfactory.h"
#include "rb_database.h"
#include "rb_objectatomic.h"
#include "rb_objectcontainer.h"
#include "rb_string.h"


/**
 * Constructor
 * @param device device to be written
 */
ACC_DbVisitor::ACC_DbVisitor() {
    mProjectId = "";
    mIsInitDone = false;
    mIsSysSettingExisting = false;
    mIsProjectSettingExisting = false;
    mMemoryResolveLevel = RB2::ResolveNone;
}

/**
 * Visit object and execute Create Read Update database action,
 * preparation for the deletion only because children need to be deleted first
 * @param obj object
 */
void ACC_DbVisitor::visitObject(RB_ObjectBase* obj) {
    mObject = obj;

    if (!mObject) {
        RB_DEBUG->error("ACC_DbVisitor::visitObject() object is NULL ERROR");
        return;
    }

    RB_String objName = mObject->getName();

    switch (mAction) {
    case DbRead :
        // ACC_Project and project settings only
        if (objName == "ACC_Project"
                || ACC_OBJECTFACTORY->isProjectSetting(objName)) {
            dbRead();
        } else if (objName == "DB_ProjectList") {
            // only one placeholder DB_Project with root id,
            // not to be read from database
            RB_ObjectBase* childObj =
                    mObject->getFactory()->newObject("", mObject, "", true);
            childObj->setId(DB_MODELFACTORY->getRootId());
        } else if (ACC_OBJECTFACTORY->isSystemSetting(objName)
                   && objName.endsWith("List", Qt::CaseInsensitive)) {
            // all system settings have the parent current DB_Project.id
            dbRead();
        }

        break;
    case DbUpdate :
        // If XXX_Project, check if system settings exist and whether there is valid project ID
        if (objName.contains("ACC_Project", Qt::CaseInsensitive)) {
            if (!isInitDone()) {
                init();
            }

            if (mProjectId.isEmpty()) {
                return; // database error
            }

            // NOTE: ACC_Project already selected, not to be inserted
            mObject->setId(mProjectId);
            return;
        }

        if (!isProjectSettingExisting()
                && ACC_OBJECTFACTORY->isProjectSetting(objName)
                && objName.endsWith("List")) {
            // currently only ACC_ChartMaster(List)
            dbUpdateList();
        } else if (isProjectSettingExisting()
                   && ACC_OBJECTFACTORY->isProjectSetting(objName)) {
            // skip branch, reset in leave object
            mMemoryResolveLevel = getResolveLevel();
            setResolveLevel(RB2::ResolveNone);
        } else if (objName == "DB_Project") {
            // to ensure parent is set to current DB_Project.id
            mObject->setId(DB_MODELFACTORY->getRootId());
        } else if (!isSystemSettingExisting()
                   && ACC_OBJECTFACTORY->isSystemSetting(objName)
                   && objName.endsWith("List")) {
            dbUpdateList();
        } else if (isSystemSettingExisting()
                   && ACC_OBJECTFACTORY->isSystemSetting(objName)) {
            // skip branch
            mMemoryResolveLevel = getResolveLevel();
            setResolveLevel(RB2::ResolveNone);
        } else if (!ACC_OBJECTFACTORY->isProjectSetting(objName)
                   && !ACC_OBJECTFACTORY->isSystemSetting(objName)) {
            dbUpdateList();
        }

        break;
    case DbDelete :
        // dbBeforeDelete();
        break;
    case DbNone :
        RB_DEBUG->error("ACC_DbVisitor::visitObject() action not set ERROR");
        break;
    default :
        RB_DEBUG->error("ACC_DbVisitor::visitObject() action not set ERROR");
        break;
    }
}

void ACC_DbVisitor::leaveObject(RB_ObjectBase *obj) {
    if (mMemoryResolveLevel != RB2::ResolveNone) {
        setResolveLevel(mMemoryResolveLevel);
    } else {
        RB_DbVisitor::leaveObject(obj);
    }
}

/**
 * Update or insert objects from list into database
 * @param db database connection
 * @param level resolve level
 * @return true on success
 */
bool ACC_DbVisitor::dbUpdateList() {
    if (!mObject->isList()) {
        return dbUpdate();
    }

    mCalledFromList = true;

    bool isFirstUpdateObject = true;
    bool isFirstInsertObject = true;
    int memberCount = 0;
    bool success = true;

    QSqlDatabase db = RB_DATABASE->database(mDatabaseConnection);
    QSqlQuery qUpdate(db);
    QSqlQuery qInsert(db);
    RB_ObjectIterator* iter = mObject->createIterator();

    for (iter->first(); !iter->isDone() && success; iter->next()) {
        // This is a list container otherwise no objects
        RB_ObjectBase* obj = iter->currentObject();

        if (obj->getFlag(RB2::FlagFromDatabase)
                && obj->getFlag(RB2::FlagIsDeleted)) {
            RB_DbVisitor* vis = new RB_DbVisitor();
            vis->setDatabase(db);
            vis->setDbDelete();
            obj->acceptVisitor(*vis);
            delete vis;
            continue;
        }

        if(!obj->getFlag(RB2::FlagFromDatabase) && !obj->isList()) {
            // can be an object from outside
            if (obj->getValue("created").toString() <= "1900-01-01Z") { // Z to include time
                obj->setValue("created", QDateTime::currentDateTime().toString(Qt::ISODate));
                obj->setValue("changed", QDateTime::currentDateTime().toString(Qt::ISODate));
            }

            if (isFirstInsertObject) {
                prepareInsertQuery(qInsert, obj);
                memberCount = obj->memberCount();
                isFirstInsertObject = false;
            }

            for (int i = 0; i < memberCount; ++i) {
                RB_ObjectMember* mem = obj->getMember(i);
                if (mem->getName() == "id") {
                    if (!mIsUpdateNewId) {
                        qInsert.addBindValue(obj->getMember(i)->getValue(), QSql::In);
                    } else {
                        RB_String strId = RB_Uuid::createUuid().toString();

//                        if (!isSystemSettingExisting()
//                                && obj->getName() == "ACC_AccountGroup") {
//                            // This assumes that ACC_ChartMaster is always
//                            // before ACC_AccountGroup, find the new
//                            // ACC_ChartMaster ID corresponding to
//                            // the old ID from import XML file
//                            strId = findNewGroupId(mem->getValue().toString());
//                        }

                        // setValue() required for parent setting
                        obj->getMember(i)->setValue(strId);
                        qInsert.addBindValue(strId, QSql::In);
                    }
                } else if (mem->getName() == "parent") {
                    // make sure that children have correct parent ID
                    RB_ObjectBase* grParent = obj->getGrandParent();

                    if (grParent) {
                        qInsert.addBindValue(grParent->getId(), QSql::In);
                    }
                } else if (mem->getName().endsWith("_idx")) {
                    qInsert.addBindValue(obj->getMember(i)->getValue().toString()
                                         + obj->getMember(i)->getDisplayValue().toString(),
                                         QSql::In);
//                } else if (!isSystemSettingExisting()
//                           && obj->getName() == "ACC_ChartMaster"
//                           && mem->getName() == "accountgroup_id") {
//                    // Find (ACC_AccountGroup) ID already used in
//                    // previous accountgroup_id of ACC_ChartMaster
//                    RB_String strId = findNewGroupId(mem->getValue().toString());

//                    if (strId == "0") {
//                        // not yet used, not found
//                        strId = RB_Uuid::createUuid().toString();
//                        // Store old ID with new ACC_AccountGroup ID
//                        mGroupIdMap[mem->getValue().toString()] = strId;
//                    }

//                    qInsert.addBindValue(strId, QSql::In);
//                } else if (isSystemSettingExisting()
//                           && obj->getName() == "ACC_ChartMaster"
//                           && mem->getName() == "accountgroup_id") {
//                    qInsert.addBindValue("0", QSql::In);
                } else {
                    qInsert.addBindValue(mem->getValue(), QSql::In);
                }
            }

            if (qInsert.exec()) {
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
                memberCount = obj->memberCount();
                isFirstUpdateObject = false;
            }

            for (int i = 1; i < memberCount; ++i) { // not ID
                RB_ObjectMember* mem = obj->getMember(i);
                if (mem->getName().endsWith("_idx")) {
                    qUpdate.addBindValue(obj->getMember(i)->getValue().toString()
                                         + obj->getMember(i)->getDisplayValue().toString(),
                                         QSql::In);
                } else {
                    qUpdate.addBindValue(obj->getMember(i)->getValue(), QSql::In);
                }

            }
            // ID is last in where statement
            qUpdate.addBindValue(obj->getMember(0)->getValue(), QSql::In);

            if (qUpdate.exec()) {
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
 * Initialize visitor: check system settings exist
 * and valid project ID available
 */
void ACC_DbVisitor::init() {
    mIsInitDone = true;

    if (!ACC_MODELFACTORY->getDatabase().isOpen()) {
        RB_DEBUG->error("ACC_DbVisitor::init() database not open ERROR");
        return;
    }

    mProjectId = ACC_MODELFACTORY->getRootId();

    ACC_SqlCommonFunctions f;
    QSqlQuery query(ACC_MODELFACTORY->getDatabase());
    f.selectAllFromWhere(query, "acc_accountgroup", "`parent`='"
                         + DB_MODELFACTORY->getRootId() + "'");

    if (query.first()) {
        mIsSysSettingExisting = true;
    }

    query.clear();
    f.selectAllFromWhere(query, "acc_chartmaster", "`parent`='" + mProjectId + "'");

    if (query.first()) {
        mIsProjectSettingExisting = true;
    }
}

/**
 * @returns true when initialization has been done: check system settings exist
 * and valid project ID available
 */
bool ACC_DbVisitor::isInitDone() {
    return mIsInitDone;
}

/**
 * @returns true if system setting already exists
 */
bool ACC_DbVisitor::isSystemSettingExisting() {
    return mIsSysSettingExisting;
}

/**
 * @returns true if acc_chartmaster already exists for this project
 */
bool ACC_DbVisitor::isProjectSettingExisting() {
    return mIsProjectSettingExisting;
}

