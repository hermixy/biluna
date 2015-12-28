/*****************************************************************
 * $Id: db_modelobjectdialog.cpp 2210 2015-12-28 14:59:25Z rutger $
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_modelobjectdialog.h"

#include "db_dialogfactory.h"
#include "rb_cmbdelegate.h"
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

    readSettings();
}

void DB_ModelObjectDialog::addClicked() {
    int row = ui->twMemberList->currentRow();

    if (row < 0 || row > ui->twMemberList->rowCount() - 1) {
        row = ui->twMemberList->rowCount() - 1;
    }

    ui->twMemberList->insertRow(row + 1);
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
    DB_DIALOGFACTORY->requestInformationDialog("test");
    writeSettings();
    accept();
}

void DB_ModelObjectDialog::on_pbCancel_clicked() {
    writeSettings();
    reject();
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
