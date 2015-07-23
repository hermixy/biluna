/*****************************************************************
 * $Id: db_prepostfixtextdialog.cpp 1435 2011-07-14 17:15:58Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna LDT project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_prepostfixtextdialog.h"

#include "db_dialogfactory.h"


DB_PrePostfixTextDialog::DB_PrePostfixTextDialog(QWidget* parent) : RB_Dialog(parent) {

    setupUi(this);

}

DB_PrePostfixTextDialog::~DB_PrePostfixTextDialog() {
    // nothing
}

RB_String DB_PrePostfixTextDialog::getPrefix() {
    return this->lePrefix->text();
}

RB_String DB_PrePostfixTextDialog::getPostfix() {
    return this->lePostfix->text();
}

void DB_PrePostfixTextDialog::on_pbHelp_clicked() {
    RB_MainWindow* mw = DB_DIALOGFACTORY->getMainWindow();
    mw->slotHelpSubject(this->objectName());
}

void DB_PrePostfixTextDialog::on_pbOk_clicked() {
    accept();
}

void DB_PrePostfixTextDialog::on_pbCancel_clicked() {
    reject();
}

