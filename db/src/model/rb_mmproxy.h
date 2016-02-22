/*****************************************************************
 * $Id: rb_mmproxy.h 2248 2015-06-21 09:13:00Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna db project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_MMPROXY_H
#define RB_MMPROXY_H

#include <QSortFilterProxyModel>
#include <QtSql>

#include "rb_mmsource.h"
#include "rb_objectatomic.h"
#include "rb_objectcontainer.h"
#include "rb_objectfactory.h"

class RB_ModelFactory;


/**
 * Proxy model view controller or Model Manager. This proxy model
 * defines the minimum required methods for the model managers. The interface
 * is used to pass model managers from one lib or application to the other. For
 * example the JOB application has no knowledge about EQL (equipment) or LDT
 * (pipe line list). But EQL and LDT can pass JOB the model manager of each of
 * the applications and fill a selection dialog with all equipment numbers or
 * line numbers.
 */
class DB_EXPORT RB_MmProxy : public QSortFilterProxyModel {

    Q_OBJECT

public:
    RB_MmProxy(QObject* parent = 0);
    virtual ~RB_MmProxy();

    virtual int getModelType() const;
    virtual void setModelType(int type);
    virtual RB_ModelFactory* getModelFactory();
    virtual void setModelFactory(RB_ModelFactory* mf);

    virtual RB_ObjectBase* setRoot(const RB_String& id);
    virtual void setRoot(RB_ObjectBase* root = NULL);
    virtual void setRoot(const QModelIndex& index);
    virtual RB_ObjectBase* getRoot();

    virtual RB_ObjectBase* getProject();
    virtual RB_MmProxy* getParentManager();
    virtual void setParentManager(RB_MmProxy* pMm);
    virtual void setObjectFactory(RB_ObjectFactory* f);
    virtual RB_ObjectFactory* getObjectFactory();

    // Starting point: set subModel (on same window)
    virtual void setSourceModel(QAbstractItemModel* sourceModel);
    virtual bool setTableModel(const RB_String& list, RB_MmProxy* pMm);
    virtual bool setTreeModel(const RB_String& list, RB_MmProxy* pMm,
                              const QSqlDatabase& db = QSqlDatabase());
    virtual bool setCloneModel(RB_ObjectBase* root, const RB_String& list,
                               RB_MmProxy* pMm);
    virtual bool saveCloneModel();
    virtual void fetchAll(const QModelIndex& parent = QModelIndex());

    virtual RB_ObjectBase* getObject(const QModelIndex& index,
                                     RB2::ResolveLevel level = RB2::ResolveAll);
    virtual RB_ObjectBase* getCurrentObject();
    virtual void updateCurrentObject(RB_ObjectBase* obj);
    virtual void swapObject(const QModelIndex& fromIndex,
                            const QModelIndex& toIndex);
    virtual QModelIndex promote(const QModelIndex& index);
    virtual QModelIndex demote(const QModelIndex& idx,
                               const RB_String& parentId);

    //// Minimum model view controler functions as required in Qt4 for a
    // an editable table model which can also be a relation table model,
    // sort filter model and/or tree model, QAbstractItemModel implementation

    virtual QModelIndex index(int row, int column,
                            const QModelIndex& parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex& index) const;
    virtual bool hasChildren(const QModelIndex& parent) const;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
//    virtual int rowCountProxy(const QModelIndex& parent = QModelIndex()) const;
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
    virtual bool setHiddenData(const QModelIndex& index, const RB_Variant &value,
                               int role = Qt::EditRole);
    virtual RB_Variant headerData(int section, Qt::Orientation orientation,
                            int role = Qt::DisplayRole) const;

    virtual bool copyCurrentRow();
    virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

    virtual int fieldIndex(const QString& fieldName) const;
    virtual QSqlRelation relation(int column) const;
    virtual void setRelation(int column, const RB_SqlRelation& relation);
    virtual QSqlTableModel* relationModel(int column) const;
    virtual bool select();
    virtual void clear();
    virtual RB_MmAbstract* sourceModel() const;
    virtual QObject* sender() const;
    ////

    // Only for database
    virtual QSqlDatabase database() const;
    virtual bool createTables() { return false; }
    virtual void setCurrentConnection(const RB_String&) { }
    virtual RB_String getCurrentConnection() const { return RB_String(); }
    virtual bool setPermissions(const RB_String& /* loginUserName */,
                                const RB_String& /* loginPassword */) { return false; }

