/*****************************************************************
 * $Id: db_connectiondialog.cpp 2215 2015-01-31 14:37:26Z rutger $
 *
 * Copyright (C) 2005 Red-Bag. All rights reserved.
 * This file is part of the DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/


#include "db_connectiondialog.h"

#include <QFileDialog>
#include <QNetworkInterface>
#include <QPluginLoader>
#include <QSqlDatabase>

#include "db_dialogfactory.h"
#include "db_utilityfactory.h"
#include "rb_mainwindow.h"
#include "rb_modelfactory.h"
#include "rb_settings.h"

int DB_ConnectionDialog::mConnectionNumber = 1;

/**
 * Constructor
 */
DB_ConnectionDialog::DB_ConnectionDialog(QWidget* parent) : RB_Dialog(parent) {
    RB_DEBUG->print("DB_ConnectionDialog::DB_ConnectionDialog()");

    mConnectionSeparator = "}{";
    mConnectionItemSeparator = ")(";
    mDataIsChanged = false;

    setupUi(this);
}

/**
 * Destructor
 */
DB_ConnectionDialog::~DB_ConnectionDialog() {
    // nothing
    RB_DEBUG->print("DB_ConnectionDialog::~DB_ConnectionDialog() OK");
}

/**
 * Initialize this dialog
 * TODO: test setting of plugin path
 */
void DB_ConnectionDialog::init() {
    RB_StringList drivers = QSqlDatabase::drivers();

    // Start with dynamic loading driver plugins
    mPluginsDir = QDir(qApp->applicationDirPath());

    // remove compat names
    drivers.removeAll("QMYSQL3");
    drivers.removeAll("QOCI8");
    drivers.removeAll("QODBC3");
    drivers.removeAll("QPSQL7");
    drivers.removeAll("QTDS7");
    cbDbDriver->addItem("None");
    cbDbDriver->addItems(drivers);

    // Read position of dialog and database connections
    readSettings();

    RB_SETTINGS->beginGroup("databaselist");
    QString dbConStr = RB_SETTINGS->value("dbconnectionlist", "").toString();
    RB_SETTINGS->endGroup();

    QStringList viewList;

    if (!dbConStr.isEmpty()) {
        mConnectionList = dbConStr.split(mConnectionSeparator);
        int size = mConnectionList.size();

        for (int i = 0; i < size; ++i) {
            QStringList connectionItems = mConnectionList.at(i).split(mConnectionItemSeparator);
            if (connectionItems.at(0).toInt() < 1) {
                viewList << connectionItems.at(6) + " @ " + connectionItems.at(2);
            } else {
                viewList << connectionItems.at(8);
            }
        }
    }

    QStringListModel* prevModel = new QStringListModel();
    prevModel->setStringList(viewList);
    this->lvPrevious->setModel(prevModel);

    connect(lvPrevious->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(slotSetDbConnectionWidgets(QModelIndex, QModelIndex)));

    connect(rbServerDatabase, SIGNAL(toggled(bool)),
            this, SLOT(slotDbTypeChanged(bool)));
    connect(rbLocalAccessDb, SIGNAL(toggled(bool)),
            this, SLOT(slotDbTypeChanged(bool)));
    connect(rbLocalAccessDb, SIGNAL(toggled(bool)),
            this, SLOT(slotLocalDbSelected(bool)));
    connect(rbLocalSqliteDb, SIGNAL(toggled(bool)),
            this, SLOT(slotDbTypeChanged(bool)));
    connect(rbLocalSqliteDb, SIGNAL(toggled(bool)),
            this, SLOT(slotLocalDbSelected(bool)));
    connect(cbDbDriver, SIGNAL(editTextChanged(QString)),
            this, SLOT(slotDbDataChanged(QString)));

    connect(leDbHost, SIGNAL(editingFinished()),
            this, SLOT(slotDbEditFinished()));
    connect(sbDbPort, SIGNAL(editingFinished()),
            this, SLOT(slotDbEditFinished()));
    connect(leDbUser, SIGNAL(editingFinished()),
            this, SLOT(slotDbEditFinished()));
    connect(leDbPassword, SIGNAL(editingFinished()),
            this, SLOT(slotDbEditFinished()));
    connect(leDbName, SIGNAL(editingFinished()),
            this, SLOT(slotDbEditFinished()));
    connect(sbKeepAliveInterval, SIGNAL(editingFinished()),
            this, SLOT(slotDbEditFinished()));

    connect(leUserName, SIGNAL(editingFinished()),
            this, SLOT(slotDbEditFinished()));
    connect(leUserPassword, SIGNAL(editingFinished()),
            this, SLOT(slotDbEditFinished()));
    connect(leLocalDbName, SIGNAL(textChanged(QString)),
            this, SLOT(slotDbDataChanged(QString)));

    if (mConnectionList.size() > 0) {
        tabWidget->setCurrentIndex(0);
        lvPrevious->selectionModel()->select(prevModel->index(0,0),
                                             QItemSelectionModel::Select);
        slotSetDbConnectionWidgets(prevModel->index(0,0), QModelIndex());

        leUserPassword->setFocus();
        leUserPassword->selectAll();
    } else {
        // first time
        tabWidget->setCurrentIndex(1);
        rbLocalSqliteDb->click();
        leLocalDbName->setFocus();
        leLocalDbName->selectAll();
    }

    // Emits customContexMenuRequested()
    lvPrevious->setContextMenuPolicy(Qt::ActionsContextMenu);
    QAction* actionDelete = new QAction("Delete", lvPrevious);
    lvPrevious->addAction(actionDelete);
    connect(actionDelete, SIGNAL(triggered()),
            this, SLOT(slotDeletePreviousConnection()));

}

