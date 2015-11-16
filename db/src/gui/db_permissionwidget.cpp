/*****************************************************************
 * $Id: db_permissionwidget.cpp 2251 2015-06-22 11:56:22Z rutger $
 * Created: Dec 2, 2013 8:27 PM - rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_permissionwidget.h"
#include "ui_db_permissionwidget.h"

#include "db_dialogfactory.h"
#include "db_modelfactory.h"
#include "db_utilityfactory.h"



DB_PermissionWidget::DB_PermissionWidget(QWidget* parent)
                : RB_Widget(parent), ui(new Ui::DB_PermissionWidget) {
    ui->setupUi(this);

    mPermissionProjectModel = NULL;
    mPermissionProjectMapper = NULL;
    mPermissionGroupModel = NULL;
    mPermissionGroupMapper = NULL;
    mSystemUserModel = NULL;
    mSystemUserMapper = NULL;
    mSystemUserGroupModel = NULL;
    mSystemUserGroupMapper = NULL;
}

DB_PermissionWidget::~DB_PermissionWidget() {
    delete mPermissionProjectModel;
    delete mPermissionGroupModel;
    delete mSystemUserModel;
    delete mSystemUserGroupModel;
    delete ui;
}

void DB_PermissionWidget::init() {
    RB_String hostName = DB_MODELFACTORY->getDatabase().hostName();
    RB_String dbName = DB_MODELFACTORY->getDatabase().databaseName();
    ui->leHostName->setText(hostName);
    ui->leDatabaseName->setText(dbName);
    ui->lePassword->setEchoMode(QLineEdit::Password);

    //
    // 0. Set button toolbar
    //
    ui->tbbProject->initEdit(false, true, true);
    ui->tvProject->setToolButtonBar(ui->tbbProject);

    ui->tbbGroup->initEdit(false, false, true);
    ui->tvGroup->setToolButtonBar(ui->tbbGroup);

    ui->tbbUser->initEdit(false, true, true);
    ui->tvUser->setToolButtonBar(ui->tbbUser);

    ui->tbbUserGroup->initEdit(false, false, true);
    ui->tvUserGroup->setToolButtonBar(ui->tbbUserGroup);

    //
    // 1. Set model with ID
    //
    mPermissionProjectModel = DB_MODELFACTORY->getModel(DB_ModelFactory::ModelPermissionProject);
    mPermissionProjectModel->setRoot(DB_MODELFACTORY->getRootId());
    mPermissionGroupModel = DB_MODELFACTORY->getModel(DB_ModelFactory::ModelPermissionGroup);
    mSystemUserModel = DB_MODELFACTORY->getModel(DB_ModelFactory::ModelSystemUser);
    mSystemUserModel->setRoot(DB_MODELFACTORY->getRootId());
    mSystemUserGroupModel = DB_MODELFACTORY->getModel(DB_ModelFactory::ModelSystemUserGroup);


    //
    // 2. Set relations and mapper for line edits etc.
    //
    mPermissionProjectMapper = mPermissionProjectModel->getMapper();
    mPermissionProjectMapper->addMapping(ui->leProjectNumber, mPermissionProjectModel->fieldIndex("number"));
    mPermissionProjectMapper->addMapping(ui->leDescription, mPermissionProjectModel->fieldIndex("description"));
    mPermissionProjectMapper->addMapping(ui->leCustomer, mPermissionProjectModel->fieldIndex("customer"));
    mPermissionProjectMapper->addMapping(ui->ilePerspectiveProject,
                        mPermissionProjectModel->fieldIndex("persproject_idx"));
    ui->ilePerspectiveProject->setDefaultDialog(DB_DIALOGFACTORY,
                                                DB_DialogFactory::DialogPermissionPerspectiveProject,
                                                "persproject_idx", "description");
    RB_StringList items;
    items << tr("Live") << tr("Locked") << tr("Hidden") << tr("Test");
    ui->cbStatus->setModel(new QStringListModel(items, this));
    mPermissionProjectModel->setTextList(mPermissionProjectModel->fieldIndex("mstatus_id"), items);
    mPermissionProjectMapper->addMapping(ui->cbStatus, mPermissionProjectModel->fieldIndex("mstatus_id"),
                                         "currentIndex");
    mPermissionProjectMapper->addMapping(ui->deStartProject, mPermissionProjectModel->fieldIndex("start"));
    mPermissionProjectMapper->addMapping(ui->deEndProject, mPermissionProjectModel->fieldIndex("end"));

    mPermissionGroupMapper = mPermissionGroupModel->getMapper();
    mPermissionGroupMapper->addMapping(ui->ileProjectSystemGroup,
                                       mPermissionGroupModel->fieldIndex("group_idx"));
    ui->ileProjectSystemGroup->setDefaultDialog(DB_DIALOGFACTORY,
                                                DB_DialogFactory::DialogSystemGroup,
                                                "group_idx", "code");

    mSystemUserMapper = mSystemUserModel->getMapper();
    mSystemUserMapper->addMapping(ui->leFirstName, mSystemUserModel->fieldIndex("firstname"));
    mSystemUserMapper->addMapping(ui->leLastName, mSystemUserModel->fieldIndex("lastname"));
    mSystemUserMapper->addMapping(ui->leTelephone, mSystemUserModel->fieldIndex("telephone"));
    mSystemUserMapper->addMapping(ui->leEmail, mSystemUserModel->fieldIndex("email"));
    mSystemUserMapper->addMapping(ui->leUserName, mSystemUserModel->fieldIndex("username"));
//    mSystemUserMapper->addMapping(ui->lePassword, mSystemUserModel->fieldIndex("password"));
    mSystemUserMapper->addMapping(ui->deStartUser, mSystemUserModel->fieldIndex("start"));
    mSystemUserMapper->addMapping(ui->deEndUser, mSystemUserModel->fieldIndex("end"));

    mSystemUserGroupMapper = mSystemUserGroupModel->getMapper();
    mSystemUserGroupMapper->addMapping(ui->ileUserSystemGroup, mSystemUserGroupModel->fieldIndex("group_idx"));
    ui->ileUserSystemGroup->setDefaultDialog(DB_DIALOGFACTORY,
                                                  DB_DialogFactory::DialogSystemGroup,
                                                  "group_idx", "code");

    //
    // 3. Select after relations have been set, only for database models
    //
    mPermissionProjectModel->select();
    mSystemUserModel->select();
    // mPermissionProjectModel->setHeaderData(RB2::HIDDENCOLUMNS, Qt::Horizontal, tr("Rutger"));

    //
    // 4. Connect model to view
    //
    formatTableView(ui->tvProject, mPermissionProjectModel);
    formatTableView(ui->tvGroup, mPermissionGroupModel);
    formatTableView(ui->tvUser, mSystemUserModel);
    formatTableView(ui->tvUserGroup, mSystemUserGroupModel);
    // tvCompany->setItemDelegate(new DB_TeBlockDelegate(this));

    int colCount = mPermissionProjectModel->columnCount();
    setTableColumnHide(ui->tvProject, colCount);
    colCount = mPermissionGroupModel->columnCount();
    setTableColumnHide(ui->tvGroup, colCount);
    colCount = mSystemUserModel->columnCount();
    setTableColumnHide(ui->tvUser, colCount);
    colCount = mSystemUserGroupModel->columnCount();
    setTableColumnHide(ui->tvUserGroup, colCount);

    readSettings();

    //
    // 5. Connections for setting additional data or state
    //
    connect(ui->tbbProject, SIGNAL(addClicked()),
            this, SLOT(slotProjectAdded()));
    connect(ui->tbbGroup, SIGNAL(addClicked()),
            this, SLOT(slotGroupAdded()));
    connect(ui->tbbUser, SIGNAL(addClicked()),
            this, SLOT(slotUserAdded()));
    connect(ui->tbbUserGroup, SIGNAL(addClicked()),
            this, SLOT(slotUserGroupAdded()));

    // Prevent duplicates
    connect(ui->ilePerspectiveProject, SIGNAL(clicked()),
            this, SLOT(slotCheckDuplicatePerspectiveProject()));
    connect(ui->ileProjectSystemGroup, SIGNAL(clicked()),
            this, SLOT(slotCheckDuplicateProjectGroup()));
    connect(ui->ileUserSystemGroup, SIGNAL(clicked()),
            this, SLOT(slotCheckDuplicateUserGroup()));

    // Handle encrypted password field
    ui->lePassword->setEchoMode(QLineEdit::Password);
    connect(ui->lePassword, SIGNAL(editingFinished()),
            this, SLOT(slotUserPassWordChanged()));
    connect(mSystemUserModel,
            SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
            this, SLOT(slotUserCurrentRowChanged(QModelIndex,QModelIndex)));
    connect(mSystemUserModel, SIGNAL(rootChanged()),
            this, SLOT(slotUserRootChanged()));
    slotUserRootChanged();
}

bool DB_PermissionWidget::fileSave(bool withSelect) {
    beforeFileSave();

    if (withSelect) {
        mPermissionGroupModel->submitAll();
        mPermissionProjectModel->submitAllAndSelect();
        mSystemUserGroupModel->submitAll();
        mSystemUserModel->submitAllAndSelect();
    } else {
        mPermissionGroupModel->submitAll();
        mPermissionProjectModel->submitAll();
        mSystemUserGroupModel->submitAll();
        mSystemUserModel->submitAll();
    }

    setWindowModified(false);
    return true;
}

void DB_PermissionWidget::fileRevert() {
    mPermissionGroupModel->revertAll();
    mPermissionProjectModel->revertAll();
    mSystemUserGroupModel->revertAll();
    mSystemUserModel->revertAll();

    setWindowModified(false);
}

void DB_PermissionWidget::slotProjectAdded() {
    QModelIndex idx = mPermissionProjectModel->index(mPermissionProjectModel->getCurrentIndex().row(),
                                    mPermissionProjectModel->fieldIndex("start"));
    QDate date = QDateTime::currentDateTime().date();
    mPermissionProjectModel->setData(idx, date.toString(Qt::ISODate));
    date = date.addDays(1);
    idx = mPermissionProjectModel->index(mPermissionProjectModel->getCurrentIndex().row(),
                        mPermissionProjectModel->fieldIndex("end"));
    mPermissionProjectModel->setData(idx, date.toString(Qt::ISODate));

    ui->leProjectNumber->setFocus();
    ui->leProjectNumber->selectAll();
}

void DB_PermissionWidget::slotGroupAdded() {
    ui->ileUserSystemGroup->setFocus();
}

void DB_PermissionWidget::slotUserAdded() {
    QModelIndex idx = mSystemUserModel->index(mSystemUserModel->getCurrentIndex().row(),
                                    mSystemUserModel->fieldIndex("start"));
    QDate date = QDateTime::currentDateTime().date();
    mSystemUserModel->setData(idx, date.toString(Qt::ISODate));
    date = date.addMonths(1);
    idx = mSystemUserModel->index(mSystemUserModel->getCurrentIndex().row(),
                        mSystemUserModel->fieldIndex("end"));
    mSystemUserModel->setData(idx, date.toString(Qt::ISODate));

    ui->leFirstName->setFocus();
    ui->leFirstName->selectAll();
}

void DB_PermissionWidget::slotUserGroupAdded() {
    ui->ileProjectSystemGroup->setFocus();
}

void DB_PermissionWidget::slotUserPassWordChanged() {
    QModelIndex currentIndex = ui->tvUser->currentIndex();

    if (!currentIndex.isValid()) {
        return;
    }

    int row = currentIndex.row();
    QModelIndex idx = mSystemUserModel->index(
                row, mSystemUserModel->fieldIndex("password"));
    mSystemUserModel->setData(
                idx, DB_UTILITYFACTORY->encrypt(ui->lePassword->text()));
}

void DB_PermissionWidget::slotUserCurrentRowChanged(QModelIndex current,
                                                    QModelIndex /*previous*/) {
    if (!current.isValid() || !ui->tvUser->currentIndex().isValid()) {
        slotUserRootChanged();
        return;
    }

    ui->lePassword->setEnabled(true);
    int row = current.row();
    QModelIndex idx = mSystemUserModel->index(
                row, mSystemUserModel->fieldIndex("password"));
    ui->lePassword->setText(DB_UTILITYFACTORY->decrypt(idx.data()));
}

