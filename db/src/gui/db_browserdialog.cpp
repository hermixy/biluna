/*****************************************************************
 * $Id: db_browserdialog.cpp 2210 2015-01-22 14:59:25Z rutger $
 *
 * Copyright (C) 2005 Red-Bag. All rights reserved.
 * This file is part of the DB project. Based on Qt Example.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/


#include "db_browserdialog.h"

#include <QtWidgets>
#include <QtSql>
#include <QList>
#include <iostream>

#include "db_actionfilesaveas.h"
#include "db_connectiondialog.h"
#include "db_dialogfactory.h"
#include "db_modelfactory.h"
#include "db_objectfactory.h"
#include "db_sqlhighlighter.h"
#include "db_transferdataaction.h"
#include "rb_settings.h"

DB_BrowserDialog::DB_BrowserDialog(QWidget *parent) : RB_Dialog(parent) {

    setupUi(this);

    // HACK 2012-05-19 otherwise orientation other way around:
    splitterHorizontal->setOrientation(Qt::Horizontal);
    splitterVertical->setOrientation(Qt::Vertical);

    insertRowAction = new QAction(tr("Insert Row"), this->tvDefault);
    connect(insertRowAction, SIGNAL(triggered()), SLOT(insertRow()));
    deleteRowAction = new QAction(tr("Delete Row"), tvDefault);
    connect(deleteRowAction, SIGNAL(triggered()), SLOT(deleteRow()));

    tvDefault->addAction(insertRowAction);
    tvDefault->addAction(deleteRowAction);
    tvDefault->setContextMenuPolicy(Qt::ActionsContextMenu);

    mCodeHighlighter = new DB_SqlHighlighter(teSqlEdit->document());
    teSqlEdit->setTabStopWidth(20); // in pixels, default is 80

    initListWidget();

    if (QSqlDatabase::drivers().isEmpty()) {
        QMessageBox::information(this, tr("No database drivers found"),
                     tr("This function requires at least one database driver. "
                        "Please check the documentation how to invoke the "
                        "SQL drivers."));
    } else {
//       QMetaObject::invokeMethod(this, "addConnection", Qt::QueuedConnection);
		
//		QList<int> list;
//		list << 200 << 443;
//		splitter->setSizes(list);
		
        browseDatabase();
        formatEditFields();
    }

    connect(tabWidget, SIGNAL(currentChanged(int)),
            this, SLOT(slotCurrentTabChanged(int)));
    connect(DB_MODELFACTORY, SIGNAL(databaseIsSet(int)),
            this, SLOT(slotDatabaseConnectionChanged(int)));
    updateActions();
    readSettings();
}

DB_BrowserDialog::~DB_BrowserDialog() {
    delete mCodeHighlighter;
}

/**
 * Initialize listwidgets with the selection for source and destination of data
 * transfer
 */
void DB_BrowserDialog::initListWidget() {
    RB_DEBUG->print("DB_BrowserDialog::initListWidget()");

    // source
    QListWidgetItem* newItem = new QListWidgetItem(tr("All Tables"), lwSource, 0);
    newItem->setData(Qt::UserRole, DB_BrowserDialog::SrcAllTables);
    lwSource->addItem(newItem);
    newItem = new QListWidgetItem(tr("Selected Table"), lwSource, 0);
    newItem->setData(Qt::UserRole, DB_BrowserDialog::SrcSelectedTable);
    lwSource->addItem(newItem);
    // TODO: implement 'Selected Rows'
    newItem = new QListWidgetItem(tr("Selected Rows"), lwSource, 0);
    newItem->setData(Qt::UserRole, DB_BrowserDialog::SrcSelectedRows);
    lwSource->addItem(newItem);
    lwSource->setSelectionMode(QAbstractItemView::SingleSelection);
	
    // destination
    newItem = new QListWidgetItem(tr("Database"), lwDestination, 0);
    newItem->setData(Qt::UserRole, DB_BrowserDialog::DestDatabase);
    lwDestination->addItem(newItem);
    newItem = new QListWidgetItem(tr("XML File"), lwDestination, 0);
    newItem->setData(Qt::UserRole, DB_BrowserDialog::DestXmlFile);
    lwDestination->addItem(newItem);
    newItem = new QListWidgetItem(tr("XML File (hierarchy)"), lwDestination, 0);
    newItem->setData(Qt::UserRole, DB_BrowserDialog::DestXmlFileHierarchy);
    lwDestination->addItem(newItem);
    newItem = new QListWidgetItem(tr("CSV File (;)"), lwDestination, 0);
    newItem->setData(Qt::UserRole, DB_BrowserDialog::DestCsvFileSemiColon);
    lwDestination->addItem(newItem);
    newItem = new QListWidgetItem(tr("CSV File (,)"), lwDestination, 0);
    newItem->setData(Qt::UserRole, DB_BrowserDialog::DestCsvFileComma);
    lwDestination->addItem(newItem);
    newItem = new QListWidgetItem(tr("CSV File (TAB)"), lwDestination, 0);
    newItem->setData(Qt::UserRole, DB_BrowserDialog::DestCsvFileTab);
    lwDestination->addItem(newItem);
    lwDestination->setSelectionMode(QAbstractItemView::SingleSelection);
}

void DB_BrowserDialog::formatEditFields() {
    cbTables->addItem("None");

    std::vector<RB_ObjectFactory*> fl = DB_OBJECTFACTORY->getFactoryList();
    std::vector<RB_ObjectFactory*>::iterator iter;
    iter = fl.begin();
    while (iter != fl.end()) {
        cbTables->addItem((*iter)->getName());
        ++iter;
    }
}