/**
 * Update user interface
 */
void DB_ConnectionDialog::updateUi() {
    if (rbLocalAccessDb->isChecked()) {
        int index = cbDbDriver->findText("QODBC");
        if (index > 0) {
            cbDbDriver->setCurrentIndex(index);
        } else {
            cbDbDriver->setCurrentIndex(0);
        }
    } else if (rbLocalSqliteDb->isChecked()) {
        int index = cbDbDriver->findText("QSQLITE");
        if (index > 0) {
            cbDbDriver->setCurrentIndex(index);
        } else {
            cbDbDriver->setCurrentIndex(0);
        }
    }

    cbDbDriver->setEnabled(rbServerDatabase->isChecked());

    leDbName->setEnabled(rbServerDatabase->isChecked());
    leDbUser->setEnabled(rbServerDatabase->isChecked());
    leDbPassword->setEnabled(rbServerDatabase->isChecked());
    leDbHost->setEnabled(rbServerDatabase->isChecked());
    sbDbPort->setEnabled(rbServerDatabase->isChecked());
    sbKeepAliveInterval->setEnabled(rbServerDatabase->isChecked());
    pbTestConnection->setEnabled(rbServerDatabase->isChecked());
    leTestConnection->setEnabled(rbServerDatabase->isChecked());

    leLocalDbName->setEnabled(!rbServerDatabase->isChecked());
    pbSelectLocalDatabase->setEnabled(!rbServerDatabase->isChecked());
}


void DB_ConnectionDialog::writeSettings() {
    RB_DEBUG->print("DB_ConnectionDialog::writeSettings()");

    if (!mDataIsChanged) {
        return;
    }

    if (this->tabWidget->currentIndex() == 0) {
        // move selected database connection to top
        int row = lvPrevious->currentIndex().row();

        if (row > 0) {
            QString connectionData = mConnectionList.at(row);
            mConnectionList.removeAt(row);
            mConnectionList.insert(0, connectionData);
        }
    } else {
        // add new database connection
        int selection = 0;
        if (rbLocalAccessDb->isChecked()) {
            selection = 1;
        } else if (rbLocalSqliteDb->isChecked()) {
            selection = 2;
        }

        RB_String dbPwd = DB_UTILITYFACTORY->encrypt(leDbPassword->text());

        // Note: user password is also stored.
        RB_String userPwd = "";

        if (!leUserPassword->text().isEmpty()) {
            userPwd = DB_UTILITYFACTORY->encrypt(leUserPassword->text());
        }

        QString dbConnection;
        dbConnection = QString::number(selection) + mConnectionItemSeparator
                + cbDbDriver->currentText() + mConnectionItemSeparator
                + leDbHost->text() + mConnectionItemSeparator
                + QString::number(sbDbPort->value()) + mConnectionItemSeparator
                + leDbUser->text() + mConnectionItemSeparator
                + dbPwd + mConnectionItemSeparator
                + leDbName->text() + mConnectionItemSeparator
                + QString::number(sbKeepAliveInterval->value()) + mConnectionItemSeparator
                + leLocalDbName->text() + mConnectionItemSeparator
                + leUserName->text() + mConnectionItemSeparator
                + userPwd;

        mConnectionList.insert(0, dbConnection);

        while (mConnectionList.size() > 20) {
            mConnectionList.removeLast();
        }
    }

    RB_String dbConnectionStr = mConnectionList.toString(mConnectionSeparator);
    RB_SETTINGS->beginGroup("databaselist");
    RB_SETTINGS->setValue("dbconnectionlist", dbConnectionStr);
    RB_SETTINGS->endGroup();

    RB_Dialog::writeSettings();
}

