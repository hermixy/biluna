/*****************************************************************
 * $Id: rb_mmsource.cpp 2218 2015-02-20 16:03:26Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna db project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_mmsource.h"

#include <QtWidgets>
#include <QItemSelection>
#include <QSqlQuery>
#include "rb.h"
#include "rb_database.h"
#include "rb_flagvisitor.h"
#include "rb_mmproxy.h"
#include "rb_modelfactory.h"
#include "rb_settings.h"


#define RBSQL_PREFETCH 255

/**
 * Constructor
 */
RB_MmSource::RB_MmSource(QObject* parent, const QSqlDatabase& db, int hiddenColumns)
        : RB_MmAbstract(parent, db) {
    RB_DEBUG->print("RB_MmSource::RB_MmSource()");

    // RB_MmAbstract
    mRoot = NULL;
    mListName = "";
    mTableName = "";
    mTableNamePrev = "";
    mObjectFactory = NULL;
    mBaseClone = false;
    mModified = false;
    mIsTreeModel = false;
    mIsInMemoryModel = false;
    mDatabaseConnection = db.connectionName();
    mNoColumns = 0;
    mPrimaryParent = "parent";
    mSecondParent = "";
    mWhere = "";
    mHiddenColumns = hiddenColumns;

    mObject = NULL;

    mLightBlue = "#CCCCFF";     //QBrush(QColor(204, 204, 255));
    mLightGray = "#CCCCCC";     //QBrush(QColor(204, 204, 204));
    mLightGreen = "#CCFFCC";    //QBrush(QColor(204, 255, 204));
    mLightRed = "#FFCCCC";      //QBrush(QColor(255, 204, 204));
    mLightYellow = "#FFFFCC";   //QBrush(QColor(255, 255, 204));

    mCurrentIndex = QModelIndex();
    // setEditStrategy(QSqlTableModel::OnRowChange);
    // setEditStrategy(QSqlTableModel::OnFieldChange);
    setEditStrategy(QSqlTableModel::OnManualSubmit);

    mSortOrder = RB2::SortOrderNone;
    mSortColumnName1 = "";
    mSortColumnName2 = "";
    mSortColumnName3 = "";

    mShared = false;
    mIsSubmitAllInProgress = false;
}


RB_MmSource::~RB_MmSource() {
    if (mRoot) { 
		if (database().isOpen() || this->isInMemoryModel()) {
			// Root object for database or in-memory object model
			// For child objects the root has been set to NULL
			// in RB_MmProxy();;~RB_MmProxy
			delete mRoot;
			mRoot = NULL;
		}
		
		// if full object model without database, 
		// root is to be deleted in main application
    }

    if (mObject) {
        delete mObject;
        mObject = NULL;
    }

    std::map<int, RB_MmSource*>::const_iterator iter = mRelationalModelMap.begin();
    while (iter != mRelationalModelMap.end()) {
        RB_MmSource* mM = iter->second;
        delete mM;
        ++iter;
    }

    RB_DEBUG->print(QString("RB_MmSource::~RB_MmSource() - %1 OK")
                    .arg(mTableName));
}

/**
 * Set root new (id == "" selects all), creates root if root object does not exist.
 * Use in case of database table- and database tree view only.
 * TODO: improve createTree methods
 * @return root object
 */
RB_ObjectBase* RB_MmSource::setRoot(const RB_String& id) {
    RB_DEBUG->print("RB_MmSource::setRoot(id=" + id + ")");

    // Create root if not yet exists,
    // root object is place holder for id and root object data
    // refer also to setTableModel()
    if (!mRoot) {
        if (!isTreeModel() && database().isOpen()) {
            mRoot = mObjectFactory->newParentObject(mTableName);
        } else if (database().isOpen()) {
            // in tree model the parent is always the same type as the tree objects
            // make sure that the type as a container with same types in objectfactory
            mRoot = mObjectFactory->newSingleObject(mTableName);
        }
        if (!mRoot) {
            // Only at creation of this object manager
            // Set dummy root object XXX_Project with new uuid
            // If possible do not use other than for top level model,
            // use actual root object because always a XXX_Project is created
            mRoot = mObjectFactory->newObject("", NULL, "", true);
            mRoot->setId(id);

            RB_DEBUG->print(RB_Debug::D_WARNING,
                            "RB_MmSource::setRoot(id) mRoot is NULL "
                            " or no database connection WARNING");
            return mRoot;
        }
    }

    // Set root id
    if (!isTreeModel() && database().isOpen()) { // database table model
        if (id != "") { // valid id
            if (mSecondParent == "") {
                mRoot->setId(id);
            } else {
                mSecondParentId = id;
            }
        } else { // no id
            if (mSecondParent == "") {
                mRoot->setId(""); // selects all
            } else {
                mSecondParent = "";
                mSecondParentId = "";
            }
        }
        // set filter and select
        createTableFilter();
    } else if (database().isOpen()) { // database tree model
        mRoot->setId(id);
    }

    return mRoot;
}

/**
 * Set root based on object (= pointer) from selected parent in main tableView.
 * If parameter root is NULL than the top level (XXX_Project) object will be
 * created, irrespective whether this is a child model or not.
 * This is applicable for main model when application is first started.
 * But if this is a tree model than a dummy root object will be created
 * according the listname or tablename.
 * If the root already exists in this model, the next time that root=NULL
 * is used, than the root will be set to NULL (and detached of an
 * in-memory model), used for example on temporary models around an in-memory
 * tree model.
 * @param root root object of this model
 */
void RB_MmSource::setRoot(RB_ObjectBase* root) {
    RB_DEBUG->print("RB_MmSource::setRoot(root)");

    if (!root && !mObjectFactory) {
        RB_DEBUG->error("RB_MmSource::setRoot(root) "
                        "root and object factory is NULL ERROR");
        return;
    }

    if (!mRoot && !root && !mIsTreeModel) {
        // Only at creation of this object manager
        // Set dummy root object XXX_Project with new uuid
        // If possible do not use other than for top level model,
        // use actual root object because always a XXX_Project is created
        mRoot = mObjectFactory->newObject("", NULL, "", true);

        if (database().isOpen()) {
            // id == "" shows all
            setRoot("");
        }
    } else if (!mRoot && !root && mIsTreeModel) {
        // Only at creation of this object manager
        // Set dummy root object mTableName with new uuid
        // If possible do not use other than for top level model,
        // use actual root object because always a XXX_Project is created
        mRoot = mObjectFactory->newObject("", NULL, "", true);
    } else {
        if (mListName.isEmpty()) {
            RB_DEBUG->error("RB_MmSource::setRoot(root) "
                            "list name is empty ERROR");
        }

        // Normal in-memory update of root for table and tree model
        mRoot = root;
    }
}

/**
 * Set root, used to set root for widget on other MDI-, dock- or dialog window
 * @param index current index of parent model
 */
void RB_MmSource::setRoot(const QModelIndex& index) {
    if (/* !isTreeModel() && */ database().isOpen()) {
        // database table and tree
        if (index.isValid()) {
            const RB_MmSource* sourceModel = qobject_cast<const RB_MmSource*>(index.model());
            RB_Variant varId = sourceModel->hiddenData(
                    sourceModel->index(index.row(), 0, index.parent()));
            setRoot(varId.toString());
        } else {
            RB_DEBUG->print(RB_Debug::D_INFORMATIONAL,
                            "RB_MmSource::setRoot(QModelIndex()) "
                            "index not valid INFO");
            setRoot("NULL"); // show nothing
        }
    } else {
        // in-memory and/or tree model
        if (index.isValid()) {
            RB_ObjectBase* obj = static_cast<RB_ObjectBase*>(
                                            index.internalPointer());
            setRoot(obj);
        } else {
            RB_DEBUG->print(RB_Debug::D_WARNING,
                            "RB_MmSource::setRoot(QModelIndex()) "
                            "index not valid WARNING");
            setRoot();
        }
    }
}

/**
 * @return root object
 */
RB_ObjectBase* RB_MmSource::getRoot() {
    // RB_DEBUG->print("RB_MmSource::getRoot()");
    return mRoot;
}

/**
 * Set model based on root object and list. Is used when a model
 * does not need to be cloned. For in-memory only.
 * @param root root object usually a RB_ObjectContainer, is null if no root
 * object is selected
 * @param list name of the list for example 'EQL_TypeSectionList'
 */
bool RB_MmSource::setTableModel(const RB_String& list) {
    RB_DEBUG->print("RB_MmSource::setTableModel(list, pMm)");

    if (list == "" || !mObjectFactory ) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "RB_MmSource::setModel() object NULL ERROR");
        return false;
    }

    mListName = list;
    RB_String tName = list;
    mTableName = tName.remove("List");
    setTable(mTableName.toLower()); // database model only

    // refer also to setRoot()
    if (!mRoot) {
        if (!isTreeModel() && database().isOpen()) {
            mRoot = mObjectFactory->newParentObject(mTableName);
        } else if (database().isOpen()) {
            // in tree model the parent is always the same type as the tree objects
            // make sure that the type as a container with same types in objectfactory
            mRoot = mObjectFactory->newSingleObject(mTableName);
        }
        if (!mRoot) {
            // Only at creation of this object manager
            // Set dummy root object XXX_Project with new uuid
            // If possible do not use other than for top level model,
            // use actual root object because always a XXX_Project is created
            mRoot = mObjectFactory->newObject("", NULL, "", true);

            RB_DEBUG->warning("RB_MmSource::setTableModel() mRoot is NULL "
                              " or no database connection NOTICE");
        }
    }


    if (mObject) {
        delete mObject;
        mObject = NULL;
    }

    mObject = mObjectFactory->newSingleObject(mTableName);

    if (mObject) {
        mNoColumns = mObject->memberCount();
    } else {
        mNoColumns = 0;
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "RB_MmSource::setListName() object ERROR");
    }

    return true;
}

/**
 * TODO: Set model based on root object, list and parent model. Is used when a model
 * does needs to be cloned or be able to cancel edited items. Overwrite this
 * function/method if complex lists are to be created.
 * @param root root object usually a RB_ObjectContainer. Root is NULL if no
 * object is selected which results in an error
 * @param list name of the list for example 'EQL_TypeSectionList'
 */
bool RB_MmSource::setCloneModel(RB_ObjectBase* root, const RB_String& list) {
    RB_DEBUG->print("RB_MmSource::setCloneModel(root, list)");

    if (!root || list == "" || !mObjectFactory) {
        RB_DEBUG->print("RB_MmSource::setCloneModel() "
                                "object factory or list not set ERROR");
        return false;
    }

    // make sure this mRoot is deleted
    if (mRoot != NULL) {
        delete mRoot;
        mRoot = NULL;
    }

    // prevent child models to delete none existing model root and/or lists
    setBaseCloneModel(true);
    mListName = list;

    // the cloned root object keeps a reference to the original
    mRoot = (RB_ObjectContainer*)root->clone(RB2::ResolveNone);
    RB_ObjectContainer* objC = root->getContainer(mListName);

    if (!objC) {
        RB_DEBUG->print(QString("RB_MmSource::setCloneModel(root, list, pMm) "
                                "list %s not found ERROR")
                        .arg(mListName.toStdString().c_str()));
        delete mRoot;
        mRoot = NULL;
        return false;
    }

    return true;
}


/**
 * TODO: Save this cloned model by exchanging the objects with the original objects and
 * remove the old and setting the original root object with data of this cloned
 * model root. Overwrite this function in case of compiled modelListAll.
 */
bool RB_MmSource::saveCloneModel() {
    RB_DEBUG->print("RB_MmSource::saveCloneModel()");

    if (!mRoot || !mRoot->isClone()) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "RB_MmSource::saveCloneModel() mRoot ERROR");
        return false;
    }

    RB_ObjectBase* root = (RB_ObjectContainer*)mRoot->getOriginal();
    *root = *mRoot; // exchange member values not the pointer!
    // mRoot is deleted in destructor of this class


