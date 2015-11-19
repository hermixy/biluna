/*****************************************************************
 * $Id: db_connectiondialog.h 2248 2015-06-21 09:13:00Z rutger $
 *
 * Copyright (C) 2005 Red-Bag. All rights reserved.
 * This file is part of the DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_CONNECTIONDIALOG_H
#define DB_CONNECTIONDIALOG_H

#include <QDir>
#include <qmessagebox.h>
// #include "rb_aesencryption.h"
#include "rb_database.h"
#include "rb_dialog.h"
#include "rb_stringlist.h"
#include "rb_string.h"
#include "rb.h"
#include "ui_db_connectiondialog.h"


/**
 * Database connection dialog
 */
class DB_EXPORT DB_ConnectionDialog : public RB_Dialog, private Ui::DB_ConnectionDialog {
	
    Q_OBJECT
    
public:
    DB_ConnectionDialog(QWidget *parent,
                        RB2::DatabaseDriver serverDriver,
    					const RB_String& dbName = "",
    					const RB_String& uName = "",
    					const RB_String& pwd = "",
    					const RB_String& hName = "",
    					const RB_String& prt = "",
    					const RB_String& loginUser = "",
    					RB2::DatabaseDriver localDriver = RB2::DatabaseNone,
    					const RB_String& localDbName = "");
    DB_ConnectionDialog(QWidget *parent);
    virtual ~DB_ConnectionDialog();

    virtual void init();

    RB2::DatabaseDriver databaseDriver() const;
    RB_String databaseName() const;
    RB_String databaseUser() const;
    RB_String databasePassword() const;
    RB_String databaseHost() const;
    int databasePort() const;
    RB_String localDatabaseName() const;
    RB_String userName() const;
    RB_String userPassword() const;
    
private slots:
    void on_pbHelp_clicked();
    void on_okButton_clicked();
    void on_cancelButton_clicked() { reject(); }
    void on_pbSelectLocalDatabase_clicked();
    void on_rbLocalAccessDb_clicked();
    void on_rbLocalSqliteDb_clicked();
    void on_rbServerDatabase_clicked();
    void on_pbTestConnection_clicked();

    void slotSetDbConnectionWidgets(const QModelIndex& curr,
                                    const QModelIndex& prev);
    void slotDbTypeChanged(bool);
    void slotDbDataChanged(QString);
    void slotDbEditFinished();
    void slotLocalDbSelected(bool);
	
    void slotDeletePreviousConnection();

private:
    void writeSettings();
    void updateUi();
    void createDatabase(QSqlDatabase& db);
    bool isValidNetworkConnection(const RB_String& hostStr);

    QDir mPluginsDir;
    static int mConnectionNumber;
    RB_StringList mConnectionList;
    RB_String mConnectionSeparator;
    RB_String mConnectionItemSeparator;
    bool mDataIsChanged;
};

#endif
