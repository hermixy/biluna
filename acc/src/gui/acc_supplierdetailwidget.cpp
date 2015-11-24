/*****************************************************************
 * $Id: acc_supplierdetailwidget.cpp 2217 2015-02-13 18:18:14Z rutger $
 * Created: Feb 10, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_supplierdetailwidget.h"

#include <QStringListModel>
#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "acc_selectstockitemdialog.h"
#include "db_dialogfactory.h"
#include "db_internetbrowserfactory.h"
#include "db_internetbrowserwidget.h"
#include "rb_datawidgetmapper.h"
#include "rb_mdiwindow.h"
#include "rb_sqlrelationaldelegate.h"

/**
 * Constructor
 */
ACC_SupplierDetailWidget::ACC_SupplierDetailWidget(QWidget *parent)
        : RB_Widget(parent) {

    setupUi(this);

    mModel = NULL;
    mMapper = NULL;
//    mSizeHint = QSize(600, 400);
}

/**
 * Destructor
 */
ACC_SupplierDetailWidget::~ACC_SupplierDetailWidget() {
    // unique models are always deleted by the widgets,
    // can be MDI window or dialog, but also a dockWindow with table or tree
    // delete mModel; not here deleted by ACC_DebtorsMasterWidget (dockwindow)

    delete mItemModel;

    // HACK: otherwise crash (BTW do not use delete mMapper)
    mModel->deleteMapper();

    RB_DEBUG->print("ACC_SupplierDetailWidget::~ACC_SupplierDetailWidget() OK");
}

/**
 * Initialize widget and models
 */