RB2::DatabaseDriver DB_ConnectionDialog::databaseDriver() const {
    return RB2::driverNameToEnum(cbDbDriver->currentText());
}


RB_String DB_ConnectionDialog::databaseName() const {
    return leDbName->text();
}


RB_String DB_ConnectionDialog::databaseUser() const {
    return leDbUser->text();
}


RB_String DB_ConnectionDialog::databasePassword() const {
    return leDbPassword->text();
}


RB_String DB_ConnectionDialog::databaseHost() const {
    return leDbHost->text();
}


int DB_ConnectionDialog::databasePort() const {
    return sbDbPort->value();
}


/**
 * @return local database name
 */
RB_String DB_ConnectionDialog::localDatabaseName() const {
    return leLocalDbName->text();
}

RB_String DB_ConnectionDialog::userName() const {
    return leUserName->text();
}


RB_String DB_ConnectionDialog::userPassword() const {
    return leUserPassword->text();
}

void DB_ConnectionDialog::on_pbHelp_clicked() {
    RB_MainWindow* mw = DB_DIALOGFACTORY->getMainWindow();
    mw->slotHelpSubject(this->objectName());
}

void DB_ConnectionDialog::on_okButton_clicked() {
    if (cbDbDriver->currentText().isEmpty()
                    || cbDbDriver->currentText().toLower() == "none") {
        QMessageBox::information(this, tr("No database driver selected"),
                                 tr("Please select a database driver"));
        cbDbDriver->setFocus();
        return;
    } else {
        QApplication::setOverrideCursor(Qt::WaitCursor);
        if (!RB_Database::database().isOpen()) {
            // Create default connection
            QSqlDatabase db = RB_DATABASE->addDatabase(cbDbDriver->currentText()
                                                     /* ,"defaultConnection" */);
            createDatabase(db);

            if (!db.open()) {
                DB_DIALOGFACTORY->requestWarningDialog("Could not connect to database.\n"
                                                       + db.lastError().text());
                RB_String defaultConn = db.connectionName();
                RB_DATABASE->removeDatabase(defaultConn);
                reject();
                QApplication::restoreOverrideCursor();
                return;
            }

            RB_DATABASE->keepConnectionAlive(db, sbKeepAliveInterval->value() * 1000);
        } else {
            // Default connection already exists and new connection
            // to be created, used in DB_BrowserDialog
            RB_String conNr = "connection" + RB_String::number(++mConnectionNumber);
            QSqlDatabase extraDb = RB_DATABASE->addDatabase(cbDbDriver->currentText(), conNr);
            createDatabase(extraDb);

            if (!extraDb.open()) {
                DB_DIALOGFACTORY->requestWarningDialog("Could not connect to database.\n"
                                                       + extraDb.lastError().text());
                extraDb = QSqlDatabase();
                RB_DATABASE->removeDatabase(conNr);
                reject();
                QApplication::restoreOverrideCursor();
                return;
            }

            RB_DATABASE->keepConnectionAlive(extraDb, sbKeepAliveInterval->value() * 1000);
        }

        writeSettings();
        accept();
        QApplication::restoreOverrideCursor();
    }
}

/**
 * Select local (MSAccess or SQLite) database path/name
 */