/*	TODO: revisit
    // old approach, problem was that by root->addObject() the pointer
    // is changed. This gives errors in an original modelManager which is
    // the same as this cloned mManager. The pointer to modelListAll
    // becomes invalid in the original mManager. 2007-06-09

    RB_ObjectContainer* tmpList = root->getContainer(mListName);
    bool deleteObj = true;
    root->remove(tmpList, deleteObj);

    modelListAll->setCloned(false);
    root->addObject(modelListAll);
    modelListAll = NULL;

    modelIsModified(true);
*/
    // copy possible modified members only
    RB_ObjectContainer* orgList = root->getContainer(mListName);
    RB_ObjectContainer* modelList = mRoot->getContainer(mListName);
    *orgList = *modelList;

    // remove pointers and delete the objects from original list
    orgList->erase();

    RB_ObjectIterator* iter = modelList->createIterator();
    for (iter->first(); !iter->isDone(); iter->next()) {
        // (RB_ObjectBase*) reparent of children not necessary
        ((RB_ObjectBase*)iter->currentObject())->setParent(orgList);
        // setting 'cloned' = false also of children
        iter->currentObject()->setCloned(false);
        // add to orgList container
        orgList->addObject(iter->currentObject());
    }
    delete iter;

    // remove pointers from cloned modelList, but do not delete objects
    // which are now part of original list, modelList will be deleted
    // in the destructor of this class
    modelList->clear();

    return true;
}

/**
 * Get object, used for in-memory and for non-modelview objects
 * such as drawings. Tree models and in-memory models return actual object,
 * database table model returns the helper object. Do not delete after use.
 * @param idx index
 * @return object at index row
 */
RB_ObjectBase* RB_MmSource::getObject(const QModelIndex& currentIndex,
                                      RB2::ResolveLevel level) {
    if (!currentIndex.isValid()) {
        RB_DEBUG->error("RB_MmSource::getObject() index invalid ERROR");
        return NULL;
    }

    // In-memory model
    RB_ObjectBase* obj = static_cast<RB_ObjectBase*>(currentIndex.internalPointer());

    if (obj) {
        return obj;
    }

    // QSqlRelationalTableModel
    int row = currentIndex.row();
    int colCount = columnCount();
    RB_Variant var;

    if (level == RB2::ResolveNone) {
        // Set data from current model
        QModelIndex idx;

        for (int col = 0; col < colCount; ++col) {
            idx = this->index(row, col, currentIndex.parent());
            RB_ObjectMember* mem = mObject->getMember(col);

            if (!mem) {
                RB_DEBUG->error("RB_MmSource::getObject() member col = "
                                + QString::number(col) + " ERROR");
                continue;
            }

            var = data(idx, Qt::EditRole);
            mObject->setValue(col, var);
        }

        mObject->deleteFlag(RB2::FlagIsDirty);
    } else {
        // Children are to be added because of resolve level,
        // only possible by reading from database
        // database table model, TODO table model does not have hidden data anymore
        QModelIndex idx = this->index(row, 0, currentIndex.parent());
        RB_String id = hiddenData(idx, Qt::EditRole).toString();
        // Alternative: RB_String id = index(idx.row(), 0, idx.parent()).data().toString();
        mObject->eraseChildren();
        mObject->setId(id);
        mObject->dbRead(database(), level);
    }


    return mObject;
}

RB_ObjectBase *RB_MmSource::getBaseObject() {
    return mObject;
}

RB_ObjectBase* RB_MmSource::getCurrentObject() {
    if (!mCurrentIndex.isValid()) {
        return NULL;
    }

    return getObject(mCurrentIndex, RB2::ResolveNone);

    // test only
//    for (int i = 0; i < colCount; ++i) {
//        RB_DEBUG->print(headerData(i, Qt::Horizontal).toString());
//    }

}

/**
 * Update current object. Works only if model current object and
 * obj have the same ID.
 * @brief RB_MmSource::updateCurrentObject
 * @param obj
 */
void RB_MmSource::updateCurrentObject(RB_ObjectBase* obj) {
    if (!obj || !obj->getFlag(RB2::FlagIsDirty)) {
        return;
    }

    QModelIndex idx = getCurrentIndex();
    int row = idx.row();
    idx = index(row, 0, idx.parent()); // id

    if (idx.data().toString() != obj->getId()) {
        RB_DEBUG->error("RB_MmSource::updateCurrentObject() ERROR");
        return;
    }

    QString fieldName = "";
    int colCount = columnCount();

    for (int col = RB2::HIDDENCOLUMNS; col < colCount; ++col) {
        RB_ObjectMember* mem = mObject->getMember(col);

        if (!mem) {
            RB_DEBUG->error("RB_MmSource::updateCurrentObject() member ERROR");
            continue;
        }

        fieldName = mem->getName();
        idx = index(row, fieldIndex(fieldName), idx.parent());
        RB_String currentStr = mem->getValue().toString();
        RB_String previousStr = mem->getPreviousValue().toString();

        if (currentStr != previousStr) {
            setData(idx, mem->getValue());
        }
    }
}

/**
 * Relocate an object (row) to another position in the model,
 * @param fromIndex from_index or row
 * @param toIndex to_index or row
 */
void RB_MmSource::swapObject(const QModelIndex& fromIndex,
                             const QModelIndex& toIndex,
                             const QModelIndex& parent) {
    if (toIndex.row() < 0 || toIndex.row() >= rowCount()) {
        RB_DEBUG->print(RB_Debug::D_INFORMATIONAL,
                        "RB_MmSource::swapObject() row out of bounds INFO");
        return;
    }

    // RB_DEBUG->print("RB_MmSource::swapObject()");
    if (database().isOpen()) {
        QModelIndex fIndex = index(fromIndex.row(), 0, parent);
        QModelIndex tIndex = index(toIndex.row(), 0, parent);

        RB_String fromId = data(fIndex, Qt::DisplayRole).toString();
        RB_String toId = data(tIndex, Qt::DisplayRole).toString();

        // Select the data
        // Create objects with the data
        // Switch object data
        RB_ObjectBase* fromObj = mObjectFactory->newSingleObject(mTableName);
        RB_ObjectBase* toObj = mObjectFactory->newSingleObject(mTableName);
        RB_String fromParentId = "";
        RB_String toParentId = "";

        RB_String qStr = "SELECT * FROM " + mTableName
                         + " WHERE id='" + fromId + "';";

        QSqlQuery query;
        if (database().isOpen()) {
            // database table model
            query = QSqlQuery(database());
        }

        if (query.exec(qStr)) {
            query.first();
            QSqlRecord record = query.record();
            fromObj->setId(record.value("id").toString());
            fromParentId = record.value("parent").toString();
            fromObj->setName(record.value("name").toString());
            for (int i = 3; i < record.count(); ++i) {
                fromObj->setValue(record.fieldName(i), record.value(i));
            }
        }

        qStr = "SELECT * FROM " + mTableName + " WHERE id='" + toId + "';";
        query.clear();

        if (query.exec(qStr)) {
            query.first();
            QSqlRecord record = query.record();
            toObj->setId(record.value("id").toString());
            toParentId = record.value("parent").toString();
            toObj->setName(record.value("name").toString());
            for (int i = 3; i < record.count(); ++i) {
                toObj->setValue(record.fieldName(i), record.value(i));
            }
        }

        // Update 'to row' in database,
        // (do not use query otherwise model cache will not be updated)
        QModelIndex idx = index(toIndex.row(), 0, parent);
        setData(idx, fromObj->getId() + "_", Qt::EditRole);
        idx = index(toIndex.row(), 1, parent);
        setData(idx, fromParentId, Qt::EditRole);
        idx = index(toIndex.row(), 2, parent);
        setData(idx, fromObj->getName(), Qt::EditRole);
        for (int i = 0; i < fromObj->memberCount(); ++i) {
            idx = index(toIndex.row(), i + 3, parent);
            setData(idx, fromObj->getValue(i), Qt::EditRole);
        }

        // Update 'from row' in database,
        idx = index(fromIndex.row(), 0, parent);
        setData(idx, toObj->getId(), Qt::EditRole);
        idx = index(fromIndex.row(), 1, parent);
        setData(idx, toParentId, Qt::EditRole);
        idx = index(fromIndex.row(), 2, parent);
        setData(idx, toObj->getName(), Qt::EditRole);
        for (int i = 0; i < fromObj->memberCount(); ++i) {
            idx = index(fromIndex.row(), i + 3, parent);
            setData(idx, toObj->getValue(i), Qt::EditRole);
        }

        // Set correct 'from id'
        idx = index(toIndex.row(), 0, parent);
        setData(idx, fromObj->getId(), Qt::EditRole);

        delete fromObj;
        delete toObj;

        if (!isTreeModel() && database().isOpen()) {
            submitAll(); // TODO: required?
        }

        /*-- update or else insert statement, might not work with SQLite
        IF (EXISTS (SELECT * FROM AA_TestTable AS t1
          WHERE t1.ord_num = 'FFF'))
        begin
          UPDATE AA_TestTable
          SET ord_qty = 999
          WHERE ord_num = 'FFF'
        end
        else
        begin
          INSERT INTO AA_TestTable (ord_num, top_assy, ord_qty)
          VALUES('FFF', 'XYZ', 567)
        end
        */
    } else {
        // in-memory model
        RB_ObjectContainer* tmpParent;
        if (parent.isValid()) {
            tmpParent = static_cast<RB_ObjectContainer*>(parent.internalPointer());
        } else if (mRoot) {
            tmpParent = dynamic_cast<RB_ObjectContainer*>(mRoot);
        } else {
            return;
        }

        // get named container with corresponding child at row
        RB_ObjectContainer* objC = tmpParent->getContainer(mListName);
        RB_ObjectBase* objFrom = static_cast<RB_ObjectBase*>(fromIndex.internalPointer());
        RB_ObjectBase* objTo = static_cast<RB_ObjectBase*>(toIndex.internalPointer());
        objC->swapObject(objFrom, objTo);

    }

    QModelIndex fIndex;
    QModelIndex tIndex;
    if (fromIndex.row() < toIndex.row()) {
        fIndex = index(fromIndex.row(), 0, parent);
        tIndex = index(toIndex.row(), columnCount()-1, parent);
    } else {
        fIndex = index(toIndex.row(), 0, parent);
        tIndex = index(fromIndex.row(), columnCount()-1, parent);
    }

    emit dataChanged(fIndex, tIndex);
}

/**
 * Promote item to grandparent in tree model. The change is directly
 * updated in the database, no signal is emitted with 'dataChanged()'
 * @param idx index of item
 * @returns index of item new position
 */
QModelIndex RB_MmSource::promote(const QModelIndex& idx) {
    if (!isTreeModel() || !idx.isValid()) return idx;
    QModelIndex parentIdx = idx.parent();
    if (parentIdx == QModelIndex()) return idx;

    QModelIndex grParentIdx = parentIdx.parent();
    RB_ObjectBase* grpObj = NULL;
    if (grParentIdx == QModelIndex()) {
        grpObj = getRoot();
    } else {
        grpObj = static_cast<RB_ObjectBase*>(grParentIdx.internalPointer());
    }

    beginMoveRows(parentIdx, idx.row(), idx.row(),
                  grParentIdx, rowCount(grParentIdx));
    RB_ObjectBase* obj = static_cast<RB_ObjectBase*>(idx.internalPointer());
    obj->detachFromParent();
    obj->setFlag(RB2::FlagIsDirty);
    RB_ObjectContainer* grpList = grpObj->getContainer(obj->getName() + "List");
    grpList->addObject(obj);
    int rowCount = grpList->objectCount();
    createIndex(rowCount - 1, 0, obj); // only to create new index in index list
    endMoveRows();

    obj->dbUpdate(database(), RB2::ResolveNone);

    return index(rowCount - 1, 0, grParentIdx); // col=0 is first visible in tree
}

