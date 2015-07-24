/*****************************************************************
 * $Id: acc_custbranchwidget.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Dec 24, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_custbranchwidget.h"

#include <QStringListModel>
#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "db_dialogfactory.h"
#include "db_internetbrowserfactory.h"
#include "db_internetbrowserwidget.h"
#include "rb_datawidgetmapper.h"
#include "rb_mdiwindow.h"
#include "rb_sqlrelationaldelegate.h"

/**
 * Constructor
 */
ACC_CustBranchWidget::ACC_CustBranchWidget(QWidget *parent)
        : RB_Widget(parent) {

    setupUi(this);

    mCustModel = NULL;
    mCustMapper = NULL;
    mNoteModel = NULL;
    mNoteMapper = NULL;
}

/**
 * Destructor
 */
ACC_CustBranchWidget::~ACC_CustBranchWidget() {
    delete mNoteModel;
    // HACK: otherwise crash (BTW do not use delete mCustMapper)
    mCustModel->deleteMapper();
    RB_DEBUG->print("ACC_CustBranchWidget::~ACC_CustBranchWidget() OK");
}

/**
 * Initialize widget and models
 */
void ACC_CustBranchWidget::init() {
    //
    // 1. Set model for customer mapper
    //
    mCustModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelCustomer);

    // Required because not setFormatTableView()
    connect(mCustModel, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
            this, SLOT(slotDataIsChanged(const QModelIndex&, const QModelIndex&)));
    connect(mCustModel, SIGNAL(modelBeforeSubmitted()),
            this, SLOT(slotDataIsSaved()));

    //
    // Get mapper for line edits etc.
    //
    mCustMapper = mCustModel->getMapper();
    mCustMapper->addMapping(ileCustomerParent, mCustModel->fieldIndex("custparent_idx"));
    mCustMapper->addMapping(this->leCustomerCode, mCustModel->fieldIndex("customerno"));
    mCustMapper->addMapping(this->leCustomerName, mCustModel->fieldIndex("mname"));
    mCustMapper->addMapping(this->lePostAddress1, mCustModel->fieldIndex("brpostaddress1"));
    mCustMapper->addMapping(this->lePostAddress2, mCustModel->fieldIndex("brpostaddress2"));
    mCustMapper->addMapping(this->lePostAddress3, mCustModel->fieldIndex("brpostaddress3"));
    mCustMapper->addMapping(this->lePostAddress4, mCustModel->fieldIndex("brpostaddress4"));
    mCustMapper->addMapping(this->lePostAddress5, mCustModel->fieldIndex("brpostaddress5"));
    mCustMapper->addMapping(this->lePostAddress6, mCustModel->fieldIndex("brpostaddress6"));
    mCustMapper->addMapping(this->leStreetAddress1, mCustModel->fieldIndex("brstreetaddress1"));
    mCustMapper->addMapping(this->leStreetAddress2, mCustModel->fieldIndex("brstreetaddress2"));
    mCustMapper->addMapping(this->leStreetAddress3, mCustModel->fieldIndex("brstreetaddress3"));
    mCustMapper->addMapping(this->leStreetAddress4, mCustModel->fieldIndex("brstreetaddress4"));
    mCustMapper->addMapping(this->leStreetAddress5, mCustModel->fieldIndex("brstreetaddress5"));
    mCustMapper->addMapping(this->leStreetAddress6, mCustModel->fieldIndex("brstreetaddress6"));

    addComboBoxMapping(mCustModel, "salestype_id", "ACC_SalesType", "id", "typeabbrev",
                       cbSalesType, mCustMapper);
    addComboBoxMapping(mCustModel, "type_id", "ACC_DebtorType", "id", "typename",
                       cbCustomerType, mCustMapper);
    mCustMapper->addMapping(this->dteCustomerSince, mCustModel->fieldIndex("clientsince"));
    mCustMapper->addMapping(this->leDiscountPercent, mCustModel->fieldIndex("discount"));
    mCustMapper->addMapping(this->leDiscountCode, mCustModel->fieldIndex("discountcode"));
    mCustMapper->addMapping(this->lePaymentDiscount, mCustModel->fieldIndex("pymtdiscount"));
    mCustMapper->addMapping(this->leBankName, mCustModel->fieldIndex("bankname"));
    mCustMapper->addMapping(this->leBankAddress, mCustModel->fieldIndex("bankaddress"));
    mCustMapper->addMapping(this->leBankCode, mCustModel->fieldIndex("bankcode"));
    mCustMapper->addMapping(this->leBankAccountNumber, mCustModel->fieldIndex("bankaccountnumber"));
    mCustMapper->addMapping(this->leCreditLimit, mCustModel->fieldIndex("creditlimit"));
    mCustMapper->addMapping(this->leTaxReference, mCustModel->fieldIndex("taxref"));
    addComboBoxMapping(mCustModel, "paymentterm_id", "ACC_PaymentTerm", "id", "terms",
                       cbPaymentTerm, mCustMapper);
    addComboBoxMapping(mCustModel, "holdreason_id", "ACC_HoldReason", "id", "reasondescription",
                       cbCreditStatus, mCustMapper);
    addComboBoxMapping(mCustModel, "currency_id", "ACC_Currency", "id", "currcode",
                       cbCustomerCurrency, mCustMapper);

    // create and attach fixed comboBox model, currentIndex is property of combobox
    RB_StringList items;
    items << tr("No") << tr("Yes");
    cbCustomerPoLine->setModel(new QStringListModel(items, this));
    mCustMapper->addMapping(cbCustomerPoLine, mCustModel->fieldIndex("customerpoline"),
                            "currentIndex");

    items.clear();
    items << tr("Address to branch") << tr("Address to HO") << tr("Address to order contact");
    cbInvoiceAddressing->setModel(new QStringListModel(items, this));
    mCustMapper->addMapping(cbInvoiceAddressing,
                            mCustModel->fieldIndex("invaddrbranch"), "currentIndex");

    //
    // Get mapper for line edits etc.
    //
    // mCustMapper->addMapping(this->le, mCustModel->fieldIndex("lat"));
    // mCustMapper->addMapping(this->le, mCustModel->fieldIndex("lng"));
    mCustMapper->addMapping(this->leDefaultDays, mCustModel->fieldIndex("estdeliverydays"));
    addComboBoxMapping(mCustModel, "area_id", "ACC_Area", "id", "areadescription",
                       cbSalesArea, mCustMapper);
    addComboBoxMapping(mCustModel, "salesman_id", "ACC_SalesMan", "id", "salesmanname",
                       cbSalesPerson, mCustMapper);
    mCustMapper->addMapping(this->leForwardDate, mCustModel->fieldIndex("fwddate"));
    mCustMapper->addMapping(this->lePhoneNumber, mCustModel->fieldIndex("phoneno"));
    mCustMapper->addMapping(this->leFaxNumber, mCustModel->fieldIndex("faxno"));
    mCustMapper->addMapping(this->leBranchContact, mCustModel->fieldIndex("contactname"));
    mCustMapper->addMapping(this->leEmail, mCustModel->fieldIndex("email"));
    addComboBoxMapping(mCustModel, "defaultlocation_id", "ACC_Location", "id", "locationname",
                       cbDrawStockFrom, mCustMapper);
    addComboBoxMapping(mCustModel, "taxgroup_id", "ACC_TaxGroup", "id", "taxgroupdescr",
                       cbTaxGroup, mCustMapper);
    addComboBoxMapping(mCustModel, "defaultshipvia_id", "ACC_Shipper", "id", "shippername",
                       cbDefaultShipper, mCustMapper);
    // create and attach fixed comboBox model, currentIndex is property of combobox
    items.clear();
    items << tr("Show company details and logo") << tr("Hide company details and logo");
    cbDefaultPackList->setModel(new QStringListModel(items, this));
    mCustMapper->addMapping(cbDefaultPackList, mCustModel->fieldIndex("deliverblind"),
                            "currentIndex");
    items.clear();
    items << tr("Enabled") << tr("Disabled");
    cbTransactions->setModel(new QStringListModel(items, this));
    mCustMapper->addMapping(cbTransactions, mCustModel->fieldIndex("disabletrans"),
                            "currentIndex");
    mCustMapper->addMapping(leSpecialInstructions, mCustModel->fieldIndex("specialinstructions"));
    mCustMapper->addMapping(leCustomerEDI, mCustModel->fieldIndex("custbranchcode"));

    //
    // 1. Set model
    //
    mNoteModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelCustomerNote);

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
    int count = mNoteModel->columnCount(QModelIndex());

    for (int i = 0; i < count; ++i) {
        if (i != RB2::HIDDENCOLUMNS && i != RB2::HIDDENCOLUMNS+1) {
            tvNote->hideColumn(i);
        } else {
            if (tvNote->columnWidth(i) < 5) {
                tvNote->setColumnWidth(i, 100);
            }
        }
    }

    // Navigation widget has been set to the correct tab
    // Request modelindex in case of already selected index
    RB_Widget* wgt = ACC_DIALOGFACTORY->getWidget(ACC_DialogFactory::WidgetNavigation, NULL);
    QModelIndex idx = wgt->getCurrentModelIndex(ACC_ModelFactory::ModelCustomer);
    mCustMapper->setCurrentModelIndex(idx);
    mNoteModel->setRoot(idx);

    // Debug only
