/*****************************************************************
 * $Id: crm_contactwidget.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Feb 18, 2012 3:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "crm_contactwidget.h"

#include <QStringListModel>
#include "crm_actioncustomer.h"
#include "crm_dialogfactory.h"
#include "crm_modelfactory.h"
#include "db_dialogfactory.h"
#include "rb_datawidgetmapper.h"
#include "rb_mdiwindow.h"
#include "rb_sqlrelationaldelegate.h"

/**
 * Constructor
 */
CRM_ContactWidget::CRM_ContactWidget(QWidget *parent)
        : RB_Widget(parent) {

    setupUi(this);

    mCustomerModel = NULL;
    mContactModel = NULL;
    mContactMapper = NULL;
    mContactDetModel = NULL;
    mContactDetMapper = NULL;

    mCurrentId = "";
    mIsInitialized = false;
}

/**
 * Destructor
 */
CRM_ContactWidget::~CRM_ContactWidget() {
    // unique models are always deleted by the widgets
    delete mContactModel;
    delete mContactDetModel;
    RB_DEBUG->print("CRM_ContactWidget::~CRM_ContactWidget() OK");
}

/**
 * Initialize widget and models
 */
void CRM_ContactWidget::init() {
    RB_StringList items;
    items << tr("Customer") << tr("Account") << tr("All");
    cbSelectedBy->addItems(items);
    connect(cbSelectedBy, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotSelectedByChanged(int)));

    //
    //  1. Set model, ID by parent model
    //
    mContactModel = CRM_MODELFACTORY->getModel(CRM_ModelFactory::ModelContact);
//    mContactModel->setPrimaryParent("rm_id");
//    mContactModel->setWhere("", false); // not select
//    mContactModel->setRoot(CRM_MODELFACTORY->getRootId());
//    leSelectedBy->setText(CRM_MODELFACTORY->getRoot()->getValue("code").toString());

    //
    // 2. Set relations and mapper for line edits etc.
    //
    mContactMapper = mContactModel->getMapper();
    // create and attach fixed comboBox model, currentIndex is property of combobox
    items.clear();
    items << tr("Mr.") << tr("Ms.") << tr("Mrs.") << tr("Dr.") << tr("Prof.");
    cbSalutation->setModel(new QStringListModel(items, this));
    mContactMapper->addMapping(cbSalutation, mContactModel->fieldIndex("salutation"),
                               "currenttext");
    mContactMapper->addMapping(this->leFirstName, mContactModel->fieldIndex("firstname"));
    mContactMapper->addMapping(this->leLastName, mContactModel->fieldIndex("lastname"));
    // leadsource
    mContactMapper->addMapping(this->leTitle, mContactModel->fieldIndex("title"));
    mContactMapper->addMapping(this->leDepartment, mContactModel->fieldIndex("department"));
    // reportsto_id
    // birthday
    mContactMapper->addMapping(this->ckbDoNotCall, mContactModel->fieldIndex("donotcall"));
    // phonehome
    mContactMapper->addMapping(this->leOfficePhone, mContactModel->fieldIndex("phonework"));
    mContactMapper->addMapping(this->leMobile, mContactModel->fieldIndex("phonemobile"));
    mContactMapper->addMapping(this->leFax, mContactModel->fieldIndex("phonefax"));
    mContactMapper->addMapping(this->leEmail1, mContactModel->fieldIndex("email1"));
    mContactMapper->addMapping(this->leEmail2, mContactModel->fieldIndex("email2"));
    mContactMapper->addMapping(this->ckbOptOut, mContactModel->fieldIndex("emailoptout"));
    mContactMapper->addMapping(this->ckbInvalid, mContactModel->fieldIndex("invalidemail"));
    mContactMapper->addMapping(this->lePrimaryAddress1, mContactModel->fieldIndex("primaryaddress1"));
    mContactMapper->addMapping(this->lePrimaryAddress2, mContactModel->fieldIndex("primaryaddress2"));
    mContactMapper->addMapping(this->lePrimaryAddress3, mContactModel->fieldIndex("primaryaddress3"));
    mContactMapper->addMapping(this->lePrimaryAddress4, mContactModel->fieldIndex("primaryaddress4"));
    mContactMapper->addMapping(this->lePrimaryAddress5, mContactModel->fieldIndex("primaryaddress5"));
    mContactMapper->addMapping(this->lePrimaryAddress6, mContactModel->fieldIndex("primaryaddress6"));
    mContactMapper->addMapping(this->leOtherAddress1, mContactModel->fieldIndex("altaddress1"));
    mContactMapper->addMapping(this->leOtherAddress2, mContactModel->fieldIndex("altaddress2"));
    mContactMapper->addMapping(this->leOtherAddress3, mContactModel->fieldIndex("altaddress3"));
    mContactMapper->addMapping(this->leOtherAddress4, mContactModel->fieldIndex("altaddress4"));
    mContactMapper->addMapping(this->leOtherAddress5, mContactModel->fieldIndex("altaddress5"));
    mContactMapper->addMapping(this->leOtherAddress6, mContactModel->fieldIndex("altaddress6"));
    mContactMapper->addMapping(this->pteDescription, mContactModel->fieldIndex("description"));

    //
    // 3. Select after relations have been set, initiated by parent model
    //
