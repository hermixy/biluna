/*****************************************************************
 * $Id: db_mappingdialog.cpp 2210 2015-01-22 14:59:25Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna LDT project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_mappingdialog.h"

#include <QtWidgets>
#include <QtSql>
#include <iostream>

#include "db_map.h"
#include "db_mapping.h"
#include "db_mappingdelegate.h"
#include "db_dialogfactory.h"
#include "db_modelfactory.h"
#include "db_selectmappingdialog.h"
#include "db_xmlreader.h"
#include "rb_debug.h"
#include "rb_settings.h"
#include "rb_xmlwriter.h"


DB_MappingDialog::DB_MappingDialog(QWidget* parent) : RB_Dialog(parent) {

    setupUi(this);
    
    mMapObject = NULL;
    mSeparator = "";

    browseDatabase();
    updateUi();
    readSettings();
}

DB_MappingDialog::~DB_MappingDialog() {
    // nothing
}

/**
 * Set parent is done from the main application such as LDT. The parent ID will
 * be set, the name of the object (e.g. LDT_Line which is also the table name)
 * and the checkBox for creating Uuid's will be set. The Uuid's are generated
 * to create an ID for each individual row. The creation of an Uuid works only
 * for a column with name 'id'.
 * 
 * In case of stand-alone use, this function is not used. The parent ID can be set
 * manually if necessary. The creation of Uuid's can be set manually if an 'id'
 * column exists and is not a automatic generated field by the database.
 */
void DB_MappingDialog::setParent(const QString& objectName,
								const QString& parentId, 
//								const QString& parentField,
								bool setUuid) {
    leSelectedTable->setText(objectName); // same as table name
    leParentId->setText(parentId);
//    leParentName->setText(parentField);
    cbUuid->setChecked(setUuid);
//    this->leIdName->setText("id");

    updateUi();
}

/**
 * (Re)populate connection widget with database tables
 */
void DB_MappingDialog::browseDatabase() {
	connectionWidget->refresh();
}

/**
 * A table in the connection has been activated
 * @param tableName table name
 */
void DB_MappingDialog::on_connectionWidget_tableActivated(const QString &tableName) {
    showTable(tableName, tableView);
    updateUi();
}


//void DB_MappingDialog::on_connectionWidget_tableClicked(const QString& table) {
//    leSelectedTable->setText(table);
//    updateUi();
//    return;
//
//
//	tableWidget->clear();
//
//   	tableWidget->setRowCount(1);
//	tableWidget->setColumnCount(1);
//
//	QTableWidgetItem* headerItem = new QTableWidgetItem(
//						tr("select CSV file to import in .."));
//	QTableWidgetItem* newItem = new QTableWidgetItem(
//						tr("  %1").arg(table));
//
//	tableWidget->setHorizontalHeaderItem(0, headerItem);
//	tableWidget->verticalHeader()->hide();
//	tableWidget->setItem(0, 0, newItem);
//	tableWidget->resizeColumnsToContents();
//
//
//}

void DB_MappingDialog::on_pbOpenMappingFile_clicked() {
    // get file name
    RB_SETTINGS->beginGroup("paths");
    RB_String mappingPath = RB_SETTINGS->value("mappingPath", "").toString();
    RB_SETTINGS->endGroup();

    RB_String fn = "";
    QFileDialog dlg(this, "Open mapping file", mappingPath,
                        "eXtensible Markup Language (*.xml);;all files (*.*)");
    dlg.setFileMode(QFileDialog::ExistingFile);
    dlg.setAcceptMode(QFileDialog::AcceptOpen);
    dlg.exec();

    if (!dlg.result()) return;
    fn = dlg.selectedFiles().at(0);
    if (fn.isEmpty()) return;

    mappingPath = QFileInfo(fn).absolutePath();
    RB_SETTINGS->beginGroup("paths");
    RB_SETTINGS->setValue("mappingPath", mappingPath);
    RB_SETTINGS->endGroup();

    RB_ObjectBase* mapList = DB_MODELFACTORY->getRoot()->getContainer("DB_MapList");
    mMapObject = DB_OBJECTFACTORY->newObject("", mapList, "", true);

    loadFile(fn, "DB_XML");
    leMappingFile->setText(fn);
    setWidgets();
}

