/*****************************************************************
 * $Id: db_modelfactory.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Dec 6, 2009 11:29 AM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_MODELFACTORY_H
#define DB_MODELFACTORY_H

#include "rb_modelfactory.h"


#define DB_MODELFACTORY DB_ModelFactory::getInstance()

/**
 * DB base model factory
 */
class DB_EXPORT DB_ModelFactory : public RB_ModelFactory {

    Q_OBJECT

public:
    virtual ~DB_ModelFactory();
    static DB_ModelFactory* getInstance(RB_MainWindow* mw = 0);

    virtual void setAllRootId(const RB_String& id);

#ifdef DB_TEST
    void setTestRootId(const RB_String& id);
    RB_String getTestRootId();
#endif

    virtual RB_MmProxy* getModel(int type, bool shared = true);
    virtual RB_MmProxy* getParentModel(int type);
//    virtual void setDatabase(const QSqlDatabase& db);

    virtual std::vector<RB_ModelFactory*> getFactoryList();
    virtual void registerFactory(RB_ModelFactory* factory);
    virtual void unregisterFactory(RB_ModelFactory* factory);
    virtual void closeAllFactories();
    virtual void disconnectFromDatabase();

    virtual RB_String createTables(const RB_String& perspective);
    virtual void createSql(RB_String& sqlStr, const RB_String& perspective);
    virtual void createUpdateTableSql(RB_String& sqlStr, const RB_String& tableName);
    virtual bool createMissingTables(const RB_String& perspective,
                                     int major, int medior, int minor,
                                     int build = -1);
    virtual bool checkExistingTables(const RB_String& perspective, RB_String& result);

    virtual RB_ObjectFactory* getObjectFactory(const RB_String& objName = "");
    virtual bool isObject(const RB_String& objName);

    virtual void emitState();


    /**
     * Model manager types, example = 101XXX:
     * - 10 = DB,
     * - 1 = model,
     * - XXX = sequence number
     * 101100 is starting number leaving 101000 to 101099 for test/debug cases
     * Refer also WidgetType and DialogType
     */
    enum ModelType {
#ifdef DB_TEST
        // start numbering from 101000
        ModelTest = 101000,                 /**< Table model */
        ModelTestChild,                     /**< Child table model */
        ModelTestDialog,                    /**< Dialog model */
        ModelTestProject,                   /**< Test project model */
        ModelTestRelation,                  /**< Test relation table model */
        ModelTestTree,                      /**< Tree model */
        ModelTestTreeChild,                 /**< Child Tree model */
        ModelTvTrv,                         /**< Tableparent (treechild) model */
        ModelTvTrvChild,                    /**< (Tableparent) treechild model */
#endif
        // start numbering from 101100
        ModelNone = 101100,                 /**< Invalid model */
        ModelActivity,                      /**< (Calendar) activity model */
        ModelPermissionGroup,               /**< Permission group model */
        ModelPermissionPerspectiveProject,  /**< Permission perspective project model */
        ModelPermissionProject,             /**< Permission project model */
        ModelProject,                       /**< Project model */
        ModelProjectEdit,                   /**< Project edit model */
        ModelSelectMapping,                 /**< Select mapping model */
        ModelSimpleReport,                  /**< Simple report model */
        ModelSystemGroup,                   /**< Group (users) model */
        ModelSystemUser,                    /**< User model */
        ModelSystemUserGroup,               /**< User group model */
        ModelVersion,                       /**< Version view only model */
        ModelDefault                        /**< Default- or all models */
    };

private:
    DB_ModelFactory(RB_MainWindow* mw);

    void createSql(RB_String& sqlStr, RB_ObjectBase* obj, bool isTempTable = false);
    void createTableSql(RB_String& sqlStr, RB_ObjectBase* obj, bool isTempTable = false);
    bool createTables(RB_String& sqlStr, RB_ObjectBase* obj);
    bool createDefaultRows(RB_String& sqlStr, RB_ObjectBase* obj);
    void createDefaultRowSql(RB_String& sqlStr, RB_ObjectBase* obj);
    bool checkTables(RB_ObjectBase* obj, RB_String& result);
    void createNonExistingTables(RB_String& sqlStr, RB_ObjectBase* obj);

    RB_ObjectBase* getObjectFactoryRoot(const RB_String& objectFactoryName);
    bool isTableExisting(RB_ObjectBase* obj);

    void beginTransaction(RB_String& sqlStr);
    void commitTransaction(RB_String& sqlStr);
    bool executeQuery(const RB_String& sqlStr);

    //! Unique factory instance
    static DB_ModelFactory* mActiveFactory;
    //! List of all currently created object factories
    std::vector<RB_ModelFactory*> mFactoryList;
    //! Database and SQL (error) message
    RB_String mMessage;
    /**
     * List of existing tables
     * \todo a duplication between creation of tables from the database
     * browser and from the select project action exists. The first uses
     * the mTableList to check whether a table exists and the second uses
     * the function isTableExisting. This class needs complete refactoring
     */
    QStringList mTableList;

#ifdef DB_TEST
    RB_String mTestRootId;
#endif
};

#endif // DB_MODELFACTORY_H
