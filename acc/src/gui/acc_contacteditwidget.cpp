/*****************************************************************
 * $Id: acc_contacteditwidget.cpp 2186 2014-10-16 14:04:01Z rutger $
 * Created: Jan 06, 2011 4:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_contacteditwidget.h"

#include <QStringListModel>
#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "db_dialogfactory.h"
#include "rb_datawidgetmapper.h"
#include "rb_mdiwindow.h"
#include "rb_sqlrelationaldelegate.h"

/**
 * Constructor
 */
ACC_ContactEditWidget::ACC_ContactEditWidget(QWidget *parent, bool parentIsCustomer)
        : RB_Widget(parent) {

    mParentIsCustomer = parentIsCustomer;

    setupUi(this);

    mCustSuppModel = NULL;
    mContactModel = NULL;
    mContactMapper = NULL;
}

/**
 * Destructor
 */
ACC_ContactEditWidget::~ACC_ContactEditWidget() {
    // unique models are always deleted by the widgets,
    // can be MDI window or dialog, but also a dockWindow with table or tree
    // delete mContactModel; not here deleted by ACC_DebtorsMasterWidget (dockwindow)
    // If not revert() here, than destructor of model will save data
    mContactModel->revert();
    delete mContactModel;
    RB_DEBUG->print("ACC_ContactEditWidget::~ACC_ContactEditWidget() OK");
}

QString ACC_ContactEditWidget::getName() const {
    if (!mParentIsCustomer) {
        return "ACC Supplier Contact Edit";
    }
    return "ACC Customer Contact Edit";
}

/**
 * Initialize widget and models
 */
void ACC_ContactEditWidget::init() {
    // TODO: implement
    pbShowAll->hide();

    //
    // Set parent model, customer, for name mapper only
    //
    if (mParentIsCustomer) {
        mCustSuppModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelCustomer);
    } else {
        lblCustSupp->setText("Supplier");
        mCustSuppModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelSupplier);
    }

    // mapper can only be used once on a field, therefore:
    connect(mCustSuppModel, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(slotParentRowChanged(QModelIndex,QModelIndex)));
    connect(mCustSuppModel, SIGNAL(modelBeforeSubmitted()),
            this, SLOT(slotParentSubmitted()));

    //
    //  1. Set model, ID by parent model
    //
    if (mParentIsCustomer) {
        mContactModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelContactCustomer);
    } else {
        mContactModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelContactSupplier);
    }

    //
    // 2. Set relations and mapper for line edits etc.
    //
    mContactMapper = mContactModel->getMapper();
    // create and attach fixed comboBox model, currentIndex is property of combobox
    RB_StringList items;
    items << tr("Mr.") << tr("Ms.") << tr("Mrs.") << tr("Dr.") << tr("Prof.");
    cbSalutation->setModel(new QStringListModel(items, this));
    mContactMapper->addMapping(cbSalutation, mContactModel->fieldIndex("salutation"),
                               "currentText");
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
    mContactMapper->addMapping(this->leVoip, mContactModel->fieldIndex("voip"));
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
    // mModel->select();
    // mModel->setHeaderData(RB2::HIDDENCOLUMNS, Qt::Horizontal, tr("Rutger"));

    //
    // 4. Connect model to main view
    //
    setFormatTableView(tvContact, mContactModel);
    readSettings();

    // Hidden columns only for tableviews
    int colCount = mContactModel->columnCount();
    for (int i = 0; i < colCount; ++i) {
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

    // Request modelindex in case of already selected index
    RB_Widget* wgt = ACC_DIALOGFACTORY->getWidget(ACC_DialogFactory::WidgetNavigation, NULL);
    QModelIndex idx;

    if (mParentIsCustomer) {
        idx = wgt->getCurrentModelIndex(ACC_ModelFactory::ModelCustomer);
    } else {
        idx = wgt->getCurrentModelIndex(ACC_ModelFactory::ModelSupplier);
    }

    slotParentRowChanged(idx, QModelIndex()); // set customer name in lineEdit
    mContactModel->setRoot(idx);
    mContactModel->select();
}

/**
 * Save data, can be called directly or indirectly via closeEvent()
 * and maybeSave().
 * @param withSelect true if select() is to be done after submitAll()
 */