/**
 * Demote item to new parent (should be a sibling of item)
 * @param idx index of item
 * @param parentIdx index of new parent
 * @returns index of item new position
 */
QModelIndex RB_MmSource::demote(const QModelIndex& idx, const RB_String& parentId) {
    if (!isTreeModel() || !idx.isValid()) return idx;

    RB_ObjectContainer* obj = static_cast<RB_ObjectContainer*>(idx.internalPointer());
    RB_ObjectContainer* pObj = obj->getGrandParent();
    if (!obj || !pObj) {
        RB_DEBUG->error("RB_MmSource::demote(idx, parentId) 1 ERROR");
        return QModelIndex();
    }
    RB_ObjectContainer* pList = pObj->getContainer(obj->getName() + "List");
    if (!pList) {
        RB_DEBUG->error("RB_MmSource::demote(idx, parentId) 2 ERROR");
        return QModelIndex();
    }
    RB_ObjectBase* sibling = pList->getObject(parentId);
    if (!sibling) {
        RB_DEBUG->error("RB_MmSource::demote(idx, parentId) 3 ERROR");
        return QModelIndex();
    }
    QModelIndex parentIdx = idx.parent();
    QModelIndex siblIdx = index(pList->getRow(sibling), 0, parentIdx);

    beginMoveRows(parentIdx, idx.row(), idx.row(),
                  siblIdx, rowCount(siblIdx));
    obj->detachFromParent();
    obj->setFlag(RB2::FlagIsDirty);
    RB_ObjectContainer* sList = sibling->getContainer(sibling->getName() + "List");
    sList->addObject(obj);
    int rowCount = sList->objectCount();
    createIndex(rowCount - 1, 0, obj); // only to create index in index list
    endMoveRows();

    obj->dbUpdate(database(), RB2::ResolveNone);

    return index(rowCount - 1, 0, siblIdx); // col=0 is first visible in tree
}

/**
 * @return index of current child of parent
 */
QModelIndex RB_MmSource::index(int row, int column,
                                        const QModelIndex& parent) const {
    // RB_DEBUG->print("RB_MmSource::index()");

    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    if (!isTreeModel() && database().isOpen()) {
        return QSqlRelationalTableModel::index(row, column, parent);
    }

    if (!mRoot) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "RB_MmSource::index() root is NULL ERROR");
        return QModelIndex();
    }

    RB_ObjectBase* tmpParent = mRoot;

    if (parent.isValid()) {
        tmpParent = static_cast<RB_ObjectContainer*>(parent.internalPointer());
    }

    // get named container with corresponding child at row
    RB_ObjectContainer* objC = tmpParent->getContainer(mListName);
    RB_ObjectBase* child = objC->getObject(row);

    if (child) {
        return createIndex(row, column, child);
    }

    return QModelIndex();
}

/**
 * @return index of parent used for tree view (otherwise QModelIndex())
 */
QModelIndex RB_MmSource::parent(const QModelIndex& index) const {
    // RB_DEBUG->print("RB_MmSource::parent()");
    if (!index.isValid() || (!isTreeModel() && database().isOpen())) {
        return QModelIndex();
    }

    RB_ObjectBase* child = static_cast<RB_ObjectBase*>(index.internalPointer());
    if (!child) return QModelIndex();

    RB_ObjectBase* parent = child->getGrandParent(); // container, parent
    int row = 0;

    if (parent == mRoot || !parent) {
        return QModelIndex();
    } else {
        // get list container of parent
        RB_ObjectContainer* objC = (RB_ObjectContainer*)parent->getParent();
        row = objC->getRow(parent);
    }

    return createIndex(row, 0, parent);
}

/**
 * @return true if parent has any children
 */
bool RB_MmSource::hasChildren(const QModelIndex& parent) const {
    // RB_DEBUG->print("RB_MmSource::hasChildren()");
    if (/*isTreeModel() &&*/ rowCount(parent) > 0) {
        return true;
    }

    return false;
}

/**
 * @return the number of rows in the model or child objects in a tree model.
 */
int RB_MmSource::rowCount(const QModelIndex& parent) const {
//    RB_DEBUG->print("RB_MmSource::rowCount()");

    // Database table model
    if (!isTreeModel() && database().isOpen()) {
        return QSqlRelationalTableModel::rowCount(parent);
    }

    // Tree model or in-memory model
    RB_ObjectBase* obj = mRoot;

    if (!obj) {
        return 0;
    }

    RB_ObjectContainer* objC = obj->getContainer(mListName);

    if (!objC) {
        return 0;
    }

    if (!parent.isValid()) {
        // invalid QModelIndex is root, thus all rows in table or root tree
        int count = objC->objectCount();
        return count;
    } else if (isTreeModel()) {
        // child in tree
        obj = static_cast<RB_ObjectContainer*>(parent.internalPointer());
        objC = obj->getContainer(mListName);
    }

    // Tree or in-memory model
    if (obj->getFlag(RB2::FlagChildrenCreated)) {
        int count = objC->objectCount();
        return count;
    }

    // Should never continue here?
    if (obj->getGrandParent()
        && obj->getGrandParent()->getFlag(RB2::FlagChildrenCreated)) {
        return objC->objectCount();
    }

    if (obj->getGrandParent()
        && obj->getGrandParent()->getGrandParent()
        && obj->getGrandParent()->getGrandParent()->getFlag(RB2::FlagChildrenCreated)) {
        return objC->objectCount();
    }

    if (obj->getGrandParent()
        && obj->getGrandParent()->getGrandParent()
        && obj->getGrandParent()->getGrandParent()->getGrandParent()
        && obj->getGrandParent()->getGrandParent()->getGrandParent()->getFlag(RB2::FlagChildrenCreated)) {
        return objC->objectCount();
    }

    if (objC) {
        // HACK: benefit of the doubt always shows a [+]
        // TODO: improve refer also to createSubTree() already creating grandchildren
        return 1;
    }

    return 0;
}

/**
 * Number of columns in the model, to be overloaded by the relevant number
 * of the actual model.
 * @return the number of columns
 */
int RB_MmSource::columnCount(const QModelIndex& parent) const {
    // RB_DEBUG->print("RB_MmSource::columnCount()");
    if (!isTreeModel() && database().isOpen()) {
        return QSqlRelationalTableModel::columnCount(parent) - mHiddenColumns;
    }

    return mNoColumns - mHiddenColumns;
}

/**
 * Model data (RB_Variant) at 'column' and 'row'
 * @param index
 * @param role, display role, edit role, RB2::RoleOrigData (= Qt::UserRole + 1)
 * @return data
 */
RB_Variant RB_MmSource::data(const QModelIndex& index, int role) const {
    // RB_DEBUG->print("RB_MmSource::data()");
    if (!index.isValid()) return QVariant();

    if (!isTreeModel() && database().isOpen()) {
        return hiddenData(index, role);
    }

    // Qt namespace reference Qt::ItemDataRole for example Qt::DisplayRole
    QModelIndex idx = createIndex(index.row(), index.column() + mHiddenColumns,
                                  index.internalPointer());
    return hiddenData(idx, role);
}

/**
 * Model hidden data (RB_Variant) at 'column' and 'row', such as id, parent, name
 * Handles the fields ending with '_idx', these fields contain the id (Uuid)
 * and the text in one string. Used when the relational table is larger than
 * can be reasonably handled with a combobox.
 *
 * If taxprovince_idx field in acc_project has rows with:
 * {7c6d6bb5-9308-4440-930a-86ff36d90e21}Nederland
 * {7c6d6bb5-9308-4440-930a-86ff36d90e21}Belgie
 *
 * SELECT SUBSTR(taxprovince_idx, 39) as ID
 * FROM acc_project
 * WHERE SUBSTR(taxprovince_idx, 1, 38)='{7c6d6bb5-9308-4440-930a-86ff36d90e21}';
 *
 * returns:
 *    ID
 * Nederland
 * Belgie
 *
 * Qt::DisplayRole:     Nederland
 * Qt::EditRole:        {7c6d6bb5-9308-4440-930a-86ff36d90e21}Nederland
 * RB2::RoleOrigData:   {7c6d6bb5-9308-4440-930a-86ff36d90e21}
 *
 * @param index
 * @param role, display role, edit role, etc.
 * @return data
 */
RB_Variant RB_MmSource::hiddenData(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    if (role != Qt::DisplayRole && role != Qt::EditRole
            && role != Qt::TextAlignmentRole && role != Qt::BackgroundRole
            && role != RB2::RoleOrigData) {
        return QVariant();
    }

    if (!isTreeModel() && database().isOpen()) {
        return hiddenTableData(index, role);
    }

    return hiddenTreeData(index, role);
}

/**
 * Get hidden table data
 * \todo: make clear and clean-up display, edit and original data role
 * @param index model index
 * @param role model role
 * @return QVariant
 */
RB_Variant RB_MmSource::hiddenTableData(const QModelIndex& index, int role) const {
    // Note 1: Qt relational model does not have a mObject:
    // Note 2: Next code assumes database is always inline with objectmodel
    RB_String fieldName = "";
    bool isIntegerField = false;

    if (mObject && mObject->memberCount() > index.column()) {
        fieldName = mObject->getMember(index.column())->getName();
        isIntegerField = (mObject->getMember(index.column())->getType()
                == RB2::MemberInteger);
    }

    if (role == Qt::DisplayRole) {
        if (fieldName.endsWith("_idx")) {
            // example: {2deac140-ffb4-11df-a976-0800200c9a66}Debtor 8-4-4-4-12
            RB_String str = QSqlRelationalTableModel::data(index, role).toString();
            // remove the Uuid part including the curly braces
            // Still necessary? refer to edit role
            str.remove(0, 38);
            return str;
        } else if (getDisplayRole(index.column()) == RB2::AlignFinancialAmount) {
            double amount = QSqlRelationalTableModel::data(index, role).toDouble();
            return RB_String::number(amount, 'f', 2);
        } else if (isIntegerField) {
            RB_StringList strL = getTextList(index.column());
            if (strL.count() > 0) {
                return strL.at(QSqlRelationalTableModel::data(index, role).toInt());
            }
        }

        return QSqlRelationalTableModel::data(index, role);
    } else if (role == Qt::EditRole) {

        // TODO: is this necessary or can Qt::EditRole take the RB2_RoleOrigDat role
        if (fieldName.endsWith("_idx")) {
            // example: {2deac140-ffb4-11df-a976-0800200c9a66}Debitor 8-4-4-4-12
            RB_String str = QSqlRelationalTableModel::data(index, role).toString();
            // Remove the Uuid part including the curly braces, not required
            // anymore. This is done where necessary by the _idx delegate
            // str.remove(0, 38);
            return str;
        } else if (getDisplayRole(index.column()) == RB2::AlignFinancialAmount) {
            double amount = QSqlRelationalTableModel::data(index, role).toDouble();
            return RB_String::number(amount, 'f', 2);
        }

        return QSqlRelationalTableModel::data(index, role);
    } else if (role == Qt::TextAlignmentRole) {
        int displayRole = getDisplayRole(index.column());

        if (displayRole == RB2::AlignCenter) {
            return Qt::AlignCenter; // both horizontal and vertical
        } else if (displayRole == RB2::AlignRight
                   || displayRole == RB2::AlignFinancialAmount) {
            return (int)(Qt::AlignRight | Qt::AlignVCenter);
        }
    } else if(role == RB2::RoleOrigData) {
        if (fieldName.endsWith("_idx")) {
            RB_String str = QSqlRelationalTableModel::data(index, Qt::EditRole).toString();
            str.remove(38, str.length()); // remove the part after the Uuid
            return str;
        } else if (isIntegerField) {
            return QSqlRelationalTableModel::data(index, Qt::EditRole);
        }

        RB_Variant displVar = QSqlRelationalTableModel::data(index, Qt::DisplayRole);
        QSqlTableModel* relModel = QSqlRelationalTableModel::relationModel(index.column());

        if (!relModel) {
            return displVar;
        }

        // find first value in relational model
        QSqlRelation relation = QSqlRelationalTableModel::relation(index.column());
        QModelIndex relIdx = relModel->index(0, relModel->fieldIndex(relation.displayColumn()));
        QModelIndexList matchList = relModel->match(relIdx, Qt::DisplayRole, displVar);

        if (matchList.count() <= 0) {
            return displVar;
        }

        QModelIndex matchIdx = matchList.at(0);
        QModelIndex editIdx = relModel->index(matchIdx.row(),
                             relModel->fieldIndex(relation.indexColumn()));
        return editIdx.data();
    }

    return QSqlRelationalTableModel::data(index, role);
}

