/*****************************************************************
 * $Id: acc_factorcompanydialog.cpp 1845 2012-12-23 14:39:11Z rutger $
 * Created: Mar 18, 2010 11:14:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_factorcompanydialog.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "db_dialogfactory.h"
#include "rb_datawidgetmapper.h"
#include "rb_mdiwindow.h"
#include "rb_sqlrelationaldelegate.h"

/**
 * Constructor
 */
ACC_FactorCompanyDialog::ACC_FactorCompanyDialog(QWidget *parent)
        : RB_Dialog(parent) {

    setupUi(this);

    mModel = NULL;
    mMapper = NULL;
}

/**
 * Destructor
 */
ACC_FactorCompanyDialog::~ACC_FactorCompanyDialog() {
    // unique models are always deleted by the widgets,
    // can be MDI window or dialog, but also a dockWindow with table or tree
    delete mMapper;
    delete mModel;
    RB_DEBUG->print("ACC_FactorCompanyDialog::~ACC_FactorCompanyDialog() OK");
}

/**
 * Initialize widget and models
 */
void ACC_FactorCompanyDialog::init() {

    setWindowTitle(tr("Edit Factor Companies[*]"));

    //
    // Set model and connect to orders table view
    //
    mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelFactorCompany);
    mModel->setRoot("syssetting" /*ACC_MODELFACTORY->getRootId()*/);

    mMapper = mModel->getMapper();
    mMapper->addMapping(leFactorName, mModel->fieldIndex("coyname"));
    mMapper->addMapping(leAddressLine1, mModel->fieldIndex("address1"));
    mMapper->addMapping(leAddressLine2, mModel->fieldIndex("address2"));
    mMapper->addMapping(leAddressLine3, mModel->fieldIndex("address3"));
    mMapper->addMapping(leAddressLine4, mModel->fieldIndex("address4"));
    mMapper->addMapping(leAddressLine5, mModel->fieldIndex("address5"));
    mMapper->addMapping(leAddressLine6, mModel->fieldIndex("address6"));
    mMapper->addMapping(leContactName, mModel->fieldIndex("contact"));
    mMapper->addMapping(leTelephone, mModel->fieldIndex("telephone"));
    mMapper->addMapping(leFax, mModel->fieldIndex("fax"));
    mMapper->addMapping(leEmail, mModel->fieldIndex("email"));

    mModel->select();

    setFormatTableView(tvFactorCompany, mModel);
    readSettings();

    // Hidden columns only for tableviews
    int colCount = mModel->columnCount();
    for (int i = 0; i < colCount; ++i) {
        if (i < RB2::HIDDENCOLUMNS) {
            tvFactorCompany->hideColumn(i);
        } else {
            tvFactorCompany->showColumn(i);

            if (tvFactorCompany->columnWidth(i) < 5) {
                tvFactorCompany->setColumnWidth(i, 100);
            }
        }
    }

}

/**
 * Save data, can be called directly or indirectly via closeEvent()
 * and maybeSave().
 * @param withSelect true if select() is to be done after submitAll()
 */
bool ACC_FactorCompanyDialog::fileSave(bool withSelect) {
    // QApplication::setOverrideCursor(Qt::WaitCursor);
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
    // QApplication::restoreOverrideCursor();
    return false;
}

/**
 * Reverse all changes
 */
void ACC_FactorCompanyDialog::fileRevert() {
    mModel->revert();
    setWindowModified(false);
}

/**
 * Button add factor company clicked
 */
void ACC_FactorCompanyDialog::on_pbAdd_clicked() {
    if (!mModel) return;
    mModel->undoFilterSort();

    // always insert at the end
    int row = mModel->rowCount();
    mModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
//    QModelIndex index = mModel->index(row, mModel->fieldIndex("salestaxglacct_idx"));
//    mModel->setData(index, "0", Qt::EditRole);
//    index = mModel->index(row, mModel->fieldIndex("purchtaxglaccount"));
//    mModel->setData(index, "0", Qt::EditRole);
//    mModel->submit();
    // end NOTE

    tvFactorCompany->setCurrentIndex(mModel->index(row, RB2::HIDDENCOLUMNS));
    tvFactorCompany->scrollTo(tvFactorCompany->currentIndex());
    leFactorName->setFocus();
    leFactorName->selectAll();
}

/**
 * Button delete factor company clicked
 */
void ACC_FactorCompanyDialog::on_pbDelete_clicked() {
    if (!mModel) return;
    int res = DB_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) return;

    QModelIndex index = tvFactorCompany->currentIndex();
    if (index.isValid()) {
        mModel->removeRows(index.row(), 1, QModelIndex());
        mModel->submitAllAndSelect();
        setWindowModified(false);
    }
}

/**
 * Button help clicked
 */
void ACC_FactorCompanyDialog::on_pbHelp_clicked() {
    RB_MainWindow* mw = ACC_DIALOGFACTORY->getMainWindow();
    mw->slotHelpSubject(this->objectName());
}

/**
 * Button OK clicked
 */
void ACC_FactorCompanyDialog::on_pbOk_clicked() {
    fileSave(true);
    accept();
}

/**
 * Button cancel clicked
 */
void ACC_FactorCompanyDialog::on_pbCancel_clicked() {
    reject();
}

/**
 * Change event, for example translation
 */
void ACC_FactorCompanyDialog::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
