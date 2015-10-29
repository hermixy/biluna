/*****************************************************************
 * $Id: rb_mmproxy.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Aug 13, 2009 5:09:49 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna db project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_mmproxy.h"

#include "rb_modelfactory.h"

/**
 * Constructor
 * @param parent
 */
RB_MmProxy::RB_MmProxy(QObject* parent)
    : QSortFilterProxyModel(parent) {

    mParentManager = NULL;
    mModelType = -1;
    mModelFactory = NULL;
    mSourceModel = NULL;
    mSelectionModel = NULL;
//    mMapper = NULL;
}



RB_MmProxy::~RB_MmProxy() {
    RB_DEBUG->print("RB_MmProxy::~RB_MmProxy()");

    // Deactivate model, non-shared models are normally not listed
    // but some modeltypes are used for selection and can be shared
    // and non-shared, therefore check of shared flag also.
    if (mModelFactory && getRbModel()->isShared()) {
        mModelFactory->removeModel(getModelType());
    }

    // Delete source model
    if (getRbModel()) {
        RB_MmAbstract* m = getRbModel();
        delete m;
        // setSourceModel(NULL); not required
    }

    // mMapper is deleted via QObject
    RB_DEBUG->print("RB_MmProxy::~RB_MmProxy() - OK");
}

/**
 * @return int model type
 */
int RB_MmProxy::getModelType() const {
    if (mModelType < 0) {
        RB_DEBUG->print("RB_MmProxy::getModelType() type not set ERROR");
    }
    return mModelType;
}

/**
 * Set model type
 * @param type model type
 */
void RB_MmProxy::setModelType(int type) {
    mModelType = type;
}

/**
 * Get model factory
 * @return modelfactory
 */
RB_ModelFactory* RB_MmProxy::getModelFactory() {
    return mModelFactory;
}

/**
 * Set model factory
 * @param mf modelfactory
 */
void RB_MmProxy::setModelFactory(RB_ModelFactory* mf) {
    mModelFactory = mf;
}

/**
 * Set or create root, initial setting
 * @param id id or root object
 * @return success of root setting/creation
 */
RB_ObjectBase* RB_MmProxy::setRoot(const RB_String& id) {
    if (!database().isOpen() && !mParentManager && mModelFactory) {
        // top level modelmanager is based on root object (of modelfactory)
        getRbModel()->setRoot(mModelFactory->getRoot());
        return mModelFactory->getRoot();
    }
    return getRbModel()->setRoot(id);
}

/**
 * Set root based on object id (or pointer), for example: due to a newly
 * selected parent in a 'parent' model and this is the child model
 * @param root root object of this model
 */
void RB_MmProxy::setRoot(RB_ObjectBase* root) {
    getRbModel()->setRoot(root);

    // TODO: emit currentRowChanged(QModelIndex(), QModelIndex());
    // for in-memory models? See below.
}

/**
 * Set root, used to set root for new dialog and child models
 * @param index current index of parent model
 */
void RB_MmProxy::setRoot(const QModelIndex& index) {
    const RB_MmProxy* proxy = dynamic_cast<const RB_MmProxy*>(index.model());
    if (proxy) {
        getRbModel()->setRoot(proxy->mapToSource(index));
    } else {
        getRbModel()->setRoot(index);
    }

    emit currentRowChanged(QModelIndex(), QModelIndex());
}

/**
 * @return root object
 */
RB_ObjectBase* RB_MmProxy::getRoot() {
    return getRbModel()->getRoot();
}

/**
 * Save data of root object in database, does nothing when in memory model
 * @return success
 */
//bool RB_MmProxy::saveRoot() {
//    return getRbModel()->saveRoot();
//}

/**
 * @return project object
 */
RB_ObjectBase* RB_MmProxy::getProject() {
    RB_MmProxy* pm = this;
    while (pm->getParentManager()) {
        pm = pm->getParentManager();
    }

    RB_ObjectBase* obj = pm->getRbModel()->getRoot();

    if (obj) {
        RB_String objName = obj->getName();

        if (!objName.endsWith("Project", Qt::CaseSensitive)) {
            // Apparently the top model is not related to XXX_Project,
            // the 'relations' should be direct relation of the root XXX_Project
            RB_DEBUG->print(RB_Debug::D_ERROR,
                    "RB_MmProxy::getProject() "
                    "root name not XXX_Project ERROR");
            return NULL;
        }
    }

    return obj;
}

/**
 * @return parent proxy model manager
 */
RB_MmProxy* RB_MmProxy::getParentManager() {
    return mParentManager;
}

/**
 * Set parent proxy model manager
 */
