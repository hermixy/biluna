/*****************************************************************
 * $Id: rb_mmabstract.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Aug 14, 2009 6:34:25 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna db project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_MMABSTRACT_H
#define RB_MMABSTRACT_H

#include <QMap>
#include <QtSql>
#include <QStringList>
#include "db_global.h"
#include "rb.h"
#include "rb_sqlrelation.h"
#include "rb_string.h"
#include "rb_stringlist.h"
#include "rb_variant.h"

class RB_DataWidgetMapper;
class RB_MmSource;
class RB_ModelFactory;
class RB_ObjectBase;
class RB_ObjectContainer;
class RB_ObjectFactory;



/**
 * Abstract base model manager class for the Biluna specific functions. The
 * model managers also inherit from QAbstractItemModel directly for the
 * in memory model manager and indirectly via QSqlRelationalTableModel for the
 * database model manager, resp. RB_MmObject and RB_DmObject.
 */
class DB_EXPORT RB_MmAbstract : public QSqlRelationalTableModel   {

    Q_OBJECT

public:
    RB_MmAbstract(QObject* parent = 0, const QSqlDatabase& db = QSqlDatabase())
            : QSqlRelationalTableModel(parent, db) { }

    virtual ~RB_MmAbstract() { }

    virtual RB_ObjectBase* setRoot(const RB_String& id = "") = 0;
    virtual void setRoot(RB_ObjectBase* root) { mRoot = root; }
    virtual void setRoot(const QModelIndex& index) = 0;
    virtual RB_ObjectBase* getRoot() { return mRoot; }

    // virtual bool saveRoot() = 0; // only for database
    // virtual RB_ObjectContainer* getProject() = 0;
    // virtual void setParentManager(RB_MmAbstract* p) { mParentModel = p; }
    // virtual RB_MmAbstract* getParentManager() { return mParentModel; }

    virtual void setListName(const RB_String& listName) { mListName = listName; }
    virtual RB_String getListName() const { return mListName; }
    virtual void setObjectFactory(RB_ObjectFactory* f) { mObjectFactory = f; }
    virtual RB_ObjectFactory* getObjectFactory() { return mObjectFactory; }
    virtual void setNumberOfColumns(int count) { mNoColumns = count; }

    virtual bool setTableModel(const RB_String& /*list*/) = 0;
    virtual bool setTreeModel(const RB_String& /*list*/, const QSqlDatabase& /*db*/) = 0;

//    virtual bool setModel(const RB_String& id, const RB_String& list) = 0;
    virtual bool setCloneModel(RB_ObjectBase* /*root*/, const RB_String& /*list*/) = 0;
    virtual bool saveCloneModel() = 0;

    virtual RB_ObjectBase* getObject(const QModelIndex& /*current*/,
                                     RB2::ResolveLevel level = RB2::ResolveAll) = 0;
    virtual RB_ObjectBase* getBaseObject() = 0;
    virtual RB_ObjectBase* getCurrentObject() = 0;
    virtual void updateCurrentObject(RB_ObjectBase* /*obj*/) { }
    virtual QModelIndex promote(const QModelIndex& index) = 0;
    virtual QModelIndex demote(const QModelIndex& idx,
                               const RB_String& parentId) = 0;
//    virtual int getCurrentRow() = 0;
    virtual void swapObject(const QModelIndex& /*fromIndex*/,
                            const QModelIndex& /*toIndex*/,
                            const QModelIndex& /*parent*/) = 0;

    virtual RB_Variant hiddenData(const QModelIndex& index,
                                  int role = Qt::DisplayRole) const = 0;
    virtual int getDisplayRole(int /*col*/) const { return -1; }
    virtual void setDisplayRole(int /*col*/, int /*role*/) { }
    virtual RB_StringList getTextList(int /*col*/) const { return RB_StringList(); }
    virtual void setTextList(int /*col*/, const RB_StringList& /*strL*/) { }

    virtual bool setHiddenData(const QModelIndex& index, const RB_Variant &value,
                               int role = Qt::EditRole) = 0;
    virtual bool copyCurrentRow() = 0;

    // QSqlRelationalTableModel functions, to be overriden for in-memory model
    virtual int fieldIndex(const QString& /*fieldName*/) const { return -1; }
    virtual QModelIndex parent(const QModelIndex& /*index*/) const { return QModelIndex(); }
    virtual bool hasChildren(const QModelIndex& /*parent*/) const { return false; }
    using QSqlRelationalTableModel::setRelation;
    virtual void setRelation(int /*column*/, const RB_SqlRelation& /*relation*/,
                             RB_ObjectBase* /*project, const RB_String& relWhere */) = 0;