void ACC_SupplierDetailWidget::init() {
    //
    // Set supplier model and connect to mapper
    //
    mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelSupplier);
    // id is set at ACC_SupplierMasterWidget, no relation setting required

    // because no setFormatTableView()
    connect(mModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            this, SLOT(slotDataIsChanged(QModelIndex,QModelIndex)));
    connect(mModel, SIGNAL(modelBeforeSubmitted()),
            this, SLOT(slotDataIsSaved()));

    //
    // Get mapper for line edits etc.
    //
    mMapper = mModel->getMapper();
    mMapper->addMapping(leVendorCode, mModel->fieldIndex("suppliercode"));
    mMapper->addMapping(leVendorName, mModel->fieldIndex("suppname"));
    mMapper->addMapping(leAddressLine1, mModel->fieldIndex("address1"));
    mMapper->addMapping(leAddressLine2, mModel->fieldIndex("address2"));
    mMapper->addMapping(leAddressLine3, mModel->fieldIndex("address3"));
    mMapper->addMapping(leAddressLine4, mModel->fieldIndex("address4"));
    mMapper->addMapping(leAddressLine5, mModel->fieldIndex("address5"));
    mMapper->addMapping(leAddressLine6, mModel->fieldIndex("address6"));
    mMapper->addMapping(leSupplierContact, mModel->fieldIndex("contactname"));
    mMapper->addMapping(leEmail, mModel->fieldIndex("email"));
    mMapper->addMapping(lePhoneNumber, mModel->fieldIndex("phoneno"));
    mMapper->addMapping(leFaxNumber, mModel->fieldIndex("faxno"));

    // Tab Accounting
    //! ISO4217 Code
    addComboBoxMapping(mModel, "currency_id", "ACC_Currency", "id", "currcode",
                       cbSupplierCurrency, mMapper);
    mMapper->addMapping(deSupplierSince, mModel->fieldIndex("suppliersince"));
    addComboBoxMapping(mModel, "paymentterm_id", "ACC_PaymentTerm", "id", "terms",
                       cbPaymentTerm, mMapper);
    mMapper->addMapping(leBankName, mModel->fieldIndex("bankname"));
    mMapper->addMapping(leBankAddress, mModel->fieldIndex("bankaddress"));
    mMapper->addMapping(leBankCode, mModel->fieldIndex("bankcode"));
    mMapper->addMapping(leBankAccountNumber, mModel->fieldIndex("bankaccountnumber"));

    // use propertyName from QComboBox as "currentText" user properties
    // also possible, however currentIndex only is working correctly
    // create and attach fixed comboBox model
    RB_StringList items;
    items << tr("Not required") << tr("Required");
    QStringListModel* comboBoxModel = new QStringListModel(items, this);
    cbRemittanceAdvice->setModel(comboBoxModel);
    mMapper->addMapping(cbRemittanceAdvice, mModel->fieldIndex("remittance"), "currentIndex");
    addComboBoxMapping(mModel, "taxgroup_id", "ACC_TaxGroup", "id", "taxgroupdescr",
                       cbTaxGroup, mMapper);
    addComboBoxMapping(mModel, "factorcompany_id", "ACC_FactorCompany", "id", "company",
                       cbFactorCompany, mMapper);
    mMapper->addMapping(leTaxReference, mModel->fieldIndex("taxref"));
    addComboBoxMapping(mModel, "area_id", "ACC_Area", "id", "areacode",
                       cbProcurementArea, mMapper);

    //
    // 1. Set child model
    //
    mItemModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelPurchData);

    //
    // 2. Mapper
    //
    mItemMapper = mItemModel->getMapper();
    mItemMapper->addMapping(ileStockCode, mItemModel->fieldIndex("stockmaster_idx"));

    mItemMapper->addMapping(leItemCode, mItemModel->fieldIndex("supplier_partno"));
    mItemMapper->addMapping(leItemDescription, mItemModel->fieldIndex("supplierdescription"));
    mItemMapper->addMapping(lePrice, mItemModel->fieldIndex("price"));
    mItemMapper->addMapping(leUom, mItemModel->fieldIndex("supplieruom"));
    mItemMapper->addMapping(leConversionFactor, mItemModel->fieldIndex("conversionfactor"));
    mItemMapper->addMapping(leLeadTime, mItemModel->fieldIndex("leadtime"));
    items.clear();
    items << tr("No") << tr("Yes");
    cbPreferredSupplier->setModel(new QStringListModel(items, this));
    mItemMapper->addMapping(cbPreferredSupplier, mItemModel->fieldIndex("preferred"), "currentIndex");
    mItemMapper->addMapping(dteEffectiveFrom, mItemModel->fieldIndex("effectivefrom"));

    // 3. select by parent selection

    //
    // 4. Connect model to view
    //
    setFormatTableView(tvItems, mItemModel);

    //
    // 1. Set model
    //
    mNoteModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelSupplierNote);

    //
    // 2. Set relations and mapper for line edits etc.
    //
    mNoteMapper = mNoteModel->getMapper();
    mNoteMapper->addMapping(leSubject, mNoteModel->fieldIndex("subject"));
    mNoteMapper->addMapping(pteDescription, mNoteModel->fieldIndex("description"));

    //
    // 3. Select after relations have been set, only for database models
    //
    // Not required for child model

    //
    // 4. Connect model to main view
    //
    setFormatTableView(tvNote, mNoteModel);
    readSettings();

    int count = mItemModel->columnCount();

    for (int i = 0; i < count; ++i) {
        if (i != RB2::HIDDENCOLUMNS && i != RB2::HIDDENCOLUMNS + 1
                && i != RB2::HIDDENCOLUMNS + 2 && i != RB2::HIDDENCOLUMNS + 3) {
            tvItems->hideColumn(i);
        } else {
            tvItems->showColumn(i);

            if (tvItems->columnWidth(i) < 5) {
                tvItems->setColumnWidth(i, 100);
            }
        }
    }

    count = mNoteModel->columnCount(QModelIndex());

    for (int i = 0; i < count; ++i) {
        if (i != RB2::HIDDENCOLUMNS && i != RB2::HIDDENCOLUMNS+1) {
            tvNote->hideColumn(i);
        } else {
            tvNote->showColumn(i);

            if (tvNote->columnWidth(i) < 5) {
                tvNote->setColumnWidth(i, 100);
            }
        }
    }

    // Navigation widget has been set to the correct tab
    // Request modelindex in case of already selected index
    RB_Widget* wgt = ACC_DIALOGFACTORY->getWidget(ACC_DialogFactory::WidgetNavigation, NULL);
    QModelIndex idx = wgt->getCurrentModelIndex(ACC_ModelFactory::ModelSupplier);
    mMapper->setCurrentModelIndex(idx);
    mItemModel->setRoot(idx);
    mItemModel->select();
}