void DB_BrowserDialog::sqlExec() {
    QSqlQueryModel* model = dynamic_cast<QSqlQueryModel*>(tvDefault->model());
    if (!model) {
        model = new QSqlTableModel(tvDefault);
    }

    QString str = teSqlEdit->toPlainText();
    if (str.startsWith("select", Qt::CaseInsensitive)
            || str.startsWith("show", Qt::CaseInsensitive)) {
        QSqlQuery query(teSqlEdit->toPlainText(), connectionWidget->currentDatabase());
        model->setQuery(query);
        tvDefault->setModel(model);

        if (model->lastError().type() != QSqlError::NoError) {
            lineEditStatus->setText(model->lastError().text());
            return;
        }

        lineEditStatus->setText("Query OK.");
    } else {
        tvDefault->setModel(NULL);

        // handle multiple line sql
        QStringList sqlList = teSqlEdit->toPlainText().split(";", QString::SkipEmptyParts);
        int noRows = 0;

        for (int i = 0; i < sqlList.count(); ++i) {
            QSqlQuery query(connectionWidget->currentDatabase());
            QString sql = sqlList.at(i) + ";";
            query.exec(sql);

            if (query.lastError().type() != QSqlError::NoError) {
                lineEditStatus->setText(query.lastError().text());
                return;
            } else {
                ++noRows;
            }
        }

        lineEditStatus->setText(tr("Query OK, number of affected rows: %1").arg(noRows));
    }

    updateActions();
}

void DB_BrowserDialog::browseDatabase() {
    RB_DEBUG->print("DB_BrowserDialog::browseDatabase()");
    connectionWidget->refresh();
    connectionWidgetSrc->refresh();
    connectionWidgetDest->refresh();
}

void DB_BrowserDialog::showTable(const QString &t, QTableView* tv) {
    RB_DEBUG->print("DB_BrowserDialog::showTable()");

    QSqlTableModel* model = qobject_cast<QSqlTableModel*>(tv->model());

    if (model) {
        // Save unsaved data
        model->submitAll();
        delete model;
        tv->setModel(NULL);
    }

    QStandardItemModel* simodel = qobject_cast<QStandardItemModel*>(tv->model());
    if (simodel) {
        delete simodel;
    }

    if (tv == tvSource) {
        model = new QSqlTableModel(tv, connectionWidgetSrc->currentDatabase());
    } else if (tv == tvDestination) {
        model = new QSqlTableModel(tv, connectionWidgetDest->currentDatabase());
    } else { // tvDefault
        model = new QSqlTableModel(tv, connectionWidget->currentDatabase());
    }

    model->setTable(t);
    model->select();

    if (model->lastError().type() != QSqlError::NoError)
        lineEditStatus->setText(model->lastError().text());

    tv->setModel(model);
    tv->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed);

    connect(tv->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(slotCurrentRowChanged()));

    updateActions();
}

void DB_BrowserDialog::showMetaData(const QString& t, QTableView* tv) {
    if (connectionWidget->currentDatabase().driverName() == "QMYSQL") {
        showMysqlMetaData(t, connectionWidget->currentDatabase(), tv);
        return;
    }

    QSqlTableModel* tblmodel = qobject_cast<QSqlTableModel*>(tv->model());

    if (tblmodel) {
        // Save unsaved data
        tblmodel->submitAll();
        disconnect(tv->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
                   this, SLOT(slotCurrentRowChanged()));
        delete tblmodel;
        tv->setModel(NULL);
    }

    QSqlRecord rec = connectionWidget->currentDatabase().record(t);
    QStandardItemModel* model = new QStandardItemModel(tv);

    model->insertRows(0, rec.count());
    model->insertColumns(0, 7);

    model->setHeaderData(0, Qt::Horizontal, "Fieldname");
    model->setHeaderData(1, Qt::Horizontal, "Type");
    model->setHeaderData(2, Qt::Horizontal, "Length");
    model->setHeaderData(3, Qt::Horizontal, "Precision");
    model->setHeaderData(4, Qt::Horizontal, "Required");
    model->setHeaderData(5, Qt::Horizontal, "AutoValue");
    model->setHeaderData(6, Qt::Horizontal, "DefaultValue");


    for (int i = 0; i < rec.count(); ++i) {
        QSqlField fld = rec.field(i);
        model->setData(model->index(i, 0), fld.name());
        model->setData(model->index(i, 1), fld.typeID() == -1
                ? QString(QVariant::typeToName(fld.type()))
                : QString("%1 (%2)").arg(QVariant::typeToName(fld.type())).arg(fld.typeID()));
        model->setData(model->index(i, 2), fld.length());
        model->setData(model->index(i, 3), fld.precision());
        model->setData(model->index(i, 4), fld.requiredStatus() == -1 ? QVariant("?")
                : QVariant(bool(fld.requiredStatus())));
        model->setData(model->index(i, 5), fld.isAutoValue());
        model->setData(model->index(i, 6), fld.defaultValue());
    }

    tv->setModel(model);
    tv->setEditTriggers(QAbstractItemView::NoEditTriggers);

    updateActions();
}

