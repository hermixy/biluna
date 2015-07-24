/*****************************************************************
 * $Id: acc_supplcostpartwidget.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Sept 25, 2012 11:14:42 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_supplcostpartwidget.h"

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
ACC_SupplCostPartWidget::ACC_SupplCostPartWidget(QWidget *parent)
        : RB_Widget(parent) {

    setupUi(this);

    mModel = NULL;
    mMapper = NULL;
}

/**
 * Destructor
 */
ACC_SupplCostPartWidget::~ACC_SupplCostPartWidget() {
    mModel->revert();
    delete mModel;
    RB_DEBUG->print("ACC_SupplCostPartWidget::~ACC_SupplCostPartWidget() OK");
}

/**
 * Initialize widget and models
 */
void ACC_SupplCostPartWidget::init() {
    //
    // 1. Set model for customer mapper
    //
    mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelSupplCostPart); // shared
    // mModel->setRoot(ACC_MODELFACTORY->getRootId()); not here

    // mapper can only be used once on a field, therefore:
    mSupplModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelSupplier); // shared
    connect(mSupplModel, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(slotParentRowChanged(QModelIndex,QModelIndex)));

    //
    // 2. Get mapper for line edits etc.
    //
    mMapper = mModel->getMapper();
    mMapper->addMapping(leShortName, mModel->fieldIndex("shortname"));
    mMapper->addMapping(leDescription, mModel->fieldIndex("description"));
    RB_StringList typeItems;
    typeItems << tr("Budget") << tr("Forecast") << tr("Actual");
    mModel->setTextList(mModel->fieldIndex("type_id"), typeItems);
    cbValueType->setModel(new QStringListModel(typeItems, this));
    mMapper->addMapping(cbValueType, mModel->fieldIndex("type_id"), "currentIndex");
    mMapper->addMapping(leWeight, mModel->fieldIndex("weight"));
    mMapper->addMapping(deStart, mModel->fieldIndex("startdate"));
    mMapper->addMapping(deEnd, mModel->fieldIndex("enddate"));
    mMapper->addMapping(ileCostGroup, mModel->fieldIndex("costgroup_idx"));
    mMapper->addMapping(ileCostCenter, mModel->fieldIndex("costcenter_idx"));

    //
    // 3. Select after relations have been set, only for database models
    //
    // mModel->select();

    //
    // 4. Connect model to main view
    //
    setFormatTableView(tv, mModel);
    readSettings();

    int count = mModel->columnCount(QModelIndex());

    for (int i = 0; i < count; ++i) {
        if (i < RB2::HIDDENCOLUMNS) {
            tv->hideColumn(i);
        } else {
            tv->showColumn(i);

            if (tv->columnWidth(i) < 5) {
                tv->setColumnWidth(i, 100);
            }
        }
    }

    // Request modelindex in case of already selected index
    RB_Widget* wgt = ACC_DIALOGFACTORY->getWidget(ACC_DialogFactory::WidgetNavigation, NULL);
    QModelIndex idx = wgt->getCurrentModelIndex(ACC_ModelFactory::ModelSupplier);
    slotParentRowChanged(idx, QModelIndex()); // set supplier name in lineEdit
    mModel->setRoot(idx);
}

/**
 * Save data, can be called directly or indirectly via closeEvent()
 * and maybeSave().
 * @param withSelect true if select() is to be done after submitAll()
 */