/**
 * Get hidden tree data
 * @param index model index
 * @param role model role
 * @return QVariant
 */
RB_Variant RB_MmSource::hiddenTreeData(const QModelIndex& index, int role) const {
    // Qt namespace reference Qt::ItemDataRole for example Qt::DisplayRole
    if (role == Qt::DisplayRole || role == Qt::EditRole
            || role == RB2::RoleOrigData) {
        return objectData(index, role);
    } else if (role == Qt::BackgroundRole && index.column() == 3 /* 3 is example
         && mIsDeleted == true, status, etc. */) {
        // background color
        // return QBrush(QColor(mLightYellow));
        return QVariant();
    } else if (role == Qt::TextAlignmentRole) {
        int displayRole = getDisplayRole(index.column());

        if (displayRole == RB2::AlignCenter) {
            return Qt::AlignCenter; // both horizontal and vertical
        } else if (displayRole == RB2::AlignRight
                   || displayRole == RB2::AlignFinancialAmount) {
            return (int)(Qt::AlignRight | Qt::AlignVCenter);
        }
    } else if (role == Qt::UserRole && index.column() == 3 /* 3 is example */) {
        // TODO: style sheet background for widgets, used in RB_DataWidgetMapper
        // return "background: " + mLightYellow + ";";
        return QVariant();
    }

    return QVariant();
}

/**
 * @returns RB2::RoleType
 */
int RB_MmSource::getDisplayRole(int col) const {
    int displayRole = -1;
    std::map<int, int>::const_iterator iter;
    iter = mRoleMap.find(col);

    if (iter != mRoleMap.end()) {
        displayRole = iter->second;
    }
    return displayRole;
}

/**
 * Set display role for column
 * @param col column
 * @param role RB2::RoleType
 */
void RB_MmSource::setDisplayRole(int col, int role) {
    mRoleMap[col] = role;
}

/**
 * @returns text list in case of fixed texts in a combobox
 */
RB_StringList RB_MmSource::getTextList(int col) const {
    RB_StringList strL;
    std::map<int, RB_StringList>::const_iterator iter;
    iter = mIntegerToTextList.find(col);

    if (iter != mIntegerToTextList.end()) {
        strL = iter->second;
    }
    return strL;
}

/**
 * Set display role for column
 * @param col column
 * @param strL list with texts from the combobox
 */
void RB_MmSource::setTextList(int col, const RB_StringList& strL) {
    int actualCol = col;
    if (isTreeModel()) {
        actualCol = col + RB2::HIDDENCOLUMNS;
    }
    mIntegerToTextList[actualCol] = strL;
}


/**
 * Set data in model at index (row and column)
 * @param index Index contains the row and column of the value to be set
 * @param value Value to be set at index
 * @param role Role type such as EditRole
 * @return success of setting the data in the model
 */
bool RB_MmSource::setData(const QModelIndex &index,
                          const QVariant &value, int role) {
    if (!index.isValid()) {
        return false;
    }

    if (role != Qt::EditRole && role != RB2::RoleOrigData) {
        return true;
    }

    // Check if data is changed, otherwise do nothing
    RB_String fieldName = "";
    if (mObject && mObject->memberCount() > index.column()) {
        fieldName = mObject->getMember(index.column())->getName();
    }

    if (fieldName.endsWith("_idx")) {
        // below after || to capture CTRL+C -> editingFinished() signal readonly lineEdit
        if (((this->data(index, RB2::RoleOrigData).toString()
              + this->data(index, Qt::DisplayRole).toString()) == value.toString())
                || this->data(index, Qt::DisplayRole).toString() == value.toString()) {
            return true;
        }
    } else if (fieldName.endsWith("_id")) {
        if (this->data(index, RB2::RoleOrigData) == value) {
            return true;
        }
    } else if (!isTreeModel() && database().isOpen()) {
        RB_Variant dbValue = QSqlRelationalTableModel::data(index, Qt::EditRole);
        if (dbValue == value) {
            return true;
        }
    } else {
        if (this->data(index, role) == value) {
            return true;
        }
    }

    // Data is changed
    RB_SETTINGS->beginGroup("database");
    RB_String username = RB_SETTINGS->value("username", "unknown").toString();
    RB_SETTINGS->endGroup();
    bool success = false;
    QModelIndex idx;

    if (!isTreeModel() && database().isOpen()) {
        success = QSqlRelationalTableModel::setData(index, value, Qt::EditRole);

        if (!success) {
            printErrorSetData(index, value, role);
            return success;
        }

        RB_String dt = QDateTime::currentDateTime().toString(Qt::ISODate);
        idx = createIndex(index.row(), 5);
        success = QSqlRelationalTableModel::setData(idx, dt, Qt::EditRole);
        if (!success) { printErrorSetData(index, value, role); }

        idx = createIndex(index.row(), 6);
        success = QSqlRelationalTableModel::setData(idx, username, Qt::EditRole);
        if (!success) { printErrorSetData(index, value, role); }
    } else {
        void* obj = index.internalPointer();
        idx = createIndex(index.row(), index.column() + mHiddenColumns, obj);
        success = setHiddenData(idx, value, role);

        if (!success) {
            printErrorSetData(index, value, role);
            return success;
        }

        RB_String dt = QDateTime::currentDateTime().toString(Qt::ISODate);
        idx = createIndex(index.row(), 5, obj);
        success = setHiddenData(idx, dt, role);
        if (!success) { printErrorSetData(index, value, role); }

        idx = createIndex(index.row(), 6, obj);
        success = setHiddenData(idx, username, role);
        if (!success) { printErrorSetData(index, value, role); }

        emit dataChanged(index, index);
    }

    if (success) {
        setModelIsModified(true);
    }

    return success;
}

void RB_MmSource::printErrorSetData(const QModelIndex& index,
                                    const QVariant& value,
                                    int role) {
    RB_DEBUG->print("RB_MmSource::printSetDataError() row, col = "
                    + QString::number(index.row()) + ", "
                    + QString::number(index.column()));
    RB_DEBUG->print("RB_MmSource::printSetDataError() data(displayRole) = "
                    + index.data(Qt::DisplayRole).toString());
    RB_DEBUG->print("RB_MmSource::printSetDataError() data(editRole) = "
                    + index.data(Qt::EditRole).toString());
    RB_DEBUG->print("RB_MmSource::printSetDataError() value = "
                    + value.toString());
    RB_DEBUG->print("RB_MmSource::printSetDataError() role = "
                    + QString::number(role));
    RB_DEBUG->error("RB_MmSource::printSetDataError() ERROR :  "
                    + this->lastError().text());
}

/**
 * Set hidden data in model at index (row and column), such as id, parent, name
 * @param index Index contains the row and column of the value to be set
 * @param value Value to be set at index
 * @param role Role type such as EditRole
 * @return success of setting the data in the model
 */
bool RB_MmSource::setHiddenData(const QModelIndex &index,
                          const QVariant &value, int role) {
    if (!index.isValid()) {
        return false;
    }

    if (role != Qt::EditRole && role != RB2::RoleOrigData) {
        return true;
    }

    bool success = false;

    if (!isTreeModel() && database().isOpen()) {
        success = QSqlRelationalTableModel::setData(index, value, role);
    } else  {
        success = setObjectData(index, value, role);
    }

    return success;
}

/**
 * Get header data for horizontal only. Row header gives only row number.
 * Function to be overloaded to show the relevant header data.
 * @return row and column header data
 */
RB_Variant RB_MmSource::headerData(int section, Qt::Orientation orientation,
                                    int role) const {
    int sect = section + mHiddenColumns;

    if (!isTreeModel() && database().isOpen()) {
        return QSqlRelationalTableModel::headerData(sect, orientation, role);
    }

    if (role != Qt::DisplayRole) return QVariant();

    // Header data has been set in implementation
    if (orientation == Qt::Horizontal &&
            RB_MmAbstract::headerData(sect, orientation, role).toInt() != sect + 1) {
        return RB_MmAbstract::headerData(sect, orientation, role);
    }

    // Field names (instead of Qt default column number starting with 1)
    if ((isTreeModel() || !database().isOpen()) && orientation == Qt::Horizontal) {
        if (mObject && sect >= 0
                && mObject->memberCount() > sect) {
            return mObject->getMember(sect)->getName();
        }

        return RB_String("Col %1").arg(sect + 1);
    }

    // Vertical header
    return QString("%1").arg(sect + 1);
}

/**
 * Insert count rows at position row
 * @param row row number after which the rows are to be inserted
 * @param count number of rows to be inserted
 * @param parent parent model index
 * @return success of insertion of rows
 */
bool RB_MmSource::insertRows(int row, int count,
                                    const QModelIndex& parent) {
    // RB_DEBUG->print("RB_MmSource::insertRows()");
    RB_SETTINGS->beginGroup("database");
    RB_String username = RB_SETTINGS->value("username", "unknown").toString();
    RB_SETTINGS->endGroup();
    bool success = false;

    if (!isTreeModel() && database().isOpen()) {
        success = true;

        for (int i = 0; i < count && success; ++i) {
            success = RB_MmAbstract::insertRows(row + i, 1, parent);
            setHiddenData(index(row + i, 0), RB_Uuid::createUuid().toString());
            RB_String parentId = mRoot->getId() != "" ? mRoot->getId() : "none";
            setHiddenData(index(row + i, 1), parentId);
            setHiddenData(index(row + i, 2), mTableName);
            setHiddenData(index(row + i, 3), (int)RB2::StatusDefault);
            RB_String dt = QDateTime::currentDateTime().toString(Qt::ISODate);
            setHiddenData(index(row + i, 4), dt);
            setHiddenData(index(row + i, 5), dt);
            setHiddenData(index(row + i, 6), username);

            // HACK: set integer and double fields to 0
            // otherwise mapper does not clear these fields with insertRows()
            if (mObject) {
                RB_ObjectMember* mem;
                int count = mObject->memberCount();

                for (int j = RB2::HIDDENCOLUMNS; j < count; ++j) {
                    mem = mObject->getMember(j);

                    if (mem->getType() == RB2::MemberInteger
                            || mem->getType() == RB2::MemberDouble) {
                        setHiddenData(index(row + i, j), 0);
                    }
                }
            }
        }

        setModelIsModified(true);
        return success;
    }

    beginInsertRows(parent, row, row+count-1);
    for (int i = 0; i < count; ++i) {
        RB_ObjectBase* obj = NULL;
        RB_ObjectContainer* objC = NULL;

        obj = static_cast<RB_ObjectBase*>(parent.internalPointer());

        if (obj) {
            objC = obj->getContainer(mListName);
        } else if (mRoot) {
            obj = mRoot;
            objC = mRoot->getContainer(mListName);
        } else {
            RB_DEBUG->print(RB_Debug::D_ERROR,
                            "RB_MmSource::insertRows() mRoot is NULL ERROR");
            endInsertRows();
            return false;
        }

        // addToParent is set to false to insert the object at the right place
        RB_ObjectBase* newObj = mObjectFactory->newObject(
                RB_Uuid::createUuid().toString(), objC, "", false);

        success = objC->insert(row, newObj);

        newObj->setValue("status", (int)RB2::StatusDefault);
        RB_String dt = QDateTime::currentDateTime().toString(Qt::ISODate);
        newObj->setValue("created", dt);
        newObj->setValue("changed", dt);
        newObj->setValue("muser", username);
        newObj->setFlag(RB2::FlagChildrenCreated);
        newObj->setFlag(RB2::FlagIsDirty);
    }
    endInsertRows();
    setModelIsModified(success);
    emit dataChanged(index(row, 0), index(row + count - 1, 2));
    return success;
}