void RB_MmProxy::setParentManager(RB_MmProxy* pMm) {
    if (mParentManager) {
        disconnect(mParentManager, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
                   this, SLOT(slotParentCurrentRowChanged(QModelIndex,QModelIndex)));
        disconnect(mParentManager, SIGNAL(modelBeforeSubmitted()),
                   this, SLOT(slotParentModelSubmitted()));
        disconnect(mParentManager, SIGNAL(modelReverted()),
                   this, SLOT(slotParentModelReverted()));

        disconnect(this, SIGNAL(modelModified(bool)),
                   mParentManager, SLOT(slotChildModelModified(bool)));
    }

    mParentManager = pMm;

    if (mParentManager) {
        connect(mParentManager, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
                this, SLOT(slotParentCurrentRowChanged(QModelIndex,QModelIndex)));
        connect(mParentManager, SIGNAL(modelBeforeSubmitted()),
                this, SLOT(slotParentModelSubmitted()));
        connect(mParentManager, SIGNAL(modelReverted()),
                this, SLOT(slotParentModelReverted()));

        connect(getRbModel(), SIGNAL(modelModified(bool)),
                mParentManager->sourceModel(), SLOT(slotChildModelModified(bool)));
    }
}

/**
 * Set object factory (for the creation of objects and the complete model)
 * @param f object factory
 */
void RB_MmProxy::setObjectFactory(RB_ObjectFactory* f) {
    if (!getRbModel()) return;
    getRbModel()->setObjectFactory(f);
}

/**
 * @return object factory which defines the entire model
 */
RB_ObjectFactory* RB_MmProxy::getObjectFactory() {
    if (!getRbModel()) return NULL;
    return getRbModel()->getObjectFactory();
}

/**
 * Initialize table model.
 * @param list name of container list and used as basis for table name
 * @param pMm parent model manager
 */
bool RB_MmProxy::setTableModel(const RB_String& list,
                                    RB_MmProxy* pMm) {
    // Map dataChanged() signal for table and treeviews
    connect(getRbModel(), SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            this, SLOT(slotDataChanged(QModelIndex,QModelIndex)));
    setParentManager(pMm);
//    mParentManager = pMm;
//    // Connect parent model row change to child model
//    if (mParentManager) {
//        connect(mParentManager, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
//                this, SLOT(slotParentCurrentRowChanged(QModelIndex,QModelIndex)));
//    }
    return getRbModel()->setTableModel(list);
}

/**
 * Initialize tree model and connect expanded signal from this proxy to the
 * expand slot of the source model
 * @param list name of container list and used as basis for table name
 * @param pMm parent model manager
 * @param db database connection
 */
bool RB_MmProxy::setTreeModel(const RB_String& list,
                                        RB_MmProxy* pMm,
                                        const QSqlDatabase& db) {

    RB_MmSource* mm = dynamic_cast<RB_MmSource*>(getRbModel());
    connect(this, SIGNAL(expanded(const QModelIndex&)),
            mm, SLOT(slotExpanded(const QModelIndex&)));
    // map dataChanged() signal for table and treeviews
    connect(getRbModel(), SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            this, SLOT(slotDataChanged(QModelIndex,QModelIndex)));
    setParentManager(pMm);
//    mParentManager = pMm;
//    // Connect parent model row change to child model
//    if (mParentManager) {
//        connect(mParentManager, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
//                this, SLOT(slotParentCurrentRowChanged(QModelIndex,QModelIndex)));
//    }
    return getRbModel()->setTreeModel(list, db);
}

/**
 * Set clone model. This function is called when a model needs to be cloned,
 * for example when edited items need to be canceled or a temporary/custom
 * model is required for special actions.
 * @param root root object for this model and current object of parent
 * @param list name of container list or table name in case of database
 * @param pMm parent model manager
 */
bool RB_MmProxy::setCloneModel(RB_ObjectBase* root,
                                    const RB_String& list,
                                    RB_MmProxy* pMm) {
    bool success = getRbModel()->setCloneModel(root, list);
    setParentManager(pMm);
//    mParentManager = pMm;
//    // Connect parent model row change to child model
//    if (mParentManager) {
//        connect(mParentManager, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
//                this, SLOT(slotParentCurrentRowChanged(QModelIndex,QModelIndex)));
//    }
    return success;
}

/**
 * Save clone model
 */
bool RB_MmProxy::saveCloneModel() {
    getRbModel()->setModelIsModified(true);
    return getRbModel()->saveCloneModel();
}

/**
 * Get object at current index
 * @param index
 * @return object. In case of database an object
 *          with relevant children will be created
 *          and does not need to be deleted after use
 */
RB_ObjectBase* RB_MmProxy::getObject(const QModelIndex& index,
                                     RB2::ResolveLevel level) {
    return getRbModel()->getObject(mapToSource(index), level);
}

/**
 * Get curren selected object
 * @return object. In case of database an object
 *          with relevant children will be created
 *          and does not need to be deleted after use
 */
