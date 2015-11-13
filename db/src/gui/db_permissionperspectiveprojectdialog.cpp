/*****************************************************************
 * $Id: db_permissionperspectiveprojectdialog.cpp 2201 2015-01-08 14:30:23Z rutger $
 * Created: Jan 2, 2015 17:15:42 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_permissionperspectiveprojectdialog.h"
#include "ui_db_permissionperspectiveprojectdialog.h"

#include "db_dialogfactory.h"
#include "db_modelfactory.h"

DB_PermissionPerspectiveProjectDialog
        ::DB_PermissionPerspectiveProjectDialog(QWidget *parent)
        : RB_Dialog(parent), ui(new Ui::DB_PermissionPerspectiveProjectDialog) {

    ui->setupUi(this);
    mModel = NULL;
}

DB_PermissionPerspectiveProjectDialog::~DB_PermissionPerspectiveProjectDialog() {
    delete mModel;
    delete ui;
}

void DB_PermissionPerspectiveProjectDialog::init() {
    setWindowTitle(tr("DB Permission Perspective Projects[*]"));

    //
    // 1. Set model with ID
    //
    mModel = DB_MODELFACTORY->getModel(DB_ModelFactory::ModelPermissionPerspectiveProject, false);
    // mModel->setRoot();

    //
    // 2. Set relations and mapper for line edits etc.
    //
    // none

    //
    // 3. Execute SELECT on model
    //
    mModel->select();

    //
    // 4. Connect model to main view
    //
    formatTableView(ui->tvProject, mModel);

    //
    // 5. Set toolbuttonbar
    //
    ui->tbbProject->initSelect(true, false);
    ui->tvProject->setToolButtonBar(ui->tbbProject);

    //
    // 6. Set visible columns in tableView
    //
    int colCount = mModel->columnCount(QModelIndex());

    for (int i = 0; i < colCount; ++i) {
        if (i < RB2::HIDDENCOLUMNS) {
            ui->tvProject->hideColumn(i);
        } else {
            ui->tvProject->showColumn(i);

            if (ui->tvProject->columnWidth(i) < 5) {
                ui->tvProject->setColumnWidth(i, 100);
            }
        }
    }

    readSettings();
}

RB_ObjectBase *DB_PermissionPerspectiveProjectDialog::getCurrentObject() {
    return mCurrentObject;
}

/**
 * Button help clicked
 */
void DB_PermissionPerspectiveProjectDialog::on_pbHelp_clicked() {
    RB_MainWindow* mw = DB_DIALOGFACTORY->getMainWindow();
    mw->slotHelpSubject(this->objectName());
}

/**
 * Button OK clicked
 */
void DB_PermissionPerspectiveProjectDialog::on_pbOk_clicked() {
    if (!mModel) return;
    beforeFileSave();

    if (!ui->tvProject->currentIndex().isValid()) {
        mCurrentObject = NULL;
    } else {
        // for selection windows
        setCurrentObject();
    }

    if (isWindowModified()) {
        mModel->submitAll();
    }

    accept();
}

/**
 * Button cancel clicked
 */
void DB_PermissionPerspectiveProjectDialog::on_pbCancel_clicked() {
    reject();
}

/**
 * Change event, for example translation
 */
void DB_PermissionPerspectiveProjectDialog::changeEvent(QEvent* e) {
    RB_Dialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        // retranslateUi(this);
        break;
    default:
        break;
    }
}

/**
 * Set current object with data from current selected index
 */
void DB_PermissionPerspectiveProjectDialog::setCurrentObject() {
    if (ui->tvProject->currentIndex().isValid()) {
        mCurrentObject = mModel->getCurrentObject();
    } else {
        mCurrentObject = NULL;
    }
}
