/*****************************************************************
 * $Id: acc_locationdialog.cpp 1859 2013-01-05 15:18:28Z rutger $
 * Created: Feb 18, 2010 8:44:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_locationdialog.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "db_dialogfactory.h"
#include "db_modelfactory.h"
#include "rb_cmbdelegate.h"
#include "rb_sqlrelationaldelegate.h"

/**
 * Constructor
 */
ACC_LocationDialog::ACC_LocationDialog(QWidget *parent)
        : RB_Dialog(parent) {

    setupUi(this);

    mModel = NULL;
}

/**
 * Destructor
 */
ACC_LocationDialog::~ACC_LocationDialog() {
    // unique models are always deleted by the widgets/dialogs,
    // can be MDI window or dialog, but also a dockWindow with table or tree
    delete mModel;
    RB_DEBUG->print("ACC_LocationDialog::~ACC_LocationDialog() OK");
}

/**
 * Initialize widget and models
 */
void ACC_LocationDialog::init() {

    setWindowTitle(tr("Edit Company (Warehouse) Location[*]"));

    mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelLocation, false);
    mModel->setRoot(DB_MODELFACTORY->getRootId());

    mMapper = mModel->getMapper();
    mMapper->addMapping(leLocationName, mModel->fieldIndex("locationname"));
    mMapper->addMapping(leDeliveryAddress1, mModel->fieldIndex("deladd1"));
    mMapper->addMapping(leDeliveryAddress2, mModel->fieldIndex("deladd2"));
    mMapper->addMapping(leDeliveryAddress3, mModel->fieldIndex("deladd3"));
    mMapper->addMapping(leDeliveryAddress4, mModel->fieldIndex("deladd4"));
    mMapper->addMapping(leDeliveryAddress5, mModel->fieldIndex("deladd5"));
    mMapper->addMapping(leDeliveryAddress6, mModel->fieldIndex("deladd6"));
    mMapper->addMapping(leContactForDeliveries, mModel->fieldIndex("contact"));
    mMapper->addMapping(leTelephone, mModel->fieldIndex("tel"));
    mMapper->addMapping(leFacsimile, mModel->fieldIndex("fax"));
    mMapper->addMapping(leEmail, mModel->fieldIndex("email"));
    addComboBoxMapping(mModel, "taxprovince_id", "ACC_TaxProvince", "id",
                       "taxprovincename", this->cbTaxProvince, mMapper);
    // create and attach fixed comboBox model, currentIndex is property of combobox
    RB_StringList items;
    items << tr("No") << tr("Yes");
    mModel->setTextList(mModel->fieldIndex("managed"), items);
    cbEnableWarehouseMgt->setModel(new QStringListModel(items, this));
    mMapper->addMapping(cbEnableWarehouseMgt, mModel->fieldIndex("managed"),
                        "currentIndex");

    mModel->select();

    setFormatTableView(tvLocation, mModel);
    tvLocation->setItemDelegateForColumn(mModel->fieldIndex("managed"),
                                          new RB_CmbDelegate(this, items, mModel));

    readSettings();

    // Hidden columns only for tableviews
    int colCount = mModel->columnCount();
    for (int i = 0; i < colCount; ++i) {
        if (i < RB2::HIDDENCOLUMNS) {
            tvLocation->hideColumn(i);
        } else {
            tvLocation->showColumn(i);

            if (tvLocation->columnWidth(i) < 5) {
                tvLocation->setColumnWidth(i, 100);
            }
        }
    }
}

/**
 * Save data, can be called directly or indirectly via closeEvent()
 * and maybeSave().
 * @param withSelect true if select() is to be done after submitAll()
 */
bool ACC_LocationDialog::fileSave(bool withSelect) {
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
void ACC_LocationDialog::fileRevert() {
    mModel->revert();
    setWindowModified(false);
}

/**
 * Button add location clicked
 */
void ACC_LocationDialog::on_pbAdd_clicked() {
    if (!mModel) return;
    mModel->undoFilterSort();

    // always insert at the end
    int row = mModel->rowCount();
    mModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex index = mModel->index(row, mModel->fieldIndex("taxprovince_id"));
    mModel->setData(index, "0", Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("managed"));
    mModel->setData(index, 0, Qt::EditRole);
    // end NOTE

    tvLocation->setCurrentIndex(mModel->index(row, RB2::HIDDENCOLUMNS));
    tvLocation->scrollTo(tvLocation->currentIndex());
    leLocationName->setFocus();
    leLocationName->selectAll();
}

/**
 * Button delete location clicked
 */
void ACC_LocationDialog::on_pbDelete_clicked() {
    if (!mModel) return;
    int res = DB_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) return;

    QModelIndex index = tvLocation->currentIndex();
    if (index.isValid()) {
        mModel->removeRows(index.row(), 1, QModelIndex());
        mModel->submitAllAndSelect();
        setWindowModified(false);
    }
}

/**
 * Button Help clicked
 */
void ACC_LocationDialog::on_pbHelp_clicked() {
    RB_MainWindow* mw = ACC_DIALOGFACTORY->getMainWindow();
    mw->slotHelpSubject(this->objectName());
}

/**
 * Button OK clicked
 */
void ACC_LocationDialog::on_pbOk_clicked() {
    fileSave(true);
    accept();
}

/**
 * Button Cancel clicked
 */
void ACC_LocationDialog::on_pbCancel_clicked() {
    reject();
}

/**
 * Change event, for example translation
 */
void ACC_LocationDialog::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
