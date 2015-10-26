/*****************************************************************
 * $Id: crm_customerwidget.cpp 2241 2015-05-22 10:22:19Z rutger $
 * Created: Feb 18, 2012 3:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "crm_customerwidget.h"

#include <QStringListModel>
#include "crm_dialogfactory.h"
#include "crm_modelfactory.h"
#include "crm_selectprojectdialog.h"
#include "db_dialogfactory.h"
#include "db_internetbrowserfactory.h"
#include "db_internetbrowserwidget.h"
#include "rb_datawidgetmapper.h"
#include "rb_mdiwindow.h"
#include "rb_sqlrelationaldelegate.h"

/**
 * Constructor
 */
CRM_CustomerWidget::CRM_CustomerWidget(QWidget *parent)
        : RB_Widget(parent) {

    setupUi(this);

    mCustModel = NULL;
    mCustMapper = NULL;
    mCustDetModel = NULL;
    mCustDetMapper = NULL;

    mCurrentId = "";
}

/**
 * Destructor
 */
CRM_CustomerWidget::~CRM_CustomerWidget() {
    delete mCustModel;
    delete mCustDetModel;
    RB_DEBUG->print("CRM_CustomerWidget::~CRM_CustomerWidget() OK");
}

/**
 * Initialize widget and models
 */
void CRM_CustomerWidget::init() {
    //
    // 1. Set model for customer mapper
    //
    mCustModel = CRM_MODELFACTORY->getModel(CRM_ModelFactory::ModelCustomer); // shared
    mCustModel->setRoot(CRM_MODELFACTORY->getRootId());

    //
    // 2. Get mapper for line edits etc.
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
    mCustMapper->addMapping(this->lePhoneNumber, mCustModel->fieldIndex("phoneno"));
    mCustMapper->addMapping(this->leFaxNumber, mCustModel->fieldIndex("faxno"));
    mCustMapper->addMapping(this->leBranchContact, mCustModel->fieldIndex("contactname"));
    mCustMapper->addMapping(this->leEmail, mCustModel->fieldIndex("email"));

    addComboBoxMapping(mCustModel, "db_systemuser_id", "DB_SystemUser", "id", "username",
                       cbInternalAccountHolder, mCustMapper);
    QStringList items;
    items.clear();
    items << tr("No") << tr("Yes");
    cbExistingCustomer->setModel(new QStringListModel(items, this));
    mCustMapper->addMapping(cbExistingCustomer, mCustModel->fieldIndex("crm_type_id"),
                            "currentIndex");

    //
    // 3. Select after relations have been set, only for database models
    //
    mCustModel->select();
//    mCustModel->setHeaderData(0, Qt::Horizontal, tr("Code"));
//    mCustModel->setHeaderData(1, Qt::Horizontal, tr("Name"));

    //
    // 4. Connect model to main view
    //
    readSettings();
    setFormatTableView(tvCustomer, mCustModel);

    int count = mCustModel->columnCount(QModelIndex());

    for (int i = 0; i < count; ++i) {
        if (i != RB2::HIDDENCOLUMNS && i != RB2::HIDDENCOLUMNS+1
                && i != mCustModel->fieldIndex("contactname")
                && i != mCustModel->fieldIndex("phoneno")
                && i != mCustModel->fieldIndex("email")) {
            tvCustomer->hideColumn(i);
        } else {
            tvCustomer->showColumn(i);

            if (tvCustomer->columnWidth(i) < 5) {
                tvCustomer->setColumnWidth(i, 100);
            }
        }
    }

    // Detail model
    mCustDetModel = CRM_MODELFACTORY->getModel(CRM_ModelFactory::ModelCustomerDetail); // shared
    mCustDetMapper = mCustDetModel->getMapper();

    items.clear();
    items << "Other" << "Analyst" << "Competitor" << "Customer" << "Integrator"
          << "Investor" << "Partner" << "Press" << "Prospect" << "Reseller";
    cbType->setModel(new QStringListModel(items, this));
    mCustDetMapper->addMapping(cbType, mCustDetModel->fieldIndex("customertype_id"),
                               "currentIndex");
    mCustDetMapper->addMapping(this->leOwnership, mCustDetModel->fieldIndex("ownership"));
    mCustDetMapper->addMapping(this->leClassificationCode, mCustDetModel->fieldIndex("classifcode"));

    items.clear();
    items  << "Other" << "Apparel" << "Banking" << "Biotechnology" << "Chemicals"
          << "Communications" << "Construction" << "Consulting"
          << "Education" << "Electronics" << "Energy" << "Engineering"
          << "Entertainment" << "Environmental" << "Finance"
          << "Government" << "Healthcare" << "Hospitality" << "Insurance"
          << "Machinery" << "Manufacturing" << "Media" << "Not For Profit"
          << "Recreation" << "Retail" << "Shipping" << "Technology"
          << "Telecommunications" << "Transportation" << "Utilities";
    cbIndustry->setModel(new QStringListModel(items, this));
    mCustDetMapper->addMapping(cbIndustry, mCustDetModel->fieldIndex("industrytype_id"),
                               "currentIndex");
    mCustDetMapper->addMapping(this->leNoEmployees, mCustDetModel->fieldIndex("employeecount"));
    mCustDetMapper->addMapping(this->leAnnualRevenue, mCustDetModel->fieldIndex("annualrevenue"));
    mCustDetMapper->addMapping(this->leRating, mCustDetModel->fieldIndex("rating"));

    // Add row if not exists
    connect(mCustModel, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(slotParentRowChanged(QModelIndex,QModelIndex)));
    // Because mCustDetModel has no setFormatTableView()
    connect(mCustDetModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            this, SLOT(slotDataIsChanged(QModelIndex, QModelIndex)));
}

