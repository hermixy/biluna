/*****************************************************************
 * $Id: db_browserdialog.h 2248 2015-06-21 09:13:00Z rutger $
 *
 * Copyright (C) 2005 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project. Based on Qt Example.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/


#ifndef DB_BROWSERDIALOG_H
#define DB_BROWSERDIALOG_H

#include <QDialog>
#include "rb.h"
#include "rb_dialog.h"
#include "ui_db_browserdialog.h"

class DB_ConnectionWidget;
class DB_SqlHighlighter;
class QTableView;
class QPushButton;
class QTextEdit;
class QSqlError;
class RB_ObjectBase;
class RB_ObjectFactoryInterface;

/**
 * Database browser dialog
 */
class DB_EXPORT DB_BrowserDialog : public RB_Dialog, private Ui::DB_BrowserDialog {
	
    Q_OBJECT
    
public:
    DB_BrowserDialog(QWidget *parent = 0);
    virtual ~DB_BrowserDialog();

    void init() { }
    void browseDatabase();
    void updateActions();

    //! Selection from source database
    enum SrcType {
        SrcAllTables,
        SrcSelectedTable,
        SrcSelectedRows
    };

    //! Data destination type
    enum DestType {
        DestDatabase,
        DestXmlFile,
        DestXmlFileHierarchy,
        DestCsvFileSemiColon,
        DestCsvFileComma,
        DestCsvFileTab
    };

public slots:
    void sqlExec();
    void showTable(const QString& t, QTableView* tv);
    void showMetaData(const QString& t, QTableView* tv);
    void slotCurrentRowChanged() { updateActions(); }
    void slotCurrentTabChanged(int index);

    void insertRow();
    void deleteRow();

    void on_connectionWidget_tableActivated(const QString &t);
    void on_connectionWidget_metaDataRequested(const QString &t);
    void on_connectionWidgetSrc_tableActivated(const QString &t);
    void on_connectionWidgetSrc_metaDataRequested(const QString &t);
    void on_connectionWidgetDest_tableActivated(const QString &t);
    void on_connectionWidgetDest_metaDataRequested(const QString &t);

    void on_pbSubmit_clicked();
    void on_pbClear_clicked();
    void on_pbShowSql_clicked();
    void on_pbCreateTables_clicked();
    void on_pbCheckTables_clicked();
    void on_pbUpdateTable_clicked();
    void on_pbSettings_clicked();

    void on_pbTransferData_clicked();

    void on_pbAdd_clicked();
    void on_pbRemove_clicked();

    void on_pbHelp_clicked() { }
    void on_pbClose_clicked();

protected slots:
    void slotDatabaseConnectionChanged(int connectState);

//protected: now in RB_Dialog
//    virtual void keyPressEvent(QKeyEvent* e);

private:
    void initListWidget();
    void formatEditFields();

    void showMysqlMetaData(const QString& t, const QSqlDatabase& currentDb,
                           QTableView *tv);

    QAction* insertRowAction;
    QAction* deleteRowAction;

    RB_ObjectFactoryInterface* mObjectFactory;
    DB_SqlHighlighter* mCodeHighlighter;
};

#endif