//    int childCount = children().count();
//    for (int i = 0; i < childCount; ++i) {
//        QObject* obj = children().at(i);
//        RB_DEBUG->print(obj->objectName());
//    }

}

/**
 * Save data, can be called directly or indirectly via closeEvent()
 * and maybeSave().
 * @param withSelect true if select() is to be done after submitAll()
 */
bool ACC_CustBranchWidget::fileSave(bool withSelect) {
    // QApplication::setOverrideCursor(Qt::WaitCursor);
    // HACK to emit editingFinished() for current QLineEdit:
    beforeFileSave();
//    if (mNoteModel) {
//        mNoteModel->submitAllAndSelect();

//        if (withSelect) {
//            mNoteModel->setRoot(QModelIndex());
//        }
//    }

    if (withSelect && mCustModel) {
        if (mCustModel->submitAllAndSelect()) {

            setWindowModified(false);
            return true;
        }
    } else if (mCustModel) {
        if (mCustModel->submitAll()) {
            setWindowModified(false);
            return true;
        }
    }
    // QApplication::restoreOverrideCursor();
    return false;
}

/**
 * Revert changes
 */
void ACC_CustBranchWidget::fileRevert() {
    mCustModel->revertAll();
    setWindowModified(false);
}

/**
 * Button previous clicked
 */