/**
 * Save data, can be called directly or indirectly via closeEvent()
 * and maybeSave().
 * @param withSelect true if select() is to be done after submitAll()
 */
bool CRM_CustomerWidget::fileSave(bool withSelect) {
    // QApplication::setOverrideCursor(Qt::WaitCursor);
    // HACK to emit editingFinished() for current QLineEdit:
    beforeFileSave();

    if (withSelect && mCustModel && mCustDetModel) {
        if (mCustModel->submitAllAndSelect()
                && mCustDetModel->submitAllAndSelect()) {

            setWindowModified(false);
            return true;
        }
    } else if (mCustModel && mCustDetModel) {
        if (mCustModel->submitAll()
                && mCustDetModel->submitAll()) {
            setWindowModified(false);
            return true;
        }
    }
    // QApplication::restoreOverrideCursor();
    return false;
}

/**
 * Button add clicked
 */
void CRM_CustomerWidget::on_pbAdd_clicked() {
    if (!mCustModel) return;
    mCustModel->undoFilterSort();

    // always insert at the begin
    int row = 0; // mCustModel->rowCount();
    mCustModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex index;
    index = mCustModel->index(row, mCustModel->fieldIndex("crm_type_id"));
    mCustModel->setData(index, 0, Qt::EditRole); // 0 = potential ACC customer
    index = mCustModel->index(row, mCustModel->fieldIndex("db_systemuser_id"));
    mCustModel->setData(index, "0", Qt::EditRole);
    // end NOTE

    // add related row
    mCurrentId = mCustModel->getCurrentId();
    slotHandleParentRowChanged();

    tvCustomer->setCurrentIndex(mCustModel->index(row, RB2::HIDDENCOLUMNS));
    tvCustomer->scrollTo(tvCustomer->currentIndex());
    tabWidget->setCurrentIndex(0);
    leCustomerCode->setFocus();
    leCustomerCode->selectAll();
}

/**
 * Button delete clicked
 */
void CRM_CustomerWidget::on_pbDelete_clicked() {
    int ret = DB_DIALOGFACTORY->requestYesNoDialog(
            tr("Delete contact ..."),
            tr("This action cannot be undone.\n"
               "Do you want to delete this contact?"));
    if (ret == QMessageBox::No) {
        DB_DIALOGFACTORY->statusBarMessage(tr("Deletion canceled"), 2000);
        return;
    }

    QModelIndex index = tvCustomer->currentIndex();
    if (!index.isValid()) return;

    mCustModel->removeRows(index.row(), 1, QModelIndex());
    fileSave(true);
}

/**
 * Push button save clicked
 */
void CRM_CustomerWidget::on_pbSave_clicked() {
    fileSave(true);
}

void CRM_CustomerWidget::on_pbFilterOn_clicked() {
    RB_String str = leFilter->text();
    if (str.isEmpty()) {
        return;
    }
    if (!str.endsWith("%")) {
        str += "%";
    }
    RB_String filter = "`acc_customer`.`customerno` LIKE '";
    filter += str + "'";
    mCustModel->setWhere(filter, true);
}

void CRM_CustomerWidget::on_pbFilterOff_clicked() {
    mCustModel->setWhere("", true);
}

/**
 * Button select parent company clicked
 */
void CRM_CustomerWidget::on_ileCustomerParent_clicked() {
    if (!mCustModel->getProxyIndex().isValid()) {
        CRM_DIALOGFACTORY->requestWarningDialog(tr("No customer or branch selected.\n"
                                                   "Please select a customer/branch first."));
        return;
    }

    RB_Dialog* dlg = CRM_DIALOGFACTORY->getDialog(CRM_DialogFactory::DialogSelectCustomer);

    if (dlg && dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();
        if (obj) {
            int row = mCustModel->getProxyIndex().row();
            QModelIndex parentIdx = mCustModel->getProxyIndex().parent();
            QModelIndex idx = mCustModel->index(row, mCustModel->fieldIndex("custparent_idx"), parentIdx);
            mCustModel->setData(idx, obj->getId()
                               + obj->getValue("mname").toString(), Qt::EditRole);
        }
    } else {
        CRM_DIALOGFACTORY->requestWarningDialog(tr("No branch selected.\n"
                                                   "Customer branch is unchanged."));
    }

    dlg->deleteLater();
}