void DB_PermissionWidget::slotUserRootChanged() {
    ui->lePassword->clear();
    ui->lePassword->setEnabled(false);
}

void DB_PermissionWidget::slotCheckDuplicatePerspectiveProject() {
    QModelIndex idx = ui->tvProject->currentIndex();
    if (!idx.isValid()) {
        return;
    }

    RB_String persProjId =
            mPermissionProjectModel->getCurrentValue(
                "persproject_idx").toString();
    persProjId = persProjId.remove(38, persProjId.length());

    if (isDuplicateIdxFound(mPermissionProjectModel, "persproject_idx",
                            persProjId, idx.row())) {
        mPermissionProjectModel->setCurrentValue(
            "persproject_idx", "0", Qt::EditRole);
        DB_DIALOGFACTORY->requestWarningDialog(tr("Item already selected,\n"
                                                  "duplicate entry not allowed."));
    }
}

void DB_PermissionWidget::slotCheckDuplicateProjectGroup() {
    QModelIndex idx = ui->tvGroup->currentIndex();
    if (!idx.isValid()) {
        return;
    }

    RB_String persGroupId =
            mPermissionGroupModel->getCurrentValue(
                "group_idx").toString();
    persGroupId = persGroupId.remove(38, persGroupId.length());

    if (isDuplicateIdxFound(mPermissionGroupModel, "group_idx",
                            persGroupId, idx.row())) {
        mPermissionGroupModel->setCurrentValue("group_idx", "0", Qt::EditRole);
        DB_DIALOGFACTORY->requestWarningDialog(tr("Item already selected,\n"
                                                  "duplicate entry not allowed."));
    }
}

