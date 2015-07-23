/*****************************************************************
 * $Id: db_seltreesiblingdialog.cpp 1965 2013-08-08 16:33:15Z rutger $
 * Created: Jan 30, 2011 8:34:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_seltreesiblingdialog.h"

#include <QtWidgets>
#include "db_dialogfactory.h"
#include "rb_mainwindow.h"
#include "rb_mmproxy.h"


/**
 * Constructor
 * @param parent parent widget such as a mainwindow
 */
DB_SelTreeSiblingDialog::DB_SelTreeSiblingDialog(QWidget *parent)
        : RB_Dialog(parent) {

    setupUi(this);

    mSiblingList = NULL;
    mModel = NULL;
    mCurrentId = "";
    mHideId = "";
}

/**
 * Destructor
 */
DB_SelTreeSiblingDialog::~DB_SelTreeSiblingDialog() {
    // nothing
    mModel->setRoot(NULL);
    delete mModel;
    RB_DEBUG->print("DB_SelTreeSiblingDialog::~DB_SelTreeSiblingDialog() OK");
}

void DB_SelTreeSiblingDialog::init() {
    if (!mSiblingList || mHideId.isEmpty()) return;

    // Create custom (in-memory) model, partly from RB_ModelFactory::getTableModel()
    mModel = new RB_MmProxy(NULL);
    // Create source model
    RB_MmSource* sModel = new RB_MmSource(NULL);

    mModel->setSourceModel(sModel);
    mModel->setObjectFactory(mSiblingList->getFactory());
    mModel->setTableModel(mSiblingList->getName(), NULL);
    mModel->setRoot(mSiblingList->getParent());
    // mModel->select(); only for database models, instead:
    mModel->setInMemoryModel(true);
    tableView->setWindowTitle(tr("Demote"));

    // Part of the tree, first set tableView model than itemSelectionModel
    tableView->setModel(mModel);
    QItemSelectionModel* selModel = new QItemSelectionModel(mModel);
    tableView->setSelectionModel(selModel);
    mModel->setSelectionModel(selModel);

    // Set row of selected item to be hidden, cannot select itself
    RB_ObjectBase* obj = mSiblingList->getObject(mHideId);
    int row = mSiblingList->getRow(obj);
    tableView->setRowHidden(row, true);
    tableView->verticalHeader()->setDefaultSectionSize(20);

    int colCount = mModel->columnCount();

    for (int i = 0; i < colCount; ++i) {
        if (i != RB_MmSource::HiddenColumns) {
            tableView->hideColumn(i);
        }
    }

    readSettings();
}

/**
 * Set sibling list to select from before init()
 * @param sList list container with siblings
 */
void DB_SelTreeSiblingDialog::setSiblingList(RB_ObjectContainer* sList) {
    mSiblingList = sList;
}

/**
 * Set row hided corresponding to ID before init(). This is the original
 * object to be demoted in tree and cannot be selected.
 * @param id ID
 */
void DB_SelTreeSiblingDialog::setHideId(const RB_String& id) {
    mHideId = id;
}

/**
 * Get current selected model ID.
 * @return selected model ID
 */
RB_String DB_SelTreeSiblingDialog::getCurrentId() const {
    return mCurrentId;
}

void DB_SelTreeSiblingDialog::on_pbHelp_clicked() {
    RB_MainWindow* mw = DB_DIALOGFACTORY->getMainWindow();
    mw->slotHelpSubject("DB_SelTreeSiblingDialog");
}

void DB_SelTreeSiblingDialog::on_pbOk_clicked() {
    if (!mModel || !tableView->currentIndex().isValid()) {
        reject();
        return;
    }

    mCurrentId = mModel->getCurrentId();
    QDialog::accept();
}

void DB_SelTreeSiblingDialog::on_pbCancel_clicked() {
    reject();
}

/**
 * Capture the close action by user at the right top corner. In a mainwindow
 * the closeEvent(event) is used.
 */
void DB_SelTreeSiblingDialog::reject() {
    QDialog::reject();
}

/**
 * Event handler to handle state changes, such as language change
 * @param e event
 */
void DB_SelTreeSiblingDialog::changeEvent(QEvent* e) {
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