/**
 * @brief RB_MmSource::copyRow
 * @return true on success
 */
bool RB_MmSource::copyCurrentRow() {
    RB_ObjectBase* obj = getCurrentObject();

    if (!obj) {
        return false;
    }

    QModelIndex idx = QModelIndex();
    QModelIndex parentIdx = mCurrentIndex.parent();

    insertRows(0, 1, parentIdx);
    int count = obj->memberCount();

    for (int i = RB2::HIDDENCOLUMNS; i < count; i++) {
        idx = index(0, i, parentIdx);
        QString fieldName = obj->getMember(i)->getName();

        if (!fieldName.endsWith("_idx")) {
            setData(idx, obj->getValue(i));
        } else {
            setData(idx, obj->getIdValue(1).toString()
                    + obj->getDValue(1).toString());
        }
    }

    QModelIndex copyIdx = index(0, 0, parentIdx);
    QString copyId = copyIdx.data().toString();
    emit currentRowCopied(copyId); // to slotCopyRows(const QString& parentId);
    return true;
}

/**
 * Delete count rows (here only one) at after row,
 * TODO: set status = -1 instead of deleting row
 * @return success of row deletion
 */
bool RB_MmSource::removeRows(int row, int count,
                             const QModelIndex& parent) {
    bool success = false;

    if (!isTreeModel() && database().isOpen()) {

        success = true;

        for (int i = 0; i < count && success; ++i) {
            RB_String delId = hiddenData(index(row, 0), Qt::DisplayRole).toString();

            // Alternative, does not work properly:
            // beginRemoveRows(parent, row, row+count-1);
            // removeRelatedRow(delId);
            // endRemoveRows();

            // Original:
            removeRelatedRow(delId);
            success = RB_MmAbstract::removeRows(row, 1, parent);
        }

        setModelIsModified(true);
        return success;
    }

    // TODO: revisit
    beginRemoveRows(parent, row, row+count-1);
    for (int i = 0; i < count; ++i) {
        // i = multiple rows not yet implemented
        RB_ObjectBase* obj = NULL;
        RB_ObjectContainer* objC = NULL;
        obj = static_cast<RB_ObjectContainer*>(parent.internalPointer());

        if (obj) {
            objC = obj->getContainer(mListName);
        } else  if (mRoot) {
            obj = mRoot;
            objC = mRoot->getContainer(mListName);
        } else {
            RB_DEBUG->print(RB_Debug::D_ERROR,
                            "RB_MmSource::deleteRows() mRoot is NULL ERROR");
            endRemoveRows();
            return false;
        }

        bool deleteObject = false; // will be deleted further down
        RB_ObjectBase* delObj = objC->getObject(row);
        success = objC->remove(delObj, deleteObject);

        if (!success) {
            RB_DEBUG->print(RB_Debug::D_ERROR, "RB_MmSource::removeRows() ERROR");
        }

        // Delete row in database in case of a tree model with database
        if (mIsTreeModel && database().isOpen() && success) {
            delObj->dbRead(database(), RB2::ResolveAll);
            delObj->setFlag(RB2::FlagIsDeleted);
            success = delObj->dbDelete(database());
        }

        if (success) {
            delete delObj;
        } else {
            delObj->deleteFlag(RB2::FlagIsDeleted);
            objC->insert(row, delObj);
        }
    }
    endRemoveRows();
    setModelIsModified(success);
    emit dataChanged(index(row, 0), index(row + count - 1, 2));
    return success;
}

/**
 * Remove rows in other tables that have a relation to this row
 * TODO: set status = -1 instead of deleting row
 * @param id ID of row in this model
 * @param objRel related objects, first call from removeRows() is always NULL
 */
bool RB_MmSource::removeRelatedRow(const RB_String& id) {
    RB_ObjectBase* obj = mObjectFactory->newSingleObject(tableName());

    if (obj->isAtomic()) {
        delete obj;
        return true;
    }

    /*
    obj->setId(id);
    obj->dbRead(database(), RB2::ResolveAll);
    obj->setFlag(RB2::FlagIsDeleted);
    obj->dbDelete(database());
    */

    obj->setId(id);
    obj->dbRead(database(), RB2::ResolveAll);
    RB_ObjectIterator* iter = obj->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* objL = iter->currentObject();
        objL->setFlag(RB2::FlagIsDeleted);
        objL->dbDelete(database());
    }

    delete iter;

    delete obj;
    return true;
}

/**
 * @return flags, such as enabled or editable
 */
Qt::ItemFlags RB_MmSource::flags(const QModelIndex &index) const {
    if (!isTreeModel() && database().isOpen()) {
        return QSqlRelationalTableModel::flags(index);
    }

    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }
    // return Qt::ItemIsEditable | Qt::ItemIsEnabled;
    return (Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable
            |Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled);
}

/**
 * Get the index of the field fieldName
 * @param fieldName field name of the column
 * @returns the index or column number for fieldName
 */
int RB_MmSource::fieldIndex(const QString& fieldName) const {
    if (!isTreeModel() && database().isOpen()) {
        // HACK:
        // next line does not always work because due to relation
        // the field has been renamed to the relation field.
        int col = QSqlRelationalTableModel::fieldIndex(fieldName);
        if (mObject && col < 0) {
            col = mObject->getMemberNo(fieldName);
        }
        if (col < 0) {
            RB_DEBUG->error("RB_MmSource::fieldIndex() 1 ERROR");
        }
        return col;
    } else if (mObject) {
        // return mObject->getMemberNo(fieldName) - mHiddenColumns;
        int col = mObject->getMemberNo(fieldName);
        if (col >= 0 && col < mHiddenColumns) {
            if (col < 0) {
                RB_DEBUG->error("RB_MmSource::fieldIndex() 2 ERROR");
            }
            // a hidden column is asked for such as id, parent, name
            return col;
        }
        if (col - mHiddenColumns < 0) {
            RB_DEBUG->error("RB_MmSource::fieldIndex() 3 ERROR");
        }
        return col - mHiddenColumns;
    }
    RB_DEBUG->error("RB_MmSource::fieldIndex() ERROR");
    return -1;
}

/**
 * @returns the relation object for the column
 */
QSqlRelation RB_MmSource::relation(int column) const {
    int col = column + mHiddenColumns;
    return QSqlRelationalTableModel::relation(col);
}


/**
 * Lets the specified column be a foreign index specified by relation
 * Note: For a Biluna in-memory model, the list with the possible relation
 * data should always be a direct child of the root XXX_Project object.
 * Relational model has 0 hidden columns
 * @param column
 * @param relation
 * @param relWhere where clause for relation table
 */
void RB_MmSource::setRelation(int column, const RB_SqlRelation& relation,
                              RB_ObjectBase* project) {
    int col = column + mHiddenColumns;

    if (!isTreeModel() && database().isOpen()) {
        // relational model with 0 hidden columns
        QSqlRelationalTableModel::setRelation(col, relation);
        return;
    } else if (isTreeModel() && database().isOpen()) {
        // create on the fly, refer QRelation::populateModel(), 0 hidden columns
        RB_MmSource* relModel = new RB_MmSource(NULL, database(), 0);
        relModel->setTable(relation.tableName().toLower());
        relModel->select();
        QSqlRelationalTableModel::setRelation(col, relation);
        mRelationalModelMap[col] = relModel;
    } else if (project) {
        // in-memory relational table model, 0 hidden columns
        RB_MmSource* relModel = new RB_MmSource(NULL, QSqlDatabase(), 0);
        relModel->setObjectFactory(this->getObjectFactory());
        RB_String listName = relation.tableName() + "List";
        relModel->setTableModel(listName);
        relModel->setRoot(project);
        QSqlRelationalTableModel::setRelation(col, relation);
        mRelationalModelMap[col] = relModel;
    }
}

/**
 * Returns a QSqlTableModel object for accessing the table for
 * which column is a foreign key, or 0 if there is no relation for
 * the given column. The returned object is owned by the
 * QSqlRelationalTableModel.
 * @param column
 */
QSqlTableModel* RB_MmSource::relationModel(int column) const {
    int col = column + mHiddenColumns;

    if (!isTreeModel() && database().isOpen()) {
        return QSqlRelationalTableModel::relationModel(col);
    }

    std::map<int, RB_MmSource*>::const_iterator iter = mRelationalModelMap.find(col);
    if (iter != mRelationalModelMap.end()) {
        return iter->second;
    }

    return NULL;
}

bool RB_MmSource::select() {
    if (mIsSubmitAllInProgress) {
        return true;
    }

    if (isTreeModel() && database().isOpen()) {
        // Create or recreate tree
        beginResetModel();
        createTree();
        endResetModel();
        return true;
	} else if (mSortColumnName2 != "" && database().isOpen()) {
		// setQuery(QSqlQuery) is set in createTableSortFilter()
        beginResetModel();
        endResetModel();
        return true;
    } else if (!database().isOpen()) {
        beginResetModel();
        endResetModel();
        return true;
    }

    bool success = QSqlRelationalTableModel::select();
//    RB_DEBUG->print(this->query().lastQuery());
//    RB_DEBUG->print("RB_MmSource::select() table: " + this->tableName());
//    RB_DEBUG->print("RB_MmSource::select() filter: " + this->filter());
//    RB_DEBUG->print("RB_MmSource::select() last error: " + this->lastError().text());
    return success;
}

/**
 * Clears the in-memory model and replaces root with new root,
 * does nothing with database model
 */
void RB_MmSource::clear() {
    RB_DEBUG->print("RB_MmSource::clear()");
    if (mRoot) {
        // replace root by new root
        RB_ObjectFactory* objF = mRoot->getFactory();
        RB_ObjectBase* obj = objF->newSingleObject(mRoot->getName());
        delete mRoot;
        mRoot = obj;
    }

    RB_MmAbstract::clear();
}

/**
 * @returns database for normal table model or for tree model
 */
QSqlDatabase RB_MmSource::database() const {
    return RB_DATABASE->database(mDatabaseConnection);
}

/**
 * Pass expanded signal to source model
 * @param index index of item which is expanded
 */
void RB_MmSource::slotExpanded(const QModelIndex& index) {
    RB_DEBUG->print("RB_MmSource::slotExpanded()");
    if (!index.isValid() || !database().isOpen()) return;
    RB_ObjectBase* obj = static_cast<RB_ObjectBase*>(index.internalPointer());
    createSubTree(obj);
}

/**
 * Change current row for mapper.
 * @param current current selected index
 * @param current previous selected index, not used
 */
void RB_MmSource::slotChangeCurrentRow(const QModelIndex& current,
                                   const QModelIndex& /*previous*/) {
    mCurrentIndex = current;

    if (!current.isValid()) {
        RB_DEBUG->print(RB_Debug::D_INFORMATIONAL,
            "RB_MmSource::slotChangeCurrentRow() current index not valid INFO");
    }
}

/**
 * @brief slot to copy all rows of this child model based on parent ID
 * @param parentId
 */