void DB_MappingDialog::on_pbExportMapping_clicked() {
    // RB_DEBUG->print("DB_MappingDialog::on_pbExportMapping_clicked()");
    
    RB_SETTINGS->beginGroup("paths");
    RB_String mappingPath = RB_SETTINGS->value("mappingPath", "").toString();
    RB_SETTINGS->endGroup();

    QFileDialog dlg(this, "Save mapping file", mappingPath,
                    "eXtensible Markup Language (*.xml);;all files (*.*)");
    dlg.setFileMode(QFileDialog::AnyFile);
    dlg.setAcceptMode(QFileDialog::AcceptSave);

    if (dlg.exec() != QDialog::Accepted) return;

    RB_StringList fileNames = dlg.selectedFiles();
    RB_String fileName = fileNames.at(0);

    mappingPath = QFileInfo(fileName).absolutePath();
    RB_SETTINGS->beginGroup("paths");
    RB_SETTINGS->setValue("mappingPath", mappingPath);
    RB_SETTINGS->endGroup();


    if (fileName.isEmpty()) {
		QMessageBox::warning(this, tr("DB mapping save warning"),
                     tr("File name was empty, file NOT saved."));
        return;
    }

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("DB mapping save warning"),
                            tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

    readMappingTable();

    RB_XmlWriter* xw = new RB_XmlWriter(&file);
    mMapObject->acceptVisitor(*xw);
    delete xw;

    QApplication::restoreOverrideCursor();
}

/**
 * Save mapping to database
 */
void DB_MappingDialog::on_pbSaveMapping_clicked() {
    // RB_DEBUG->print("DB_MappingDialog::on_pbSaveMapping_clicked()");
//    this->saveMappingToDatabase("");

    readMappingTable();

    bool success = mMapObject->dbUpdate(connectionWidget->currentDatabase(), RB2::ResolveAll);
    if (success) {
        RB_DEBUG->print("DB_MappingDialog::on_pbSaveMapping_clicked() ERROR");
    }
}

/**
 * Select mapping from database of in-memory model
 */
void DB_MappingDialog::on_pbSelectMapping_clicked() {
    // Prepare model(s)
    RB_MmProxy* model = DB_MODELFACTORY->getModel(DB_ModelFactory::ModelSelectMapping);
    model->setRoot(DB_MODELFACTORY->getRootId());

    // Create dialog
    RB_Dialog* dlg = DB_DIALOGFACTORY->getDialog(DB_DialogFactory::DialogSelectMapping);
    if (dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();
        if (!obj) {
            dlg->deleteLater();
            return;
        }
        RB_String id = obj->getId();
        RB_String number = obj->getValue("number").toString();

        leMappingFile->setText(number);

        if (!mMapObject) {
            mMapObject = DB_OBJECTFACTORY->newSingleObject("DB_Map");
        }

        mMapObject->setId(id);
        bool success = mMapObject->dbRead(connectionWidget->currentDatabase(),
                                          RB2::ResolveOne);

//        readMappingFromDatabase(id);
        setWidgets();

        if (success) {
            this->updateUi();
        }
    }

    dlg->deleteLater();
}

/**
 * Set the CSV file name to be imported
 */
void DB_MappingDialog::on_pbOpenCsvFile_clicked() {
    RB_SETTINGS->beginGroup("paths");
    RB_String openPath = RB_SETTINGS->value("openPath", "").toString();
    RB_SETTINGS->endGroup();

    RB_String fn = "";
    QFileDialog dlg(this, "Open CSV file", openPath,
                        "Comma Separated Value (*.csv);;all files (*.*)");
    dlg.setFileMode(QFileDialog::ExistingFile);
    dlg.setAcceptMode(QFileDialog::AcceptOpen);
    dlg.exec();

    if (!dlg.result()) return;
    fn = dlg.selectedFiles().at(0);
    if (fn.isEmpty()) return;

    openPath = QFileInfo(fn).absolutePath();
    RB_SETTINGS->beginGroup("paths");
    RB_SETTINGS->setValue("openPath", openPath);
    RB_SETTINGS->endGroup();

    leCsv->setText(fn);
    updateUi();
}