void ACC_CustBranchWidget::on_pbPrevious_clicked() {
    mCustMapper->toPrevious();
}

/**
 * Button next clicked
 */
void ACC_CustBranchWidget::on_pbNext_clicked() {
    mCustMapper->toNext();
}

/**
 * Button save clicked
 */
void ACC_CustBranchWidget::on_pbSave_clicked() {
    fileSave(true);
}

/**
 * Button revert clicked
 */
void ACC_CustBranchWidget::on_pbRevert_clicked() {
    fileRevert();
}

/**
 * Button select parent company clicked
 */
void ACC_CustBranchWidget::on_ileCustomerParent_clicked() {
    if (!mCustModel->getProxyIndex().isValid()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No customer or branch selected.\n"
                                                   "Please select a customer/branch first."));
        return;
    }

    RB_Dialog* dlg = ACC_DIALOGFACTORY->getDialog(ACC_DialogFactory::DialogSelectCustBranch);

    if (dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();
        if (obj) {
            int row = mCustModel->getProxyIndex().row();
            QModelIndex parentIdx = mCustModel->getProxyIndex().parent();
            QModelIndex idx = mCustModel->index(row, mCustModel->fieldIndex("custparent_idx"), parentIdx);
            mCustModel->setData(idx, obj->getId()
                               + obj->getValue("mname").toString(), Qt::EditRole);
        }
    } else {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No branch selected.\n"
                                                   "Customer branch is unchanged."));
    }

    dlg->deleteLater();
}

/**
 * Push button copy post address to street address clicked
 */