void RB_MmSource::slotCopyRows(const QString& parentId) {
    // child models: always a table model
    QModelIndex idx = QModelIndex();
    QVariant value;
    int cCount = columnCount();
    int rCount = rowCount();
    insertRows(rCount, rCount);

    for (int row = 0; row < rCount; row++) {
        // set parent ID
        idx = index(rCount + row, 1);
        setData(idx, parentId);

        for (int col = RB2::HIDDENCOLUMNS; col < cCount; col++) {
            // copy other values
            idx = index(row, col);
            value = idx.data(Qt::EditRole);
            idx = index(rCount + row, col);
            setData(idx, value);
        }
    }
}

void RB_MmSource::reset() {
    // TODO: in-memory and tree model reset
    //QSqlRelationalTableModel::reset();
    select();
}

void RB_MmSource::revert() {
    if (isTreeModel()) {
        this->revertAll();
        return;
    }
    QSqlRelationalTableModel::revert();
}

void RB_MmSource::revertAll() {
    if (isTreeModel() && mRoot) {
        beginResetModel();
        mRoot->revert();
        endResetModel();
        return;
    }
    QSqlRelationalTableModel::revertAll();
}

/**
 * Does nothing if strategy is set to on-manual submit
 */
bool RB_MmSource::submit() {
    // TODO: in-memory and tree model submit
    if (!isTreeModel() && database().isOpen()) {
        return QSqlRelationalTableModel::submit();
    }

    // Nothing for treemodel with database or in-memory models
    return true;
}

/**
 * Submit all changes to the database, do not use after inserting
 * or deletion of rows. Use for example in fileSave when closing a dialog.
 * NOTE: does not execute a select, does not update the tableview and
 * does not change the current index.
 * @returns true on success
 */
bool RB_MmSource::submitAll() {
    if (!isTreeModel() && database().isOpen()) {
        mIsSubmitAllInProgress = true;
        bool success = QSqlRelationalTableModel::submitAll();
        mIsSubmitAllInProgress = false;
        return success;
    }
    // Not used below, because root is usually a dummy in a tree model
    // mRoot->dbUpdate(database(), RB2::ResolveAll);
    RB_ObjectContainer* list = mRoot->getContainer(mListName);
    if (database().transaction()) {
        list->dbUpdateList(database(), RB2::ResolveAll);
        if (!database().commit()) {
            database().rollback();
        }
    }

    return true;
}

/**
 * Submit the changes to the database and (re)select the data. Updates
 * the table- or treeview.
 * @returns true on success
 */
bool RB_MmSource::submitAllAndSelect() {
    if (!isTreeModel() && database().isOpen()) {
        mIsSubmitAllInProgress = false;
        return QSqlRelationalTableModel::submitAll(); // triggers select()
    }
    RB_ObjectContainer* list = mRoot->getContainer(mListName);
    if (database().transaction()) {
        list->dbUpdateList(database(), RB2::ResolveAll);
        if (!database().commit()) {
            database().rollback();
        }
        mIsSubmitAllInProgress = false;
        select();
    }

    return true;
}

/**
 * Get the data of an object at index (is at row and column)
 */
RB_Variant RB_MmSource::objectData(const QModelIndex& index, int role) const {
    if (!isTreeModel() && database().isOpen()) {
        RB_DEBUG->print("RB_MmSource::objectData() not tree model and database is open ERROR");
        return RB_Variant();
    }

    if (!index.isValid()) return RB_Variant();

    RB_ObjectBase* obj = static_cast<RB_ObjectBase*>(index.internalPointer());

    int col = index.column();
    if (!obj || col < 0 || col > obj->memberCount() - 1) {
        return RB_Variant();
    }

    if (role == Qt::DisplayRole) {

        // HACK: relational data
        // START TODO: set relational data as display value during loading?
        std::map<int, RB_MmSource*>::const_iterator iter;
        iter = mRelationalModelMap.find(col);

        if (iter != mRelationalModelMap.end()) {
            QSqlTableModel* relModel = iter->second;
            int count = relModel->rowCount();

            for (int relRow = 0; relRow < count; ++relRow) {
                QModelIndex idx = relModel->index(relRow, 0);

                if (idx.data().toString() == obj->getValue(col)) {
                    // ID found
                    QSqlRelation rel = relation(col - mHiddenColumns);
                    QString fieldName = rel.displayColumn();
                    int relCol = relModel->fieldIndex(fieldName);
                    idx = relModel->index(relRow, relCol);
                    return idx.data();
                }
            }
        }
        // END TODO

        bool isIntegerField = false;
        isIntegerField = (obj->getMember(col)->getType()
                          == RB2::MemberInteger);

        if (isIntegerField) {
            RB_StringList strL = getTextList(index.column());
            if (!strL.isEmpty()) {
                return strL.at(obj->getDValue(col).toInt());
            }
        }

        return obj->getDValue(col);
    } else if (role ==  Qt::EditRole) {

        // HACK: relational data
        // START TODO: set relational data as display value during loading?
        std::map<int, RB_MmSource*>::const_iterator iter;
        iter = mRelationalModelMap.find(col);

        if (iter != mRelationalModelMap.end()) {
            QSqlTableModel* relModel = iter->second;
            int count = relModel->rowCount();

            for (int relRow = 0; relRow < count; ++relRow) {
                QModelIndex idx = relModel->index(relRow, 0);

                if (idx.data().toString() == obj->getIdValue(col)) {
                    // ID found
                    QSqlRelation rel = relation(col - mHiddenColumns);
                    QString fieldName = rel.displayColumn();
                    int relCol = relModel->fieldIndex(fieldName);
                    idx = relModel->index(relRow, relCol);
                    return idx.data();
                }
            }
        }
        // END TODO

        bool isIntegerField = false;
        isIntegerField = (obj->getMember(col)->getType()
                          == RB2::MemberInteger);

        if (isIntegerField) {
            RB_StringList strL = getTextList(index.column());
            if (!strL.isEmpty()) {
                return strL.at(obj->getDValue(col).toInt());
            }
        }

        if (obj->getMember(col)->getName().endsWith("_idx")) {
            return obj->getValue(col).toString() + obj->getDValue(col).toString();
        } else {
            return obj->getDValue(col);
        }
    } else if (role == RB2::RoleOrigData) {
        return obj->getIdValue(col);
    }

    return RB_Variant();
}

/**
 * Set the object data with value at index (at row and column)
 */
bool RB_MmSource::setObjectData(const QModelIndex& index,
                                const RB_Variant& value, int role) {
    if (!index.isValid() || (!isTreeModel() && database().isOpen())) {
        RB_DEBUG->error("RB_MmSource::setObjectData() ERROR");
        return false;
    }
    if (role != Qt::EditRole && role != RB2::RoleOrigData) {
        RB_DEBUG->error("RB_MmSource::setObjectData() role ERROR");
        return false;
    }

    RB_ObjectBase* obj = static_cast<RB_ObjectBase*>(index.internalPointer());
    int col = index.column();
    obj->setValue(col, value);

    return true;
}

// TODO: to be implemented for database tree models, param to include object
void RB_MmSource::setRelationalValue(int col, RB_Variant& /*val*/) const {
    if (!isTreeModel() || !database().isOpen() || !(col >= RB2::HIDDENCOLUMNS)) return;

//    const RB_SqlRelation& relation = QSqlRelationalTableModel::relation(col);
//    if (!relation.isValid()) {
//        val = RB_Variant();
//        return;
//    }

//    RB_MmSource* relModel = mRelationalModelMap.value(col, NULL);
//    if (!relModel) return;
//    int relRowNo = -1;

//    if (database().isOpen()) {
//        // Database tree model only
//        QModelIndex idx;
//        QVariant idVal;
//        for (int i = 0; i < relModel->rowCount(); ++i) {
//            idx = relModel->index(i, 0, QModelIndex());
//            idVal = relModel->data(idx, Qt::DisplayRole);
//            if (idVal == val) {
//                relRowNo = i;
//                i = relModel->rowCount();
//            }
//        }
//    } else {
//        // In-memory models, quicker possibility than !mDatabase.isOpen()
//        relRowNo = relModel->getRow(relation.indexColumn(), val);
//    }
//    RB_Variant rValue = relModel->data(relModel->index(
//            relRowNo, relModel->fieldIndex(relation.displayColumn()),
//            QModelIndex()));

//    if (rValue.isValid()) {
//        val = rValue;
//    } else {
//        val = QVariant();
//    }


}

/**
 *  Only for debugging, for export of base model structure
 */
void RB_MmSource::setModelStructure() {
    RB_DEBUG->print("RB_MmSource::setModelStructure()");
    // store the pointer to the main model
    tmpRoot = mRoot;
    // get model structure from object factory and set as model root
    mRoot = (RB_ObjectContainer*)mObjectFactory->getModelStructure("XXX_Project");
}

/**
 *  Only for debugging, for export of base model structure
 */
void RB_MmSource::unsetModelStructure() {
    RB_DEBUG->print("RB_MmSource::unsetModelStructure()");
    // restore model
    mRoot = tmpRoot;
}

/**
 * Get row (sequence) number for object, only applicable for in-memory models
 * @param member column member
 * @param value value of the member
 * @return row number in model
 */
int RB_MmSource::getRow(const RB_String& member, const RB_Variant& value) {
    if (!mRoot) return -1;
    RB_ObjectContainer* objC = mRoot->getContainer(mListName);
    if (!objC) return -1;
    return objC->getRow(member, value);
}

/**
 * Set this model as a tree model. Make sure to first set:
 * - setObjectFactory() with the object factory
 * @param list name of list
 * @param db database connection
 */
bool RB_MmSource::setTreeModel(const RB_String& list, const QSqlDatabase &db) {
    if (!mObjectFactory) {
        return false;
    }

    mIsTreeModel = true;
    mDatabaseConnection = db.connectionName();
    return setTableModel(list);
}

/**
 * Create sort filter
 */
void RB_MmSource::createTableSortFilter(RB_String& filter) {
    RB_String sortOrder = "";

    if (mSortOrder == RB2::SortOrderAscending) {
        sortOrder = " ASC";
    } else if (mSortOrder == RB2::SortOrderDescending) {
        sortOrder = " DESC";
    }

    // Normally only sort one column or none
    if (mSortColumnName1 != "" && mSortColumnName2 == "") {
        if (mSortOrder == RB2::SortOrderAscending) {
            setSort(fieldIndex(mSortColumnName1), Qt::AscendingOrder);
        } else if (mSortOrder == RB2::SortOrderDescending) {
            setSort(fieldIndex(mSortColumnName1), Qt::DescendingOrder);
        }
        // setFilter() also executes select!
        if (filter.isEmpty()) {
            setFilter("`" + mTableName.toLower() + "`.`status` >= 0");
        } else {
            setFilter(filter + " AND `" + mTableName.toLower() + "`.`status` >= 0");
        }
        return;
    } else if (mSortColumnName1 == "" || sortOrder == "") {
        // setFilter() also executes select
        if (filter.isEmpty()) {
            setFilter("`" + mTableName.toLower() + "`.`status` >= 0");
        } else {
            setFilter(filter + " AND `" + mTableName.toLower() + "`.`status` >= 0");
        }
        return;
    }

    // Exceptional: more columns
    RB_String sortBy = "";

    if (mSortColumnName1 != "") {
        sortBy = " ORDER BY " + mSortColumnName1;
    }

    if (mSortColumnName1 != "" && mSortColumnName2 != "") {
        sortBy += ", " + mSortColumnName2;
    }

    if (mSortColumnName1 != "" && mSortColumnName2 != "" && mSortColumnName3 != "") {
        sortBy += ", " + mSortColumnName3;
    }

    RB_String qStr = "SELECT * FROM " + mTableName;

    if (filter != "") {
        qStr += " WHERE " + filter  + " AND `" + mTableName.toLower() + "`.`status` >= 0";
    } else {
        qStr +=  " WHERE `" + mTableName.toLower() + "`.`status` >= 0";
    }

    if (sortBy != "" && sortOrder != "") {
        qStr += sortBy + sortOrder;
    }

    QSqlQuery q(qStr, database());
	// setQuery() also executes select internally
    setQuery(q);
}


