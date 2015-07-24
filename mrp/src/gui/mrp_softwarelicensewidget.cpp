/*****************************************************************
 * $Id: sail_mapnavigationwidget.cpp 1729 2012-08-08 20:48:52Z rutger $
 * Created: Jul 26, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "mrp_softwarelicensewidget.h"

#include "db_actionfactory.h"
#include "db_dialogfactory.h"
#include "mrp_dialogfactory.h"
#include "mrp_modelfactory.h"
#include "mrp_selectcontactdialog.h"
#include "mrp_selectorderdialog.h"
#include "rb_datawidgetmapper.h"
#include "rb_mdiwindow.h"
#include "rb_sqlrelationaldelegate.h"


/**
 * Constructor
 */
MRP_SoftwareLicenseWidget::MRP_SoftwareLicenseWidget(QWidget* parent)
                                    : RB_Widget(parent) {
    setupUi(this);

    mModel = NULL;
    mMapper = NULL;
}

/**
 * Destructor
 */
MRP_SoftwareLicenseWidget::~MRP_SoftwareLicenseWidget() {
    delete mModel;
    RB_DEBUG->print("MRP_SoftwareLicenseWidget::~MRP_SoftwareLicenseWidget() OK");
}

/**
 * Initialize widget and models
 */
void MRP_SoftwareLicenseWidget::init() {
    //
    // 1. Set model and mapper
    //
    mModel = MRP_MODELFACTORY->getModel(MRP_ModelFactory::ModelSoftwareLicense);
    mModel->setRoot(MRP_MODELFACTORY->getRootId());

    //
    // 2. Get mapper for line edits etc.
    //
    mMapper = mModel->getMapper();
    mMapper->addMapping(this->leLicenseCode, mModel->fieldIndex("licensecode"));
    mMapper->addMapping(deStartDate, mModel->fieldIndex("startdate"));
    mMapper->addMapping(deEndDate, mModel->fieldIndex("enddate"));
    mMapper->addMapping(ileCustomer, mModel->fieldIndex("customer_idx"));
    mMapper->addMapping(ileSalesOrder, mModel->fieldIndex("salesorder_idx"));
    mMapper->addMapping(ileContact, mModel->fieldIndex("contact_idx"));
    mMapper->addMapping(leComment, mModel->fieldIndex("comment"));

    //
    // 3. Select after relations have been set, only for database models
    //
    mModel->select();

    //
    // 4. Connect model to main view
    //
    formatTableView(tvSoftwareLicense, mModel);

    //
    // 5. Set toolbuttonbar
    //
    tbbSoftwareLicense->initEdit(false, true, true);
    tvSoftwareLicense->setToolButtonBar(tbbSoftwareLicense);
    connect(tbbSoftwareLicense, SIGNAL(addClicked()),
            this, SLOT(slotPbAddClicked()));

    //
    // 6. Set visible columns in tableView
    //
    readSettings();
    int count = mModel->columnCount(QModelIndex());

    for (int i = 0; i < count; ++i) {
        if (i < RB2::HIDDENCOLUMNS) {
            tvSoftwareLicense->hideColumn(i);
        } else {
            tvSoftwareLicense->showColumn(i);

            if (tvSoftwareLicense->columnWidth(i) < 5) {
                tvSoftwareLicense->setColumnWidth(i, 100);
            }
        }
    }
}

/**
 * Save data to database
 */
bool MRP_SoftwareLicenseWidget::fileSave(bool withSelect) {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    // HACK to emit editingFinished() for current QLineEdit:
    beforeFileSave();

    if (withSelect && mModel) {
        mModel->submitAllAndSelect();
        setWindowModified(false);
        QApplication::restoreOverrideCursor();
        return true;
    } else if (mModel) {
        mModel->submitAll();
        setWindowModified(false);
        QApplication::restoreOverrideCursor();
        return true;
    }
    QApplication::restoreOverrideCursor();
    return false;
}

/**
 * Button add track clicked
 */
void MRP_SoftwareLicenseWidget::slotPbAddClicked() {
    QDate date = QDate::currentDate();

    QModelIndex idx = mModel->getCurrentIndex();
    int row = idx.row();
    idx = mModel->index(row, mModel->fieldIndex("startdate"));
    mModel->setData(idx, date);
    date = date.addMonths(1);
    idx = mModel->index(row, mModel->fieldIndex("enddate"));
    mModel->setData(idx, date);

    setWindowModified(true);
    leLicenseCode->setFocus();
    leLicenseCode->selectAll();
}

