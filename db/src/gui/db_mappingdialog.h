/*****************************************************************
 * $Id: db_mappingdialog.h 2248 2015-06-21 09:13:00Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna LDT project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_MAPPINGDIALOG_H
#define DB_MAPPINGDIALOG_H

#include <QDialog>
#include "db_objectfactory.h"
#include "rb_dialog.h"

#include "ui_db_mappingdialog.h"



class DB_EXPORT DB_MappingDialog: public RB_Dialog, private Ui::DB_MappingDialog {
	
    Q_OBJECT
    
public:
    DB_MappingDialog(QWidget* parent = 0);
    virtual ~DB_MappingDialog();
    
    void init() {
        this->readSettings();
    }
    void setParent(const QString& objectName, const QString& parentId,
                   /* const QString& parentField,*/	bool setUuid);

public slots:
    void on_connectionWidget_tableActivated(const QString &tableName);
//    void on_connectionWidget_tableClicked(const QString& table);
    void on_pbOpenMappingFile_clicked();
    void on_pbExportMapping_clicked();
    void on_pbSaveMapping_clicked();
    void on_pbSelectMapping_clicked();
    void on_pbOpenCsvFile_clicked();
    void on_pbImportTable_clicked();
    void on_pbParentId_clicked();
    
    void on_pbHelp_clicked();
    void on_pbCancel_clicked();
    void on_pbBack_clicked();
    void on_pbNext_clicked();
    

protected slots:
    virtual void closeEvent(QCloseEvent* event);

private:
    void importCsvFile();
    void browseDatabase();
	void loadFile(const RB_String &fileName, const RB_String& fileType);
    void initMappingTable();
    void setWidgets();
    void readMappingTable();
    void updateUi();
    void showTable(const QString &t, QTableView* tv);

    //! List of lines (string) in a CSV file
    QStringList mLines;
    //! Current map object with detail mappings
    RB_ObjectBase* mMapObject;
    //! CSV separator, such as ';' 'tab' and ','
    RB_String mSeparator;
};

#endif /*DB_MAPPINGDIALOG_H*/