RB_ObjectBase *RB_MmProxy::getCurrentObject() {
    return getRbModel()->getCurrentObject();
}

/**
 * Update current object. Works only if model current object and
 * obj have the same ID.
 * @brief RB_MmProxy::updateCurrentObject
 * @param obj
 */
void RB_MmProxy::updateCurrentObject(RB_ObjectBase *obj) {
    getRbModel()->updateCurrentObject(obj);
}

/**
 * Relocate an object (row) to another position in the model,
 * @param fromPosition from sequence or row number
 * @param toPosition to sequence or row number
 */
void RB_MmProxy::swapObject(const QModelIndex& fromIndex,
                            const QModelIndex& toIndex) {
    getRbModel()->swapObject(mapToSource(fromIndex), mapToSource(toIndex),
                             mapToSource(fromIndex.parent()));
}

/**
 * Promote item to grandparent in tree model
 * @param idx index of item
 * @returns index of item new position
 */
QModelIndex RB_MmProxy::promote(const QModelIndex& idx) {
    if (!isTreeModel() || !idx.isValid()) return idx;

    RB_MmAbstract* sourceModel = getRbModel();
    QModelIndex newIndex = mapFromSource(sourceModel->promote(mapToSource(idx)));
    return newIndex;
}

/**
 * Demote item to new parent (should be a sibling of item)
 * @param idx index of item
 * @param parentIdx index of new parent
 * @returns index of item new position
 */
QModelIndex RB_MmProxy::demote(const QModelIndex& idx, const RB_String& parentId) {
    if (!isTreeModel() || !idx.isValid()) return idx;

    RB_MmAbstract* sourceModel = getRbModel();
    QModelIndex newIndex = mapFromSource(sourceModel->demote(mapToSource(idx), parentId));
    return newIndex;
}

/**
 * Overrides base function to also correctly map indices of tree models
 * @param proxyIndex index from proxy model
 * @return index from source model
 */
QModelIndex RB_MmProxy::mapToSource(const QModelIndex& proxyIndex) const {
    return QSortFilterProxyModel::mapToSource(proxyIndex);
}

/**
 * Map source indices to relational indices including the hidden columns,
 * the relational delegate does not know of the hidden columns
 * @param proxyIndex index from proxy model
 * @return index from source model
 */
QModelIndex RB_MmProxy::mapFromSource(const QModelIndex& sourceIndex) const {
    return QSortFilterProxyModel::mapFromSource(sourceIndex);
}

/**
 * Slot receiving current row change from view, will set the source model
 * current row which sets mapper to and to emit the actual index mapped
 * to the source model.
 * @param current current index
 * @param previous previous index
 */
void RB_MmProxy::slotChangeCurrentRow(const QModelIndex& current,
                                  const QModelIndex& previous) {
    QModelIndex cur = mapToSource(current);
    QModelIndex prev = mapToSource(previous);

    getRbModel()->slotChangeCurrentRow(cur, prev);
    emit currentRowChanged(current, previous);
}

/**
 * Slot to update the model based on the row selected in the parent model,
 * not applicable for base clone model
 */
void RB_MmProxy::slotParentCurrentRowChanged(const QModelIndex& current,
                                const QModelIndex& /* previous */) {
    if (!getParentManager()) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "RB_MmProxy::slotParentCurrentRowChanged() "
                        "no parent manager ERROR");
        return;
    }

    if (getRbModel()->database().isOpen() && isModelModified()) {
        submitAll();
    }

    setRoot(current);
    select();

    // refresh related views, such as RB_DataWidgetMapper
    emit rootChanged();
    // HACK: select does not emit currentRowChanged if 'current' is not valid
    // or row count = 0
    if (!current.isValid() || rowCount() == 0) {
        emit currentRowChanged(QModelIndex(), QModelIndex());
    }
}

/**
 * @brief Parent model submitted to database and loses selection
 */
void RB_MmProxy::slotParentModelSubmitted() {
    submitAll();
    emit rootChanged();
}

void RB_MmProxy::slotParentModelReverted() {
    revertAll();
    emit rootChanged();
}

/**
 * Slot for source dataChanged() signal to map to proxy indices,
 * i.e.: minus hidden columns
 */
void RB_MmProxy::slotDataChanged(const QModelIndex& topLeft,
                                 const QModelIndex& bottomRight) {
    if (!topLeft.isValid() || !bottomRight.isValid() || !getRbModel()) return;

    QModelIndex tlIdx = QSortFilterProxyModel::mapFromSource(topLeft);
    QModelIndex brIdx = QSortFilterProxyModel::mapFromSource(bottomRight);
    emit this->dataChanged(tlIdx, brIdx);
}

/**
 * Set current model row, receives signal from datawidgetmapper.
 * Refer to getMapper(). NOTE: do not use with treeview mappers,
 * because parentIndex is undefined.
 * @param row
 */