bool ACC_ContactEditWidget::fileSave(bool withSelect) {
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
void ACC_ContactEditWidget::fileRevert() {
    mContactModel->revertAll();
    leCustSupp->setText("-");
    setWindowModified(false);
}

/**
 * Button add clicked
 */
void ACC_ContactEditWidget::on_pbAddContact_clicked() {
    if (!mCustSuppModel || !mCustSuppModel->getProxyIndex().isValid()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("Not a valid customer,\n"
                                                   "select a customer first."));
        return;
    }

    if (!mContactModel) return;
    mContactModel->undoFilterSort();

    // always insert at the beginning
    int row = 0; // mContactModel->rowCount();
    mContactModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex index = mContactModel->index(row, mContactModel->fieldIndex("salutation"));
    mContactModel->setData(index, cbSalutation->itemText(0), Qt::EditRole);
    // end NOTE

    tvContact->setCurrentIndex(mContactModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
    tvContact->scrollTo(tvContact->currentIndex());
    tabWidget->setCurrentIndex(0);
    cbSalutation->setFocus();
}

/**
 * Button delete clicked
 */
void ACC_ContactEditWidget::on_pbDeleteContact_clicked() {
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
    mContactModel->submitAllAndSelect();
    setWindowModified(false);
}

/**
 * Button save clicked
 */
void ACC_ContactEditWidget::on_pbSave_clicked() {
    fileSave(true);
}

void ACC_ContactEditWidget::on_pbFind_clicked() {
    RB_String searchStr = leFind->text();
    findInTable(searchStr, tvContact);
}

void ACC_ContactEditWidget::on_pbShowAll_clicked() {
    // TODO: implement
}

/**
 * Push button send email clicked
 */
void ACC_ContactEditWidget::on_pbSendEmail1_clicked() {
    RB_String email = leEmail1->text();
    if (email.isEmpty()) {
        return;
    }
    if (ckbOptOut->isChecked() || ckbInvalid->isChecked()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("Email is opted-out or invalid"));
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
void ACC_ContactEditWidget::on_pbSendEmail2_clicked() {
    RB_String email = leEmail2->text();
    if (email.isEmpty()) {
        return;
    }
    if (ckbOptOut->isChecked() || ckbInvalid->isChecked()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("Email is opted-out or invalid"));
        return;
    }
    RB_String subjectBody = "?subject=<Subject>&body=" + tr("Dear");
    subjectBody += " " + cbSalutation->currentText();
    subjectBody += " " + leLastName->text() + ",";
    QDesktopServices::openUrl(QUrl("mailto:" + email + subjectBody));
}


/**
 * Push button copy post address to street address clicked
 */
void ACC_ContactEditWidget::on_pbCopyParent_clicked() {
    if (!mContactModel->getProxyIndex().isValid()
        || !mContactModel->getProxyIndex().isValid()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No item selected.\n"
                                                   "Please select a contact first."));
        return;
    }

    int res = ACC_DIALOGFACTORY->requestYesNoDialog(tr("Copy data"),
                                  tr("The data will be copied to this item.\n"
                                     "Do you want to replace the current data?"));
    if (res != QMessageBox::Yes) return;

    RB_MmProxy* m = mContactModel;
    QModelIndex cIdx = m->getProxyIndex();
    int row = cIdx.row();
    QModelIndex pIdx = QModelIndex(); // m->parent(cIdx); cIdx.parent();

    if (mParentIsCustomer) {
        RB_MmProxy* custM = mCustSuppModel;
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
    } else {
        // Supplier model
        RB_MmProxy* suppM = mCustSuppModel;
        QModelIndex suppIdx = suppM->getProxyIndex();
        int suppRow = suppIdx.row();
        QModelIndex suppParentIdx = suppM->parent(suppIdx); // custIdx.parent();

        QModelIndex idx; // supplier branch index
        idx = suppM->index(suppRow, suppM->fieldIndex("address1"), suppParentIdx);
        cIdx = m->index(row, m->fieldIndex("primaryaddress1"), pIdx);
        m->setData(cIdx, suppM->data(idx));
        idx = suppM->index(suppRow, suppM->fieldIndex("address2"), suppParentIdx);
        cIdx = m->index(row, m->fieldIndex("primaryaddress2"), pIdx);
        m->setData(cIdx, suppM->data(idx));
        idx = suppM->index(suppRow, suppM->fieldIndex("address3"), suppParentIdx);
        cIdx = m->index(row, m->fieldIndex("primaryaddress3"), pIdx);
        m->setData(cIdx, suppM->data(idx));
        idx = suppM->index(suppRow, suppM->fieldIndex("address4"), suppParentIdx);
        cIdx = m->index(row, m->fieldIndex("primaryaddress4"), pIdx);
        m->setData(cIdx, suppM->data(idx));
        idx = suppM->index(suppRow, suppM->fieldIndex("address5"), suppParentIdx);
        cIdx = m->index(row, m->fieldIndex("primaryaddress5"), pIdx);
        m->setData(cIdx, suppM->data(idx));
        idx = suppM->index(suppRow, suppM->fieldIndex("address6"), suppParentIdx);
        cIdx = m->index(row, m->fieldIndex("primaryaddress6"), pIdx);
        m->setData(cIdx, suppM->data(idx));

//        idx = suppM->index(suppRow, suppM->fieldIndex("address1"), suppParentIdx);
//        cIdx = m->index(row, m->fieldIndex("altaddress1"), pIdx);
//        m->setData(cIdx, suppM->data(idx));
//        idx = suppM->index(suppRow, suppM->fieldIndex("address2"), suppParentIdx);
//        cIdx = m->index(row, m->fieldIndex("altaddress2"), pIdx);
//        m->setData(cIdx, suppM->data(idx));
//        idx = suppM->index(suppRow, suppM->fieldIndex("address3"), suppParentIdx);
//        cIdx = m->index(row, m->fieldIndex("altaddress3"), pIdx);
//        m->setData(cIdx, suppM->data(idx));
//        idx = suppM->index(suppRow, suppM->fieldIndex("address4"), suppParentIdx);
//        cIdx = m->index(row, m->fieldIndex("altaddress4"), pIdx);
//        m->setData(cIdx, suppM->data(idx));
//        idx = suppM->index(suppRow, suppM->fieldIndex("address5"), suppParentIdx);
//        cIdx = m->index(row, m->fieldIndex("altaddress5"), pIdx);
//        m->setData(cIdx, suppM->data(idx));
//        idx = suppM->index(suppRow, suppM->fieldIndex("address6"), suppParentIdx);
//        cIdx = m->index(row, m->fieldIndex("altaddress6"), pIdx);
//        m->setData(cIdx, suppM->data(idx));
    }
}