void DB_PermissionWidget::slotCheckDuplicateUserGroup() {
    QModelIndex idx = ui->tvUserGroup->currentIndex();
    if (!idx.isValid()) {
        return;
    }

    RB_String userGroupId =
            mSystemUserGroupModel->getCurrentValue(
                "group_idx").toString();
    userGroupId = userGroupId.remove(38, userGroupId.length());

    if (isDuplicateIdxFound(mSystemUserGroupModel, "group_idx",
                            userGroupId, idx.row())) {
        mSystemUserGroupModel->setCurrentValue("group_idx", "0", Qt::EditRole);
        DB_DIALOGFACTORY->requestWarningDialog(tr("Item already selected,\n"
                                                  "duplicate entry not allowed."));
    }
}

void DB_PermissionWidget::setTableColumnHide(RB_TableView* tv,
                                             int columnCount) {
    if (!tv) {
        RB_DEBUG->error("DB_PermissionWidget::setTableColumnHide() ERROR");
        return;
    }

    for (int i = 0; i < columnCount; ++i) {
        if (i < RB2::HIDDENCOLUMNS) {
            tv->hideColumn(i);
        } else {
            tv->showColumn(i);

            if (tv->columnWidth(i) < 5) {
                tv->setColumnWidth(i, 100);
            }
        }
    }
}

void DB_PermissionWidget::changeEvent(QEvent* e) {
    RB_Widget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

