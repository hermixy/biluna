/*****************************************************************
 * $Id: crm_activitywidget.cpp 1925 2013-04-22 12:31:48Z rutger $
 * Created: Feb 20, 2012 3:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "crm_activitywidget.h"

#include <QStringListModel>
#include "crm_actioncontact.h"
#include "crm_actioncustomer.h"
#include "crm_dialogfactory.h"
#include "crm_modelfactory.h"
#include "crm_selectprojectdialog.h"
#include "crm_sqlcommonfunctions.h"
#include "db_dialogfactory.h"
#include "db_internetbrowserfactory.h"
#include "db_internetbrowserwidget.h"
#include "rb_cmbdelegate.h"
#include "rb_datawidgetmapper.h"
#include "rb_mdiwindow.h"
#include "rb_sqlrelationaldelegate.h"

/**
 * Constructor
 */
CRM_ActivityWidget::CRM_ActivityWidget(QWidget *parent)
        : RB_Widget(parent) {

    setupUi(this);

    mActivityModel = NULL;
    mActivityMapper = NULL;
    mCustomerModel = NULL;
    mContactModel = NULL;

    mIsOpenContactDialogShowed = false;
    mIsOpenCustomerDialogShowed = false;
}

/**
 * Destructor
 */
CRM_ActivityWidget::~CRM_ActivityWidget() {
    delete mActivityModel;
    RB_DEBUG->print("CRM_ActivityWidget::~CRM_ActivityWidget() OK");
}

/**
 * Initialize widget and models
 */
void CRM_ActivityWidget::init() {
    RB_StringList items;
    items << tr("All") << tr("Active only");
    cbSelectStatus->addItems(items);
    connect(cbSelectStatus, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotSelectStatusChanged(int)));

    items.clear();
    items << tr("Contact") << tr("Customer") << tr("Account") << tr("All");
    cbSelectedBy->addItems(items);
    connect(cbSelectedBy, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotSelectedByChanged(int)));

    //
    // 1. Set model for customer mapper
    //
    mActivityModel = CRM_MODELFACTORY->getModel(CRM_ModelFactory::ModelActivity); // shared
    mActivityModel->setPrimaryParent("parent");
    mActivityModel->setRoot(CRM_MODELFACTORY->getRootId());
    leSelectedBy->setText(CRM_MODELFACTORY->getRoot()->getValue("code").toString());

    //
    // 2. Get mapper for line edits etc.
    //
    mActivityMapper = mActivityModel->getMapper();

    RB_StringList typeItems;
    typeItems << tr("Call") << tr("Case") << tr("Email") << tr("Log")
             << tr("Meeting") << tr("Note") << tr("Opportunity")
             << tr("Reminder") << tr("Solution") << tr("Task");
    mActivityModel->setTextList(mActivityModel->fieldIndex("activitytype_id"), typeItems);
    cbActivityType->setModel(new QStringListModel(typeItems, this));
    mActivityMapper->addMapping(cbActivityType, mActivityModel->fieldIndex("activitytype_id"),
                                "currentIndex");
    mActivityMapper->addMapping(leCode, mActivityModel->fieldIndex("activitycode"));
    mActivityMapper->addMapping(leSubject, mActivityModel->fieldIndex("subject"));
    mActivityMapper->addMapping(dteStart, mActivityModel->fieldIndex("mstart"));
    mActivityMapper->addMapping(dteEnd, mActivityModel->fieldIndex("mend"));
    mActivityMapper->addMapping(chbReminder, mActivityModel->fieldIndex("isreminder"));
    mActivityMapper->addMapping(leReminder, mActivityModel->fieldIndex("remindertime"));
    RB_StringList prioItems;
    prioItems << tr("None") << tr("Low") << tr("Medium") << tr("High");
    mActivityModel->setTextList(mActivityModel->fieldIndex("priority_id"), prioItems);
    cbPriority->setModel(new QStringListModel(prioItems, this));
    mActivityMapper->addMapping(cbPriority, mActivityModel->fieldIndex("priority_id"),
                                "currentIndex");
    mActivityMapper->addMapping(pteDescription, mActivityModel->fieldIndex("description"));
    RB_StringList statusItems;
    statusItems << tr("None") << tr("Not started") << tr("In progress") << tr("Pending input")
              << tr("Completed") << tr("Deferred");
    mActivityModel->setTextList(mActivityModel->fieldIndex("status_id"), statusItems);
    cbStatus->setModel(new QStringListModel(statusItems, this));
    mActivityMapper->addMapping(cbStatus, mActivityModel->fieldIndex("status_id"),
                                "currentIndex");
    mActivityMapper->addMapping(leLocation, mActivityModel->fieldIndex("location"));
    mActivityMapper->addMapping(leValue, mActivityModel->fieldIndex("mvalue"));

    // Align and validator
    leReminder->setAlignment(Qt::AlignRight);
    leReminder->setValidator(new QIntValidator(0, 999999, this));
    leValue->setAlignment(Qt::AlignRight);
    leValue->setValidator(new QDoubleValidator(0.0, 9999999999.99, 2, this));

    //
    // 3. Select after relations have been set, only for database models
    //
    mActivityModel->select();
    //    mActivityModel->setHeaderData(0, Qt::Horizontal, tr("Code"));
    //    mActivityModel->setHeaderData(1, Qt::Horizontal, tr("Name"));

    //
    // 4. Connect model to main view
    //
    setFormatTableView(tvActivity, mActivityModel);
    tvActivity->setItemDelegateForColumn(mActivityModel->fieldIndex("activitytype_id"),
                                         new RB_CmbDelegate(this, typeItems, mActivityModel));
    tvActivity->setItemDelegateForColumn(mActivityModel->fieldIndex("priority_id"),
                                         new RB_CmbDelegate(this, prioItems, mActivityModel));
    tvActivity->setItemDelegateForColumn(mActivityModel->fieldIndex("status_id"),
                                         new RB_CmbDelegate(this, statusItems, mActivityModel));

    readSettings();
    pteDescription->setTabChangesFocus(true);
    dteStart->setDisplayFormat("yyyy-MM-dd");
    dteEnd->setDisplayFormat("yyyy-MM-dd");
    connect(dteStart, SIGNAL(editingFinished()),
            this, SLOT(slotUpdateEndDateTime()));

    int count = mActivityModel->columnCount(QModelIndex());

    for (int i = 0; i < count; ++i) {
        if (i != mActivityModel->fieldIndex("activitytype_id")
                && i != mActivityModel->fieldIndex("activitycode")
                && i != mActivityModel->fieldIndex("subject")
                && i != mActivityModel->fieldIndex("mstart")
                && i != mActivityModel->fieldIndex("mend")
                && i != mActivityModel->fieldIndex("priority_id")
                && i != mActivityModel->fieldIndex("status_id")
                && i != mActivityModel->fieldIndex("mvalue")) {
            tvActivity->hideColumn(i);
        } else {
            tvActivity->showColumn(i);

            if (tvActivity->columnWidth(i) < 5) {
                tvActivity->setColumnWidth(i, 100);
            }
        }
    }

    cbSelectedBy->setCurrentIndex(2); // account

    connect(mActivityModel, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(slotChangedCurrentRow(QModelIndex,QModelIndex)));
}

