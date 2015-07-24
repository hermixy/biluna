/*****************************************************************
 * $Id: acc_selectyeardialog.cpp 1515 2012-01-12 17:46:38Z rutger $
 * Created: Jan 09, 2012 4:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_selectyeardialog.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"

/**
 * Constructor
 */
ACC_SelectYearDialog::ACC_SelectYearDialog(QWidget *parent)
        : RB_Dialog(parent) {

    setupUi(this);

}

/**
 * Destructor
 */
ACC_SelectYearDialog::~ACC_SelectYearDialog() {
    RB_DEBUG->print("ACC_SelectYearDialog::~ACC_SelectYearDialog() OK");
}

/**
 * Initialize dialog and models
 */
void ACC_SelectYearDialog::init() {

    setWindowTitle(tr("Select Year")); // not editable

    int year = QDateTime::currentDateTime().date().year();
    sbYear->setValue(year);

    readSettings();
}

/**
 * @returns year
 */
int ACC_SelectYearDialog::getYear() const {
    return sbYear->value();
}

/**
 * Button Help clicked
 */
void ACC_SelectYearDialog::on_pbHelp_clicked() {
    RB_MainWindow* mw = ACC_DIALOGFACTORY->getMainWindow();
    mw->slotHelpSubject(objectName());
}

/**
 * Button Select item clicked
 */
void ACC_SelectYearDialog::on_pbSelect_clicked() {
    QDialog::accept();
}

/**
 * Button Cancel clicked
 */
void ACC_SelectYearDialog::on_pbCancel_clicked() {
    reject();
}

/**
 * Change event, for example translation
 */
void ACC_SelectYearDialog::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