//    mContactModel->select();
    // mModel->setHeaderData(RB2::HIDDENCOLUMNS, Qt::Horizontal, tr("Rutger"));
    cbSelectedBy->setCurrentIndex(1); // account, triggers slotSelectedByChanged(int)

    //
    // 4. Connect model to main view
    //
    setFormatTableView(tvContact, mContactModel);
    readSettings();
    pteDescription->setTabChangesFocus(true);

    int count = mContactModel->columnCount(QModelIndex());

    for (int i = 0; i < count; ++i) {
        if (i != mContactModel->fieldIndex("firstname")
                && i != mContactModel->fieldIndex("lastname")
                && i != mContactModel->fieldIndex("title")
                && i != mContactModel->fieldIndex("email1")
                && i != mContactModel->fieldIndex("phonework")) {
            tvContact->hideColumn(i);
        } else {
            tvContact->showColumn(i);

            if (tvContact->columnWidth(i) < 5) {
                tvContact->setColumnWidth(i, 100);
            }
        }
    }

    // Detail model
    mContactDetModel = CRM_MODELFACTORY->getModel(CRM_ModelFactory::ModelContactDetail); // shared
    mContactDetMapper = mContactDetModel->getMapper();

    addComboBoxMapping(mContactDetModel,"leadsource_id", "CRM_LeadSourceType",
                       "id", "mname", cbLeadSource, mContactDetMapper);
    mContactDetMapper->addMapping(dteBirthday, mContactDetModel->fieldIndex("birthday"));
    mContactDetMapper->addMapping(leSpousName, mContactDetModel->fieldIndex("spousname"));
    mContactDetMapper->addMapping(leChildrenNames, mContactDetModel->fieldIndex("childrennames"));
    mContactDetMapper->addMapping(leHobbies, mContactDetModel->fieldIndex("hobbies"));
    mContactDetMapper->addMapping(pteBackground, mContactDetModel->fieldIndex("background"));

    // Add row if not exists
    connect(mContactModel, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(slotParentRowChanged(QModelIndex,QModelIndex)));
    // Because mCustDetModel has no setFormatTableView()
    connect(mContactDetModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            this, SLOT(slotDataIsChanged(QModelIndex, QModelIndex)));

    mIsInitialized = true;
}

/**
 * Save data, can be called directly or indirectly via closeEvent()
 * and maybeSave().
 * @param withSelect true if select() is to be done after submitAll()
 */
bool CRM_ContactWidget::fileSave(bool withSelect) {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    beforeFileSave();
    if (withSelect && mContactModel) {
        if (mContactModel->submitAllAndSelect()) {

            setWindowModified(false);
            QApplication::restoreOverrideCursor();
            return true;
        }
    } else if (mContactModel) {
        if (mContactModel->submitAll()) {
            setWindowModified(false);
            QApplication::restoreOverrideCursor();
            return true;
        }
    }
    QApplication::restoreOverrideCursor();
    return false;
}

/**
 * Reverse all changes
 */