/**
 * Push button copy postal to visit address clicked
 */
void ACC_ContactEditWidget::on_pbCopyPrimaryAddress_clicked() {
    if (!mContactModel->getProxyIndex().isValid()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No branch selected.\n"
                                                   "Please select a customer/branch first."));
        return;
    }

    int res = ACC_DIALOGFACTORY->requestYesNoDialog(tr("Copy data"),
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
 * Slot parent (customer) model row changed, updates leCustomer
 * @param current current index
 * @param previous previous index
 */
void ACC_ContactEditWidget::slotParentRowChanged(const QModelIndex& current,
                                                 const QModelIndex& /* previous */) {
    if (!current.isValid()) {
        leCustSupp->setText("-");
        return;
    }

    RB_String custSuppField = "mname";

    if (!mParentIsCustomer) {
        custSuppField = "suppname";
    }

    QModelIndex idx = mCustSuppModel->index(current.row(),
                                            mCustSuppModel->fieldIndex(custSuppField),
                                            current.parent());


    RB_String str = mCustSuppModel->data(idx).toString();
    leCustSupp->setText(str);
}

void ACC_ContactEditWidget::slotParentSubmitted() {
    slotParentRowChanged(QModelIndex(), QModelIndex());
}

/**
 * Button close clicked
 */
//void ACC_ContactEditWidget::on_pbClose_clicked() {
//    emit widgetClose();
//}

/**
 * @returns the help subject, in this case the widget objectname
 */
RB_String ACC_ContactEditWidget::getHelpSubject() const {
    return objectName();
}

bool ACC_ContactEditWidget::closeWidget() {
    RB_Widget::closeWidget();
    if (maybeSave()) {
        return true;
    }
    return false;
}

/**
 * Change event, for example translation
 */
void ACC_ContactEditWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