void RB_MmProxy::slotSetCurrentModelRow(int row) {
    if (this->isTreeModel()) {
        RB_DEBUG->print(RB_Debug::D_INFORMATIONAL,
                        "RB_MmProxy::slotSetCurrentModelRow() "
                        "should not be used with treemodel INFO");
        return;
    }
    QModelIndex idx = index(row, 0, QModelIndex());
    getRbModel()->slotChangeCurrentRow(mapToSource(idx), QModelIndex());
    emit currentRowChanged(idx, QModelIndex());
}

/**
 * Slot receiving signals of other models have been updated.
 * In case the table is a relational tabel for this model,
 * the relational table will be re-populated calling select()
 * @param tableName name of the table that has been update with submitAll()
 */
void RB_MmProxy::slotRelModelUpdated(const QString& tableName) {
    if (tableName.isEmpty()) return;
    if (tableName.toLower() == getRbModel()->tableName()) return;

    int cols = columnCount();

    for (int relCol = RB2::HIDDENCOLUMNS - 1; relCol < cols; ++relCol) {
        QSqlTableModel* relModel = this->relationModel(relCol);

        if (relModel) {
            RB_String relTableName =  relModel->tableName();

            if (relTableName.toLower()== tableName.toLower()) {
                // HACK: next two lines are to repopulate the dictionary
                // after Qt 4.8.0 this should not be necessary anymore
                // RB_SqlRelation rel = this->relation(relCol);
                // this->setRelation(relCol, rel);
                // End HACK 2011-08-02, does not work in ACC

                relModel->select();
            }
        }
    }
}

void RB_MmProxy::reset() {
    getRbModel()->reset();
}

void RB_MmProxy::revert() {
    // Make sure children models are notified first
    emit modelReverted();
    getRbModel()->revert();
}

void RB_MmProxy::revertAll() {
    // Make sure children models are notified first
    emit modelReverted();
    getRbModel()->revertAll();
    // notify children models that nothing is selected
    emit currentRowChanged(QModelIndex(), QModelIndex());
    // extra for the selection model, does not emit any signal
    // not required for submitAll because a select() is done
    mSelectionModel->reset();
}

bool RB_MmProxy::submit() {
    return getRbModel()->submit();
}

/**
 * Submits all changes to the database. Does not do a select().
 *
 * The sorting is removed
 * if the number of rows in the sourcemodel is larger than 255
 * because only the first 255 rows are fetched in this method
 */
bool RB_MmProxy::submitAll() {
    // Make sure children models are notified first
    emit modelBeforeSubmitted();

    if (getRbModel()->rowCount() > 255) {
        undoFilterSort();
    }

    bool res = getRbModel()->submitAll();

    // Also emit the updated signal for relational tables to re-populate
    RB_ModelFactory* mf = getModelFactory();
    if (mf) {
        mf->slotModelUpdated(getRbModel()->tableName());
    }

    return res;
}

bool RB_MmProxy::submitAllAndSelect() {
    // Make sure children models are notified first
    emit modelBeforeSubmitted();

    bool success = getRbModel()->submitAllAndSelect();

//    if (mMapper) {
//        if (isTreeModel()) {
//            mMapper->setCurrentModelIndex(QModelIndex());
//        }
//        mMapper->slotClearWidgets();
//    }

    // TODO: there is probably more than one mapper possible 2014-05-01,
    // however this needs to be checked with PENG, multiple fields on line list.
    // Check all methods related
    RB_DataWidgetMapper* mapper;
    std::map<RB_String, RB_DataWidgetMapper*>::iterator iter;

    for (iter = mMapperMap.begin(); iter != mMapperMap.end(); ++iter) {
        mapper = iter->second;

        if (isTreeModel()) {
            mapper->setCurrentModelIndex(QModelIndex());
        }

        mapper->slotClearWidgets();
    }

    emit currentRowChanged(QModelIndex(), QModelIndex());
    return success;
}

/**
 * Fetch all data if available, not all databases support this feature
 * @param parent parent modelindex
 */
void RB_MmProxy::fetchAll(const QModelIndex& parent) {
    if (canFetchMore(parent)) {
        fetchMore(parent);
        fetchAll(parent);
    }
}

/**
 * @return index of current child of parent
 */
QModelIndex RB_MmProxy::index(int row, int column,
                                        const QModelIndex& parent) const {
    return QSortFilterProxyModel::index(row, column, parent);
}

/**
 * @return index of parent used for tree view (otherwise QModelIndex())
 */
QModelIndex RB_MmProxy::parent(const QModelIndex& index) const {
    return QSortFilterProxyModel::parent(index);
}

/**
 * @return true if parent has any children, false if not a tree view
 */
bool RB_MmProxy::hasChildren(const QModelIndex& parent) const {
    return QSortFilterProxyModel::hasChildren(parent);
}