void CRM_ContactWidget::fileRevert() {
    mContactModel->revertAll();
    setWindowModified(false);
}

/**
 * Button add clicked
 */
void CRM_ContactWidget::on_pbAddContact_clicked() {
    if (!mContactModel) return;
    mContactModel->undoFilterSort();

    // always insert at the begin
    int row = 0; // mContactModel->rowCount();
    mContactModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex idx;

    // mCustomerModel is set to NULL in case of selection by ALL or Account
    if (mCustomerModel) {
        idx = mContactModel->index(row, mContactModel->fieldIndex("parent"));
        mContactModel->setData(idx, mCustomerModel->getCurrentId(), Qt::EditRole);
    } else {
        idx = mContactModel->index(row, mContactModel->fieldIndex("parent"));
        mContactModel->setData(idx, "0", Qt::EditRole);
    }

    idx = mContactModel->index(row, mContactModel->fieldIndex("salutation"));
    mContactModel->setData(idx, cbSalutation->itemText(0), Qt::EditRole);
    idx = mContactModel->index(row, mContactModel->fieldIndex("rm_id"));
    mContactModel->setData(idx, CRM_MODELFACTORY->getRootId(), Qt::EditRole);
    // end NOTE

    tvContact->setCurrentIndex(mContactModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
    tvContact->scrollTo(tvContact->currentIndex());
    tabWidget->setCurrentIndex(0);
    cbSalutation->setFocus();
}

/**
 * Button delete clicked
 */
void CRM_ContactWidget::on_pbDeleteContact_clicked() {
    int ret = DB_DIALOGFACTORY->requestYesNoDialog(
            tr("Delete contact ..."),
            tr("This action cannot be undone.\n"
               "Do you want to delete this contact?"));
    if (ret == QMessageBox::No) {
        DB_DIALOGFACTORY->statusBarMessage(tr("Deletion canceled"), 2000);
        return;
    }

    QModelIndex index = tvContact->currentIndex();
    if (!index.isValid()) return;

    mContactModel->removeRows(index.row(), 1, QModelIndex());
    fileSave(true);
}

/**
 * Button save clicked
 */
void CRM_ContactWidget::on_pbSave_clicked() {
    fileSave(true);
}

void CRM_ContactWidget::on_pbFilterOn_clicked() {
    RB_String str = leFilter->text();
    if (str.isEmpty()) {
        return;
    }
    if (!str.endsWith("%")) {
        str += "%";
    }
    RB_String filter = "`acc_contact`.`lastname` LIKE '";
    filter += str + "'";
    mContactModel->setWhere(filter, false);
    mContactModel->setRoot("");
    mContactModel->select();
    leSelectedBy->setText(tr("Filter"));

    mCustomerModel = NULL;
}

void CRM_ContactWidget::on_pbFilterOff_clicked() {
    slotSelectedByChanged(cbSelectedBy->currentIndex());
}

/**
 * Select by changed
 */
void CRM_ContactWidget::slotSelectedByChanged(int index) {
    QApplication::setOverrideCursor(Qt::WaitCursor);

    switch (index) {
        case 0 : {
            if (!mCustomerModel) {
                QApplication::restoreOverrideCursor();
                CRM_DIALOGFACTORY->requestInformationDialog(tr("The customer window is opened\n"
                                                               "to select a customer"));
                QApplication::setOverrideCursor(Qt::WaitCursor);
                CRM_ActionCustomer a;
                a.trigger(); // create or only raise the MDI window with model

                mCustomerModel = CRM_MODELFACTORY->getModel(CRM_ModelFactory::ModelCustomer);
                connect(mCustomerModel, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
                        this, SLOT(slotCustomerRowChanged(QModelIndex,QModelIndex)));
            }
            mContactModel->setParentManager(mCustomerModel); // basic change row

            fileSave(false);
            mContactModel->setPrimaryParent("parent");
            mContactModel->setWhere("", false); // not select
            mContactModel->setRoot(mCustomerModel->getCurrentId());
            mContactModel->select();

            RB_String str = mCustomerModel->getCurrentValue("customerno").toString();
            leSelectedBy->setText(str);
            leFilter->setText("");

            break;
        }
        case 1 : {
            if (mCustomerModel) {
                disconnect(mCustomerModel, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
                           this, SLOT(slotCustomerRowChanged(QModelIndex,QModelIndex)));
                mCustomerModel = NULL;
            }
            if (mIsInitialized) {
                fileSave(false);
            }
            mContactModel->setPrimaryParent("rm_id");
            mContactModel->setWhere("", false); // not select
            mContactModel->setRoot(CRM_MODELFACTORY->getRootId());
            mContactModel->select();

            RB_ObjectBase* obj = CRM_MODELFACTORY->getRoot();
            leSelectedBy->setText(obj->getValue("code").toString());
            leFilter->setText("");

            break;
        }
        case 2 : {
            if (mCustomerModel) {
                disconnect(mCustomerModel, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
                           this, SLOT(slotCustomerRowChanged(QModelIndex,QModelIndex)));
                mCustomerModel = NULL;
            }
            fileSave(false);
            // mContactModel->setPrimaryParent("parent");
            mContactModel->setWhere("`acc_contact`.`id`<>'0'", false); // not select
            mContactModel->setRoot("");
            mContactModel->select();
            leSelectedBy->setText("");
            leFilter->setText("");

            break;
        }
        default :
            break;
    }
    QApplication::restoreOverrideCursor();
}

/**
 * Push button send email clicked
 */
void CRM_ContactWidget::on_pbSendEmail1_clicked() {
    RB_String email = leEmail1->text();
    if (email.isEmpty()) {
        return;
    }
    if (ckbOptOut->isChecked() || ckbInvalid->isChecked()) {
        CRM_DIALOGFACTORY->requestWarningDialog(tr("Email is opted-out or invalid"));
        return;
    }
    RB_String subjectBody = "?subject=<Subject>&body=" + tr("Dear");
    subjectBody += " " + cbSalutation->currentText();
    subjectBody += " " + leLastName->text() + ",";
    QDesktopServices::openUrl(QUrl("mailto:" + email + subjectBody));
}

/**
 * Push button send email clicked
 */
void CRM_ContactWidget::on_pbSendEmail2_clicked() {
    RB_String email = leEmail2->text();
    if (email.isEmpty()) {
        return;
    }
    if (ckbOptOut->isChecked() || ckbInvalid->isChecked()) {
        CRM_DIALOGFACTORY->requestWarningDialog(tr("Email is opted-out or invalid"));
        return;
    }
    RB_String subjectBody = "?subject=<Subject>&body=" + tr("Dear");
    subjectBody += " " + cbSalutation->currentText();
    subjectBody += " " + leLastName->text() + ",";
    QDesktopServices::openUrl(QUrl("mailto:" + email + subjectBody));
}


/**
 * Push button copy parent clicked
 */
void CRM_ContactWidget::on_pbCopyParent_clicked() {

    if (!mContactModel->getProxyIndex().isValid()) {
        CRM_DIALOGFACTORY->requestWarningDialog(tr("No item selected.\n"
                                                   "Please select a contact first."));
        return;
    }

    // Check for valid customer
    if (!mCustomerModel || !mCustomerModel->getProxyIndex().isValid()) {
        CRM_DIALOGFACTORY->requestWarningDialog(tr("No customer selected.\n"
                                                   "Please select a customer first."));
        return;
    }

    int res = CRM_DIALOGFACTORY->requestYesNoDialog(tr("Copy data"),
                                  tr("The data will be copied to this item.\n"
                                     "Do you want to replace the current data?"));
    if (res != QMessageBox::Yes) return;

    RB_MmProxy* m = mContactModel;
    QModelIndex cIdx = m->getProxyIndex();
    int row = cIdx.row();
    QModelIndex pIdx = QModelIndex(); // m->parent(cIdx); cIdx.parent();

    RB_MmProxy* custM = mCustomerModel;
    QModelIndex custIdx = custM->getProxyIndex();
    int custRow = custIdx.row();
    QModelIndex custParentIdx = custM->parent(custIdx); // custIdx.parent();

    QModelIndex idx; // customer branch index
    idx = custM->index(custRow, custM->fieldIndex("brpostaddress1"), custParentIdx);
    cIdx = m->index(row, m->fieldIndex("primaryaddress1"), pIdx);
    m->setData(cIdx, custM->data(idx));
    idx = custM->index(custRow, custM->fieldIndex("brpostaddress2"), custParentIdx);
    cIdx = m->index(row, m->fieldIndex("primaryaddress2"), pIdx);
    m->setData(cIdx, custM->data(idx));
    idx = custM->index(custRow, custM->fieldIndex("brpostaddress3"), custParentIdx);
    cIdx = m->index(row, m->fieldIndex("primaryaddress3"), pIdx);
    m->setData(cIdx, custM->data(idx));
    idx = custM->index(custRow, custM->fieldIndex("brpostaddress4"), custParentIdx);
    cIdx = m->index(row, m->fieldIndex("primaryaddress4"), pIdx);
    m->setData(cIdx, custM->data(idx));
    idx = custM->index(custRow, custM->fieldIndex("brpostaddress5"), custParentIdx);
    cIdx = m->index(row, m->fieldIndex("primaryaddress5"), pIdx);
    m->setData(cIdx, custM->data(idx));
    idx = custM->index(custRow, custM->fieldIndex("brpostaddress6"), custParentIdx);
    cIdx = m->index(row, m->fieldIndex("primaryaddress6"), pIdx);
    m->setData(cIdx, custM->data(idx));

    idx = custM->index(custRow, custM->fieldIndex("brstreetaddress1"), custParentIdx);
    cIdx = m->index(row, m->fieldIndex("altaddress1"), pIdx);
    m->setData(cIdx, custM->data(idx));
    idx = custM->index(custRow, custM->fieldIndex("brstreetaddress2"), custParentIdx);
    cIdx = m->index(row, m->fieldIndex("altaddress2"), pIdx);
    m->setData(cIdx, custM->data(idx));
    idx = custM->index(custRow, custM->fieldIndex("brstreetaddress3"), custParentIdx);
    cIdx = m->index(row, m->fieldIndex("altaddress3"), pIdx);
    m->setData(cIdx, custM->data(idx));
    idx = custM->index(custRow, custM->fieldIndex("brstreetaddress4"), custParentIdx);
    cIdx = m->index(row, m->fieldIndex("altaddress4"), pIdx);
    m->setData(cIdx, custM->data(idx));
    idx = custM->index(custRow, custM->fieldIndex("brstreetaddress5"), custParentIdx);
    cIdx = m->index(row, m->fieldIndex("altaddress5"), pIdx);
    m->setData(cIdx, custM->data(idx));
    idx = custM->index(custRow, custM->fieldIndex("brstreetaddress6"), custParentIdx);
    cIdx = m->index(row, m->fieldIndex("altaddress6"), pIdx);
    m->setData(cIdx, custM->data(idx));
}

/**
 * Push button copy postal to visit address clicked
 */
void CRM_ContactWidget::on_pbCopyPrimaryAddress_clicked() {
    if (!mContactModel->getProxyIndex().isValid()) {
        CRM_DIALOGFACTORY->requestWarningDialog(tr("No branch selected.\n"
                                                   "Please select a customer/branch first."));
        return;
    }

    int res = CRM_DIALOGFACTORY->requestYesNoDialog(tr("Copy data"),
                                  tr("The data will be copied to this item.\n"
                                     "Do you want to replace the current data?"));
    if (res != QMessageBox::Yes) return;

    RB_MmProxy* m = mContactModel;
    QModelIndex cIdx = m->getProxyIndex();
    int row = cIdx.row();
    QModelIndex pIdx = cIdx.parent();

    QModelIndex idx;
    idx = m->index(row, m->fieldIndex("altaddress1"), pIdx);
    m->setData(idx, lePrimaryAddress1->text(), Qt::EditRole);
    idx = m->index(row, m->fieldIndex("altaddress2"), pIdx);
    m->setData(idx, lePrimaryAddress2->text(), Qt::EditRole);
    idx = m->index(row, m->fieldIndex("altaddress3"), pIdx);
    m->setData(idx, lePrimaryAddress3->text(), Qt::EditRole);
    idx = m->index(row, m->fieldIndex("altaddress4"), pIdx);
    m->setData(idx, lePrimaryAddress4->text(), Qt::EditRole);
    idx = m->index(row, m->fieldIndex("altaddress5"), pIdx);
    m->setData(idx, lePrimaryAddress5->text(), Qt::EditRole);
    idx = m->index(row, m->fieldIndex("altaddress6"), pIdx);
    m->setData(idx, lePrimaryAddress6->text(), Qt::EditRole);
}

/**
 * Button select customer clicked. Selects or changes the (parent)
 * customer for the current contact.
 */
void CRM_ContactWidget::on_pbSelectCustomer_clicked() {
    if (!mContactModel || !mContactModel->getProxyIndex().isValid()) {
        CRM_DIALOGFACTORY->requestWarningDialog(tr("No item selected.\n"
                                                   "Please select an item first."));
        return;
    }

    RB_Dialog* dlg = CRM_DIALOGFACTORY->getDialog(CRM_DialogFactory::DialogSelectCustomer);

    if (dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();
        if (obj) {
            mContactModel->setCurrentValue("parent", obj->getId(), Qt::EditRole);
        }
    } else {
        CRM_DIALOGFACTORY->requestWarningDialog(tr("No account selected.\n"
                                                   "Data is unchanged."));
    }

    dlg->deleteLater();
}

/**
 * Slot parent (customer) model row changed, updates leSelectedBy
 * @param current current index
 * @param previous previous index
 */
void CRM_ContactWidget::slotCustomerRowChanged(const QModelIndex& current,
                                               const QModelIndex& /* previous */) {
    if (!mCustomerModel) return;
    QModelIndex idx = mCustomerModel->index(current.row(),
                                            mCustomerModel->fieldIndex("customerno"));
    RB_String str = mCustomerModel->data(idx).toString();
    leSelectedBy->setText(str);
}

/**
 * Parent row changed in ACC_Customer. Timer
 * will fire single shot after 200 milliseconds to activate
 * slotHandleParentRowChanged(). This delay is set to prevent many SQL to
 * get from related table(s) or to create
 * rows that are only browsed very quickly by the user.
 *
 * Add row in related model(s) (CRM_CustomerDetail) if not exists and
 * set QDataWidgetMapper to relevant index.
 *
 * @param curr current model index not used
 * @param prev previous model index not used
 */
void CRM_ContactWidget::slotParentRowChanged(const QModelIndex& /*curr*/,
                                             const QModelIndex& /*prev*/) {
    mCurrentId = mContactModel->getCurrentId();
    QTimer::singleShot(200, this, SLOT(slotHandleParentRowChanged()));
}

/**
 * Add row in related model(s) (CRM_ContactDetail) if not exists. The
 * current ID needs to be the same as the set ID at slotAddRowIfNotExists().
 * Otherwise the user has apparently already selected a different row.
 */
void CRM_ContactWidget::slotHandleParentRowChanged() {
    if (mCurrentId != mContactModel->getCurrentId()) {
        // User already selected another row
        return;
    }

    QModelIndex idx;

    if (mContactDetModel->rowCount() > 0) {
        // Row already exists in model
        idx = mContactDetModel->index(0, 0);
        mContactDetMapper->setCurrentModelIndex(idx);
        return;
    }

    // always insert at the begin
    int row = 0; // mCustModel->rowCount();
    mContactDetModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    idx = mContactDetModel->index(row, mContactDetModel->fieldIndex("leadsource_id"));
    mContactDetModel->setData(idx, "0", Qt::EditRole);
    // end NOTE

    mContactDetModel->submitAll();
    idx = mContactDetModel->index(0, 0);
    mContactDetMapper->setCurrentModelIndex(idx);
}


/**
 * @returns the help subject, in this case the widget objectname
 */
RB_String CRM_ContactWidget::getHelpSubject() const {
    return objectName();
}

/**
 * Override RB_Widget::closeWidget() to save changes
 */
bool CRM_ContactWidget::closeWidget() {
    if (maybeSave()) {
        return true;
    }
    return false;
}

/**
 * Change event, for example translation
 */
void CRM_ContactWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
