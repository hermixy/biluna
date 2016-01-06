/*****************************************************************
 * $Id: db_modelobjectdialog.cpp 2210 2015-12-28 14:59:25Z rutger $
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_modelobjectdialog.h"

#include "db_createmodelobject.h"
#include "db_dialogfactory.h"
#include "rb_cmbdelegate.h"
#include "rb_settings.h"
#include "ui_db_modelobjectdialog.h"

DB_ModelObjectDialog::DB_ModelObjectDialog(QWidget *parent) :
            RB_Dialog(parent), ui(new Ui::DB_ModelObjectDialog) {
    ui->setupUi(this);
}

DB_ModelObjectDialog::~DB_ModelObjectDialog() {
    delete ui;
}

void DB_ModelObjectDialog::init() {
    QStringList items;
    items << "RB_ObjectAtomic" << "RB_ObjectContainer";
    ui->cbBaseObject->addItems(items);
    ui->tbbMemberList->initEdit(false, false, false, false);

    ui->twMemberList->setColumnCount(4);
    ui->twMemberList->setRowCount(2);
    items.clear();
    items << "name" << "unit" << "default" << "type";
    ui->twMemberList->setHorizontalHeaderLabels(items);
    ui->twMemberList->setItemDelegateForColumn(3, getNewMemberTypeDelegate());

    QRegExp rxPersp("^[A-z]*$");
    ui->lePerspective->setValidator(new QRegExpValidator(rxPersp, this));
    QRegExp rxObjName("^[A-z0-9]*$");
    ui->leObjectName->setValidator(new QRegExpValidator(rxObjName, this));

    connect(ui->tbbMemberList, SIGNAL(addClicked()),
            this, SLOT(addClicked()));
    connect(ui->tbbMemberList, SIGNAL(deleteClicked()),
            this, SLOT(deleteClicked()));

    readSettings();
}

void DB_ModelObjectDialog::addClicked() {
    int row = ui->twMemberList->currentRow() + 1;

    if (row < 0) {
        row = ui->twMemberList->rowCount();
    }

    ui->twMemberList->insertRow(row);

    QTableWidgetItem* item = new QTableWidgetItem("<NEW>");
    ui->twMemberList->setItem(row, 0, item);
    ui->twMemberList->setFocus();
    QModelIndex idx = ui->twMemberList->model()->index(row, 0);
    ui->twMemberList->selectionModel()
            ->setCurrentIndex(idx, QItemSelectionModel::ClearAndSelect);
}

void DB_ModelObjectDialog::deleteClicked() {
    int row = ui->twMemberList->currentRow();

    if (row < 0 || row > ui->twMemberList->rowCount() - 1) {
        DB_DIALOGFACTORY->requestInformationDialog(tr("No item selected"));
        return;
    }

    int res = DB_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) {
        return;
    }

    ui->twMemberList->removeRow(row);
}

void DB_ModelObjectDialog::on_pbHelp_clicked() {
    RB_MainWindow* mw = DB_DIALOGFACTORY->getMainWindow();
    mw->slotHelpSubject("");
}

void DB_ModelObjectDialog::on_pbSave_clicked() {
    RB_SETTINGS->beginGroup("paths");
    QString savePath = RB_SETTINGS->value("savePath", "").toString();
    RB_SETTINGS->endGroup();

    QString filter = "All files (*.*);;Text files (*.txt);;";
    QString fn = QFileDialog::getSaveFileName(
                DB_DIALOGFACTORY->getMainWindow(),
                tr("Save file"),
                savePath + "/" + ui->lePerspective->text().toLower() + "_"
                + ui->leObjectName->text().toLower() + ".h "
                + ui->lePerspective->text().toLower() + "_"
                + ui->leObjectName->text().toLower() + ".cpp",
                filter);

    if (fn.isEmpty()) {
        DB_DIALOGFACTORY->requestInformationDialog(tr("Files not saved"));
        return;
    }

    DB_CreateModelObject oper;
    oper.setBaseObject(ui->cbBaseObject->currentText());
    oper.setPerspective(ui->lePerspective->text());
    oper.setObjectName(ui->leObjectName->text());
    oper.setDescription(ui->leDescription->text());
    oper.setFilePath(QFileInfo(fn).path());

    RB_ObjectContainer* memberList = new RB_ObjectContainer();
    createMemberList(memberList);
    oper.execute(memberList);
    delete memberList;

    savePath = QFileInfo(fn).absolutePath();
    RB_SETTINGS->beginGroup("paths");
    RB_SETTINGS->setValue("savePath", savePath);
    RB_SETTINGS->endGroup();

    writeSettings();
    accept();
}

void DB_ModelObjectDialog::on_pbCancel_clicked() {
    writeSettings();
    reject();
}

void DB_ModelObjectDialog::createMemberList(RB_ObjectContainer* memberList) {
    int memberCol = 0;
    int unitCol = 1;
    int defaultCol = 2;
    int typeCol = 3;
    RB_ObjectAtomic* memberObj = nullptr;
    bool woMembers = true;

    int rowCount = ui->twMemberList->rowCount();

    for (int row = 0; row < rowCount; ++row) {
        memberObj = new RB_ObjectAtomic("", memberList, "", nullptr, woMembers);
        memberObj->addMember("member", "-",
                             ui->twMemberList->item(row, memberCol)->text());
        memberObj->addMember("unit", "-",
                             ui->twMemberList->item(row, unitCol)->text());
        memberObj->addMember("type", "-",
                             ui->twMemberList->item(row, typeCol)->text());
        memberObj->addMember("default", "-",
                             ui->twMemberList->item(row, defaultCol)->text());
        memberList->addObject(memberObj);
    }
}

RB_CmbDelegate* DB_ModelObjectDialog::getNewMemberTypeDelegate() {
    QStringList items;
    items << ""
          << "MemberByteArray" << "MemberChar10" << "MemberChar20"
          << "MemberChar40" << "MemberChar125" << "MemberChar165"
          << "MemberChar255" << "MemberChar2500" << "MemberDateTime"
          << "MemberDouble" << "MemberInteger" << "MemberString"
          << "MemberUInteger";
    bool useText = true;
    return new RB_CmbDelegate(ui->twMemberList, items, nullptr, useText);
}