/**
 * @return the number of rows in the source model.
 */
int RB_MmProxy::rowCount(const QModelIndex& parent) const {
    // HACK 2010-11-08: otherwise more lines are drawn in QTreeView, next line does not work
    // return QSortFilterProxyModel::rowCount(parent);
    if (isTreeModel()) {
        return getRbModel()->rowCount(mapToSource(parent));
    } else {
        return QSortFilterProxyModel::rowCount(parent);
    }

    return -1;
}

/**
 * @return the number of rows in the proxy model.
 */
//int RB_MmProxy::rowCountProxy(const QModelIndex& parent) const {
//    return QSortFilterProxyModel::rowCount(parent);
//}

/**
 * Number of columns in the source model, to be overloaded by the relevant number
 * of the actual model.
 * @return the number of columns
 */
int RB_MmProxy::columnCount(const QModelIndex& parent) const {
    return getRbModel()->columnCount(parent); // TODO mapToSource(parent)?
}

/**
 * Model data (RB_Variant) at 'column + hidden columns' and 'row'
 * @param index
 * @param role, display role, edit role, etc.
 * @return data
 */
RB_Variant RB_MmProxy::data(const QModelIndex& index, int role) const {
    QModelIndex sourceIndex = mapToSource(index);
    if (sourceIndex.isValid())
        return getRbModel()->data(sourceIndex, role);
    return RB_Variant();
}

/**
 * Model hidden data (RB_Variant) at 'column' and 'row', such as id, parent, name
 * @param index
 * @param role, display role, edit role, etc.
 * @return data
 */
RB_Variant RB_MmProxy::hiddenData(const QModelIndex& index, int role) const {
    if (!isTreeModel()) {
        return QSortFilterProxyModel::data(index, role);
    }

    QModelIndex sourceIndex = mapToSource(index);
    if (sourceIndex.isValid())
        return getRbModel()->hiddenData(sourceIndex, role);
    return RB_Variant();
}

/**
 * Set data in model at index (row and column + hidden columns)
 * @param index Index contains the row and column of the value to be set
 * @param value Value to be set at index
 * @param role Role type such as EditRole
 * @return success of setting the data in the model
 */
bool RB_MmProxy::setData(const QModelIndex& index,
                          const QVariant& value, int role) {
    QModelIndex sourceIndex = mapToSource(index);
    if (sourceIndex.isValid())
        return getRbModel()->setData(sourceIndex, value, role);
    return false;
}

/**
 * Set hidden data in model at index (row and column), such as id, parent, name
 * @param index Index contains the row and column of the value to be set
 * @param value Value to be set at index
 * @param role Role type such as EditRole
 * @return success of setting the data in the model
 */
bool RB_MmProxy::setHiddenData(const QModelIndex &index,
                          const QVariant &value, int role) {
    QModelIndex sourceIndex = mapToSource(index);
    if (sourceIndex.isValid())
        return getRbModel()->setHiddenData(sourceIndex, value, role);
    return false;
}

/**
 * Get display role for column
 * @param col column
 * @returns RB2::RoleType
 */
int RB_MmProxy::getDisplayRole(int col) const {
    return getRbModel()->getDisplayRole(col);
}

/**
 * Set display role for column
 * @param col column
 * @param role RB2::RoleType
 */
void RB_MmProxy::setDisplayRole(int col, int role) {
    getRbModel()->setDisplayRole(col, role);
}

/**
 * @returns text list in case of fixed texts in a combobox
 */
RB_StringList RB_MmProxy::getTextList(int col) const {
    return getRbModel()->getTextList(col);
}

/**
 * Set text for display role of integer column
 * @param col column
 * @param strL list with texts from the combobox
 */
void RB_MmProxy::setTextList(int col, const RB_StringList& strL) {
    getRbModel()->setTextList(col, strL);
}

/**
 * Get header data for horizontal only. Row header gives only row number.
 * Function to be overloaded to show the relevant header data.
 * @return row and column header data
 */
RB_Variant RB_MmProxy::headerData(int section, Qt::Orientation orientation,
                                    int role) const {
    return QSortFilterProxyModel::headerData(section, orientation, role);
}

/**
 * Reimplementation of the base sort() function, because the hidden columns
 * need to be added to the columnnummer as clicked in the table header
 * @param column sequence number of the column as clicked on the table header
 * @param order sort order
 */
void RB_MmProxy::sort(int column, Qt::SortOrder order) {
    QSortFilterProxyModel::sort(column, order);
}


/**
 * Get the source index of the field fieldName
 * @param fieldName field name of the column
 * @returns the index or column number for fieldName
 */
int RB_MmProxy::fieldIndex(const QString& fieldName) const {
    return getRbModel()->fieldIndex(fieldName);
}

/**
 * @returns the relation object for the (visible) column
 */