    virtual void setBaseCloneModel(bool baseClone) { mBaseClone = baseClone; }
    virtual bool isBaseCloneModel() { return mBaseClone; }
    virtual void setModelIsModified(bool modified) {
        mModified = modified;
        emit modelModified(modified);
    }
    virtual bool isModelModified() { return mModified; }
    virtual bool isTreeModel() const { return mIsTreeModel; }
    virtual bool isInMemoryModel() const { return mIsInMemoryModel; }
    virtual void setInMemoryModel(bool inMemory) { mIsInMemoryModel = inMemory; }
    virtual QSqlDatabase database() const = 0;
    virtual void reset() = 0;
//    virtual RB_DataWidgetMapper* getMapper() = 0;
    virtual QModelIndex getCurrentIndex() const = 0;
    virtual RB_String getCurrentId() const = 0;
    virtual RB_Variant getCurrentValue(const RB_String& fieldName,
                                       int role) const = 0;
    virtual bool setCurrentValue(const RB_String& fieldName, const RB_Variant& val,
                                 int role) = 0;
    virtual RB_Variant getValue(const RB_String& id, const RB_String& fieldName,
                                const RB_String& keyFieldName = "",
                                const RB_Variant& keyValue = RB_Variant(),
                                int role = (Qt::UserRole + 1)) const = 0;
    virtual bool setValue(const RB_String& id, const RB_String& fieldName,
                          const RB_Variant& value, int role) = 0;
    virtual void setPrimaryParent(const RB_String& primParent) { mPrimaryParent = primParent; }
    virtual void setSecondParent(const RB_String& secParent) { mSecondParent = secParent; }
    virtual void setWhere(const RB_String& whereStatement, bool select) = 0;
    virtual void setSqlQuery(const QSqlQuery& q) = 0;

    virtual RB2::SortOrderType getSortOrder() = 0;
    virtual void setSortOrder(RB2::SortOrderType so,
                              const RB_String& colName1,
                              const RB_String& colName2 = "",
                              const RB_String& colName3 = "") = 0;
    virtual void setShared(bool shared) = 0;
    virtual bool isShared() = 0;

public slots:
    virtual void slotChildModelModified(bool modified) {
        setModelIsModified(modified);
    }
    virtual void slotChangeCurrentRow(const QModelIndex&, const QModelIndex&) = 0;
    virtual void revert() = 0;
    virtual bool submit() = 0;
    virtual bool submitAll() = 0;
    virtual bool submitAllAndSelect() = 0;

signals:
    void modelModified(bool modified);
    void currentRowCopied(const QString& copyId);

protected:
    //! Root object of this model
    RB_ObjectBase* mRoot;
    //! Container list name (same as database table name + List)
    RB_String mListName;
    //! Table name (same as container list name - List)
    RB_String mTableName;
    //! Table name with previous values (same table name + Prev)
    RB_String mTableNamePrev;
    //! Object factory
    RB_ObjectFactory* mObjectFactory;
    //! Base clone model flag, if true deletes in destructor model root
    bool mBaseClone;
    //! Model is modified flag, if true model is modified
    bool mModified;
    //! Tree model flag, true if tree model
    bool mIsTreeModel;
    //! In-memory model flag, true if in-memory model
    bool mIsInMemoryModel;
    //! Current connected database for workspace or standards
    RB_String mDatabaseConnection;
    //! Number of columns minus hidden 3 (minus id, parent, name etc.)
    int mNoColumns;
    //! True if this model is a shared model
    bool mShared;

    //! Tree SELECT SQL statement, not required for table
    RB_String mTreeSelectSql;
    //! Primary parent name
    RB_String mPrimaryParent;
    //! Second parent name
    RB_String mSecondParent;
    //! Second parent/relational id, used in setRoot(id)
    RB_String mSecondParentId;
    //! Additional WHERE fields in sql statement
    RB_String mWhere;
    //! Sort- and fetch order of model
    RB2::SortOrderType mSortOrder;
    //! Sort column1 name
    RB_String mSortColumnName1;
    //! Sort column2 name
    RB_String mSortColumnName2;
    //! Sort column3 name
    RB_String mSortColumnName3;

};

#endif /*RB_MMABSTRACT_H*/