/**
    SELECT COLUMN_NAME, DATA_TYPE, CHARACTER_MAXIMUM_LENGTH,
      NUMERIC_PRECISION, IS_NULLABLE, EXTRA, COLUMN_DEFAULT,
      COLUMN_KEY, NUMERIC_SCALE, COLUMN_COMMENT
    FROM INFORMATION_SCHEMA.COLUMNS
    WHERE table_name = 'acc_customers'
    AND TABLE_SCHEMA = 'biluna'
*/
void DB_BrowserDialog::showMysqlMetaData(const QString& t,
                                         const QSqlDatabase& currentDb,
                                         QTableView* tv) {
    RB_String qStr = "";
    qStr = "SELECT COLUMN_NAME, DATA_TYPE, CHARACTER_MAXIMUM_LENGTH, "
                "NUMERIC_PRECISION, IS_NULLABLE, EXTRA, COLUMN_DEFAULT, "
                "COLUMN_KEY, NUMERIC_SCALE, COLUMN_COMMENT "
            "FROM INFORMATION_SCHEMA.COLUMNS "
            "WHERE table_name = '" + t + "' "
            "AND TABLE_SCHEMA = '" + currentDb.databaseName() + "';";
    QSqlQuery query(currentDb);

    if (!query.exec(qStr)) {
        RB_DEBUG->error("DB_BrowserDialog::showMysqlMetaData() "
                        + query.lastError().text() + " ERROR");
        return;
    }

    QSqlTableModel* tblmodel = qobject_cast<QSqlTableModel*>(tv->model());

    if (tblmodel) {
        // Save unsaved data
        tblmodel->submitAll();
        disconnect(tv->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
                   this, SLOT(slotCurrentRowChanged()));
        delete tblmodel;
        tv->setModel(NULL);
    }

    QSqlRecord rec = currentDb.record(t); // the way to get the column names
    QStandardItemModel* model = new QStandardItemModel(tv);

    model->insertRows(0, rec.count());
    model->insertColumns(0, 10);

    model->setHeaderData(0, Qt::Horizontal, "Fieldname");
    model->setHeaderData(1, Qt::Horizontal, "Type");
    model->setHeaderData(2, Qt::Horizontal, "Length");
    model->setHeaderData(3, Qt::Horizontal, "Precision");
    model->setHeaderData(4, Qt::Horizontal, "Required");
    model->setHeaderData(5, Qt::Horizontal, "AutoValue");
    model->setHeaderData(6, Qt::Horizontal, "DefaultValue");
    model->setHeaderData(7, Qt::Horizontal, "ColumnKey");
    model->setHeaderData(8, Qt::Horizontal, "NumericScale");
    model->setHeaderData(9, Qt::Horizontal, "ColumnComment");

    int i = 0;

    while (query.next()) {
        rec = query.record();
        model->setData(model->index(i, 0), rec.value(0));
        model->setData(model->index(i, 1), rec.value(1));
        model->setData(model->index(i, 2), rec.value(2));
        model->setData(model->index(i, 3), rec.value(3));
        // update NULLABLE to required:
        model->setData(model->index(i, 4), rec.value(4) == "NO" ? "YES" : "NO");
        model->setData(model->index(i, 5), rec.value(5));
        model->setData(model->index(i, 6), rec.value(6));
        model->setData(model->index(i, 7), rec.value(7));
        model->setData(model->index(i, 8), rec.value(8));
        model->setData(model->index(i, 9), rec.value(9));
        ++i;
    }

    tv->setModel(model);
    tv->setEditTriggers(QAbstractItemView::NoEditTriggers);

    updateActions();

}

void DB_BrowserDialog::slotCurrentTabChanged(int index) {
    if (index == 1) {
        connectionWidgetSrc->refresh();
        connectionWidgetDest->refresh();
    }
}

void DB_BrowserDialog::insertRow() {
    RB_DEBUG->print("DB_BrowserDialog::insertRow()");
	
    QSqlTableModel *model = qobject_cast<QSqlTableModel *>(tvDefault->model());
    if (!model)
        return;

    QModelIndex insertIndex = tvDefault->currentIndex();
    int row = insertIndex.row() == -1 ? 0 : insertIndex.row();
    model->insertRows(row, 1, QModelIndex());
    insertIndex = model->index(row, 0);
    tvDefault->setCurrentIndex(insertIndex);
    tvDefault->edit(insertIndex);
}

void DB_BrowserDialog::deleteRow() {
    RB_DEBUG->print("DB_BrowserDialog::deleteRow()");
	
    QSqlTableModel *model = qobject_cast<QSqlTableModel *>(tvDefault->model());
    if (!model)
        return;

    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    QModelIndexList currentSelection = tvDefault->selectionModel()->selectedIndexes();
    for (int i = 0; i < currentSelection.count(); ++i) {
        if (currentSelection.at(i).column() != 0)
            continue;
        model->removeRow(currentSelection.at(i).row());
    }

    model->submitAll();
    model->setEditStrategy(QSqlTableModel::OnRowChange);

    updateActions();
}

void DB_BrowserDialog::slotDatabaseConnectionChanged(int connectState) {
    tabWidget->setCurrentIndex(0);
    browseDatabase();

    if (connectState == RB2::ValidTrue) {
        pbTransferData->setEnabled(true);
    } else {
        pbTransferData->setEnabled(false);
        tvDefault->setModel(NULL);
        tvSource->setModel(NULL);
        tvDestination->setModel(NULL);
    }
}