QSqlRelation RB_MmProxy::relation(int column) const {
    return getRbModel()->relation(column);
}

/**
 * Set relation for column.
 * @param column
 * @param relation
 * @param relWhere where clause for relation table
 */
void RB_MmProxy::setRelation(int column, const RB_SqlRelation& relation) {
    if (getRbModel()->database().isOpen()) {
        getRbModel()->setRelation(column, relation, NULL);
    } else {
        getRbModel()->setRelation(column, relation, getProject());
    }

    // If relational table is changed update the relationModel
    RB_ModelFactory* mf = getModelFactory();
    if (mf) {
        connect(mf, SIGNAL(modelUpdated(const QString&)),
                this, SLOT(slotRelModelUpdated(const QString&)));
    }
}

/**
 * Returns a QSqlTableModel object for accessing the table for
 * which column is a foreign key, or 0 if there is no relation for
 * the given column. The returned object is owned by the
 * QSqlRelationalTableModel.
 * @param column
 */
QSqlTableModel* RB_MmProxy::relationModel(int column) const {
    return getRbModel()->relationModel(column);
}

/**
 * Populates the model with data from the table that was set via setTable(),
 * using the specified filter and sorting order, and returns true if successful
 * otherwise returns false. When the number of rows are above 255 the sorting
 * is removed.
 */
bool RB_MmProxy::select() {
    bool res = getRbModel()->select();

    if (getRbModel()->rowCount() > 255) {
        sort(-1);
    }

    return res;
}

/**
 * Removes all objects of in-memory model, does nothing with database model
 */
void RB_MmProxy::clear() {
    getRbModel()->clear();
    QSortFilterProxyModel::invalidate();
}

/**
 * Pass expanded signal to source model
 * @param index index of item which is expanded
 */
void RB_MmProxy::slotExpanded(const QModelIndex& index) {
    emit expanded(mapToSource(index));
}

/**
 * @return source model as RB_MmAbstract
 */
RB_MmAbstract* RB_MmProxy::sourceModel() const {
    return getRbModel();
}

/**
 *
 */
QObject* RB_MmProxy::sender() const {
    return QSortFilterProxyModel::sender();
}

/**
 * @return database
 */
QSqlDatabase RB_MmProxy::database() const {
    return getRbModel()->database();
}

/**
 * To keep track of model changes for considering save action at exit of
 * the application
 * @returns true if model is modified
 */
bool RB_MmProxy::isModelModified() {
    return getRbModel()->isModelModified();
}

/**
 * Set model is modified flag, also sets parent model modified flag
 * @param modified true if model is modified
 */
void RB_MmProxy::setModelIsModified(bool modified) {
    if (modified && getParentManager()) {
        getParentManager()->setModelIsModified(true);
    }
    getRbModel()->setModelIsModified(modified);
}

/**
 * @return Is this a tree model
 */
bool RB_MmProxy::isTreeModel() const {
    return getRbModel()->isTreeModel();
}

bool RB_MmProxy::isInMemoryModel() const {
    return getRbModel()->isInMemoryModel();
}

void RB_MmProxy::setInMemoryModel(bool inMemory) {
    return getRbModel()->setInMemoryModel(inMemory);
}

/**
 * Set source filter
 * @param sourceFilter filter text after WHERE such as (WHERE) parent=3
 */
void RB_MmProxy::setSourceFilter(const RB_String& sourceFilter) {
    getRbModel()->setFilter(sourceFilter);
}

/**
 * Get source filter
 * @return filter text is text after WHERE in SQL
 */
RB_String RB_MmProxy::getSourceFilter() {
    return getRbModel()->filter();
}

/**
 * Sets the primary parent id name. The parent model id will be used to filter
 * on the parent id name. The defaul parent field name is 'parent'.
 * @param primParent primary parent name
 */
void RB_MmProxy::setPrimaryParent(const RB_String& primParent) {
    getRbModel()->setPrimaryParent(primParent);
}

/**
 * Sets the second parent. The parent model id will now be used to filter
 * on the second parent id not the parent id which is the direct object model
 * situation. This is used when the child object parent in the model is not
 * the same as the used parent model but only has a relation to the child.
 * The second parent id ends with '_id'. Only works with database table models.
 * @param secParent second parent id
 */
void RB_MmProxy::setSecondParent(const RB_String& secParent) {
    getRbModel()->setSecondParent(secParent);
}

/**
 * Set where statement in addition to parent and second parent.
 * If root ID is empty, only this statement will be used for filter,
 * if both are empty, WHERE id<>'0' will be used. Does not work (yet)
 * for treemodels
 * @param whereStatement additional where statement in SQL query
 * @param select is default true to execute the create where statement
 * and execute select()
 */
void RB_MmProxy::setWhere(const RB_String& whereStatement, bool select) {
    getRbModel()->setWhere(whereStatement, select);
}

