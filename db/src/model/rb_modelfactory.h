/*****************************************************************
 * $Id: rb_modelfactory.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Nov 12, 2009 9:42 AM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_MODELFACTORY_H
#define RB_MODELFACTORY_H

#include <map>
#include <QObject>
#include <QSqlDatabase>
#include "db_global.h"
#include "rb_mainwindow.h"
#include "rb_mmproxy.h"
#include "rb_objectfactory.h"


/**
 * Abstract model factory class, for the creation and deletion
 * of all model managers
 */
class DB_EXPORT RB_ModelFactory : public QObject {

    Q_OBJECT

public:
    RB_ModelFactory(RB_MainWindow* mw);
    virtual ~RB_ModelFactory();

    virtual RB_ObjectBase* getRoot();
    virtual void setRoot(RB_ObjectBase* root = NULL);
    virtual RB_String getRootId() const;
    virtual void setRootId(const RB_String& id);
    virtual RB_String getStoredId() const;
    virtual void setStoredId(const RB_String& id = "");

    virtual RB_MmProxy* getModel(int type, bool shared) = 0;
    virtual RB_MmProxy* getParentModel(int type) = 0;
    virtual bool removeModel(int type);

    virtual QSqlDatabase getDatabase();
    virtual QSqlDatabase getStandardDatabase();
    virtual QSqlDatabase getCustomDatabase();
//    virtual void setDatabase(const QSqlDatabase& db);
    virtual void setObjectFactory(RB_ObjectFactory* f);
    virtual RB_ObjectFactory* getObjectFactory(const RB_String& objName = "");
    virtual bool isModelModified();
    virtual void setModelIsModified(bool modified);

    virtual void submit();
    virtual void submitAll();
    virtual void revert();
    virtual void revertAll();

    virtual RB_MmProxy* getTableModel(const QSqlDatabase& db,
                                                RB_ObjectFactory* objF,
                                                int type,
                                                const RB_String& listName,
                                                bool share = true);
    virtual RB_MmProxy* getTreeModel(const QSqlDatabase& db,
                                               RB_ObjectFactory* objF,
                                               int type,
                                               const RB_String& listName,
                                               bool shared = true);
    virtual RB_MmProxy* getInMemoryTableModel(RB_ObjectBase* root,
                                                RB_ObjectFactory* objF,
                                                int type,
                                                const RB_String& listName,
                                                bool share = true);
    virtual int hiddenColumnCount();
    virtual void emitState();

    virtual bool addDefaultRows();
    virtual bool isValidId(const QString& id);

signals:
    //! Database is set, isOpen is true if database is open
    void databaseIsSet(int isOpen);
    //! Database is notSet, isOpen is false if database is open
    void databaseIsNotSet(int isOpen);
    //! Root id is set, isValidId is false (=0) in case id is not valid or ""
    void rootIsSet(int isValidId);
    //! Model is updated, used for relational models to update selected rows
    void modelUpdated(const QString&);

public slots:
    //! For models to send updated signal
    void slotModelUpdated(const QString& tableName);

protected:
    //! Database connection
//    RB_String mDatabaseConnection;
    //! Main window
    RB_MainWindow* mMainWindow;
    //! List of models
    std::map<int, RB_MmProxy*> mModelList;
    //! Object factory
    RB_ObjectFactory* mObjectFactory;
    //! Context and model factory root object
    RB_ObjectBase* mRoot;
    //! Model is modified flag
    bool mModelIsModified;
    //! Stored ID for immediate use after setting
    RB_String mStoredId;

};

#endif // RB_MODELFACTORY_H