void ACC_CustBranchWidget::on_pbCopyParent_clicked() {
    if (!mCustModel->getProxyIndex().isValid()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No customer or branch selected.\n"
                                                   "Please select a customer/branch first."));
        return;
    }

    int res = ACC_DIALOGFACTORY->requestYesNoDialog(tr("Copy data"),
                                  tr("The data will be copied to this item.\n"
                                     "Do you want to replace the current data?"));
    if (res != QMessageBox::Yes) return;

    RB_MmProxy* m = mCustModel;
    QModelIndex cIdx = m->getProxyIndex();
    int row = cIdx.row();
    QModelIndex pIdx = m->parent(cIdx); // cIdx.parent();

    if (!pIdx.isValid()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No parent company available."));
        return;
    }

    QModelIndex idx;
    idx = m->index(pIdx.row(), m->fieldIndex("brpostaddress1"), pIdx.parent());
    cIdx = m->index(row, m->fieldIndex("brpostaddress1"), pIdx);
    m->setData(cIdx, m->data(idx));
    idx = m->index(pIdx.row(), m->fieldIndex("brpostaddress2"), pIdx.parent());
    cIdx = m->index(row, m->fieldIndex("brpostaddress2"), pIdx);
    m->setData(cIdx, m->data(idx));
    idx = m->index(pIdx.row(), m->fieldIndex("brpostaddress3"), pIdx.parent());
    cIdx = m->index(row, m->fieldIndex("brpostaddress3"), pIdx);
    m->setData(cIdx, m->data(idx));
    idx = m->index(pIdx.row(), m->fieldIndex("brpostaddress4"), pIdx.parent());
    cIdx = m->index(row, m->fieldIndex("brpostaddress4"), pIdx);
    m->setData(cIdx, m->data(idx));
    idx = m->index(pIdx.row(), m->fieldIndex("brpostaddress5"), pIdx.parent());
    cIdx = m->index(row, m->fieldIndex("brpostaddress5"), pIdx);
    m->setData(cIdx, m->data(idx));
    idx = m->index(pIdx.row(), m->fieldIndex("brpostaddress6"), pIdx.parent());
    cIdx = m->index(row, m->fieldIndex("brpostaddress6"), pIdx);
    m->setData(cIdx, m->data(idx));
    idx = m->index(pIdx.row(), m->fieldIndex("brstreetaddress1"), pIdx.parent());
    cIdx = m->index(row, m->fieldIndex("brstreetaddress1"), pIdx);
    m->setData(cIdx, m->data(idx));
    idx = m->index(pIdx.row(), m->fieldIndex("brstreetaddress2"), pIdx.parent());
    cIdx = m->index(row, m->fieldIndex("brstreetaddress2"), pIdx);
    m->setData(cIdx, m->data(idx));
    idx = m->index(pIdx.row(), m->fieldIndex("brstreetaddress3"), pIdx.parent());
    cIdx = m->index(row, m->fieldIndex("brstreetaddress3"), pIdx);
    m->setData(cIdx, m->data(idx));
    idx = m->index(pIdx.row(), m->fieldIndex("brstreetaddress4"), pIdx.parent());
    cIdx = m->index(row, m->fieldIndex("brstreetaddress4"), pIdx);
    m->setData(cIdx, m->data(idx));
    idx = m->index(pIdx.row(), m->fieldIndex("brstreetaddress5"), pIdx.parent());
    cIdx = m->index(row, m->fieldIndex("brstreetaddress5"), pIdx);
    m->setData(cIdx, m->data(idx));
    idx = m->index(pIdx.row(), m->fieldIndex("brstreetaddress6"), pIdx.parent());
    cIdx = m->index(row, m->fieldIndex("brstreetaddress6"), pIdx);
    m->setData(cIdx, m->data(idx));
}

/**
 * Push button copy postal to visit address clicked
 */