/**
 * Save data, can be called directly or indirectly via closeEvent()
 * and maybeSave().
 * @param withSelect true if select() is to be done after submitAll()
 */
bool CRM_ActivityWidget::fileSave(bool withSelect) {
    // QApplication::setOverrideCursor(Qt::WaitCursor);
    // HACK to emit editingFinished() for current QLineEdit:
    beforeFileSave();

    if (withSelect && mActivityModel) {
        if (mActivityModel->submitAllAndSelect()) {
            setWindowModified(false);

            return true;
        }
    } else if (mActivityModel) {
        if (mActivityModel->submitAll()) {
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
void CRM_ActivityWidget::on_pbAdd_clicked() {
    if ((!mCustomerModel || !mCustomerModel->getCurrentIndex().isValid())
            && (!mContactModel || !mContactModel->getCurrentIndex().isValid())) {
        int ret = DB_DIALOGFACTORY->requestYesNoDialog(
                tr("Add activity ..."),
                tr("No customer or contact selected.\n"
                   "Do you want to continue adding a generic activity?"));
        if (ret == QMessageBox::No) {
            DB_DIALOGFACTORY->statusBarMessage(tr("Add activity canceled"), 2000);
            return;
        }
    }

    if (!mActivityModel) return;
    mActivityModel->undoFilterSort();

    // always insert at the begin
    int row = 0; // mActivityModel->rowCount();
    mActivityModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex idx;
    idx = mActivityModel->index(row, mActivityModel->fieldIndex("activitytype_id"));
    mActivityModel->setData(idx, 0, Qt::EditRole);
    idx = mActivityModel->index(row, mActivityModel->fieldIndex("priority_id"));
    mActivityModel->setData(idx, 0, Qt::EditRole);
    idx = mActivityModel->index(row, mActivityModel->fieldIndex("status_id"));
    mActivityModel->setData(idx, 0, Qt::EditRole);
    idx = mActivityModel->index(row, mActivityModel->fieldIndex("parent"));
    mActivityModel->setData(idx, CRM_MODELFACTORY->getRootId(), Qt::EditRole);
    idx = mActivityModel->index(row, mActivityModel->fieldIndex("mstart"));
    mActivityModel->setData(idx, QDateTime::currentDateTime(), Qt::EditRole);
    idx = mActivityModel->index(row, mActivityModel->fieldIndex("mend"));
    mActivityModel->setData(idx, QDateTime::currentDateTime(), Qt::EditRole);

    if (mContactModel) {
        idx = mActivityModel->index(row, mActivityModel->fieldIndex("acc_contact_id"));
        mActivityModel->setData(idx, mContactModel->getCurrentId(), Qt::EditRole);
        idx = mActivityModel->index(row, mActivityModel->fieldIndex("acc_customer_id"));
        mActivityModel->setData(idx, mContactModel->getCurrentValue("parent").toString(), Qt::EditRole);
    } else if (mCustomerModel) {
        idx = mActivityModel->index(row, mActivityModel->fieldIndex("acc_contact_id"));
        mActivityModel->setData(idx, "0", Qt::EditRole);
        idx = mActivityModel->index(row, mActivityModel->fieldIndex("acc_customer_id"));
        mActivityModel->setData(idx, mCustomerModel->getCurrentId(), Qt::EditRole);
    }
    // end NOTE

    tvActivity->setCurrentIndex(mActivityModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
    tvActivity->scrollTo(tvActivity->currentIndex());
    cbActivityType->setFocus();
}

/**
 * Button delete clicked
 */
void CRM_ActivityWidget::on_pbDelete_clicked() {
    int ret = DB_DIALOGFACTORY->requestYesNoDialog(
            tr("Delete activity ..."),
            tr("This action cannot be undone.\n"
               "Do you want to delete this activity?"));
    if (ret == QMessageBox::No) {
        DB_DIALOGFACTORY->statusBarMessage(tr("Deletion canceled"), 2000);
        return;
    }

    QModelIndex index = tvActivity->currentIndex();
    if (!index.isValid()) return;

    mActivityModel->removeRows(index.row(), 1, QModelIndex());
    fileSave(true);
}

/**
 * Push button save clicked
 */
void CRM_ActivityWidget::on_pbSave_clicked() {
    fileSave(true);
}

void CRM_ActivityWidget::slotSelectStatusChanged(int /*index*/) {
    slotSelectedByChanged(cbSelectedBy->currentIndex());
}

void CRM_ActivityWidget::slotSelectedByChanged(int index) {
    QApplication::setOverrideCursor(Qt::WaitCursor);

    switch (index) {
        case 0 : { // by contact
            if (!mContactModel) {
                QApplication::restoreOverrideCursor();

                if (!mIsOpenContactDialogShowed) {
                    CRM_DIALOGFACTORY->requestInformationDialog(tr("The contact window is opened\n"
                                                                   "to select a contact"));
                    mIsOpenContactDialogShowed = true;
                }

                QApplication::setOverrideCursor(Qt::WaitCursor);
                CRM_ActionContact a;
                a.trigger(); // create or only raise the MDI window with model

                mContactModel = CRM_MODELFACTORY->getModel(CRM_ModelFactory::ModelContact);
                connect(mContactModel, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
                        this, SLOT(slotContactRowChanged(QModelIndex,QModelIndex)));

                if (mCustomerModel) {
                    disconnect(mCustomerModel, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
                               this, SLOT(slotCustomerRowChanged(QModelIndex,QModelIndex)));
                    mCustomerModel = NULL;
                }
            }
            mActivityModel->setParentManager(mContactModel);

            fileSave(false);
            mActivityModel->setPrimaryParent("acc_contact_id");

            if (cbSelectStatus->currentIndex() == 0) {
                mActivityModel->setWhere("", false); // not select
            } else {
                mActivityModel->setWhere("status_id > 0 AND status_id < 4", false); // not select
            }

            mActivityModel->setRoot(mContactModel->getCurrentId());
            mActivityModel->select();

            RB_String str = mContactModel->getCurrentValue("lastname").toString();
            leSelectedBy->setText(str);

            break;
        }
        case 1 : { // by customer
            if (!mCustomerModel) {
                QApplication::restoreOverrideCursor();

                if (!mIsOpenCustomerDialogShowed) {
                    CRM_DIALOGFACTORY->requestInformationDialog(tr("The customer window is opened\n"
                                                                   "to select a customer"));
                    mIsOpenCustomerDialogShowed = true;
                }

                QApplication::setOverrideCursor(Qt::WaitCursor);
                CRM_ActionCustomer a;
                a.trigger(); // create or only raise the MDI window with model

                mCustomerModel = CRM_MODELFACTORY->getModel(CRM_ModelFactory::ModelCustomer);
                connect(mCustomerModel, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
                        this, SLOT(slotCustomerRowChanged(QModelIndex,QModelIndex)));
                if (mContactModel) {
                    disconnect(mContactModel, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
                               this, SLOT(slotContactRowChanged(QModelIndex,QModelIndex)));
                    mContactModel = NULL;
                }
            }
            mActivityModel->setParentManager(mCustomerModel);

            fileSave(false);
            mActivityModel->setPrimaryParent("acc_customer_id");

            if (cbSelectStatus->currentIndex() == 0) {
                mActivityModel->setWhere("", false); // not select
            } else {
                mActivityModel->setWhere("status_id > 0 AND status_id < 4", false); // not select
            }

            mActivityModel->setRoot(mCustomerModel->getCurrentId());
            mActivityModel->select();

            RB_String str = mCustomerModel->getCurrentValue("customerno").toString();
            leSelectedBy->setText(str);

            break;
        }
        case 2 : { // by account
            mCustomerModel = NULL;
            mContactModel = NULL;

            fileSave(false);
            mActivityModel->setPrimaryParent("parent");

            if (cbSelectStatus->currentIndex() == 0) {
                mActivityModel->setWhere("", false); // not select
            } else {
                mActivityModel->setWhere("status_id > 0 AND status_id < 4", false); // not select
            }

            mActivityModel->setRoot(CRM_MODELFACTORY->getRootId());
            mActivityModel->select();

            RB_ObjectBase* obj = CRM_MODELFACTORY->getRoot();
            leSelectedBy->setText(obj->getValue("code").toString());

            break;
        }
        case 3 : { // by all
            mCustomerModel = NULL;
            mContactModel = NULL;

            fileSave(false);
            mActivityModel->setPrimaryParent("parent");

            if (cbSelectStatus->currentIndex() == 0) {
                mActivityModel->setWhere("`crm_activity`.`id`<>'0'", false); // not select
            } else {
                mActivityModel->setWhere("`crm_activity`.`id`<>'0' "
                                         "AND status_id > 0 AND status_id < 4", false); // not select
            }

            mActivityModel->setRoot("");
            mActivityModel->select();

            leSelectedBy->setText("-");

            break;
        }
        default :
            break;
    }

    // TODO: Select does not clear the mapper widgets or leave as is
    if (!tvActivity->currentIndex().isValid()) {
        mActivityMapper->slotClearWidgets();
    }

    QApplication::restoreOverrideCursor();
}

/**
 * Slot parent (customer) model row changed, updates leSelectedBy
 * @param current current index
 * @param previous previous index
 */
void CRM_ActivityWidget::slotCustomerRowChanged(const QModelIndex& current,
                                                const QModelIndex& /* previous */) {
    if (!mCustomerModel) return;
    QModelIndex idx = mCustomerModel->index(current.row(),
                                            mCustomerModel->fieldIndex("customerno"));
    RB_String str = mCustomerModel->data(idx).toString();
    leSelectedBy->setText(str);
}

/**
 * Slot parent (contact) model row changed, updates leSelectedBy
 * @param current current index
 * @param previous previous index
 */
void CRM_ActivityWidget::slotContactRowChanged(const QModelIndex& current,
                                               const QModelIndex& /* previous */) {
    if (!mContactModel)  return;
    QModelIndex idx = mContactModel->index(current.row(),
                                           mContactModel->fieldIndex("lastname"));
    RB_String str = mContactModel->data(idx).toString();
    leSelectedBy->setText(str);
}

/**
 * Start date time changed, update end date time when before start.
 */
void CRM_ActivityWidget::slotUpdateEndDateTime() {
    QDateTime startDt = dteStart->dateTime();

    if (startDt > dteEnd->dateTime()) {
        dteEnd->setDateTime(startDt);
    }
}

void CRM_ActivityWidget::slotChangedCurrentRow(const QModelIndex&,
                                               const QModelIndex&) {
    if (cbSelectedBy->currentIndex() != 2
            && cbSelectedBy->currentIndex() != 3) {
        return;
    }

    // Set customer code
    RB_String id = mActivityModel->getCurrentValue("acc_customer_id").toString();
    CRM_SqlCommonFunctions f;
    RB_Variant var = f.selectFromWhere("customerno", "ACC_Customer",
                                       "id='" + id + "'").toString();
    leSelectedBy->setText(tr("Customer Code:") + " " + var.toString());
}

/**
 * @returns the help subject, in this case the widget objectname
 */
RB_String CRM_ActivityWidget::getHelpSubject() const {
    return objectName();
}

bool CRM_ActivityWidget::closeWidget() {
    RB_Widget::closeWidget();
    if (maybeSave()) {
        return true;
    }
    return false;
}

/**
 * Change event, for example translation
 */
void CRM_ActivityWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
