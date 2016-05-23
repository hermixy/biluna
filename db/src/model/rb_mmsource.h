/*****************************************************************
 * $Id: rb_mmsource.h 2248 2015-06-21 09:13:00Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna db project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_MMSOURCE_H
#define RB_MMSOURCE_H

#include <QAbstractItemModel>
#include <QItemSelection>
#include "rb.h"
#include "rb_datawidgetmapper.h"
#include "rb_mmabstract.h"
#include "rb_objectatomic.h"
#include "rb_objectcontainer.h"
#include "rb_objectfactory.h"

class RB_ModelFactory;

/**
 * Internal-Memory and database model manager or Model View Controler
 *
 * Basic implementation of the model view controller or Model Manager. To be
 * used as basis for all model managers of different applications such as LDT
 * and EQL. Methods and functions can be inherited, added or overriden by new
 * implementations. Certain functions are necessary to interface correctly
 * between the various applications. The JOB application will receive from EQL
 * and LDT a similar model manager for the selection lists of an equipment list
 * or a line designation table.
 */
class DB_EXPORT RB_MmSource : public RB_MmAbstract {

    Q_OBJECT

public:
    RB_MmSource(QObject* parent = 0, const QSqlDatabase& db = QSqlDatabase(),
                int hiddenColumns = 0);
    virtual ~RB_MmSource();

    virtual RB_ObjectBase* setRoot(const RB_String& id);
    virtual void setRoot(RB_ObjectBase* root = NULL);
    virtual void setRoot(const QModelIndex& index);
    virtual RB_ObjectBase* getRoot();

    virtual bool setTableModel(const RB_String& list);
    virtual bool setTreeModel(const RB_String& list, const QSqlDatabase& db);

    virtual bool setCloneModel(RB_ObjectBase* root, const RB_String& list);
    virtual bool saveCloneModel();

    virtual RB_ObjectBase* getObject(const QModelIndex& currentIndex,
                                     RB2::ResolveLevel level = RB2::ResolveAll);
    virtual RB_ObjectBase* getBaseObject();
    virtual RB_ObjectBase* getCurrentObject();
    virtual void updateCurrentObject(RB_ObjectBase* obj);
    virtual void swapObject(const QModelIndex& fromIndex,
                            const QModelIndex& toIndex,
                            const QModelIndex& parent);
    virtual QModelIndex promote(const QModelIndex& index);
    virtual QModelIndex demote(const QModelIndex& idx,
                               const RB_String& parentId);

    // Minimum model view controler functions as required in Qt4 for a
    // an editable table model which can also be a relation table model,
    // sort filter model and/or tree model
    virtual QModelIndex index(int row, int column,
                            const QModelIndex& parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &index) const;
    virtual bool hasChildren(const QModelIndex& parent) const;

    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;

    virtual RB_Variant data(const QModelIndex& index,
                            int role = Qt::DisplayRole) const;
    virtual RB_Variant hiddenData(const QModelIndex& index,
                                  int role = Qt::DisplayRole) const;
    virtual int getDisplayRole(int col) const;
    virtual void setDisplayRole(int col, int role);
    virtual RB_StringList getTextList(int col) const;
    virtual void setTextList(int col, const RB_StringList& strL);

    virtual bool setData(const QModelIndex& index, const RB_Variant& value,
                         int role = Qt::EditRole);
    virtual void printErrorSetData(const QModelIndex& index,
                                   const RB_Variant& value,
                                   int role = Qt::EditRole);
    virtual bool setHiddenData(const QModelIndex& index, const RB_Variant &value,
                               int role = Qt::EditRole);
    virtual RB_Variant headerData(int section, Qt::Orientation orientation,
                            int role = Qt::DisplayRole) const;

    virtual bool insertRows(int row, int count,
                            const QModelIndex& parent = QModelIndex());
    virtual bool copyCurrentRow();
    virtual bool removeRows(int row, int count,
                            const QModelIndex& parent = QModelIndex());
    virtual bool removeRelatedRow(const RB_String& id);