/**
 * Set query of source model
 * @param q query
 */
void RB_MmProxy::setQuery(const QSqlQuery &q) {
    getRbModel()->setSqlQuery(q);
}

/**
 * @returns sort order of source model filter, ascending, descending or none.
 */
RB2::SortOrderType RB_MmProxy::getSourceSortOrder() {
    return getRbModel()->getSortOrder();
}

/**
 * Set sorting of source model
 * @param so sort order ascending or descending
 * @param colName1 first column name to be sorted
 * @param colName2 second column name to be sorted (optional)
 * @param colName3 third column name to be sorted (optional)
 */
void RB_MmProxy::setSourceSortOrder(RB2::SortOrderType so,
                                const RB_String& colName1,
                                const RB_String& colName2,
                                const RB_String& colName3) {
    getRbModel()->setSortOrder(so, colName1, colName2, colName3);
}

/**
 *
 *
 */
void RB_MmProxy::setShared(bool shared) {
    getRbModel()->setShared(shared);
}

/**
 * Filter with wildcard and caseinsensitive, refer Qt Custom Sort/Filter Model
 * for more advanced options
 * @param text filter text
 * @param fieldName column or field name
 * @param matchCase match case
 */
void RB_MmProxy::setUserFilter(const RB_String& text, const RB_String& fieldName,
                               bool matchCase) {
    invalidateFilter();

    if (!fieldName.isEmpty()) {
        setFilterKeyColumn(fieldIndex(fieldName));
    }
    Qt::CaseSensitivity mCase = matchCase ? Qt::CaseSensitive : Qt::CaseInsensitive;
    QRegExp regExp(text, mCase, QRegExp::Wildcard);
    setFilterRegExp(regExp);
}

QRegExp RB_MmProxy::getUserFilterExpression() {
    return filterRegExp();
}

QString RB_MmProxy::getUserFilterFieldName() {
    int section = filterKeyColumn();
    if (section < 0) return ""; // All columns
    return headerData(section, Qt::Horizontal, Qt::DisplayRole).toString();
}

/**
 * Remove filter, uses reset()
 */
void RB_MmProxy::removeUserFilter() {
    // reset(); TODO: does not work properly 2010-08-06
    // Hack:
    QRegExp regExp("", Qt::CaseInsensitive, QRegExp::Wildcard);
    setFilterRegExp(regExp);
}

/**
 * Remove filter and sorting of proxymodel
 */
void RB_MmProxy::undoFilterSort() {
    invalidate();
    sort(-1);
}

/**
 * Set flag that this is the original base clone model to differentate from
 * child models. The child models will use the objects from this model
 * which are already cloned and do not need to be cloned again. Also deletion
 * of the cloned objects is only done by the the base clone model.
 * @param modelIsBaseClone
 */
void RB_MmProxy::setBaseCloneModel(bool modelIsBaseClone) {
    getRbModel()->setBaseCloneModel(modelIsBaseClone);
}
/**
 * @return Is this the base clone model
 */
bool RB_MmProxy::isBaseCloneModel() {
    return getRbModel()->isBaseCloneModel();
}

/**
 * Create new mapper, will be deleted when this model is deleted (via QObject)
 * @param mapperId ID of the mapper
 * @return data widget mapper
 */
RB_DataWidgetMapper* RB_MmProxy::getMapper(const RB_String& mapperId) {
    RB_DataWidgetMapper* mapper;
    std::map<RB_String, RB_DataWidgetMapper*>::iterator iter;
    iter = mMapperMap.find(mapperId);

    if (iter != mMapperMap.end()) {
        mapper = iter->second;
    } else {
        mapper = new RB_DataWidgetMapper(this);
        mapper->setModel(this);
        // model changed currentIndex, change row/index of mMapper next
        connect(this, SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
                mapper, SLOT(setCurrentModelIndex(QModelIndex)));
        // root of model changed, clean mapped widgets
        connect(this, SIGNAL(rootChanged()),
                mapper, SLOT(slotClearWidgets()));
        mMapperMap[mapperId] = mapper;
    }

    return mapper;
}
//RB_DataWidgetMapper* RB_MmProxy::getMapper() {
//    if (!mMapper) {
//        mMapper = new RB_DataWidgetMapper(this);
//        mMapper->setModel(this);
//        // model changed currentIndex, change row/index of mMapper next
//        connect(this, SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
//                mMapper, SLOT(setCurrentModelIndex(QModelIndex)));
//        // root of model changed, clean mapped widgets
//        connect(this, SIGNAL(rootChanged()),
//                mMapper, SLOT(slotClearWidgets()));
//    }

//    return mMapper;
//}