/**
 * Push button copy post address to street address clicked
 */
void CRM_CustomerWidget::on_pbCopyParent_clicked() {
    if (!mCustModel->getProxyIndex().isValid()) {
        CRM_DIALOGFACTORY->requestWarningDialog(tr("No customer or branch selected.\n"
                                                   "Please select a customer/branch first."));
        return;
    }

    int res = CRM_DIALOGFACTORY->requestYesNoDialog(tr("Copy data"),
                                  tr("The data will be copied to this item.\n"
                                     "Do you want to replace the current data?"));
    if (res != QMessageBox::Yes) return;

    RB_MmProxy* m = mCustModel;
    QModelIndex cIdx = m->getProxyIndex();
    int row = cIdx.row();
    QModelIndex pIdx = m->parent(cIdx); // cIdx.parent();

    if (!pIdx.isValid()) {
        CRM_DIALOGFACTORY->requestWarningDialog(tr("No parent company available."));
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
void CRM_CustomerWidget::on_pbCopyPostAddress_clicked() {
    if (!mCustModel->getProxyIndex().isValid()) {
        CRM_DIALOGFACTORY->requestWarningDialog(tr("No branch selected.\n"
                                                   "Please select a customer/branch first."));
        return;
    }

    int res = CRM_DIALOGFACTORY->requestYesNoDialog(tr("Copy data"),
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
void CRM_CustomerWidget::on_pbMapLocation_clicked() {
    if (!mCustModel->getProxyIndex().isValid()) {
        CRM_DIALOGFACTORY->requestWarningDialog(tr("No customer or branch selected.\n"
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
void CRM_CustomerWidget::on_pbSendEmail_clicked() {
    RB_String email = leEmail->text();
    if (email.isEmpty()) {
        return;
    }
    QDesktopServices::openUrl(QUrl("mailto:" + email + "?subject=<Subject>&body=Dear ,"));
}

void CRM_CustomerWidget::on_pbSelectCompany_clicked() {
    if (!tvCustomer->currentIndex().isValid()) {
        CRM_DIALOGFACTORY->requestWarningDialog(tr("No item selected.\n"
                                                   "Please select an item first."));
        return;
    }

    RB_Dialog* dlg = CRM_DIALOGFACTORY->getDialog(CRM_DialogFactory::DialogProject);

    if (dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();
        if (obj) {
            QModelIndex index = mCustModel->getProxyIndex();
            QModelIndex idx = mCustModel->index(
                        index.row(), mCustModel->fieldIndex("crm_parent"));
            mCustModel->setData(idx, obj->getId());
        }
    } else {
        CRM_DIALOGFACTORY->requestWarningDialog(tr("No account selected.\n"
                                                   "Data is unchanged."));
    }

    dlg->deleteLater();
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
void CRM_CustomerWidget::slotParentRowChanged(const QModelIndex& /*curr*/,
                                               const QModelIndex& /*prev*/) {
    mCurrentId = mCustModel->getCurrentId();
    QTimer::singleShot(200, this, SLOT(slotHandleParentRowChanged()));
}

/**
 * Add row in related model(s) (CRM_CustomerDetail) if not exists. The
 * current ID needs to be the same as the set ID at slotAddRowIfNotExists().
 * Otherwise the user has apparently already selected a different row.
 */
void CRM_CustomerWidget::slotHandleParentRowChanged() {
    if (mCurrentId != mCustModel->getCurrentId()) {
        // User already selected another row
        return;
    }

    QModelIndex idx;

    if (mCustDetModel->rowCount() > 0) {
        // Row already exists in model
        idx = mCustDetModel->index(0, 0);
        mCustDetModel->slotChangeCurrentRow(idx, QModelIndex());
        return;
    }

    // always insert at the begin
    int row = 0; // mCustModel->rowCount();
    mCustDetModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    idx = mCustDetModel->index(row, mCustDetModel->fieldIndex("customertype_id"));
    mCustDetModel->setData(idx, 3, Qt::EditRole);
    idx = mCustDetModel->index(row, mCustDetModel->fieldIndex("industrytype_id"));
    mCustDetModel->setData(idx, 0, Qt::EditRole);
    // end NOTE

    mCustDetModel->submitAll();
    idx = mCustDetModel->index(0, 0);
    mCustDetModel->slotChangeCurrentRow(idx, QModelIndex());
}

/**
 * @returns the help subject, in this case the widget objectname
 */
RB_String CRM_CustomerWidget::getHelpSubject() const {
    return objectName();
}

bool CRM_CustomerWidget::closeWidget() {
    RB_Widget::closeWidget();
    if (maybeSave()) {
        return true;
    }
    return false;
}

/**
 * Change event, for example translation
 */
void CRM_CustomerWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