/**
 * Create and set table filter, including createTableSortFilter()
 */
void RB_MmSource::createTableFilter() {
    // parent and second parent
    RB_String filter = "";

    if (mRoot->getId() != "") {
        filter += "`" + mTableName.toLower() + "`.`" + mPrimaryParent
                + "`='" + mRoot->getId() + "'";

        if (mSecondParent != "") {
            filter += " AND `" + mTableName.toLower() + "`.`" + mSecondParent
                      + "`='" + mSecondParentId + "'";
        }
    }

    if (!filter.isEmpty() && !mWhere.isEmpty()) {
        filter += " AND " + mWhere;
    } else if (filter.isEmpty() && !mWhere.isEmpty()) {
        filter = mWhere;
    }

    createTableSortFilter(filter);
}

/**
 * Create tree sort filter and select
 */
void RB_MmSource::createTreeSortFilter(RB_String& sqlStr) {
    RB_String sortOrder = "";

    if (mSortOrder == RB2::SortOrderAscending) {
        sortOrder = " ASC";
    } else if (mSortOrder == RB2::SortOrderDescending) {
        sortOrder = " DESC";
    }

    RB_String sortBy = "";

    if (mSortColumnName1 != "") {
        sortBy = " ORDER BY " + mSortColumnName1;
    }

    if (mSortColumnName1 != "" && mSortColumnName2 != "") {
        sortBy += ", " + mSortColumnName2;
    }

    if (mSortColumnName1 != "" && mSortColumnName2 != "" && mSortColumnName3 != "") {
        sortBy += ", " + mSortColumnName3;
    }

    if (sortBy != "" && sortOrder != "") {
        sqlStr += sortBy + sortOrder + ";";
    }
}

/**
 * Create tree sql statement
 */
void RB_MmSource::createTreeSelectSql() {
    /*
    -- up to and including great grandchildren (4 levels) of root
    -- the base list are the parents or level 1 of root,
    -- children are level 2
    -- grandchildren are level 3
    -- great grandchildren are level 4
    SELECT t4.*
    FROM acc_customers as t4
    LEFT JOIN acc_customers as t3 on t4.parent=t3.id
    LEFT JOIN acc_customers as t2 on t3.parent=t2.id
    LEFT JOIN acc_customers as t1 on t2.parent=t1.id
    WHERE t1.parent='{08c87571-20d2-4f38-9e79-c1336755b367}'
    OR t2.parent='{08c87571-20d2-4f38-9e79-c1336755b367}'
    OR t3.parent='{08c87571-20d2-4f38-9e79-c1336755b367}'
    OR t4.parent='{08c87571-20d2-4f38-9e79-c1336755b367}'
    */

    RB_String rootId = mRoot->getId();

    mTreeSelectSql = "SELECT `t4`.* FROM `" + mTableName.toLower() + "` as `t4` "
                         "LEFT JOIN `" + mTableName.toLower() + "` as `t3` on `t4`.`" + mPrimaryParent + "`=`t3`.`id` "
                         "LEFT JOIN `" + mTableName.toLower() + "` as `t2` on `t3`.`" + mPrimaryParent + "`=`t2`.`id` "
                         "LEFT JOIN `" + mTableName.toLower() + "` as `t1` on `t2`.`" + mPrimaryParent + "`=`t1`.`id` "
                     "WHERE `t1`.`" + mPrimaryParent + "`='" + rootId + "' "
                         "OR `t2`.`" + mPrimaryParent + "`='" + rootId + "' "
                         "OR `t3`.`" + mPrimaryParent + "`='" + rootId + "' "
                         "OR `t4`.`" + mPrimaryParent + "`='" + rootId + "'"
                        + " AND `t4`.`status` >= 0";
}

/**
 * Create tree model in case of database,
 * TODO: implement second parent as per tablemodel
 * TODO: finalize performance
 */
void RB_MmSource::createTree() {
    if (isTreeModel() && !database().isOpen()) {
        RB_DEBUG->print(RB_Debug::D_ERROR, "RB_MmSource::createTreeItem() "
                        "Database not open ERROR");
        return;
    }

RB_DEBUG->timestamp();
QDateTime now1 = QDateTime::currentDateTime();
QDateTime now2;
qint64 check0 = 0;
qint64 check1 = 0;
qint64 check2 = 0;
qint64 check21 = 0;
qint64 check22 = 0;
qint64 check23 = 0;
qint64 check26 = 0;
qint64 check27 = 0;
qint64 check28 = 0;
qint64 check3 = 0;
qint64 check31 = 0;
qint64 check32 = 0;
qint64 check33 = 0;
qint64 check34 = 0;
qint64 check35 = 0;
qint64 check4 = 0;
qint64 check5 = 0;

    RB_String rootId = mRoot->getId();

    QSqlQuery query(database());
    query.setForwardOnly(true);

    // Select statement
    createTreeSelectSql();
    createTreeSortFilter(mTreeSelectSql);

    if (!query.exec(mTreeSelectSql)) {
        RB_String str = query.lastError().text();
        str += " - RB_MmSource::createTreeItem() SQL not successfull ERROR";
        RB_DEBUG->print(RB_Debug::D_ERROR, str.toStdString().c_str());
        return;
    }

now2 = QDateTime::currentDateTime(); check0 += now1.msecsTo(now2); now1 = now2;

    RB_String idStr = "";
    RB_String parentStr = "";
    RB_String fieldName = "";
    RB_Variant value = "";
    RB_ObjectBase* currentParent = mRoot;
    RB_ObjectContainer* currentList = mRoot->getContainer(mListName);
    RB_ObjectBase* currentObj = NULL;

    // Clear current list
    currentList->erase();
    // Children will be created
    mRoot->setFlag(RB2::FlagChildrenCreated);
    // Map for objects whose parent is not currentParent
    std::map<RB_String, RB_ObjectBase*> objMap;
    // Map for objects (orphane) whose parent has not yet been loaded
    std::map<RB_String, RB_ObjectBase*> orphMap;

//    std::vector<QString> fieldNameList;
//    bool isFieldNameListSet = false;
    QSqlRecord record;

    while (query.next()) {
        record = query.record();
        idStr = record.value(0).toString(); // record.value("id").toString();
        parentStr = record.value(1).toString(); // record.value("parent").toString();
now2 = QDateTime::currentDateTime(); check1 += now1.msecsTo(now2); now1 = now2;
        if (parentStr == rootId) {
            currentParent = mRoot;
        } else if (parentStr == currentParent->getValue("id").toString()) {
            // same level, no change
        } else if (currentObj && parentStr == currentObj->getValue("id").toString()) {
            // child level
            currentParent = currentObj;
        } else if (currentParent->getGrandParent()
            && parentStr == currentParent->getGrandParent()->getValue("id").toString()) {
            // one up
            currentParent = currentParent->getParent()->getParent();
        } else if (currentParent->getGrandParent() && currentParent->getGrandParent()->getGrandParent()
            && parentStr == currentParent->getGrandParent()->getGrandParent()->getValue("id").toString()) {
            // two up
            currentParent = currentParent->getGrandParent()->getGrandParent();
        } else if (currentParent->getGrandParent() && currentParent->getGrandParent()->getGrandParent()
            && currentParent->getGrandParent()->getGrandParent()->getGrandParent()
            && parentStr == currentParent->getGrandParent()->getGrandParent()->getGrandParent()->getValue("id").toString()) {
            // three up
            currentParent = currentParent->getGrandParent()->getGrandParent()->getGrandParent();
        } else {
            currentParent = NULL;

            // Parent can already have been read but is not current parent,
            std::map<RB_String, RB_ObjectBase*>::iterator iterMap
                    = objMap.find(record.value(1).toString());
            if (iterMap != objMap.end()) {
                currentParent = iterMap->second;
            }
        }
now2 = QDateTime::currentDateTime(); check2 += now1.msecsTo(now2); now1 = now2;
        if (currentParent) {
            // Parent already read
            currentList = currentParent->getContainer(mListName);
now2 = QDateTime::currentDateTime(); check21 += now1.msecsTo(now2); now1 = now2;
            currentObj = mObjectFactory->newObject(idStr, currentList, "", true);
now2 = QDateTime::currentDateTime(); check22 += now1.msecsTo(now2); now1 = now2;
            objMap[record.value(0).toString()]= currentObj;
now2 = QDateTime::currentDateTime(); check23 += now1.msecsTo(now2); now1 = now2;
        } else {
            // Parent not yet read, store as orphan
            currentObj = mObjectFactory->newSingleObject(mTableName);
now2 = QDateTime::currentDateTime(); check26 += now1.msecsTo(now2); now1 = now2;
            orphMap[record.value(0).toString()]= currentObj;
now2 = QDateTime::currentDateTime(); check27 += now1.msecsTo(now2); now1 = now2;
            // Reset to currentParent to root object
//            currentParent = mRoot;
now2 = QDateTime::currentDateTime(); check28 += now1.msecsTo(now2); now1 = now2;
        }
now2 = QDateTime::currentDateTime(); check3 += now1.msecsTo(now2); now1 = now2;
        for (int i = 0; i < mNoColumns; ++i) {
//            if (!isFieldNameListSet) {
//                fieldName = record.fieldName(i);
//                fieldNameList.push_back(fieldName);
//            } else {
//                fieldName = fieldNameList.at(i);
//            }
now2 = QDateTime::currentDateTime(); check31 += now1.msecsTo(now2); now1 = now2;
            value = record.value(i);
now2 = QDateTime::currentDateTime(); check32 += now1.msecsTo(now2); now1 = now2;
            fieldName = record.fieldName(i);
now2 = QDateTime::currentDateTime(); check33 += now1.msecsTo(now2); now1 = now2;
            if (fieldName.endsWith("_id")) {
                currentObj->setValue(i /*fieldName*/, value);
                setRelationalValue(i, value);
            } else {
                currentObj->setValue(i /*fieldName*/, value);
            }
now2 = QDateTime::currentDateTime(); check34 += now1.msecsTo(now2); now1 = now2;

        }
now2 = QDateTime::currentDateTime(); check35 += now1.msecsTo(now2); now1 = now2;

//        isFieldNameListSet = true;

        // Set 'children are created' up to the third level
        if (currentParent && (currentParent == mRoot
                              || currentParent->getGrandParent() == mRoot)) {
            // first and second level
            currentObj->setFlag(RB2::FlagChildrenCreated);
        } else if (currentParent && (currentParent->getGrandParent()
               && currentParent->getGrandParent()->getGrandParent() == mRoot)) {
            // third level
            currentObj->setFlag(RB2::FlagChildrenCreated);
        }
now2 = QDateTime::currentDateTime(); check4 += now1.msecsTo(now2); now1 = now2;

        // Reset currentParent to root if not found
        if (!currentParent) {
            currentParent = mRoot;
        }
now2 = QDateTime::currentDateTime(); check5 += now1.msecsTo(now2); now1 = now2;
    }
RB_DEBUG->print("00 " + RB_String::number(check0));
RB_DEBUG->print("10 " + RB_String::number(check1));
RB_DEBUG->print("20 " + RB_String::number(check2));
RB_DEBUG->print("21 " + RB_String::number(check21));
RB_DEBUG->print("22 " + RB_String::number(check22));
RB_DEBUG->print("23 " + RB_String::number(check23));
RB_DEBUG->print("26 " + RB_String::number(check26));
RB_DEBUG->print("27 " + RB_String::number(check27));
RB_DEBUG->print("28 " + RB_String::number(check28));
RB_DEBUG->print("30 " + RB_String::number(check3));
RB_DEBUG->print("31 " + RB_String::number(check31));
RB_DEBUG->print("32 " + RB_String::number(check32));
RB_DEBUG->print("33 " + RB_String::number(check33));
RB_DEBUG->print("34 " + RB_String::number(check34));
RB_DEBUG->print("35 " + RB_String::number(check35));
RB_DEBUG->print("40 " + RB_String::number(check4));
RB_DEBUG->print("50 " + RB_String::number(check5));
RB_DEBUG->timestamp();
now1 = now2;

    // After all objects have been set, link orphans to relevant parent
    if (orphMap.size() > 0) {
        std::map<RB_String, RB_ObjectBase*>::iterator iterOrph
                = orphMap.begin();

        while (iterOrph != orphMap.end()) {
            currentParent = NULL;
            currentObj = iterOrph->second;
            RB_String objParentId = currentObj->getValue("parent").toString();

            // Look for parent in orphan map
            std::map<RB_String, RB_ObjectBase*>::iterator iterOrphChild
                    = orphMap.find(objParentId);
            if (iterOrphChild != orphMap.end()) {
                currentParent = iterOrphChild->second;
                currentList = currentParent->getContainer(mListName);
                currentList->addObject(currentObj);
            }

            // If not found look for parent in object map
            if (!currentParent) {
                std::map<RB_String, RB_ObjectBase*>::iterator iterObjChild
                        = objMap.find(objParentId);
                if (iterObjChild != objMap.end()) {
                    currentParent = iterObjChild->second;
                    currentList = currentParent->getContainer(mListName);
                    currentList->addObject(currentObj);
                }
            }

            // Set 'children are created' up to the third level
            if (currentParent && (currentParent == mRoot
                                  || currentParent->getGrandParent() == mRoot)) {
                // first and second level
                currentObj->setFlag(RB2::FlagChildrenCreated);
            } else if (currentParent && (currentParent->getGrandParent()
                   && currentParent->getGrandParent()->getGrandParent() == mRoot)) {
                // third level
                currentObj->setFlag(RB2::FlagChildrenCreated);
            }

            ++iterOrph;
        }

        // Test for remaining orphans without parent
        iterOrph = orphMap.begin();

        while (iterOrph != orphMap.end()) {
            RB_ObjectBase* obj = iterOrph->second;

            if (!obj->getParent()) {
                // Could happen in case of database id-parent error
                RB_DEBUG->error("RB_MmSource::createTree() ERROR");
                delete obj;
            }

            ++iterOrph;
        }
    }

    RB_FlagVisitor* vis = new RB_FlagVisitor();
    vis->setFlag(RB2::FlagFromDatabase);
    vis->deleteFlag(RB2::FlagIsDirty);
    mRoot->acceptVisitor(*vis);
    delete vis;


now2 = QDateTime::currentDateTime();
qint64 check6 = now1.msecsTo(now2);
RB_DEBUG->print("60 " + RB_String::number(check6));
RB_DEBUG->timestamp();
}