void DB_BrowserDialog::updateActions() {
    RB_DEBUG->print("DB_BrowserDialog::updateActions()");
	
    bool enableIns = qobject_cast<QSqlTableModel *>(tvDefault->model());
    bool enableDel = enableIns && tvDefault->currentIndex().isValid();

    insertRowAction->setEnabled(enableIns);
    deleteRowAction->setEnabled(enableDel);
}

void DB_BrowserDialog::on_connectionWidget_tableActivated(const QString &t) {
    showTable(t, tvDefault);
    lineEditStatus->clear();
}

void DB_BrowserDialog::on_connectionWidget_metaDataRequested(const QString &t) {
    showMetaData(t, tvDefault);
    lineEditStatus->clear();
}

void DB_BrowserDialog::on_connectionWidgetSrc_tableActivated(const QString &t) {
    showTable(t, tvSource);
}

void DB_BrowserDialog::on_connectionWidgetSrc_metaDataRequested(const QString &t) {
    showMetaData(t, tvSource);
}

void DB_BrowserDialog::on_connectionWidgetDest_tableActivated(const QString &t) {
    showTable(t, tvDestination);
}

void DB_BrowserDialog::on_connectionWidgetDest_metaDataRequested(const QString &t) {
    showMetaData(t, tvDestination);
}

void DB_BrowserDialog::on_pbAdd_clicked() {
    RB_Dialog* dlg = DB_DIALOGFACTORY->getDialog(DB_DialogFactory::DialogDbConnection);
    dlg->exec();
    dlg->deleteLater();
    connectionWidget->refresh();
    lineEditStatus->clear();
}

void DB_BrowserDialog::on_pbRemove_clicked() {
    QString currentConName = connectionWidget->currentDatabase().connectionName();

    if (currentConName.isEmpty()) {
        return;
    }

    QString defaultConName = RB_Database::database().connectionName();

    if (defaultConName == currentConName) {
        DB_DIALOGFACTORY->requestWarningDialog(
                tr("Cannot remove default connection here."));
        return;
    }

    QAbstractItemModel* model = tvDefault->model();
    if (model) {
        delete model;
        tvDefault->setModel(NULL);
    }

    RB_Database::removeDatabase(currentConName);
    connectionWidget->refresh();
    lineEditStatus->clear();
}

void DB_BrowserDialog::on_pbSubmit_clicked() {
    if (!teSqlEdit->toPlainText().startsWith("SELECT", Qt::CaseInsensitive)) {
        int res = DB_DIALOGFACTORY->requestYesNoDialog(tr("Submit SQL"),
                                                       tr("Changes to the database cannot be undone.\n"
                                                           "Do you want to submit the SQL?"));
        if (res != QMessageBox::Yes) {
            return;
        }
    }
    QApplication::setOverrideCursor(Qt::WaitCursor);
    sqlExec();
    QApplication::restoreOverrideCursor();
    teSqlEdit->setFocus();
}

void DB_BrowserDialog::on_pbClear_clicked() {
    teSqlEdit->clear();
    teSqlEdit->setFocus();
    lineEditStatus->clear();
}

void DB_BrowserDialog::on_pbClose_clicked() {
    writeSettings();
    reject();
}

/*
 * At CTRL+C event copy data to the global clipboard
 * only takes the surrounding square selection
 * /
void DB_BrowserDialog::keyPressEvent(QKeyEvent *e) {
    if(!(e->key() == Qt::Key_C && e->modifiers() & Qt::ControlModifier)) {
        return;
    }

    // QTableView and RB_MmProxy version
    QItemSelectionModel* sm = tvDefault->selectionModel();
    QModelIndexList idxList;

    if (sm) {
        idxList = sm->selectedIndexes();
    } else {
        DB_DIALOGFACTORY->commandMessage("No selection model in DB_BrowserDialog::keyPressEvent()");
        return;
    }

    int minRowNo = 1000000000; // 10+e09
    int maxRowNo = 0;
    int minColNo = 1000000000; // 10+e09
    int maxColNo = 0;

    QModelIndex idx;

    foreach(idx, idxList) {
        if (idx.row() < minRowNo) {
            minRowNo = idx.row();
        }
        if (idx.row() > maxRowNo) {
            maxRowNo = idx.row();
        }
        if (idx.column() < minColNo) {
            minColNo = idx.column();
        }
        if (idx.column() > maxColNo) {
            maxColNo = idx.column();
        }
    }

    QAbstractItemModel* m = tvDefault->model();
    QByteArray byteArray;

    for (int row = minRowNo; row <= maxRowNo; ++row) {
        for (int col = minColNo; col <= maxColNo; ++col) {
            idx = m->index(row, col);
            byteArray.append(idx.data().toString() + "\t");
        }
        byteArray.append("\n");
    }

    QMimeData* mimeData = new QMimeData();
    mimeData->setData("text/plain", byteArray);
    QApplication::clipboard()->setMimeData(mimeData);
}
*/
void DB_BrowserDialog::on_pbShowSql_clicked() {
    if (cbTables->currentText() == "None") {
        teSqlEdit->setPlainText("No perspective selected");
    } else {
        RB_String sqlStr = "";
        DB_MODELFACTORY->createSql(sqlStr, cbTables->currentText());
        teSqlEdit->setPlainText(sqlStr);
    }
}



void DB_BrowserDialog::on_pbCreateTables_clicked() {
    if (cbTables->currentText() == "None") {
        teSqlEdit->setPlainText("No perspective selected");
    } else {
        int res = DB_DIALOGFACTORY->requestYesNoDialog (
                tr("Confirmation"), tr("Do you want to create the tables?"));
        if (res == (int)QDialog::Rejected) {
            return;
        }

        RB_String msg = DB_MODELFACTORY->createTables(cbTables->currentText());
        teSqlEdit->setPlainText(msg);
    }
}