/**
 * Save data, can be called directly or indirectly via closeEvent()
 * and maybeSave().
 * @param withSelect true if select() is to be done after submitAll()
 */
bool ACC_SupplierDetailWidget::fileSave(bool withSelect) {
    // QApplication::setOverrideCursor(Qt::WaitCursor);
    beforeFileSave();
    bool parentSuccess = false;
    bool childSuccess = false;
    bool noteSuccess = false;

    if (withSelect) {
        noteSuccess = mNoteModel->submitAllAndSelect();
        childSuccess = mItemModel->submitAllAndSelect();
        parentSuccess = mModel->submitAllAndSelect();

    } else {
        noteSuccess = mNoteModel->submitAll();
        childSuccess = mItemModel->submitAll();
        parentSuccess = mModel->submitAll();
    }
    // QApplication::restoreOverrideCursor();

    if (!parentSuccess || !childSuccess || !noteSuccess) {
        return false;
    }

    setWindowModified(false);
    return true;
}

/**
 * Reverse all changes
 */
void ACC_SupplierDetailWidget::fileRevert() {
    mModel->revertAll();
    mItemModel->revertAll();
    setWindowModified(false);
}

/**
 * @returns the help subject, in this case the widget objectname
 */
RB_String ACC_SupplierDetailWidget::getHelpSubject() const {
    return objectName();
}

bool ACC_SupplierDetailWidget::closeWidget() {
    RB_Widget::closeWidget();
    if (maybeSave()) {
        return true;
    }
    return false;
}

/**
 * Button previous clicked
 */
void ACC_SupplierDetailWidget::on_pbPrevious_clicked() {
    mMapper->toPrevious();
}

/**
 * Button next clicked
 */
void ACC_SupplierDetailWidget::on_pbNext_clicked() {
    mMapper->toNext();
}

/**
 * Button save clicked
 */
void ACC_SupplierDetailWidget::on_pbSave_clicked() {
    fileSave(true);
}

/**
 * Button revert clicked
 */
void ACC_SupplierDetailWidget::on_pbRevert_clicked() {
    fileRevert();
}

/**
 * Push button map location clicked,
 * Uses 'http://maps.google.com/maps?q=1683%20Mass%20Ave,%20Cambridge,%20MA'
 * the %20 are spaces
 */
void ACC_SupplierDetailWidget::on_pbMapLocation_clicked() {
    if (!mModel->getProxyIndex().isValid()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No supplier selected.\n"
                                                   "Please select a supplier first."));
        return;
    }

    DB_InternetBrowserWidget* wdgt = DB_INTERNETBROWSERFACTORY->newBrowserWidget();
    QString strUrl = "http://maps.google.com/maps?q=";

    if (!leAddressLine1->text().isEmpty()) {
        strUrl.append(leAddressLine1->text().trimmed() + " ");
    }
    if (!leAddressLine2->text().isEmpty()) {
        strUrl.append(leAddressLine2->text().trimmed() + " ");
    }
    if (!leAddressLine3->text().isEmpty()) {
        strUrl.append(leAddressLine3->text().trimmed() + " ");
    }
    if (!leAddressLine4->text().isEmpty()) {
        strUrl.append(leAddressLine4->text().trimmed() + " ");
    }
    if (!leAddressLine5->text().isEmpty()) {
        strUrl.append(leAddressLine5->text().trimmed() + " ");
    }
    if (!leAddressLine6->text().isEmpty()) {
        strUrl.append(leAddressLine6->text().trimmed() + " ");
    }

    wdgt->loadPage(strUrl);
}

/**
 * Push button send email clicked
 */
void ACC_SupplierDetailWidget::on_pbSendEmail_clicked() {
    RB_String email = leEmail->text();
    if (email.isEmpty()) {
        return;
    }
    QDesktopServices::openUrl(QUrl("mailto:" + email + "?subject=<Subject>&body=Dear ,"));
}

/**
 * Button select stock item clicked
 */