void RB_MmProxy::deleteMapper(const RB_String& mapperId) {
    RB_DataWidgetMapper* mapper;
    std::map<RB_String, RB_DataWidgetMapper*>::iterator iter;
    iter = mMapperMap.find(mapperId);

    if (iter != mMapperMap.end()) {
        mapper = iter->second;
        disconnect(this, SIGNAL(rootChanged()),
                   mapper, SLOT(slotClearWidgets()));
        disconnect(this, SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
                   mapper, SLOT(setCurrentModelIndex(QModelIndex)));

        delete mapper;
        mapper = NULL;
        mMapperMap.erase(mapperId);
    } else {
        RB_DEBUG->error("RB_MmProxy::deleteMapper() "
                        "delete none existing mapper ERROR");
    }

//    if (mMapper) {
//        disconnect(this, SIGNAL(rootChanged()),
//                   mMapper, SLOT(slotClearWidgets()));
//        disconnect(this, SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
//                   mMapper, SLOT(setCurrentModelIndex(QModelIndex)));

//        delete mMapper;
//        mMapper = NULL;
//    } else {
//        RB_DEBUG->error("RB_MmProxy::deleteMapper() "
//                        "delete none existing mapper ERROR");
//    }
}

/**
 * @return model casted to RB_MmAbstract()
 */
RB_MmAbstract* RB_MmProxy::getRbModel() const {
    return dynamic_cast<RB_MmAbstract*>(QSortFilterProxyModel::sourceModel());
}

/**
 * Get current or last actual selected proxy index
 * @return index
 */
QModelIndex RB_MmProxy::getProxyIndex() const {
    if (!mSelectionModel) {
        RB_DEBUG->warning("RB_MmProxy::getProxyIndex(), "
                          "mSelectionModel is NULL WARNING");
        return mapFromSource(getRbModel()->getCurrentIndex());
    }
    return mSelectionModel->currentIndex();
}

/**
 * Get current or last actual selected index (not proxy)
 * TODO: change to getSourceIndex()
 * @return index
 */
QModelIndex RB_MmProxy::getCurrentIndex() const {
    return getRbModel()->getCurrentIndex();
}

/**
 * Get current or last actual selected ID
 * @return ID
 */
RB_String RB_MmProxy::getCurrentId() const {
    QModelIndex currentProxyIdx = getProxyIndex();
    if (!currentProxyIdx.isValid()) {
        return "NULL";
    }
    QModelIndex idProxyIdx = index(currentProxyIdx.row(), 0, currentProxyIdx.parent());
    return hiddenData(idProxyIdx, Qt::DisplayRole).toString();
}

/**
 * @return list name, is table name with 'List'
 */
RB_String RB_MmProxy::getListName() const {
    return getRbModel()->getListName();
}

/**
 * Set shared selection model
 * @param selModel selection model
 */
void RB_MmProxy::setSelectionModel(QItemSelectionModel* selModel) {
    mSelectionModel = selModel;
}

/**
 * @returns shared selection model
 */
QItemSelectionModel* RB_MmProxy::getSelectionModel() {
    return mSelectionModel;
}

/**
 * Get current value of named field
 * @param fieldName field of which value is to be returned
 * @param role for relational fields use Qt::UserRole+1
 */
RB_Variant RB_MmProxy::getCurrentValue(const RB_String& fieldName, int role) const {
    QModelIndex idx = getProxyIndex();
    return data(index(idx.row(), fieldIndex(fieldName), idx.parent()), role);
}

/**
 * Set current value of named field
 * @param fieldName field of which value is to be returned
 * @param val value
 * @param role for relational fields use Qt::UserRole+1
 */
bool RB_MmProxy::setCurrentValue(const RB_String& fieldName, const RB_Variant& val,
                                 int role) {
    QModelIndex idx = getProxyIndex();
    return setData(index(idx.row(), fieldIndex(fieldName), idx.parent()), val, role);
}

/**
 * Get value of field
 * 1- at id or
 * 2- at parent and keyfieldname with keyvalue. If id is an empty string
 *    than all in the table are search for keyFieldName with keyValue
 * @param id ID or parentID if keyFieldName and keyValue are not empty
 * @param fieldName field from which the value is to be returned
 * @param keyFieldName name of key field
 * @param keyValue value of key field
 * @return value of fieldName
 */
RB_Variant RB_MmProxy::getValue(const RB_String& id,
                                          const RB_String& fieldName,
                                          const RB_String& keyFieldName,
                                          const RB_Variant& keyValue) const {
    return getRbModel()->getValue(id, fieldName, keyFieldName, keyValue);
}

/**
 * Set value of field at id
 * @param id ID
 * @param fieldName field from which the value is to be set
 * @param value value
 * @param role default Qt::UserRole + 1 setting the actual value
 *          not the relational value
 */
bool RB_MmProxy::setValue(const RB_String& id, const RB_String& fieldName,
                                const RB_Variant& value, int role) {
    return getRbModel()->setValue(id, fieldName, value, role);
}