void DB_BrowserDialog::on_pbCheckTables_clicked() {
    RB_String result = "";
    DB_MODELFACTORY->checkExistingTables(cbTables->currentText(), result);
    teSqlEdit->setPlainText(result);
}

void DB_BrowserDialog::on_pbUpdateTable_clicked() {
    RB_String sqlStr = "";
    RB_String objName = connectionWidget->currentTableName();

    if (!DB_MODELFACTORY->isObject(objName)) {
        // request correction once
        objName = DB_DIALOGFACTORY->requestTextInputDialog(
                    tr("Table Name"), tr("Enter valid table name"), objName);
    }

    DB_MODELFACTORY->createUpdateTableSql(sqlStr, objName);
    teSqlEdit->setPlainText(sqlStr);
}

/**
Create SQL statement which will delete all rows except:
\li id = 0
\li parent = current DB_Project.id
\li table is acc_chartmaster

works only if SQL driver is SQLite
 */
void DB_BrowserDialog::on_pbSettings_clicked() {
    RB_String str = connectionWidget->currentDatabase().driverName();

    if ("QSQLITE" != str){
        teSqlEdit->setPlainText(str + " is not a local SQLite database.");
        return;
    }

    str.clear();
    RB_StringList strL =
            connectionWidget->currentDatabase().driver()->tables(QSql::Tables);

    str = "BEGIN TRANSACTION;\n";

    foreach (const RB_String& s, strL) {
        if (s != "acc_project" && s != "acc_chartmaster") {
            str += "DELETE from " + s + " WHERE id<>0 and parent<>'"
                    + DB_MODELFACTORY->getRootId() + "';\n";
        }
    }

    str += "END TRANSACTION;";
    teSqlEdit->setPlainText(str);
}