void DB_ConnectionDialog::on_pbSelectLocalDatabase_clicked() {
    RB_SETTINGS->beginGroup("paths");
    RB_String openLocalDbPath = RB_SETTINGS->value("openLocalDbPath", "").toString();
    RB_SETTINGS->endGroup();

    RB_String dbn = "";

    if (rbLocalAccessDb->isChecked()) {
        // file needs to exist
        dbn = QFileDialog::getOpenFileName(
                    this, 	// parent
                    "DB - open MS(Access) database",		// dialog title
                    openLocalDbPath,                        // path
                    "MSAccess database (*.mdb);;"
                    "MS database (*.accdb);;"
                    "MS spreadsheet 2003 (*.xls);;"
                    "all files (*.*)");                     // filters
                    // "MSAccess database (*.mdb)",         // selected filter
                    // 0); 					// option
    } else if (rbLocalSqliteDb->isChecked()) {
        // Gives different smaller file dialog and allows the creation
        // any file, existing or not, if not the db will be created
        //QFileDialog fileDlg(
        //            this,
        //            "DB - open SQLite database",
        //            openLocalDbPath,
        //            "SQLite (*.db3);;all files (*.*)");
        //fileDlg.setFileMode(QFileDialog::AnyFile);
        //fileDlg.exec();

        // Native dialog but does not allow creation of file
        dbn = QFileDialog::getOpenFileName(
                    this, 	// parent
                    "DB - open SQLite database",
                    openLocalDbPath,
                    "SQLite (*.db3);;"
                    "SQLite (*.db);;"
                    "all files (*.*)");
                    // "SQLite (*.db3)",	// selected filter
                    // 0); 					// option
        if (!dbn.isEmpty()) {
            if (!dbn.endsWith(".db") && !dbn.endsWith(".db3")) {
                dbn.append(".db3");
            }
        }
    }

    if (!dbn.isEmpty()) {
        leLocalDbName->setText(QFileInfo(dbn).absoluteFilePath());

        RB_String localDbPath = QFileInfo(dbn).absolutePath();
        RB_SETTINGS->beginGroup("paths");
        RB_SETTINGS->setValue("openLocalDbPath", localDbPath);
        RB_SETTINGS->endGroup();
    }
}

/**
 * Format edit widgets based on local database selected
 */
void DB_ConnectionDialog::on_rbLocalAccessDb_clicked() {
    // change of radio button selection already done
    updateUi();
}

/**
 * Format edit widgets based on local database selected
 */
void DB_ConnectionDialog::on_rbLocalSqliteDb_clicked() {
    // change of radio button selection already done
    updateUi();
}

/**
 * Format edit widgets based on server database selected
 */
void DB_ConnectionDialog::on_rbServerDatabase_clicked() {
    // change of radio button selection already done
    updateUi();
}

/**
 * Push button Test Connection clicked
 */
void DB_ConnectionDialog::on_pbTestConnection_clicked() {
    QApplication::setOverrideCursor(Qt::WaitCursor);

    if (!RB_DATABASE->database("testConnection").isOpen()) {
        QSqlDatabase testDb = RB_DATABASE->addDatabase(cbDbDriver->currentText(),
                                                      "testConnection");
        createDatabase(testDb);

        if (!testDb.open()) {
            leTestConnection->setText(testDb.lastError().text());
        } else {
            leTestConnection->setText(tr("Connection OK"));
        }
    } else {
        leTestConnection->setText(tr("Connection OK"));
    }

    // Remove when testDb is out of scope
    RB_DATABASE->removeDatabase("testConnection");
    QApplication::restoreOverrideCursor();
}