void ACC_SupplierDetailWidget::on_ileStockCode_clicked() {
    if (!tvItems->currentIndex().isValid()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No item selected.\n"
                                                   "Please select an item."));
        return;
    }

    RB_Dialog* dlga = ACC_DIALOGFACTORY->getDialog(ACC_DialogFactory::DialogSelectStockMaster);
    ACC_SelectStockItemDialog* dlg = dynamic_cast<ACC_SelectStockItemDialog*>(dlga);
    int res = dlg->exec();

    QModelIndex index = dlg->getCurrentChild1ModelIndex();

    if (res == QDialog::Accepted && index.isValid()) {
        RB_MmProxy* m = mItemModel;
        int row = tvItems->currentIndex().row();

        QModelIndex idx = m->index(row, m->fieldIndex("stockmaster_idx"));
        m->setData(idx, dlg->getStockId() + dlg->getCode()
                   + " - " + dlg->getDescription(), Qt::EditRole);
        idx = m->index(row, m->fieldIndex("price"));
        m->setData(idx, dlg->getMaterialCost(), Qt::EditRole);
        idx = m->index(row, m->fieldIndex("supplieruom"));
        m->setData(idx, dlg->getUnitOfMeasurement(), Qt::EditRole);
    }

    dlg->deleteLater();
}

/**
 * Push button add (stock) item clicked
 */
void ACC_SupplierDetailWidget::on_pbAddItem_clicked() {
    if (!mItemModel) return;
    mItemModel->undoFilterSort();

    // always insert at the end
    int row = mItemModel->rowCount();
    mItemModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex index = mItemModel->index(row, mItemModel->fieldIndex("stockmaster_idx"));
    mItemModel->setData(index, "0", Qt::EditRole);
    index = mItemModel->index(row, mItemModel->fieldIndex("price"));
    mItemModel->setData(index, 0.0, Qt::EditRole);
    index = mItemModel->index(row, mItemModel->fieldIndex("supplieruom"));
    mItemModel->setData(index, "-", Qt::EditRole);
    QDateTime date = QDateTime::currentDateTime();
    index = mItemModel->index(row, mItemModel->fieldIndex("effectivefrom"));
    mItemModel->setData(index, date, Qt::EditRole);
    // end NOTE

    tvItems->setCurrentIndex(mItemModel->index(row, RB2::HIDDENCOLUMNS));
    tvItems->scrollTo(tvItems->currentIndex());
}

/**
 * Push button delete (stock) item clicked
 */
void ACC_SupplierDetailWidget::on_pbDeleteItem_clicked() {
    if (!mItemModel) return;

    int res = DB_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) return;

    QModelIndex index = tvItems->currentIndex();
    if (index.isValid()) {
        mItemModel->removeRows(index.row(), 1, QModelIndex());
        mItemModel->submitAllAndSelect();
        // no modified to false because parent could have been changed
    }
}

/**
 * Push button add note clicked
 */
void ACC_SupplierDetailWidget::on_pbAddNote_clicked() {
    if (!mNoteModel) return;
    mNoteModel->undoFilterSort();

    // always insert at the beginning
    int row = 0; // mModel->rowCount();
    mNoteModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table (in tableView otherwise new row will not show)
    // ...
    // end NOTE

    tvNote->setCurrentIndex(mNoteModel->index(row,  RB2::HIDDENCOLUMNS, QModelIndex()));
    tvNote->scrollTo(mNoteModel->index(row,  RB2::HIDDENCOLUMNS, QModelIndex()));
    leSubject->setFocus();
    leSubject->selectAll();
}

/**
 * Push button delete note clicked
 */
void ACC_SupplierDetailWidget::on_pbDeleteNote_clicked() {
    if (!mNoteModel) return;

    int res = ACC_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) return;

    QModelIndex index = tvNote->currentIndex();
    if (index.isValid()) {
        mNoteModel->removeRows(index.row(), 1, QModelIndex());
        mNoteModel->submitAllAndSelect();
        // no setModified(false) here because parent could have changed
    }
}

/**
 * Reset widgets to default state, after insertion of row in model
 */
void ACC_SupplierDetailWidget::slotResetWidgets() {
    tabWidget->setCurrentIndex(0);
    leVendorCode->setFocus();
    leVendorCode->selectAll();
}

/**
 * Change event, for example translation
 */
void ACC_SupplierDetailWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