void ACC_CustBranchWidget::on_pbCopyPostAddress_clicked() {
    if (!mCustModel->getProxyIndex().isValid()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No branch selected.\n"
                                                   "Please select a customer/branch first."));
        return;
    }

    int res = ACC_DIALOGFACTORY->requestYesNoDialog(tr("Copy data"),
                                  tr("The data will be copied to this item.\n"
                                     "Do you want to replace the current data?"));
    if (res != QMessageBox::Yes) return;

    RB_MmProxy* m = mCustModel;
    QModelIndex cIdx = m->getProxyIndex(); // mCustMapper->rootIndex();
    int row = cIdx.row();
    QModelIndex pIdx = cIdx.parent();

    QModelIndex idx;
    idx = m->index(row, m->fieldIndex("brstreetaddress1"), pIdx);
    m->setData(idx, lePostAddress1->text(), Qt::EditRole);
    idx = m->index(row, m->fieldIndex("brstreetaddress2"), pIdx);
    m->setData(idx, lePostAddress2->text(), Qt::EditRole);
    idx = m->index(row, m->fieldIndex("brstreetaddress3"), pIdx);
    m->setData(idx, lePostAddress3->text(), Qt::EditRole);
    idx = m->index(row, m->fieldIndex("brstreetaddress4"), pIdx);
    m->setData(idx, lePostAddress4->text(), Qt::EditRole);
    idx = m->index(row, m->fieldIndex("brstreetaddress5"), pIdx);
    m->setData(idx, lePostAddress5->text(), Qt::EditRole);
    idx = m->index(row, m->fieldIndex("brstreetaddress6"), pIdx);
    m->setData(idx, lePostAddress6->text(), Qt::EditRole);
}

/**
 * Push button map location clicked,
 * Uses 'http://maps.google.com/maps?q=1683%20Mass%20Ave,%20Cambridge,%20MA'
 * the %20 are spaces
 */
void ACC_CustBranchWidget::on_pbMapLocation_clicked() {
    if (!mCustModel->getProxyIndex().isValid()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No customer or branch selected.\n"
                                                   "Please select a customer/branch first."));
        return;
    }

    DB_InternetBrowserWidget* wdgt = DB_INTERNETBROWSERFACTORY->newBrowserWidget();
    QString strUrl = "http://maps.google.com/maps?q=";

    if (!leStreetAddress1->text().isEmpty()) {
        strUrl.append(leStreetAddress1->text().trimmed() + " ");
    }
    if (!leStreetAddress2->text().isEmpty()) {
        strUrl.append(leStreetAddress2->text().trimmed() + " ");
    }
    if (!leStreetAddress3->text().isEmpty()) {
        strUrl.append(leStreetAddress3->text().trimmed() + " ");
    }
    if (!leStreetAddress4->text().isEmpty()) {
        strUrl.append(leStreetAddress4->text().trimmed() + " ");
    }
    if (!leStreetAddress5->text().isEmpty()) {
        strUrl.append(leStreetAddress5->text().trimmed() + " ");
    }
    if (!leStreetAddress6->text().isEmpty()) {
        strUrl.append(leStreetAddress6->text().trimmed() + " ");
    }

    wdgt->loadPage(strUrl);
}

/**
 * Push button send email clicked
 */
void ACC_CustBranchWidget::on_pbSendEmail_clicked() {
    RB_String email = leEmail->text();
    if (email.isEmpty()) {
        return;
    }
    QDesktopServices::openUrl(QUrl("mailto:" + email + "?subject=<Subject>&body=Dear "));
}

/**
 * Push button add note clicked
 */
void ACC_CustBranchWidget::on_pbAddNote_clicked() {
    if (!mNoteModel || !mCustModel->getProxyIndex().isValid()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No customer selected"));
        return;
    }
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
void ACC_CustBranchWidget::on_pbDeleteNote_clicked() {
    if (!mNoteModel) return;

    int res = ACC_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) return;

    QModelIndex index = tvNote->currentIndex();
    if (index.isValid()) {
        mNoteModel->removeRows(index.row(), 1, QModelIndex());
        mNoteModel->submitAllAndSelect();
        // setWindowModified(false); not here because parent could have changed

    }
}

/**
 * Reset widgets to default state, after insertion of row in model
 */
void ACC_CustBranchWidget::slotResetWidgets() {
    tabWidget->setCurrentIndex(0);
    leCustomerCode->setFocus();
    leCustomerCode->selectAll();
}

/**
 * @returns the help subject, in this case the widget objectname
 */
RB_String ACC_CustBranchWidget::getHelpSubject() const {
    return objectName();
}

bool ACC_CustBranchWidget::closeWidget() {
//    RB_Widget::closeWidget();
    if (maybeSave()) {
        return true;
    }
    return false;
}

/**
 * Change event, for example translation
 */
void ACC_CustBranchWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