void DB_ConnectionDialog::slotSetDbConnectionWidgets(const QModelIndex& curr,
                                                     const QModelIndex& prev) {
    Q_UNUSED(prev);

    if (!curr.isValid()) {
        rbServerDatabase->setChecked(false);
        rbLocalAccessDb->setChecked(false);
        rbLocalSqliteDb->setChecked(false);

        cbDbDriver->setCurrentIndex(0);

        leDbName->clear();
        leDbUser->clear();
        leDbPassword->clear();
        leDbHost->clear();

        sbDbPort->setSpecialValueText("Default");
        sbKeepAliveInterval->setValue(0);

        leLocalDbName->clear();
        leUserName->clear();
        leUserPassword->clear();

        return;
    }

    // Set data in relevant widgets
    int row = curr.row();

    if (row >= mConnectionList.size()) {
        DB_DIALOGFACTORY->commandMessage("Invalid database data row.");
        return;
    }

    QString connectionData = mConnectionList.at(row);
    QStringList connectionItemList = connectionData.split(mConnectionItemSeparator);

    int dbSelection = connectionItemList.at(0).toInt();
    RB_String dbDriver = connectionItemList.at(1);
    RB_String dbHost = connectionItemList.at(2);
    RB_String dbPort = connectionItemList.at(3);
    RB_String dbUser = connectionItemList.at(4);
    RB_String dbPwd = DB_UTILITYFACTORY->decrypt(QVariant(connectionItemList.at(5)));
    RB_String dbName = connectionItemList.at(6);
    RB_String dbKeepAlive = connectionItemList.at(7);
    RB_String localDbName = connectionItemList.at(8);
    RB_String userName = connectionItemList.at(9);
    RB_String userPwd = "";

    if (connectionItemList.size() > 9) { // for compatibility only
        userPwd = DB_UTILITYFACTORY->decrypt(QVariant(connectionItemList.at(9)));
    }

    rbServerDatabase->setChecked(dbSelection == 0);
    rbLocalAccessDb->setChecked(dbSelection == 1);
    rbLocalSqliteDb->setChecked(dbSelection == 2);

    int idx = cbDbDriver->findText(dbDriver);
    if (idx < 0) idx = 0;
    cbDbDriver->setCurrentIndex(idx);

    leDbName->setText(dbName);
    leDbUser->setText(dbUser);
    leDbPassword->setText(dbPwd);
    leDbHost->setText(dbHost);

    if (dbPort.toLower() == "default") {
        sbDbPort->setSpecialValueText("Default");
    } else {
        sbDbPort->setValue(dbPort.toInt());
    }

    sbKeepAliveInterval->setValue(dbKeepAlive.toInt());

    leLocalDbName->setText(localDbName);
    leUserName->setText(userName);
    leUserPassword->setText(userPwd); // to clear previous password

    updateUi();
}

void DB_ConnectionDialog::slotDbTypeChanged(bool) {
    mDataIsChanged = true;
}

void DB_ConnectionDialog::slotDbDataChanged(QString) {
    mDataIsChanged = true;
}

void DB_ConnectionDialog::slotDbEditFinished() {
    mDataIsChanged = true;
}

void DB_ConnectionDialog::slotLocalDbSelected(bool) {
    leDbName->clear();
    leDbUser->clear();
    leDbPassword->clear();
    leDbHost->clear();
    sbDbPort->clear();
    leTestConnection->clear();
    // leUserName->clear();
    leUserPassword->clear();
}

void DB_ConnectionDialog::slotDeletePreviousConnection() {
    QModelIndex idx = lvPrevious->currentIndex();
    if (!idx.isValid()) {
        DB_DIALOGFACTORY->commandMessage(tr("No connection selected"));
        return;
    }

    lvPrevious->model()->removeRow(idx.row(), QModelIndex());
    mConnectionList.removeAt(idx.row());
    if (lvPrevious->model()->rowCount() > 0) {
        idx = lvPrevious->model()->index(0, 0);
    } else {
        idx = QModelIndex();
    }
    slotSetDbConnectionWidgets(idx, QModelIndex());
    mDataIsChanged = true;
}