bool ACC_SupplCostPartWidget::fileSave(bool withSelect) {
    // QApplication::setOverrideCursor(Qt::WaitCursor);
    // HACK to emit editingFinished() for current QLineEdit:
    beforeFileSave();

    if (withSelect && mModel) {
        if (mModel->submitAllAndSelect()
                && mModel->submitAllAndSelect()) {

            setWindowModified(false);
            return true;
        }
    } else if (mModel) {
        if (mModel->submitAll() && mModel->submitAll()) {
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
void ACC_SupplCostPartWidget::on_pbAdd_clicked() {
    RB_MmProxy* supplModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelSupplier);

    if (!supplModel || !supplModel->getProxyIndex().isValid()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No supplier selected.\n"
                                                   "Please select a supplier first."));
        return;
    }

    if (!mModel) return;
    mModel->undoFilterSort();

    // always insert at the beginning
    int row = 0; // mModel->rowCount();
    mModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex idx;
    idx = mModel->index(row, mModel->fieldIndex("shortname"));
    mModel->setData(idx, "<NEW>", Qt::EditRole);
    idx = mModel->index(row, mModel->fieldIndex("type_id"));
    mModel->setData(idx, 0, Qt::EditRole);
    idx = mModel->index(row, mModel->fieldIndex("weight"));
    mModel->setData(idx, 100, Qt::EditRole);
    RB_String str = RB_String::number(QDate::currentDate().year()) + "-01-01";
    idx = mModel->index(row, mModel->fieldIndex("startdate"));
    mModel->setData(idx, str, Qt::EditRole);
    str = RB_String::number(QDate::currentDate().year()) + "-12-31";
    idx = mModel->index(row, mModel->fieldIndex("enddate"));
    mModel->setData(idx, str, Qt::EditRole);
    idx = mModel->index(row, mModel->fieldIndex("costgroup_idx"));
    mModel->setData(idx, "0", Qt::EditRole);
    idx = mModel->index(row, mModel->fieldIndex("costcenter_idx"));
    mModel->setData(idx, "0", Qt::EditRole);
    // end NOTE

    tv->setCurrentIndex(mModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
    tv->scrollTo(tv->currentIndex());
    leShortName->setFocus();
    leShortName->selectAll();
}

/**
 * Button delete clicked
 */
void ACC_SupplCostPartWidget::on_pbDelete_clicked() {
    QModelIndex index = tv->currentIndex();
    if (!index.isValid()) return;

    int ret = ACC_DIALOGFACTORY->requestYesNoDialog(
            tr("Delete item ..."),
            tr("This action cannot be undone.\n"
               "Do you want to delete this item?"));
    if (ret == QMessageBox::No) {
        ACC_DIALOGFACTORY->statusBarMessage(tr("Deletion canceled"), 2000);
        return;
    }

    mModel->removeRows(index.row(), 1, QModelIndex());
    fileSave(true);
}

/**
 * Push button save clicked
 */
void ACC_SupplCostPartWidget::on_pbSave_clicked() {
    fileSave(true);
}

void ACC_SupplCostPartWidget::on_pbFilterOn_clicked() {
    RB_String str = leFilter->text();
    if (str.isEmpty()) {
        return;
    }
    if (!str.endsWith("%")) {
        str += "%";
    }
    RB_String filter = "`acc_custcostpart`.`shortname` LIKE '";
    filter += str + "'";
    mModel->setWhere(filter);
}

void ACC_SupplCostPartWidget::on_pbFilterOff_clicked() {
    mModel->setWhere("");
    leFilter->setText("");
}

/**
 * Button select cost center group clicked
 */
void ACC_SupplCostPartWidget::on_ileCostGroup_clicked() {
    if (!tv->currentIndex().isValid()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No cost center part selected.\n"
                                                   "Please select a part first."));
        return;
    }

    RB_Dialog* dlg = ACC_DIALOGFACTORY->getDialog(ACC_DialogFactory::DialogSelectCostGroup);

    if (dlg && dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();

        if (obj) {
            int row = tv->currentIndex().row();
            QModelIndex index = mModel->index(row, mModel->fieldIndex("costgroup_idx"));
            mModel->setData(index, obj->getId()
                               + obj->getValue("groupname").toString(), Qt::EditRole);

            // Set cost group to '0' because allocation cost group or cost center
            index = mModel->index(row, mModel->fieldIndex("costcenter_idx"));
            mModel->setData(index, "0", Qt::EditRole);
        }
    } else {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No cost center group selected.\n"
                                                   "Group is unchanged."));
    }

    dlg->deleteLater();
}

/**
 * Button select cost center clicked
 */
void ACC_SupplCostPartWidget::on_ileCostCenter_clicked() {
    if (!tv->currentIndex().isValid()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No cost center part selected.\n"
                                                   "Please select a part first."));
        return;
    }

    RB_Dialog* dlg = ACC_DIALOGFACTORY->getDialog(ACC_DialogFactory::DialogSelectCostCenter);

    if (dlg && dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();

        if (obj) {
            int row = tv->currentIndex().row();
            QModelIndex index = mModel->index(row, mModel->fieldIndex("costcenter_idx"));
            mModel->setData(index, obj->getId()
                               + obj->getValue("centercode").toString(), Qt::EditRole);

            // Set cost group to '0' because allocation or cost group or cost center
            index = mModel->index(row, mModel->fieldIndex("costgroup_idx"));
            mModel->setData(index, "0", Qt::EditRole);
        }
    } else {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No cost center selected.\n"
                                                   "Cost center is unchanged."));
    }

    dlg->deleteLater();
}

/**
 * Slot parent (supplier) model row changed, updates leSupplier
 * @param current current index
 * @param previous previous index
 */
void ACC_SupplCostPartWidget::slotParentRowChanged(const QModelIndex& current,
                                                   const QModelIndex& /* previous */) {
    if (!current.isValid()) {
        leSupplier->setText("-");
        return;
    }

    QModelIndex idx = mSupplModel->index(current.row(), mSupplModel->fieldIndex("suppname"),
                                         current.parent());
    RB_String str = mSupplModel->data(idx).toString();
    leSupplier->setText(str);
}

/**
 * @returns the help subject, in this case the widget objectname
 */
RB_String ACC_SupplCostPartWidget::getHelpSubject() const {
    return objectName();
}

bool ACC_SupplCostPartWidget::closeWidget() {
    RB_Widget::closeWidget();
    if (maybeSave()) {
        return true;
    }
    return false;
}

/**
 * Change event, for example translation
 */
void ACC_SupplCostPartWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