/**
 * Set table name to which the CSV file will be imported
 */
void DB_MappingDialog::on_pbImportTable_clicked() {
    leSelectedTable->setText(connectionWidget->currentTableName());
}

/**
 * Set parent ID based on the row selection in the database table view
 */
void DB_MappingDialog::on_pbParentId_clicked() {
    QModelIndex currentIndex = tableView->currentIndex();
    if (!currentIndex.isValid()) return;
    QAbstractItemModel* model = tableView->model();
    QModelIndex idIndex = model->index(currentIndex.row(), 0, QModelIndex());
    RB_String id = model->data(idIndex, Qt::EditRole).toString();
    leParentId->setText(id);
}

/**
 * Load mapping settings from an external mapping file.
 */
void DB_MappingDialog::loadFile(const QString &fileName, const QString& fileType) {
	// RB_DEBUG->print("DB_MappingDialog::loadFile() type = %s", 
	//				fileType.toStdString().c_str());
	
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("DB Warning"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

	
    if (fileType == "DB_XML" && mMapObject) {
        QApplication::setOverrideCursor(Qt::WaitCursor);

        DB_XmlReader* xr = new DB_XmlReader(mMapObject);
        xr->setNewId(false);

        bool success = xr->read(&file);
        QApplication::restoreOverrideCursor();

        if (!success) {
            DB_DIALOGFACTORY->requestWarningDialog(
                        tr("Reading error %1:\n%2 (2).")
                        .arg(fileName)
                        .arg(xr->errorString()));
        }
        delete xr;
        file.close();

    }
}

/**
 * Open the selected CSV and create the mapping overview with the first
 * three rows of the CSV file
 */
void DB_MappingDialog::initMappingTable() {
    RB_DEBUG->print("DB_MappingDialog::initMappingTable()");
	
    tableWidget->clear();
	
    // read CSV file
    QFile file(leCsv->text());
    if (!file.open(QIODevice::ReadOnly)) return;
	
    QString data = file.readAll();
    QString lineSeparator = "";

    if (data.contains("\r\n")) {
        lineSeparator = "\r\n"; // Windows
    } else if (data.contains("\n")) {
        lineSeparator = "\n"; // *nix
    } else if (data.contains("\r")) {
        lineSeparator = "\r"; // old mac
    }

    mLines = data.split(QString(lineSeparator));
    file.close();
	
    if (mLines.count() < 1) {
        return;
    }
	
    // create comboBox of currentTable columns
    DB_MappingDelegate* delegate = new DB_MappingDelegate(this);

    QStringList fieldList;
    fieldList << tr("(none)");

    //don't actually select anything but get a reference to the record set
    QString qStr = "SELECT * FROM " + leSelectedTable->text() + " LIMIT 0, 0;";

    if (RB2::enumToDriverName(RB2::DatabaseOdbc) == DB_MODELFACTORY->getDatabase().driverName()) {
        qStr = "SELECT TOP 1 * from " + leSelectedTable->text() + ";";
    }

    QSqlQuery query(connectionWidget->currentDatabase());
    query.exec(qStr);
    QSqlRecord rec = query.record();
    int recCount = rec.count();
    for (int i = 0; i < recCount; ++i) {
        fieldList.append(rec.fieldName(i));
    }

    delegate->setComboBoxData(fieldList);
	
    // set for each column in CSV file a row with in columns
    // the comboBox, the first row (= maybe a header) and second row data
    QStringList labels;
    labels << tr("Table Columns") << tr("File Row 1") << tr("File Row 2")
           << tr("File Row 3");

    tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    tableWidget->setColumnCount(4);
    tableWidget->setHorizontalHeaderLabels(labels);
    tableWidget->setItemDelegate(delegate);

    tableWidget->horizontalHeader()->resizeSection(0, 200);
    tableWidget->horizontalHeader()->resizeSection(1, 100);
    tableWidget->horizontalHeader()->resizeSection(2, 100);
    tableWidget->horizontalHeader()->resizeSection(3, 100);
    tableWidget->verticalHeader()->hide();

    QString lineData = mLines[0];

    // Normally only the first two lines contain '--'
    for (int i = 0; i < 10 && lineData.startsWith("--"); ++i) {
        mLines.removeFirst();
        lineData = mLines.first();
    }

    // set the number of rows in the mapping table based on the columns in CSV file
    QStringList columnStrings;
    mSeparator = "";

    if (lineData.contains("\t")) {
        // tab separated CSV
        mSeparator = "\t";
    } else if (lineData.contains(";")) {
        // semicolon separated CSV
        mSeparator = ";";

    } else if (lineData.contains(",")) {
        // comma separated CSV
        mSeparator = ",";
    } else {
        // TODO: give error, first line does not contain \t, or ; or ,
        return;
    }

    columnStrings = lineData.split(mSeparator, QString::KeepEmptyParts);
    int rows = columnStrings.count();
    tableWidget->setRowCount(rows);

    // Fill first column of the mapping table and mapping model
    // Note: this is a QTableWidget, each field needs to be filled with an item
    for (int i = 0; i < rows; ++i) {
        tableWidget->setItem(i, 0, new QTableWidgetItem(tr("(none)")));
    }

    int number = 0;
    RB_String strTrimmed = "";

    while (number < 3) {
        lineData = mLines[number];

        if (!lineData.isEmpty()) {
            // Read the column data from the rest of the line.
            columnStrings = lineData.split(mSeparator, QString::KeepEmptyParts);
            
            // check number of columns with original count
            if (rows != columnStrings.count()) {
            	RB_String str = "error in CSV file on line: " 
                                + QString::number(number);
                QMessageBox::warning(this, tr("Import Error"),
                        tr("Could not complete reading of CSV file,\n") + str);
            	
                on_pbBack_clicked();
            	return;
            }

            for (int i = 0; i < rows; ++i) {
                QTableWidgetItem* newItem = new QTableWidgetItem(columnStrings[i]);
                tableWidget->setItem(i, number + 1, newItem);

                strTrimmed = columnStrings.at(i).trimmed();

                // Assumes the first column (=0) contains headers, set mapping
                if (number == 0 && fieldList.contains(strTrimmed)
                        && (!(cbUuid->isChecked() && i == 0)
                            || !(leParentId->text().isEmpty() && i == 1)) ) {
                    tableWidget->item(i, 0)->setText(strTrimmed);
                }
            }
        }

        ++number;
    }

    // set tableWidget mapping if map exists
    if (!mMapObject) return;
	
    RB_ObjectContainer* mappingList = mMapObject->getContainer("DB_MappingList");
    rows = tableWidget->rowCount();

    RB_ObjectIterator* iter = mappingList->createIterator();
    for (int i = 0; i < rows; ++i) {
        RB_ObjectBase* obj = NULL;
        bool found = false;
        for (iter->first(); !iter->isDone() && !found; iter->next()) {
            obj = iter->currentObject();
            if (obj->getValue("columnnumber").toInt() == i) {
                tableWidget->item(i, 0)->setText(obj->getValue(1).toString());
                found = true;
            }
        }
    }
    delete iter;
}

/**
 * Get data from mapping model and fill tableWidget
 */
void DB_MappingDialog::setWidgets() {
    if (!mMapObject) return;
	
    RB_DEBUG->print("DB_MappingDialog::setWidgets() "
                    + mMapObject->getValue("mdatabase").toString());

    leSelectedTable->setText(mMapObject->getValue("mtable").toString());
    leParentId->setText(mMapObject->getValue("parent_id").toString());

    if (mMapObject->getValue("createid").toString()
            .compare("true", Qt::CaseInsensitive) == 0) {
        cbUuid->setChecked(true);
    } else {
        cbUuid->setChecked(false);
    }

    if (mMapObject->getValue("fileheading").toString()
            .compare("true", Qt::CaseInsensitive) == 0) {
		checkBoxHeaderRow->setChecked(true);
    } else {
		checkBoxHeaderRow->setChecked(false);
    }
}

/**
 * Set data from tableWidget and other widgets in mapping model for save action
 */
void DB_MappingDialog::readMappingTable() {
    RB_DEBUG->print("DB_MappingDialog::readMappingTable()");
	
    if (!mMapObject) {
        // create DB_Map root object container to contain the individual mappings
        RB_ObjectContainer* mapList = DB_MODELFACTORY->getRoot()->getContainer("DB_MapList");
        mMapObject = DB_OBJECTFACTORY->newObject("", mapList, "", true);
    }

    RB_ObjectBase* mappingList = mMapObject->getContainer("DB_MappingList");
    int rows = tableWidget->rowCount();

    for (int i = 0; i < rows; ++i) {
        RB_String fieldName = tableWidget->item(i, 0)->text();

        RB_ObjectBase* mapping = DB_OBJECTFACTORY->newObject("", mappingList,
                                                             "", true);
        mapping->setValue("columnnumber", i);
        mapping->setValue("tofield", fieldName);
    }
	
    // set other mapping details in root DB_Map object
    mMapObject->setValue("mdatabase", connectionWidget->currentDatabase().databaseName());
    mMapObject->setValue("mtable", leSelectedTable->text());

    if (cbUuid->isChecked()) {
        mMapObject->setValue("createid", "true");
    } else {
        mMapObject->setValue("createid", "false");
    }

    mMapObject->setValue("parent_id", leParentId->text());
    mMapObject->setValue("importfile", leCsv->text());

    if (checkBoxHeaderRow->isChecked()) {
        mMapObject->setValue("fileheading", "true");
    } else {
        mMapObject->setValue("fileheading", "false");
    }
}

void DB_MappingDialog::on_pbHelp_clicked() {
    RB_MainWindow* mw = DB_DIALOGFACTORY->getMainWindow();
    mw->slotHelpSubject(this->objectName());
}

void DB_MappingDialog::on_pbCancel_clicked() {
    close();
}

void DB_MappingDialog::on_pbBack_clicked() {
    int page = stackedWidget->currentIndex();
    stackedWidget->setCurrentIndex(page - 1);
	
    updateUi();
}

void DB_MappingDialog::on_pbNext_clicked() {
    if (stackedWidget->currentIndex() == 0) {
        initMappingTable();
        int page = stackedWidget->currentIndex();
        stackedWidget->setCurrentIndex(page + 1);
    } else if (stackedWidget->currentIndex() == 1) {
        importCsvFile();
        int page = stackedWidget->currentIndex();
        stackedWidget->setCurrentIndex(page + 1);
    } else if (stackedWidget->currentIndex() == 2) {
        close();
    }
	
    updateUi();
}

void DB_MappingDialog::importCsvFile() {
    RB_DEBUG->print("DB_MappingDialog::importCsvFile_clicked()");
	
    int page = stackedWidget->currentIndex();
    stackedWidget->setCurrentIndex(page + 1);
	
    updateUi();

    readMappingTable();
    RB_ObjectContainer* mappingList = mMapObject->getContainer("DB_MappingList");
    int rows = tableWidget->rowCount();

    // set progress bar and create
    int number = 0;
    if (checkBoxHeaderRow->isChecked()) {
        number = 1;
    }
    int maxProgress = mLines.count() - (1+number);
    progressBar->setRange(0, maxProgress);
    RB_String lineData = "";
    RB_String qStrInsert = "";
    RB_String qStrValues = "";
    RB_String strIns = "";
    RB_String strVal = "";
    int colRunner = 0;

	
    while (number < mLines.count()) {
        progressBar->setValue(number);
        lineData = mLines[number];
        lineData.trimmed();

        if (!lineData.isEmpty()) {
            // Read the column data from the rest of the line.
            QStringList columnStrings = lineData.split(mSeparator, QString::KeepEmptyParts);
            
            // check number of columns (rows in tabelWidget) with original count
            if (rows != columnStrings.count()) {
                RB_String str = "error in CSV file on line: "
                        + QString::number(number);
                QMessageBox::warning(this, tr("Import Error"),
                                     tr("Could not complete reading of CSV file,\n") + str);
            	
                on_pbBack_clicked();
            	return;
            }
            
            //
            // Create query
            //
            qStrInsert = "INSERT INTO " + leSelectedTable->text() + " (";
            qStrValues = ") VALUES (";

            strIns = "";
            bool firstRow = true;

            for (int i = 0; i < rows; ++i) {
                RB_ObjectBase* mMapObject = mappingList->getObject(i);
                strIns = mMapObject->getValue("tofield").toString();
				
                // Skip combination of id and generated Uuid's
                // and parent id when is set by user
                if (strIns != tr("(none)") && strIns != "id"
                        && strIns != "parent" && strIns != "name") {
                    if (!firstRow) {
                        qStrInsert += ", ";
                        qStrValues += ", ";
                    } else {
                        qStrInsert += "id, parent, name, ";
                        qStrValues += "?, ?, ?, ";
                    }

                    qStrInsert += strIns;
                    qStrValues += "?";
                    firstRow = false;
                }
            }

            qStrValues += ")";

            //
            // Prepare query
            //
            QSqlQuery query(connectionWidget->currentDatabase());
            query.prepare(qStrInsert + qStrValues);

            //
            // Set binding values
            //
            if (cbUuid->isChecked()) {
                query.bindValue(0, RB_Uuid::createUuid().toString(), QSql::In);
            }

            if (!leParentId->text().isEmpty()) {
                query.bindValue(1, leParentId->text(), QSql::In);
            }

            // Set name
            query.bindValue(2, leSelectedTable->text(), QSql::In);

            strVal = "";
            colRunner = 3;

            for (int i = 0; i < rows; ++i) {
                RB_ObjectBase* mMapObject = mappingList->getObject(i);
                strIns = mMapObject->getValue("tofield").toString();
                strVal = columnStrings[i];
                strVal.remove("\'");
                strVal.remove("\"");
                strVal = strVal.simplified();

                // Skip combination of id and generated Uuid's
                // and parent id when is set by user
                if (strIns == "id" && !cbUuid->isChecked()) {
                    query.bindValue(0, strVal, QSql::In);
                } else if (strIns == "parent" && leParentId->text().isEmpty()) {
                    query.bindValue(1, strVal, QSql::In);
                } else if (strIns != tr("(none)") && strIns != "id"
                           && strIns != "parent" && strIns != "name") {
                    query.bindValue(colRunner, strVal, QSql::In);
                    ++colRunner;
                }
            }

            //
            // Execute query
            //
            if (!query.exec()) {
                QMessageBox::warning(this, tr("Unable to execute mapping query"),
                                     tr("An error occured while executing the query:\n ")
                                     + query.lastError().text() + "\n"
                                     + "line number " + RB_String::number(number)
                                     + " of CSV file\n");

                RB_DEBUG->print(query.lastError().text());
                RB_DEBUG->print(query.lastQuery());

                progressBar->setValue(maxProgress);
                lblStatus->setText(tr("File transfer aborted."));
                pbNext->setText(tr("Close"));
                return;
            }
        }

        number++;
    }

    lblStatus->setText(tr("Ready"));
}

/**
 * Handle actions before closing the application or due to clicking
 * the right top corner, in a dialog this is the reject() function.
 * @param event
 */
void DB_MappingDialog::closeEvent(QCloseEvent* event) {
    RB_DEBUG->print("DB_MappingDialog::closeEvent()");

//    if (maybeSave()) {
        event->accept();
//    } else {
//        event->ignore();
//    }
}


void DB_MappingDialog::updateUi() {
    if (stackedWidget->currentIndex() == 0) {
        pbCancel->setEnabled(true);
        pbBack->setEnabled(false);
        pbNext->setText(tr("&Next >"));
        pbNext->setEnabled(leSelectedTable->text() != ""
                           && leCsv->text() != "");
    } else if (stackedWidget->currentIndex() == 1) {
        pbCancel->setEnabled(true);
        pbBack->setEnabled(true);
        pbNext->setText(tr("&Import"));
    } else if (stackedWidget->currentIndex() == 2) {
        pbCancel->setEnabled(false);
        pbBack->setEnabled(false);
        pbNext->setText(tr("&Close"));
    }
}	

void DB_MappingDialog::showTable(const QString &t, QTableView* tv) {
    RB_DEBUG->print("DB_MappingDialog::showTable()");

    QSqlTableModel* model = NULL;
    model = new QSqlTableModel(tv,connectionWidget->currentDatabase());
    model->setTable(t);
    model->select();
    if (model->lastError().type() != QSqlError::NoError)
        leSelectedTable->setText(model->lastError().text());
    tv->setModel(model);
    tv->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

