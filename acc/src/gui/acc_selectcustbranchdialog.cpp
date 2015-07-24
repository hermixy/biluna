/*****************************************************************
 * $Id: acc_selectcustbranchdialog.cpp 1845 2012-12-23 14:39:11Z rutger $
 * Created: Jan 25, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_selectcustbranchdialog.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"

/**
 * Constructor
 */
ACC_SelectCustBranchDialog::ACC_SelectCustBranchDialog(QWidget *parent)
        : RB_Dialog(parent) {

    setupUi(this);

    mCustModel = NULL;
//    mBranchModel = NULL;
//    mSizeHint = QSize(500, 550);
//    QList<int> list;
//    list << 320 << 120;
//    splitter->setSizes(list);
}

/**
 * Destructor
 */
ACC_SelectCustBranchDialog::~ACC_SelectCustBranchDialog() {
    // unique models are only deleted by the widgets, in case no other can use
    // the model anymore. Dialogs do not delete unique models only non-unique
    delete mCustModel;
//    delete mBranchModel;
    RB_DEBUG->print("ACC_SelectCustBranchDialog::~ACC_SelectCustBranchDialog() OK");
}

/**
 * Initialize dialog and models
 */
void ACC_SelectCustBranchDialog::init() {

    setWindowTitle(tr("Select Customer Branch")); // not editable

    //
    // Set model and connect to table view
    //
    mCustModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelCustomer, false);
    mCustModel->setRoot(ACC_MODELFACTORY->getRootId());
    mCustModel->select();

    setFormatTableView(tvCustomer, mCustModel);
    readSettings();
    tvCustomer->horizontalHeader()->setSortIndicator(-1, Qt::AscendingOrder);

    // Hidden columns only for tableviews
    int colCount = mCustModel->columnCount();
    for (int i = 0; i < colCount; ++i) {
        if (i != RB2::HIDDENCOLUMNS && i != RB2::HIDDENCOLUMNS + 1) {
            tvCustomer->hideColumn(i);
        } else {
            tvCustomer->showColumn(i);

            if (tvCustomer->columnWidth(i) < 5) {
                tvCustomer->setColumnWidth(i, 100);
            }
        }
    }

}

/**
 * Get current object based on selected model index.
 * @return object
 */
RB_ObjectBase* ACC_SelectCustBranchDialog::getCurrentObject() {
    return mCurrentObject;
}

/**
 * @returns Proxy index of current selected stock item/master (product)
 */
QModelIndex ACC_SelectCustBranchDialog::getCurrentModelIndex() const {
    return mCurrentModelIndex;
}

/**
 * @returns ID of stock item
 */
RB_String ACC_SelectCustBranchDialog::getBranchId() const {
    return mCustModel->getCurrentId();
}

/**
 * Button Help clicked
 */
void ACC_SelectCustBranchDialog::on_pbHelp_clicked() {
    RB_MainWindow* mw = ACC_DIALOGFACTORY->getMainWindow();
    mw->slotHelpSubject(objectName());
}

/**
 * Button Select item clicked
 */
void ACC_SelectCustBranchDialog::on_pbSelect_clicked() {
    if (!mCustModel) return;

    mCurrentModelIndex = tvCustomer->currentIndex();
    setCurrentObject();

    if (mCurrentModelIndex.isValid()) {
        QDialog::accept();
    } else {
        QDialog::reject();
    }
}

/**
 * Button Cancel clicked
 */
void ACC_SelectCustBranchDialog::on_pbCancel_clicked() {
    reject();
}

/**
 * Set current object with data from current selected index
 */
void ACC_SelectCustBranchDialog::setCurrentObject() {
    if (mCurrentModelIndex.isValid()) {
        mCurrentObject = mCustModel->getObject(mCurrentModelIndex, RB2::ResolveNone);
    } else {
        mCurrentObject = NULL;
    }
}

/**
 * Change event, for example translation
 */
void ACC_SelectCustBranchDialog::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