/**
 * Button select customer clicked
 */
void MRP_SoftwareLicenseWidget::on_ileCustomer_clicked() {
    QModelIndex index = tvSoftwareLicense->currentIndex();

    if (!index.isValid()) {
        MRP_DIALOGFACTORY->requestWarningDialog(tr("No license selected.\n"
                                                   "Please select a license first."));
        return;
    }

    RB_Dialog* dlg = MRP_DIALOGFACTORY->getDialog(MRP_DialogFactory::DialogSelectCustomer);

    if (dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();
        if (obj) {
            int row = index.row();
            index = mModel->index(row, mModel->fieldIndex("Customer_idx"));
            mModel->setData(index, obj->getId()
                            + obj->getValue("mname").toString(), Qt::EditRole);
        }
    } else {
        MRP_DIALOGFACTORY->requestWarningDialog(tr("No customer selected.\n"
                                                   "Customer is unchanged."));
    }

    dlg->deleteLater();
}

/**
 * Button select sales order clicked, only relevant if customer is selected
 */
void MRP_SoftwareLicenseWidget::on_ileSalesOrder_clicked() {
    QModelIndex index = tvSoftwareLicense->currentIndex();

    if (!index.isValid()) {
        MRP_DIALOGFACTORY->requestWarningDialog(tr("No license selected.\n"
                                                   "Please select a license first."));
        return;
    }

    RB_String custId = mModel->getCurrentValue("customer_idx", RB2::RoleOrigData).toString();

    if (custId.count() < 38) {
        MRP_DIALOGFACTORY->requestWarningDialog(tr("Not a valid customer selected.\n"
                                                   "Please select a customer first."));
        return;
    }

    RB_Dialog* dlg = MRP_DIALOGFACTORY->getDialog(MRP_DialogFactory::DialogSelectOrder);
    MRP_SelectOrderDialog* orderDlg = dynamic_cast<MRP_SelectOrderDialog*>(dlg);
    orderDlg->setParentId(custId);

    if (orderDlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = orderDlg->getCurrentObject();
        if (obj) {
            int row = index.row();
            index = mModel->index(row, mModel->fieldIndex("salesorder_idx"));
            mModel->setData(index, obj->getId()
                            + obj->getValue("orderno").toString(), Qt::EditRole);
        }
    } else {
        MRP_DIALOGFACTORY->requestWarningDialog(tr("No sales order selected.\n"
                                                   "Sales order is unchanged."));
    }

    dlg->deleteLater();
}

/**
 * Button select 'To' coordinate clicked
 */
void MRP_SoftwareLicenseWidget::on_ileContact_clicked() {
    QModelIndex index = tvSoftwareLicense->currentIndex();

    if (!index.isValid()) {
        MRP_DIALOGFACTORY->requestWarningDialog(tr("No license selected.\n"
                                                   "Please select a license first."));
        return;
    }

    RB_String custId = mModel->getCurrentValue("customer_idx", RB2::RoleOrigData).toString();

    if (custId.count() < 38) {
        MRP_DIALOGFACTORY->requestWarningDialog(tr("Not a valid customer selected.\n"
                                                   "Please select a customer first."));
        return;
    }

    RB_Dialog* dlg = MRP_DIALOGFACTORY->getDialog(MRP_DialogFactory::DialogSelectContact);
    MRP_SelectContactDialog* contactDlg = dynamic_cast<MRP_SelectContactDialog*>(dlg);
    contactDlg->setParentId(custId);

    if (contactDlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = contactDlg->getCurrentObject();
        if (obj) {
            int row = index.row();
            index = mModel->index(row, mModel->fieldIndex("contact_idx"));
            mModel->setData(index, obj->getId()
                            + obj->getValue("lastname").toString() + ", "
                            + obj->getValue("firstname").toString(), Qt::EditRole);
        }
    } else {
        MRP_DIALOGFACTORY->requestWarningDialog(tr("No contact selected.\n"
                                                   "Contact is unchanged."));
    }

    dlg->deleteLater();
}

/**
 * Get help subject
 * @return help subject (name of widget)
 */
RB_String MRP_SoftwareLicenseWidget::getHelpSubject() const {
    return objectName();
}

/**
 * Close widget
 */
bool MRP_SoftwareLicenseWidget::closeWidget() {
    RB_Widget::closeWidget();
    if (maybeSave()) {
        return true;
    }
    return false;
}

/**
 * Change event, for example translation
 */
void MRP_SoftwareLicenseWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

