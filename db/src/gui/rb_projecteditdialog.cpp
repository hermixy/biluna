/*****************************************************************
 * $Id: rb_projecteditdialog.cpp 1965 2013-08-08 16:33:15Z rutger $
 * Created: Dec 14, 2012 8:34:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_projecteditdialog.h"

#include <QtWidgets>
#include "db_dialogfactory.h"
#include "db_filterdialog.h"
#include "rb_mmproxy.h"

/**
 * Constructor
 * @param parent parent widget such as a mainwindow
 */
RB_ProjectEditDialog::RB_ProjectEditDialog(QWidget *parent)
        : RB_Dialog(parent) {

    setupUi(this);

    mModel = NULL;
    readSettings();
}

/**
 * Destructor
 */
RB_ProjectEditDialog::~RB_ProjectEditDialog() {
    RB_DEBUG->print("RB_ProjectEditDialog::~RB_ProjectEditDialog() OK");
}

/**
 * File Save, save model changes
 */
bool RB_ProjectEditDialog::fileSave(bool withSelect) {
    beforeFileSave();
    if (withSelect && mModel) {
        if (mModel->submitAllAndSelect()) {
            
            setWindowModified(false);
            return true;
        }
    } else if (mModel) {
        if (mModel->submitAll()) {
            setWindowModified(false);
            return true;
        }
    }
    RB_DEBUG->print("RB_ProjectEditDialog::fileSave() ERROR");
    return false;
}

/**
 * File Revert, reverse modelchanges
 */
void RB_ProjectEditDialog::fileRevert() {
    if (mModel) {
        mModel->revertAll();
        setWindowModified(false);
    }
}

/**
 * Set help subject for 'Help' button. For a dialog this is usually
 * the dialog class name
 * @param subject help subject
 */
void RB_ProjectEditDialog::setHelpSubject(const RB_String& subject) {
    mHelpSubject = subject;
}

void RB_ProjectEditDialog::on_pbDelete_clicked() {
    if (!mModel) return;

    int res = DB_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) return;

    QModelIndex index = tableView->currentIndex();

    if (index.isValid()) {
        int row = index.row();
        mModel->removeRows(index.row(), 1, QModelIndex());
        mModel->submitAllAndSelect();
        setWindowModified(false);

        if (row > 0) {
            tableView->setCurrentIndex(mModel->index(row-1, RB2::HIDDENCOLUMNS, QModelIndex()));
            tableView->scrollTo(mModel->index(row-1, RB2::HIDDENCOLUMNS, QModelIndex()));
        }
    }
}

/**
 * Button filter clicked
 */
void RB_ProjectEditDialog::on_pbFilter_clicked() {
    filterTable(tableView);
}

/**
 * Button clear filter clicked
 */
void RB_ProjectEditDialog::on_pbFilterClear_clicked() {
    mModel->removeUserFilter();
}

/**
 * Button help clicked
 */
void RB_ProjectEditDialog::on_pbHelp_clicked() {
    RB_MainWindow* mw = DB_DIALOGFACTORY->getMainWindow();
    mw->slotHelpSubject(mHelpSubject);
}

/**
 * Button OK clicked
 */
void RB_ProjectEditDialog::on_pbOk_clicked() {
    if (!mModel) return;

    fileSave(false);
    RB_Dialog::accept();
}

/**
 * Button cancel clicked
 */
void RB_ProjectEditDialog::on_pbCancel_clicked() {
    if (maybeSave()) {
        RB_Dialog::accept();
        return;
    }

    RB_Dialog::reject();
}

/**
 * Event handler to handle state changes, such as language change
 * @param e event
 */
void RB_ProjectEditDialog::changeEvent(QEvent* e) {
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