    // create input container (activeObject) for actions
    virtual RB_ObjectContainer* createInputModel(RB2::ActionType,
                                                RB2::Selection) { return NULL; }

    virtual bool isModelModified();
    virtual void setModelIsModified(bool modified);
    virtual bool isTreeModel() const;
    virtual bool isInMemoryModel() const;
    virtual void setInMemoryModel(bool inMemory);

    // source model filter
    virtual void setSourceFilter(const RB_String& sourceFilter);
    virtual RB_String getSourceFilter();
    virtual void setPrimaryParent(const RB_String& primParent);
    virtual void setSecondParent(const RB_String& secParent);
    virtual void setWhere(const RB_String& whereStatement, bool select = true);
    virtual void setQuery(const QSqlQuery& q);

    virtual RB2::SortOrderType getSourceSortOrder();
    virtual void setSourceSortOrder(RB2::SortOrderType so,
                                    const RB_String& colName1,
                                    const RB_String& colName2 = "",
                                    const RB_String& colName3 = "");
    virtual void setShared(bool shared);

    // filter/find
    virtual void setUserFilter(const RB_String& text, const RB_String& fieldName,
                               bool matchCase);
    virtual QRegExp getUserFilterExpression();
    virtual QString getUserFilterFieldName();
    virtual void removeUserFilter();
    virtual void undoFilterSort();

    virtual void setBaseCloneModel(bool modelIsBaseClone);
    virtual bool isBaseCloneModel();
    virtual RB_DataWidgetMapper* getMapper(const RB_String& mapperId = "DEFAULT");
    virtual void deleteMapper(const RB_String& mapperId = "DEFAULT");
    virtual QModelIndex mapToSource(const QModelIndex& proxyIndex) const;
    virtual QModelIndex mapFromSource(const QModelIndex& sourceIndex) const;

    virtual QModelIndex getProxyIndex() const;
    virtual QModelIndex getCurrentIndex() const;
    virtual RB_String getCurrentId() const;
    virtual RB_String getListName() const;

    virtual void setSelectionModel(QItemSelectionModel* selModel);
    virtual QItemSelectionModel* getSelectionModel();

    virtual RB_Variant getCurrentValue(const RB_String& fieldName,
                                       int role = Qt::DisplayRole) const;
    virtual bool setCurrentValue(const RB_String& fieldName, const RB_Variant& val,
                                 int role);
    virtual RB_Variant getValue(const RB_String& id, const RB_String& fieldName,
                                const RB_String& keyFieldName = "",
                                const RB_Variant& keyValue = RB_Variant()) const;
    virtual bool setValue(const RB_String& id, const RB_String& fieldName,
                          const RB_Variant& value, int role);

public slots:
    virtual void slotExpanded(const QModelIndex& index);
    virtual void slotChangeCurrentRow(const QModelIndex& current,
                                  const QModelIndex& previous);
    virtual void slotParentCurrentRowChanged(const QModelIndex& current,
                                const QModelIndex& previous);
    virtual void slotParentModelSubmitted();
    virtual void slotParentModelReverted();
    virtual void slotDataChanged(const QModelIndex& topLeft,
                                 const QModelIndex& bottomRight);
    virtual void slotSetCurrentModelRow(int row);
    virtual void slotRelModelUpdated(const QString& tableName);

    virtual void revert();
    virtual void revertAll();
    virtual bool submit();
    virtual bool submitAll();
    virtual bool submitAllAndSelect();

signals:
    void currentRowChanged(const QModelIndex& current, const QModelIndex& previous);
    void rootChanged();
    void expanded(const QModelIndex& index);

    void modelBeforeSubmitted();
    void modelReverted();

protected:
    virtual void reset();

private:
    //! Parent model manager
    RB_MmProxy* mParentManager;
    //! Model type, identifier used at model factory for managing models
    int mModelType;
    //! Model factory
    RB_ModelFactory* mModelFactory;
    //! Source model
    RB_MmAbstract* mSourceModel;
    //! Item selection model
    QItemSelectionModel* mSelectionModel;
    //! Data widget mapper map
    std::map<RB_String, RB_DataWidgetMapper*> mMapperMap;
};

#endif // RB_MMPROXY_H