void DB_ConnectionDialog::createDatabase(QSqlDatabase& db) {
    if (rbServerDatabase->isChecked()) {
        db.setDatabaseName(leDbName->text());
        db.setUserName(leDbUser->text());
        db.setPassword(leDbPassword->text());
        db.setHostName(leDbHost->text());
        db.setPort(sbDbPort->value());

        // Reconnect not yet required, only of database timed out
        // does not work for server time out
//        if (this->cbDbDriver->currentText() == "QMYSQL") {
//            db.setConnectOptions("MYSQL_OPT_RECONNECT=1");
//        }
    } else if (rbLocalAccessDb->isChecked()) {
        // check if we can write to that directory:
#ifndef Q_OS_WIN
        RB_String path = QFileInfo(leLocalDbName->text()).absolutePath();
        if (QFileInfo(path).isWritable() == false) {
            RB_DEBUG->print("DB_ConnectionDialog::on_pbTestConnection_clicked() "
                            "Can't write file: no permission");
            leTestConnection->setText(tr("No write permission ERROR"));
            return;
        }
#endif
        /*
        From http://connectionstrings.com :

        Microsoft Access accdb ODBC Driver
        Type:    ODBC Driver
        Usage:  Driver={Microsoft Access Driver (*.mdb, *.accdb)}

        Standard Security
        Driver={Microsoft Access Driver (*.mdb, *.accdb};Dbq=C:\mydatabase.accdb;Uid=Admin;Pwd=;

        Workgroup
        Driver={Microsoft Access Driver (*.mdb, *.accdb};Dbq=C:\mydatabase.accdb;SystemDB=C:\mydatabase.mdw;

        Exclusive
        Driver={Microsoft Access Driver (*.mdb, *.accdb};Dbq=C:\mydatabase.accdb;Exclusive=1;Uid=admin;Pwd=;

        Enable admin statements
        To enable certain programatically admin functions such as CREATE USER, CREATE GROUP, ADD USER,
        GRANT, REVOKE and DEFAULTS (when making CREATE TABLE statements) use this connection string.
        Driver={Microsoft Access Driver (*.mdb, *.accdb};Dbq=C:\mydatabase.accdb;Uid=Admin;Pwd=;ExtendedAnsiSQL=1;

        Specifying locale identifier
        Use this one to specify the locale identifier which can help with non-US formated dates.
        Driver={Microsoft Access Driver (*.mdb, *.accdb};Dbq=C:\mydatabase.accdb;Locale Identifier=2057;Uid=Admin;Pwd=;

        From Microsoft:
        Driver={Microsoft Excel Driver (*.xls, *.xlsx, *.xlsm, *.xlsb)};DBQ=path to xls/xlsx/xlsm/xlsb file

        From Qt tutorial:
        db.setHostName("LOCALHOST\\SQLEXPRESS"); // = servername for QODBC SQL Express

        From stackoverflow.com:
        db = QSqlDatabase.addDatabase('QODBC')
        #TODO: To add support for trusted connections
        #("Driver={SQLServer};Server=Your_Server_Name;Database=Your_Database_Name;Trusted_Connection=yes;")
        db.setDatabaseName('DRIVER={SQL Server};SERVER=%s;DATABASE=%s;UID=%s;PWD=%s;'
                                % (hostname,
                                   databasename,
                                   username,
                                   password))
        db.open()
        */


        RB_String dbFn = "";
        if (leLocalDbName->text().endsWith(".mdb", Qt::CaseInsensitive)) {
            dbFn = "DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ=";
        } else if (leLocalDbName->text().endsWith(".accdb", Qt::CaseInsensitive)) {
            dbFn = "DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};FIL={MS Access};DBQ=";
        } else if (leLocalDbName->text().endsWith(".xls", Qt::CaseInsensitive)) {
            dbFn = "DRIVER={Microsoft Excel Driver (*.xls)};FIL={MS Excel};DBQ=";
        }

        dbFn += leLocalDbName->text() + ";Uid="; // is absolute (local) file path
        dbFn += leUserName->text() + ";Pwd=";
        dbFn += leUserPassword->text() + ";";
        db.setDatabaseName(dbFn);
    } else if (rbLocalSqliteDb->isChecked()) {
        // check if we can write to that directory:
#ifndef Q_OS_WIN
        RB_String path = QFileInfo(leLocalDbName->text()).absolutePath();
        if (QFileInfo(path).isWritable() == false) {
            RB_DEBUG->print("DB_ConnectionDialog::on_pbTestConnection_clicked() "
                            "Can't write file: no permission");
            leTestConnection->setText(tr("No write permission ERROR"));
            return;
        }
#endif
        db.setDatabaseName(leLocalDbName->text()); // local database name
        db.setUserName(leUserName->text());
        db.setPassword(leUserPassword->text());

    }
}

/**
 * Check whether computer is connected to the database host computer
 * \todo: is not implemented and does not work
 * @param hostStr host string, can be a host name but also an IP address
 * @return true on success
 */
bool DB_ConnectionDialog::isValidNetworkConnection(const RB_String& /*hostStr*/) {

    // not working example, check in QtNetwork
//    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();

//    for(int i = 0; i < interfaces.count();i++) {
//        QNetworkInterface interface = interfaces.at(i);

//        if(interface.IsUp && !interface.IsLoopBack) {
//            ui->lstLog->addItem(interface.name()+QDateTime::currentDateTime().toString("h:m:s ap"));
//            ui->chkConStatus->setChecked(true);
//        } else{
//            ui->chkConStatus->setChecked(false);
//        }
//    }

//    return ui->chkConStatus->checkState();
    return false;
}