void DB_BrowserDialog::on_pbTransferData_clicked() {
	RB_DEBUG->print("DB_BrowserDialog::on_pbTransferData_clicked()");
	
    if (!lwSource->currentItem() || !lwDestination->currentItem()) {
        DB_DIALOGFACTORY->requestWarningDialog(
                tr("No source and/or destination selected."));
        return;
    }

    int srcType = lwSource->currentItem()->data(Qt::UserRole).toInt();
    int destType = lwDestination->currentItem()->data(Qt::UserRole).toInt();
	
    if (srcType == DB_BrowserDialog::SrcAllTables
                && destType == DB_BrowserDialog::DestDatabase) {
        QApplication::setOverrideCursor(Qt::WaitCursor);
        DB_TransferDataAction action;
        bool success = action.transferDatabase(
            connectionWidgetSrc->currentDatabase(),
            connectionWidgetDest->currentDatabase());
        QApplication::restoreOverrideCursor();

        if (success) {
            QMessageBox::information(NULL, QObject::tr("SQL Export"),
                    QObject::tr("Export to database done."));
        } else {
            QMessageBox::warning(NULL, QObject::tr("SQL Export Warning"),
                    QObject::tr("Could not export tables, \r"
                            "no permission to directory or SQL error.\r"));
//            QMessageBox::warning(NULL, QObject::tr("SQLite SQL Warning"),
//                    action.errorMessage());
        }

    } else if (srcType == DB_BrowserDialog::SrcSelectedTable
                    && destType == DB_BrowserDialog::DestDatabase) {
        QApplication::setOverrideCursor(Qt::WaitCursor);
        DB_TransferDataAction action;
        bool success = action.transferDatabaseTable(
            connectionWidgetSrc->currentDatabase(),
            connectionWidgetDest->currentDatabase(),
            connectionWidgetSrc->currentTableName());
        QApplication::restoreOverrideCursor();

        if (success) {
            QMessageBox::information(NULL, QObject::tr("SQL Table Export"),
                    QObject::tr("Export to database done."));
        } else {
            QMessageBox::warning(NULL, QObject::tr("SQL Table Export Warning"),
                    QObject::tr("Could not export table, \r"
                            "no permission to directory or SQL error.\r"));
        }

    } else if (srcType == DB_BrowserDialog::SrcAllTables
               && destType == DB_BrowserDialog::DestXmlFile) {
        RB_SETTINGS->beginGroup("paths");
        RB_String savePath = RB_SETTINGS->value("savePath", "").toString();
        RB_SETTINGS->endGroup();

        RB_String fileName = "";
        // get any file name, existing or not, if not the db will be created
        QFileDialog fileDlg(this,                 // parent
                            "DB - save XML file", // dialog title
                            savePath,             // path
                            "eXtensible Markup Language (*.xml);;all files (*.*)"); // filters
        fileDlg.setFileMode(QFileDialog::AnyFile);
        fileDlg.setAcceptMode(QFileDialog::AcceptSave);
        fileDlg.exec();

        if (fileDlg.result()) {
            fileName = fileDlg.selectedFiles().at(0);
            if (!fileName.endsWith(".xml")) fileName += ".xml";
        }

        if (!fileName.isEmpty()) {
            savePath = QFileInfo(fileName).absolutePath();
            RB_SETTINGS->beginGroup("paths");
            RB_SETTINGS->setValue("savePath", savePath);
            RB_SETTINGS->endGroup();
        } else {
            QMessageBox::information(this, "DB Data Tranfer",
                                     "No action, XML file name is missing");
            return;
        }

        // transfer data action
        QApplication::setOverrideCursor(Qt::WaitCursor);
        DB_TransferDataAction action;
        bool success = action.transferDbXmlFile(
                    connectionWidgetSrc->currentDatabase(), fileName);
        QApplication::restoreOverrideCursor();

        if (success) {
            QMessageBox::information(NULL, QObject::tr("XML File Export"),
                                     QObject::tr("File export done."));
        } else {
            QMessageBox::warning(NULL, QObject::tr("XML Export Warning"),
                                 QObject::tr("Could not export file, \r"
                                             "no permission to directory or XML error."));
        }

    } else if (srcType == DB_BrowserDialog::SrcSelectedTable
               && destType == DB_BrowserDialog::DestXmlFile) {
        RB_SETTINGS->beginGroup("paths");
        RB_String savePath = RB_SETTINGS->value("savePath", "").toString();
        RB_SETTINGS->endGroup();

        RB_String fileName = "";
        // get any file name, existing or not, if not the db will be created
        QFileDialog fileDlg(this,
                            "DB - save XML file",
                            savePath,
                            "eXtensible Markup Language (*.xml);;all files (*.*)");
        fileDlg.setFileMode(QFileDialog::AnyFile);
        fileDlg.setAcceptMode(QFileDialog::AcceptSave);
        fileDlg.exec();

        if (fileDlg.result()) {
            fileName = fileDlg.selectedFiles().at(0);
            if (!fileName.endsWith(".xml")) fileName += ".xml";
        }
	
        if (!fileName.isEmpty()) {
            RB_String savePath = QFileInfo(fileName).absolutePath();
            RB_SETTINGS->beginGroup("paths");
            RB_SETTINGS->setValue("savePath", savePath);
            RB_SETTINGS->endGroup();
        } else {
            QMessageBox::information(this, "DB Data Tranfer",
                                     "No action, XML file name is missing");
            QApplication::restoreOverrideCursor();
            return;
        }

        // transfer data action
        QApplication::setOverrideCursor(Qt::WaitCursor);
        DB_TransferDataAction action;
        bool success = action.transferDbXmlFile(
                    connectionWidgetSrc->currentDatabase(), fileName,
                    connectionWidgetSrc->currentTableName());
        QApplication::restoreOverrideCursor();

        if (success) {
            QMessageBox::information(NULL, QObject::tr("XML File Export"),
                                     QObject::tr("File export done."));
        } else {
            QMessageBox::warning(NULL, QObject::tr("XML Export Warning"),
                                 QObject::tr("Could not export file, \r"
                                             "no permission to directory or XML error."));
        }

    } else if (srcType == DB_BrowserDialog::SrcSelectedRows
               && destType == DB_BrowserDialog::DestXmlFileHierarchy) {

        RB_String tableName = connectionWidgetSrc->currentTableName();
        RB_String fileName = tableName.toLower() + "_id.xml";
        DB_ActionFileSaveAs a;
        a.getSaveFileName(fileName);

        QModelIndex idx = tvSource->currentIndex();
        if (!idx.isValid()) {
            DB_DIALOGFACTORY->requestWarningDialog(tr("No valid row selected."));
            return;
        }
        QModelIndex rootIdx = idx.model()->index(idx.row(), 0);


        // transfer data action
        QApplication::setOverrideCursor(Qt::WaitCursor);
        DB_TransferDataAction action;
        bool success = action.transferDbXmlFileHierarchy(
                    connectionWidgetSrc->currentDatabase(),
                    fileName,
                    connectionWidgetSrc->currentTableName(),
                    rootIdx.data().toString());
        QApplication::restoreOverrideCursor();

        if (success) {
            DB_DIALOGFACTORY->requestWarningDialog(QObject::tr("File export done."));
        } else {
            DB_DIALOGFACTORY->requestWarningDialog(
                        QObject::tr("Could not export file,\n"
                                    "no permission to directory,\n"
                                    "relevant perspective is not loaded,\n"
                                    "or XML error."));
        }


    } else if (srcType == DB_BrowserDialog::SrcAllTables
               && destType == DB_BrowserDialog::DestCsvFileComma) {
        RB_SETTINGS->beginGroup("paths");
		RB_String savePath = RB_SETTINGS->value("savePath", "").toString();
		RB_SETTINGS->endGroup();

		RB_String fileName = "";
		// get any file name, existing or not, if not the db will be created
		QFileDialog fileDlg(this,							// parent
    		"DB - save CSV file", 							// dialog title
	        savePath,										// path
            "Comma Separated Value [,] (*.csv);;all files (*.*)"); // filters
		fileDlg.setFileMode(QFileDialog::AnyFile); 
		fileDlg.setAcceptMode(QFileDialog::AcceptSave);
		fileDlg.exec();
		
		if (fileDlg.result()) {
			fileName = fileDlg.selectedFiles().at(0);
            if (!fileName.endsWith(".csv")) fileName += ".csv";
		}
	
		if (!fileName.isEmpty()) {
            RB_String savePath = QFileInfo(fileName).absolutePath();
			RB_SETTINGS->beginGroup("paths");
			RB_SETTINGS->setValue("savePath", savePath);
			RB_SETTINGS->endGroup();
	    } else {
	    	QMessageBox::information(this, "DB Data Tranfer", 
	    			"No action, CSV File Name is missing");
            QApplication::restoreOverrideCursor();
	    	return;
	    }

		// transfer data action
        QApplication::setOverrideCursor(Qt::WaitCursor);
        DB_TransferDataAction action;
		bool success = action.transferDbCsvCommaFile(
                connectionWidgetSrc->currentDatabase(), fileName);
        QApplication::restoreOverrideCursor();

		if (success) {
			QMessageBox::information(NULL, QObject::tr("CSV File Export"),
					QObject::tr("File export done."));
		} else {
			QMessageBox::warning(NULL, QObject::tr("CSV Export Warning"),
					QObject::tr("Could not export file, \r"
							"no permission to directory or CSV error."));
		}

    } else if (srcType == DB_BrowserDialog::SrcSelectedTable
               && destType == DB_BrowserDialog::DestCsvFileComma) {
        RB_SETTINGS->beginGroup("paths");
		RB_String savePath = RB_SETTINGS->value("savePath", "").toString();
		RB_SETTINGS->endGroup();

		RB_String fileName = "";
		// get any file name, existing or not, if not the db will be created
		QFileDialog fileDlg(this,							// parent
    		"DB - save CSV file", 							// dialog title
	        savePath,										// path
            "Comma Separated Value [,] (*.csv);;all files (*.*)"); // filters
		fileDlg.setFileMode(QFileDialog::AnyFile); 
		fileDlg.setAcceptMode(QFileDialog::AcceptSave);
		fileDlg.exec();
		
		if (fileDlg.result()) {
			fileName = fileDlg.selectedFiles().at(0);
            if (!fileName.endsWith(".csv")) fileName += ".csv";
		}
	
		if (!fileName.isEmpty()) {
            RB_String savePath = QFileInfo(fileName).absolutePath();
			RB_SETTINGS->beginGroup("paths");
			RB_SETTINGS->setValue("savePath", savePath);
			RB_SETTINGS->endGroup();
	    } else {
	    	QMessageBox::information(this, "DB Data Tranfer", 
	    			"No action, CSV file name is missing");
            QApplication::restoreOverrideCursor();
	    	return;
	    }

		// transfer data action
        QApplication::setOverrideCursor(Qt::WaitCursor);
        DB_TransferDataAction action;
        bool success = action.transferDbCsvCommaFile(
                connectionWidgetSrc->currentDatabase(), fileName,
                connectionWidgetSrc->currentTableName());
        QApplication::restoreOverrideCursor();

		if (success) {
			QMessageBox::information(NULL, QObject::tr("CSV File Export"),
					QObject::tr("File export done."));
		} else {
			QMessageBox::warning(NULL, QObject::tr("CSV Export Warning"),
					QObject::tr("Could not export file, \r"
							"no permission to directory or CSV error."));
		}

    } else if (srcType == DB_BrowserDialog::SrcAllTables
               && destType == DB_BrowserDialog::DestCsvFileSemiColon) {
        RB_SETTINGS->beginGroup("paths");
		RB_String savePath = RB_SETTINGS->value("savePath", "").toString();
		RB_SETTINGS->endGroup();

		RB_String fileName = "";
		// get any file name, existing or not, if not the db will be created
		QFileDialog fileDlg(this,							// parent
    		"DB - save CSV file", 							// dialog title
	        savePath,										// path
            "Comma Separated Value [;] (*.csv);;all files (*.*)"); // filters
		fileDlg.setFileMode(QFileDialog::AnyFile); 
		fileDlg.setAcceptMode(QFileDialog::AcceptSave);
		fileDlg.exec();
		
		if (fileDlg.result()) {
			fileName = fileDlg.selectedFiles().at(0);
            if (!fileName.endsWith(".csv")) fileName += ".csv";
		}
	
		if (!fileName.isEmpty()) {
            RB_String savePath = QFileInfo(fileName).absolutePath();
			RB_SETTINGS->beginGroup("paths");
			RB_SETTINGS->setValue("savePath", savePath);
			RB_SETTINGS->endGroup();
	    } else {
	    	QMessageBox::information(this, "DB Data Tranfer", 
	    			"No action, CSV File Name is missing");
	    	return;
	    }

		// transfer data action
        QApplication::setOverrideCursor(Qt::WaitCursor);
        DB_TransferDataAction action;
		bool success = action.transferDbCsvSemicolonFile(
                connectionWidgetSrc->currentDatabase(), fileName);
        QApplication::restoreOverrideCursor();

		if (success) {
			QMessageBox::information(NULL, QObject::tr("CSV File Export"),
					QObject::tr("File export done."));
		} else {
			QMessageBox::warning(NULL, QObject::tr("CSV Export Warning"),
					QObject::tr("Could not export file, \r"
							"no permission to directory or CSV error."));
		}

    } else if (srcType == DB_BrowserDialog::SrcSelectedTable
               && destType == DB_BrowserDialog::DestCsvFileSemiColon) {
        RB_SETTINGS->beginGroup("paths");
		RB_String savePath = RB_SETTINGS->value("savePath", "").toString();
		RB_SETTINGS->endGroup();

		RB_String fileName = "";
		// get any file name, existing or not, if not the db will be created
		QFileDialog fileDlg(this,							// parent
    		"DB - save CSV file", 							// dialog title
	        savePath,										// path
            "Comma Separated Value [;] (*.csv);;all files (*.*)"); // filters
		fileDlg.setFileMode(QFileDialog::AnyFile); 
		fileDlg.setAcceptMode(QFileDialog::AcceptSave);
		fileDlg.exec();
		
		if (fileDlg.result()) {
			fileName = fileDlg.selectedFiles().at(0);
            if (!fileName.endsWith(".csv")) fileName += ".csv";
		}
	
		if (!fileName.isEmpty()) {
            RB_String savePath = QFileInfo(fileName).absolutePath();
			RB_SETTINGS->beginGroup("paths");
			RB_SETTINGS->setValue("savePath", savePath);
			RB_SETTINGS->endGroup();
	    } else {
	    	QMessageBox::information(this, "DB Data Tranfer", 
	    			"No action, CSV file name is missing");
            QApplication::restoreOverrideCursor();
	    	return;
	    }

		// transfer data action
        QApplication::setOverrideCursor(Qt::WaitCursor);
        DB_TransferDataAction action;
        bool success = action.transferDbCsvSemicolonFile(
                connectionWidgetSrc->currentDatabase(), fileName,
                connectionWidgetSrc->currentTableName());
        QApplication::restoreOverrideCursor();

		if (success) {
			QMessageBox::information(NULL, QObject::tr("CSV File Export"),
					QObject::tr("File export done."));
		} else {
			QMessageBox::warning(NULL, QObject::tr("CSV Export Warning"),
					QObject::tr("Could not export file, \r"
							"no permission to directory or CSV error."));
		}

    } else if (srcType == DB_BrowserDialog::SrcAllTables
               && destType == DB_BrowserDialog::DestCsvFileTab) {
        RB_SETTINGS->beginGroup("paths");
		RB_String savePath = RB_SETTINGS->value("savePath", "").toString();
		RB_SETTINGS->endGroup();

		RB_String fileName = "";
		// get any file name, existing or not, if not the db will be created
		QFileDialog fileDlg(this,							// parent
    		"DB - save CSV file", 							// dialog title
	        savePath,										// path
            "Comma Separated Value [TAB] (*.csv);;all files (*.*)"); // filters
		fileDlg.setFileMode(QFileDialog::AnyFile); 
		fileDlg.setAcceptMode(QFileDialog::AcceptSave);
		fileDlg.exec();
		
		if (fileDlg.result()) {
			fileName = fileDlg.selectedFiles().at(0);
            if (!fileName.endsWith(".csv")) fileName += ".csv";
		}
	
		if (!fileName.isEmpty()) {
            RB_String savePath = QFileInfo(fileName).absolutePath();
			RB_SETTINGS->beginGroup("paths");
			RB_SETTINGS->setValue("savePath", savePath);
			RB_SETTINGS->endGroup();
	    } else {
	    	QMessageBox::information(this, "DB Data Tranfer", 
	    			"No action, CSV File Name is missing");
            QApplication::restoreOverrideCursor();
	    	return;
	    }

		// transfer data action
        QApplication::setOverrideCursor(Qt::WaitCursor);
        DB_TransferDataAction action;
		bool success = action.transferDbCsvTabFile(
                connectionWidgetSrc->currentDatabase(), fileName);
        QApplication::restoreOverrideCursor();

		if (success) {
			QMessageBox::information(NULL, QObject::tr("CSV File Export"),
					QObject::tr("File export done."));
		} else {
			QMessageBox::warning(NULL, QObject::tr("CSV Export Warning"),
					QObject::tr("Could not export file, \r"
							"no permission to directory or CSV error."));
		}

    } else if (srcType == DB_BrowserDialog::SrcSelectedTable
               && destType == DB_BrowserDialog::DestCsvFileTab) {
        RB_SETTINGS->beginGroup("paths");
		RB_String savePath = RB_SETTINGS->value("savePath", "").toString();
		RB_SETTINGS->endGroup();

		RB_String fileName = "";
		// get any file name, existing or not, if not the db will be created
		QFileDialog fileDlg(this,							// parent
    		"DB - save CSV file", 							// dialog title
	        savePath,										// path
            "Comma Separated Value [TAB] (*.csv);;all files (*.*)"); // filters
		fileDlg.setFileMode(QFileDialog::AnyFile); 
		fileDlg.setAcceptMode(QFileDialog::AcceptSave);
		fileDlg.exec();
		
		if (fileDlg.result()) {
			fileName = fileDlg.selectedFiles().at(0);
            if (!fileName.endsWith(".csv")) fileName += ".csv";
		}
	
		if (!fileName.isEmpty()) {
            RB_String savePath = QFileInfo(fileName).absolutePath();
			RB_SETTINGS->beginGroup("paths");
			RB_SETTINGS->setValue("savePath", savePath);
			RB_SETTINGS->endGroup();
	    } else {
	    	QMessageBox::information(this, "DB Data Tranfer", 
	    			"No action, CSV file name is missing");
            QApplication::restoreOverrideCursor();
	    	return;
	    }

		// transfer data action
        QApplication::setOverrideCursor(Qt::WaitCursor);
        DB_TransferDataAction action;
        bool success = action.transferDbCsvTabFile(
                connectionWidgetSrc->currentDatabase(), fileName,
                connectionWidgetSrc->currentTableName());
        QApplication::restoreOverrideCursor();

		if (success) {
			QMessageBox::information(NULL, QObject::tr("CSV File Export"),
					QObject::tr("File export done."));
		} else {
			QMessageBox::warning(NULL, QObject::tr("CSV Export Warning"),
					QObject::tr("Could not export file, \r"
							"no permission to directory or CSV error."));
		}
	} else {
		QMessageBox::information(NULL, QObject::tr("Data Transfer"),
				QObject::tr("Function not yet supported."));
	}
}