    virtual Qt::ItemFlags flags(const QModelIndex& index) const;
    virtual int fieldIndex(const QString& fieldName) const;
    virtual QSqlRelation relation(int column) const;
    virtual void setRelation(int column, const RB_SqlRelation& relation,
                             RB_ObjectBase* project = NULL/*,
                             const QString &relWhere = ""*/);
    virtual QSqlTableModel* relationModel (int column) const;
    virtual bool select();
    virtual void clear();

    virtual QSqlDatabase database() const;
    virtual void reset();
    ////

    virtual void setModelStructure();
    virtual void unsetModelStructure();

    virtual int getRow(const RB_String& member, const RB_Variant& value);

    virtual QModelIndex getCurrentIndex() const;
    virtual RB_String getCurrentId() const;
    virtual RB_Variant getCurrentValue(const RB_String& fieldName,
                                       int role = Qt::DisplayRole) const;
    virtual bool setCurrentValue(const RB_String& fieldName, const RB_Variant& val,
                                 int role);
    virtual RB_Variant getValue(const RB_String& id, const RB_String& fieldName,
                                const RB_String& keyFieldName = "",
                                const RB_Variant& keyValue = RB_Variant(),
                                int role = (Qt::UserRole + 1)) const ;
    virtual bool setValue(const RB_String& id, const RB_String& fieldName,
                          const RB_Variant& value, int role);

    virtual void setWhere(const RB_String& whereStatement, bool isSelect = true);
    virtual void setSqlQuery(const QSqlQuery& q);

    virtual RB2::SortOrderType getSortOrder();
    virtual void setSortOrder(RB2::SortOrderType so,
                              const RB_String& colName1,
                              const RB_String& colName2 = "",
                              const RB_String& colName3 = "");

    virtual void setShared(bool shared) { mShared = shared; }
    virtual bool isShared() { return mShared; }

    // Used by RB_ModelFactory
    const static int HiddenColumns = RB2::HIDDENCOLUMNS;

public slots:
    virtual void slotExpanded(const QModelIndex& index);
    virtual void slotChangeCurrentRow(const QModelIndex& current,
                                  const QModelIndex& previous);
    virtual void slotCopyRows(const QString& parentId);
    virtual void revert();
    virtual void revertAll();
    virtual bool submit();
    virtual bool submitAll();
    virtual bool submitAllAndSelect();

signals:
    void currentRowChanged(const QModelIndex& current);

protected:
    virtual RB_Variant hiddenTableData(const QModelIndex& index,
                                      int role = Qt::DisplayRole) const;
    virtual RB_Variant hiddenTreeData(const QModelIndex& index,
                                      int role = Qt::DisplayRole) const;

private:
    RB_Variant objectData(const QModelIndex& index, int role) const;
    bool setObjectData(const QModelIndex& index, const RB_Variant& value, int role);
    void setRelationalValue(int col, RB_Variant& val) const;

    void createTableSortFilter(RB_String& filter);
    void createTableFilter();

    void createTreeSortFilter(RB_String& sqlStr);
    void createTreeSelectSql();
    void createTree();
    void createSubTree(RB_ObjectBase* obj);

    //! For temporary storage of root, for example in case of clone model
    RB_ObjectBase* tmpRoot;
    //! An object used for header data, member count and current object data
    RB_ObjectBase* mObject;
    //! List of relational in-memory models for comboboxes
    std::map<int, RB_MmSource*> mRelationalModelMap;
    //! Current or last selected index
    QModelIndex mCurrentIndex;

    RB_String mLightBlue;
    RB_String mLightGray;
    RB_String mLightGreen;
    RB_String mLightRed;
    RB_String mLightYellow;

    //! Number of internal columns not to be shown, such id, parent, name
    int mHiddenColumns;
    //! Flag for not doing a select() if submitAll() is in progress
    bool mIsSubmitAllInProgress;

    //! Data format roles
    std::map<int, int> mRoleMap;

    //! Integer to textlist map, for fields where the text is fixed not relational table
    std::map<int, RB_StringList> mIntegerToTextList;

};

#endif /*RB_MMSOURCE_H*/