/**
 * Get tree child and grandchild items from database for in-memory proxy model,
 * only applicable for tree models connect to a database. Unfortunately the
 * grandchildren have to be created also to show the correct (+) expand icon
 * already when a child item is expanded. This is done because the QTreeView
 * emits expanded(QModelIndex) after it has requested for the hasChildren().
 * @param parent object for which to get the child item(s)
 * @param grandChildren is false if this is the (first) round for the children,
 *      is true if this it the (second) round for the grandchildren
 */
void RB_MmSource::createSubTree(RB_ObjectBase* parent) {
    RB_DEBUG->print("RB_MmSource::createSubTree()");
    if (!isTreeModel() || !database().isOpen()) {
        RB_DEBUG->print(RB_Debug::D_ERROR, "RB_MmSource::createSubTree() "
                        "Database not open ERROR");
        return;
    }

    RB_String rootId = parent->getId();
    QSqlQuery query(database());
    query.setForwardOnly(true);
    RB_String qStr = "SELECT t4.* FROM " + mTableName.toLower() + " as t4 "
                     "LEFT JOIN " + mTableName.toLower() + " as t3 on t4.parent=t3.id "
                     "LEFT JOIN " + mTableName.toLower() + " as t2 on t3.parent=t2.id "
                     "LEFT JOIN " + mTableName.toLower() + " as t1 on t2.parent=t1.id "
                     "WHERE t1.parent='" + rootId + "';";

    if (!query.exec(qStr)) {
        RB_String str = query.lastError().text();
        str += " - RB_MmSource::createTreeItem() SQL not successfull ERROR";
        RB_DEBUG->print(RB_Debug::D_ERROR, str.toStdString().c_str());
        return;
    }

    RB_String idStr = "";
    RB_String parentStr = "";
    RB_String fieldName = "";
    RB_Variant value = "";
    RB_ObjectBase* currentParent = parent;
    RB_ObjectBase* currentList = mRoot->getContainer(mListName);
    RB_ObjectBase* currentObj = NULL;
    QSqlRecord record;

    while (query.next()) {
        record = query.record();
        idStr = record.value("id").toString();
        parentStr = record.value("parent").toString();

        if (parentStr != currentParent->getValue("id").toString()) {
            // find parent object and store temporarily in currentObj
            currentObj = parent->getDescendantObject(parentStr);
        }

        if (parentStr == currentParent->getValue("id").toString()) {
            // same level no change
        } else if (currentObj && parentStr == currentObj->getValue("id").toString()) {
            // child level
            currentParent = currentObj;
            currentList = currentObj->getContainer(mListName);
        } else {
            RB_DEBUG->error("RB_MmSource::createTree() parent not found ERROR");
            return;
        }

        currentObj = mObjectFactory->newObject(idStr, currentList, "", true);

        for (int i = 2; i < mNoColumns; ++i) {
           fieldName = record.fieldName(i);
           value = record.value(i);

           if (fieldName.endsWith("_id")) {
               currentObj->setValue(i /*fieldName*/, value);
               setRelationalValue(i, value);
           } else {
               currentObj->setValue(i /*fieldName*/, value);
           }
        }

        currentObj->setFlag(RB2::FlagFromDatabase);
        currentObj->deleteFlag(RB2::FlagIsDirty);
    }
}

/**
 * Get current or last selected index
 * @return index
 */
QModelIndex RB_MmSource::getCurrentIndex() const {
    return mCurrentIndex;
}

/**
 * Get current or last selected ID
 * @return ID
 */
RB_String RB_MmSource::getCurrentId() const {
    return hiddenData(index(mCurrentIndex.row(), 0, mCurrentIndex.parent()),
                                        Qt::DisplayRole).toString();
}

/**
 * Get current value of named field, not for id, parent or name which are hidden
 * @param fieldName field of which value is to be returned
 * @param role display- or editrole,
 *          use Qt::USerRole+1 for actual value in case of relational data
 * @return value
 */
RB_Variant RB_MmSource::getCurrentValue(const RB_String& fieldName, int role) const {
    return data(index(mCurrentIndex.row(), fieldIndex(fieldName),
                      mCurrentIndex.parent()), role);
}

/**
 * Set current value of named field, not for id, parent or name which are hidden
 * @param fieldName name of field which value is set
 * @param val value
 * @param role display- or editrole,
 *          use Qt::USerRole+1 for actual value in case of relational data
 */
bool RB_MmSource::setCurrentValue(const RB_String& fieldName, const RB_Variant& val,
                                  int role) {
    return setData(index(mCurrentIndex.row(), fieldIndex(fieldName),
                         mCurrentIndex.parent()), val, role);
}

/**
 * Get value of field
 * TODO: implement query option in case of database connection for better performance
 * 1- at id or
 * 2- at parent and keyfieldname with keyvalue. If id is an empty string
 *    than all in the table are search for keyFieldName with keyValue
 * @param id ID or parentID if keyFieldName and keyValue are not empty
 * @param fieldName field from which the value is to be returned
 * @param keyFieldName name of key field
 * @param keyValue value of key field
 * @param role default Qt::UserRole + 1 retrieving the actual value
 *          not the relational value
 * @return value of fieldName
 */
RB_Variant RB_MmSource::getValue(const RB_String& id, const RB_String& fieldName,
                                 const RB_String& keyFieldName,
                                 const RB_Variant& keyValue, int role) const {
    if (fieldName.isEmpty()) return RB_Variant();

    bool found = false;
    RB_Variant var;

    // Set field columns only once
    int fieldColumn = fieldIndex(fieldName);

    // TODO: implement query option in case of database connection for performance
    if (!id.isEmpty() && (keyFieldName.isEmpty() || !keyValue.isValid())) {
        // search for ID only
        for (int i = 0; i < rowCount() && !found; ++i) {
            if (id == hiddenData(index(i, 0), role).toString()) {
                var = data(index(i, fieldColumn), role);
                found = true;
            }
        }
    } else {
        // id is parent here
        int keyFieldColumn = fieldIndex(keyFieldName);
        for (int i = 0; i < rowCount() && !found; ++i) {
            if ((id == "" || id == hiddenData(index(i, 1), role).toString())
                && keyValue == data(index(i, keyFieldColumn), role)) {
                var = data(index(i, fieldColumn), role);
                found = true;
            }
        }
    }

    return var;
}

/**
 * Set value of field at id
 * TODO: implement query option in case of database connection for better performance
 * @param id ID
 * @param fieldName field from which the value is to be set
 * @param value value
 * @param role default Qt::UserRole + 1 setting the actual value
 *          not the relational value
 */
bool RB_MmSource::setValue(const RB_String& id, const RB_String& fieldName,
                                 const RB_Variant& value, int role) {
    if (id.isEmpty() || fieldName.isEmpty()) {
        RB_DEBUG->print(RB_Debug::D_ERROR, "RB_MmSource::setValue() "
                        "id, fieldName ERROR");
        return false;
    }

    bool found = false;
    bool success = false;

    // Set field columns only once
    int fieldColumn = fieldIndex(fieldName);

    // search for ID only
    for (int i = 0; i < rowCount() && !found; ++i) {
        if (id == hiddenData(index(i, 0), role).toString()) {
            success = setData(index(i, fieldColumn), value, role);
            found = true;
        }
    }
    return success;
}

/**
 * Set where statement in addition to parent and second parent.
 * If root ID is empty, only this statement will be used for filter,
 * if both are empty, WHERE id<>'0' will be used. Does not work
 * for treemodels
 * @param whereStatement additional where statement in SQL query
 * @param select is default true to execute the create where statement
 */
void RB_MmSource::setWhere(const RB_String& whereStatement, bool isSelect) {
    mWhere = whereStatement;

    if (mRoot && mRoot->getId().isEmpty() && mWhere.isEmpty()) {
        mWhere = "`" + mTableName.toLower() + "`.`id` <> '0'";
        if (!isTreeModel()) {
            createTableFilter();
        } else {
            // createTreeSelectSql(); // TODO: mWhere does not work yet for treemodels
        }
    } else /* if (!mWhere.isEmpty()) */ { // also for remove of filter mWhere is empty
        if (!isTreeModel()) {
            createTableFilter();
        } else {
            // createTreeSelectSql(); // TODO: mWhere does not work yet for treemodels
        }
    }

    if (!isSelect) {
        return;
    }

    this->select();
}

/**
 * Set sql query (setQuery() of base class is protected)
 * @param q query
 */
void RB_MmSource::setSqlQuery(const QSqlQuery &q) {
    QSqlRelationalTableModel::setQuery(q);
}

/**
 * @returns sort order
 */
RB2::SortOrderType RB_MmSource::getSortOrder() {
    return mSortOrder;
}

/**
 * Set sort order
 * @param so sort order
 */
void RB_MmSource::setSortOrder(RB2::SortOrderType so,
                            const RB_String& colName1,
                            const RB_String& colName2,
                            const RB_String& colName3) {
    if (so != RB2::SortOrderNone && colName1.isEmpty()) {
        RB_DEBUG->error("RB_MmSource::setSortOrderBy() colName1 ERROR");
        return;
    }

    mSortOrder = so;
    mSortColumnName1 = colName1;
    mSortColumnName2 = colName2;
    mSortColumnName3 = colName3;
}
