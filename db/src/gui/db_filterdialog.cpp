/*****************************************************************
 * $Id: rb_dialog.h 1234 2010-07-05 21:02:33Z rutger $
 * Created: Aug 4, 2010 9:21:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_filterdialog.h"


DB_FilterDialog::DB_FilterDialog(QWidget *parent) : QDialog(parent) {

    setupUi(this);

}

DB_FilterDialog::~DB_FilterDialog() {
    // nothing
}

void DB_FilterDialog::addFieldName(const QString& fieldName) {
    cbField->addItem(fieldName);
}

void DB_FilterDialog::setFieldName(const QString& fieldName) {
    if (fieldName == "") {
        int count = cbField->count();
        cbField->setCurrentIndex(count - 1);
    } else {
        cbField->setCurrentIndex(cbField->findText(fieldName));
    }
}

void DB_FilterDialog::clearFieldNames() {
    cbField->clear();
}

QString DB_FilterDialog::getFieldName() {
    return cbField->currentText();
}

void DB_FilterDialog::setFilterText(const QString& text) {
    leFilterFor->setText(text);
}

QString DB_FilterDialog::getFilterText() {
    return leFilterFor->text();
}

void DB_FilterDialog::setMatchCase(bool matchCase) {
    chbMatchCase->setChecked(matchCase);
}

bool DB_FilterDialog::getMatchCase() {
    return chbMatchCase->isChecked();
}

/**
 * Close the dialog and set the return value to QDialog::Accepted
 */
void DB_FilterDialog::on_pbOk_clicked() {
    accept();
}

/**
 * Close the dialog and set the return value to QDialog::Rejected
 */
void DB_FilterDialog::on_pbCancel_clicked() {
    reject();
}

void DB_FilterDialog::changeEvent(QEvent* e) {
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
