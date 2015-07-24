/*****************************************************************
 * $Id: srm_activitywidget.cpp 1925 2013-04-22 12:31:48Z rutger $
 * Created: Feb 20, 2012 3:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "srm_activitywidget.h"

#include <QStringListModel>
#include "srm_actioncontact.h"
#include "srm_actionsupplier.h"
#include "srm_dialogfactory.h"
#include "srm_modelfactory.h"
#include "srm_selectprojectdialog.h"
#include "srm_sqlcommonfunctions.h"
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
SRM_ActivityWidget::SRM_ActivityWidget(QWidget *parent)
        : RB_Widget(parent) {

    setupUi(this);

    mActivityModel = NULL;
    mActivityMapper = NULL;
    mSupplierModel = NULL;
    mContactModel = NULL;

    mIsOpenContactDialogShowed = false;
    mIsOpenSupplierDialogShowed = false;
}

/**
 * Destructor
 */
SRM_ActivityWidget::~SRM_ActivityWidget() {
    delete mActivityModel;
    RB_DEBUG->print("SRM_ActivityWidget::~SRM_ActivityWidget() OK");
}

/**
 * Initialize widget and models
 */
void SRM_ActivityWidget::init() {
    RB_StringList items;
    items << tr("All") << tr("Active only");
    cbSelectStatus->addItems(items);
    connect(cbSelectStatus, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotSelectStatusChanged(int)));

    items.clear();
    items << tr("Contact") << tr("Supplier") << tr("Account") << tr("All");
    cbSelectedBy->addItems(items);
    connect(cbSelectedBy, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotSelectedByChanged(int)));

    //
    // 1. Set model for activity mapper
    //
    mActivityModel = SRM_MODELFACTORY->getModel(SRM_ModelFactory::ModelActivity); // shared
    mActivityModel->setPrimaryParent("parent");
    mActivityModel->setRoot(SRM_MODELFACTORY->getRootId());
    leSelectedBy->setText(SRM_MODELFACTORY->getRoot()->getValue("code").toString());

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
bool SRM_ActivityWidget::fileSave(bool withSelect) {
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
void SRM_ActivityWidget::on_pbAdd_clicked() {
    if ((!mSupplierModel || !mSupplierModel->getCurrentIndex().isValid())
            && (!mContactModel || !mContactModel->getCurrentIndex().isValid())) {
        int ret = DB_DIALOGFACTORY->requestYesNoDialog(
                tr("Add activity ..."),
                tr("No supplier or contact selected.\n"
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
    mActivityModel->setData(idx, SRM_MODELFACTORY->getRootId(), Qt::EditRole);
    idx = mActivityModel->index(row, mActivityModel->fieldIndex("mstart"));
    mActivityModel->setData(idx, QDateTime::currentDateTime(), Qt::EditRole);
    idx = mActivityModel->index(row, mActivityModel->fieldIndex("mend"));
    mActivityModel->setData(idx, QDateTime::currentDateTime(), Qt::EditRole);

    if (mContactModel) {
        idx = mActivityModel->index(row, mActivityModel->fieldIndex("acc_contact_id"));
        mActivityModel->setData(idx, mContactModel->getCurrentId(), Qt::EditRole);
        idx = mActivityModel->index(row, mActivityModel->fieldIndex("acc_supplier_id"));
        mActivityModel->setData(idx, mContactModel->getCurrentValue("parent").toString(), Qt::EditRole);
    } else if (mSupplierModel) {
        idx = mActivityModel->index(row, mActivityModel->fieldIndex("acc_contact_id"));
        mActivityModel->setData(idx, "0", Qt::EditRole);
        idx = mActivityModel->index(row, mActivityModel->fieldIndex("acc_supplier_id"));
        mActivityModel->setData(idx, mSupplierModel->getCurrentId(), Qt::EditRole);
    }
    // end NOTE

    tvActivity->setCurrentIndex(mActivityModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
    tvActivity->scrollTo(tvActivity->currentIndex());
    cbActivityType->setFocus();
}

/**
 * Button delete clicked
 */
void SRM_ActivityWidget::on_pbDelete_clicked() {
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
void SRM_ActivityWidget::on_pbSave_clicked() {
    fileSave(true);
}

void SRM_ActivityWidget::slotSelectStatusChanged(int /*index*/) {
    slotSelectedByChanged(cbSelectedBy->currentIndex());
}

void SRM_ActivityWidget::slotSelectedByChanged(int index) {
    QApplication::setOverrideCursor(Qt::WaitCursor);

    switch (index) {
        case 0 : { // by contact
            if (!mContactModel) {
                QApplication::restoreOverrideCursor();

                if (!mIsOpenContactDialogShowed) {
                    SRM_DIALOGFACTORY->requestInformationDialog(tr("The contact window is opened\n"
                                                                   "to select a contact"));
                    mIsOpenContactDialogShowed = true;
                }

                QApplication::setOverrideCursor(Qt::WaitCursor);
                SRM_ActionContact a;
                a.trigger(); // create or only raise the MDI window with model

                mContactModel = SRM_MODELFACTORY->getModel(SRM_ModelFactory::ModelContact);
                connect(mContactModel, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
                        this, SLOT(slotContactRowChanged(QModelIndex,QModelIndex)));

                if (mSupplierModel) {
                    disconnect(mSupplierModel, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
                               this, SLOT(slotSupplierRowChanged(QModelIndex,QModelIndex)));
                    mSupplierModel = NULL;
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
            if (!mSupplierModel) {
                QApplication::restoreOverrideCursor();

                if (!mIsOpenSupplierDialogShowed) {
                    SRM_DIALOGFACTORY->requestInformationDialog(tr("The supplier window is opened\n"
                                                                   "to select a supplier"));
                    mIsOpenSupplierDialogShowed = true;
                }

                QApplication::setOverrideCursor(Qt::WaitCursor);
                SRM_ActionSupplier a;
                a.trigger(); // create or only raise the MDI window with model

                mSupplierModel = SRM_MODELFACTORY->getModel(SRM_ModelFactory::ModelSupplier);
                connect(mSupplierModel, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
                        this, SLOT(slotSupplierRowChanged(QModelIndex,QModelIndex)));
                if (mContactModel) {
                    disconnect(mContactModel, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
                               this, SLOT(slotContactRowChanged(QModelIndex,QModelIndex)));
                    mContactModel = NULL;
                }
            }
            mActivityModel->setParentManager(mSupplierModel);

            fileSave(false);
            mActivityModel->setPrimaryParent("acc_supplier_id");

            if (cbSelectStatus->currentIndex() == 0) {
                mActivityModel->setWhere("", false); // not select
            } else {
                mActivityModel->setWhere("status_id > 0 AND status_id < 4", false); // not select
            }

            mActivityModel->setRoot(mSupplierModel->getCurrentId());
            mActivityModel->select();

            RB_String str = mSupplierModel->getCurrentValue("suppliercode").toString();
            leSelectedBy->setText(str);

            break;
        }
        case 2 : { // by account
            mSupplierModel = NULL;
            mContactModel = NULL;

            fileSave(false);
            mActivityModel->setPrimaryParent("parent");

            if (cbSelectStatus->currentIndex() == 0) {
                mActivityModel->setWhere("", false); // not select
            } else {
                mActivityModel->setWhere("status_id > 0 AND status_id < 4", false); // not select
            }

            mActivityModel->setRoot(SRM_MODELFACTORY->getRootId());
            mActivityModel->select();

            RB_ObjectBase* obj = SRM_MODELFACTORY->getRoot();
            leSelectedBy->setText(obj->getValue("code").toString());

            break;
        }
        case 3 : { // by all
            mSupplierModel = NULL;
            mContactModel = NULL;

            fileSave(false);
            mActivityModel->setPrimaryParent("parent");

            if (cbSelectStatus->currentIndex() == 0) {
                mActivityModel->setWhere("`srm_activity`.`id`<>'0'", false); // not select
            } else {
                mActivityModel->setWhere("`srm_activity`.`id`<>'0' "
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
void SRM_ActivityWidget::slotSupplierRowChanged(const QModelIndex& current,
                                                const QModelIndex& /* previous */) {
    if (!mSupplierModel) return;
    QModelIndex idx = mSupplierModel->index(current.row(),
                                            mSupplierModel->fieldIndex("suppliercode"));
    RB_String str = mSupplierModel->data(idx).toString();
    leSelectedBy->setText(str);
}

/**
 * Slot parent (contact) model row changed, updates leSelectedBy
 * @param current current index
 * @param previous previous index
 */
void SRM_ActivityWidget::slotContactRowChanged(const QModelIndex& current,
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
void SRM_ActivityWidget::slotUpdateEndDateTime() {
    QDateTime startDt = dteStart->dateTime();

    if (startDt > dteEnd->dateTime()) {
        dteEnd->setDateTime(startDt);
    }
}

void SRM_ActivityWidget::slotChangedCurrentRow(const QModelIndex&,
                                               const QModelIndex&) {
    if (cbSelectedBy->currentIndex() != 2
            && cbSelectedBy->currentIndex() != 3) {
        return;
    }

    // Set customer code
    RB_String id = mActivityModel->getCurrentValue("acc_supplier_id").toString();
    SRM_SqlCommonFunctions f;
    RB_Variant var = f.selectFromWhere("suppliercode", "ACC_Supplier",
                                       "id='" + id + "'").toString();
    leSelectedBy->setText(tr("Supplier Code:") + " " + var.toString());
}

/**
 * @returns the help subject, in this case the widget objectname
 */
RB_String SRM_ActivityWidget::getHelpSubject() const {
    return objectName();
}

bool SRM_ActivityWidget::closeWidget() {
    RB_Widget::closeWidget();
    if (maybeSave()) {
        return true;
    }
    return false;
}

/**
 * Change event, for example translation
 */
void SRM_ActivityWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
