/*****************************************************************
 * $Id: db_systemgroupdialog.cpp 2222 2015-02-26 19:38:49Z rutger $
 * Created: Dec 18, 2014 10:24:42 AM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_systemgroupdialog.h"
#include "ui_db_systemgroupdialog.h"

#include "db_dialogfactory.h"
#include "db_modelfactory.h"


DB_SystemGroupDialog::DB_SystemGroupDialog(QWidget *parent)
                : RB_Dialog(parent), ui(new Ui::DB_SystemGroupDialog) {
    ui->setupUi(this);

    mModel = NULL;
    mMapper = NULL;
    mCurrentObject = NULL;
}

DB_SystemGroupDialog::~DB_SystemGroupDialog() {
    delete mModel;
    delete ui;
    RB_DEBUG->print("DB_SystemGroupDialog::~DB_SystemGroupDialog() OK");
}

void DB_SystemGroupDialog::init() {
    setWindowTitle(tr("DB Edit System Groups[*]"));

    //
    // 1. Set model with ID
    //
    mModel = DB_MODELFACTORY->getModel(DB_ModelFactory::ModelSystemGroup, false);
    mModel->setRoot(DB_MODELFACTORY->getRootId());

    //
    // 2. Set relations and mapper for line edits etc.
    //
    mMapper = mModel->getMapper();
    mMapper->addMapping(ui->leSystemGroupCode, mModel->fieldIndex("code"));
    mMapper->addMapping(ui->leDescription, mModel->fieldIndex("description"));
    RB_StringList items;
    items << tr("None") << tr("[R] Read") << tr("[RU] Read Update")
          << tr("[CRUD] Create Read Update Delete")
          << tr("[CRUDX] Create Read Update Delete Execute");
    mModel->setTextList(mModel->fieldIndex("permission_id"), items);
    ui->cbStatus->setModel(new QStringListModel(items, this));
    mMapper->addMapping(ui->cbStatus, mModel->fieldIndex("permission_id"),
                        "currentIndex");

    //
    // 3. Execute SELECT on model
    //
    mModel->select();

    //
    // 4. Connect model to main view
    //
    formatTableView(ui->tvSystemGroup, mModel);

    //
    // 5. Set toolbuttonbar
    //
    ui->tbbSystemGroup->initEdit(false, false, true);
    ui->tvSystemGroup->setToolButtonBar(ui->tbbSystemGroup);
    connect(ui->tbbSystemGroup, SIGNAL(addClicked()),
            this, SLOT(slotPbAdd_clicked()));

    //
    // 6. Set visible columns in tableView
    //
    int colCount = mModel->columnCount(QModelIndex());

    for (int i = 0; i < colCount; ++i) {
        if (i < RB2::HIDDENCOLUMNS) {
            ui->tvSystemGroup->hideColumn(i);
        } else {
            ui->tvSystemGroup->showColumn(i);

            if (ui->tvSystemGroup->columnWidth(i) < 5) {
                ui->tvSystemGroup->setColumnWidth(i, 100);
            }
        }
    }

    readSettings();
}

RB_ObjectBase *DB_SystemGroupDialog::getCurrentObject() {
    return mCurrentObject;
}

void DB_SystemGroupDialog::slotPbAdd_clicked() {
    if(!mModel)  return;

    // insert already done
    // set default columns already done
    // set current index and scroll to already done

    ui->leSystemGroupCode->setFocus();
    ui->leSystemGroupCode->selectAll();
}

/**
 * Button help clicked
 */
void DB_SystemGroupDialog::on_pbHelp_clicked() {
    RB_MainWindow* mw = DB_DIALOGFACTORY->getMainWindow();
    mw->slotHelpSubject(this->objectName());
}

/**
 * Button OK clicked
 */
void DB_SystemGroupDialog::on_pbOk_clicked() {
    if (!mModel) return;
    beforeFileSave();

    if (!ui->tvSystemGroup->currentIndex().isValid()) {
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
void DB_SystemGroupDialog::on_pbCancel_clicked() {
    reject();
}

/**
 * Change event, for example translation
 */
void DB_SystemGroupDialog::changeEvent(QEvent* e) {
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
void DB_SystemGroupDialog::setCurrentObject() {
    if (ui->tvSystemGroup->currentIndex().isValid()) {
        mCurrentObject = mModel->getCurrentObject();
    } else {
        mCurrentObject = NULL;
    }
}

