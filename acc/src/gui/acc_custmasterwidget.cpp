/*****************************************************************
 * $Id: acc_custmasterwidget.cpp 2186 2014-10-16 14:04:01Z rutger $
 * Created: Dec 20, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_custmasterwidget.h"

#include <QStringListModel>
#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "rb_datawidgetmapper.h"
#include "rb_mdiwindow.h"
#include "rb_sqlrelation.h"
#include "rb_sqlrelationaldelegate.h"

/**
 * Constructor
 */
ACC_CustMasterWidget::ACC_CustMasterWidget(QWidget *parent)
        : RB_Widget(parent) {

    setupUi(this);

    mModel = NULL;
    mSizeHint = QSize(190, 600);
}

/**
 * Destructor
 */
ACC_CustMasterWidget::~ACC_CustMasterWidget() {
    // unique models are always deleted by the widgets,
    // can be MDI window or dialog, but also a dockWindow with table or tree
    // not revert() here, the destructor of the model will save the data
    delete mModel; // shared model
    RB_DEBUG->print("ACC_CustMasterWidget::~ACC_CustMasterWidget() OK");
}

/**
 * Initialize widget and models
 */
void ACC_CustMasterWidget::init() {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    //
    // 1. Model with ID
    //
    mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelCustomer);
    mModel->setRoot(ACC_MODELFACTORY->getRootId());

    //
    // 2. Relations
    //    mModel->setRelation(mModel->fieldIndex(field),
    //           RB_SqlRelation(relationTable, relationId, relationField));
    mModel->setRelation(mModel->fieldIndex("salestype_id"),
           RB_SqlRelation("ACC_SalesType", "id", "typeabbrev"));
    mModel->setRelation(mModel->fieldIndex("type_id"),
           RB_SqlRelation("ACC_DebtorType", "id", "typename"));
    mModel->setRelation(mModel->fieldIndex("paymentterm_id"),
           RB_SqlRelation("ACC_PaymentTerm", "id", "terms"));
    mModel->setRelation(mModel->fieldIndex("holdreason_id"),
           RB_SqlRelation("ACC_HoldReason", "id", "reasondescription"));
    mModel->setRelation(mModel->fieldIndex("currency_id"),
           RB_SqlRelation("ACC_Currency", "id", "currcode"));

    mModel->setRelation(mModel->fieldIndex("defaultlocation_id"),
           RB_SqlRelation("ACC_Location", "id", "locationname"));
    mModel->setRelation(mModel->fieldIndex("defaultshipvia_id"),
           RB_SqlRelation("ACC_Shipper", "id", "shippername"));
    mModel->setRelation(mModel->fieldIndex("salesman_id"),
           RB_SqlRelation("ACC_SalesMan", "id", "salesmanname"));
    mModel->setRelation(mModel->fieldIndex("area_id"),
           RB_SqlRelation("ACC_Area", "id", "areadescription"));
    mModel->setRelation(mModel->fieldIndex("taxgroup_id"),
           RB_SqlRelation("ACC_TaxGroup", "id", "taxgroupdescr"));

    //
    // 3. Select after relations have been set, only for database models
    //
    mModel->select();
    mModel->setHeaderData(0, Qt::Horizontal, tr("Code"));
    mModel->setHeaderData(1, Qt::Horizontal, tr("Name"));

    //
    // 4. Connect model to main view, set delegates and hide columns
    //
    setFormatTableView(tvCustomer, mModel);

    // Example only
    // tvCustomer->setItemDelegateForColumn(mModel->fieldIndex("text"),
    //                                       new RB_TeDelegate(this));

    readSettings();

    // Hidden columns only for tableviews
    int colCount = mModel->columnCount();
    for (int i = 0; i < colCount; ++i) {
        if (i != RB2::HIDDENCOLUMNS && i != RB2::HIDDENCOLUMNS+1) {
            tvCustomer->hideColumn(i);
        } else {
            tvCustomer->showColumn(i);

            if (tvCustomer->columnWidth(i) < 5) {
                tvCustomer->setColumnWidth(i, 100);
            }
        }
    }

    // tvCustomer->horizontalHeader()->setSortIndicator(RB2::HIDDENCOLUMNS, Qt::AscendingOrder);
    QApplication::restoreOverrideCursor();
}

/**
 * Save data to disk
 * @param withSelect true if select() is to be done after submitAll()
 */
bool ACC_CustMasterWidget::fileSave(bool withSelect) {
    // QApplication::setOverrideCursor(Qt::WaitCursor);
    // HACK to emit editingFinished() for current QLineEdit:
    beforeFileSave();
    if (withSelect && mModel) {
        if (mModel->submitAllAndSelect()) {
            
            setWindowModified(false);
            QApplication::restoreOverrideCursor();
            return true;
        }
    } else if (mModel) {
        if (mModel->submitAll()) {
            setWindowModified(false);
            QApplication::restoreOverrideCursor();
            return true;
        }
    }
    // QApplication::restoreOverrideCursor();
    return false;
}

/**
 * Button add (customer) clicked
 */
void ACC_CustMasterWidget::on_pbAddCustomer_clicked() {
    if (!mModel) return;
    mModel->submitAll();
    mModel->undoFilterSort();

    // Make sure corresponding detail MDI window is open for maybeSave()
    RB_MdiWindow* mdiWin = ACC_DIALOGFACTORY->getMdiWindow(
            ACC_DialogFactory::WidgetCustBranch);
    mdiWin->show();
    ACC_DIALOGFACTORY->getMainWindow()->setActiveMdiWindow(mdiWin);

    int row = 0; // mModel->rowCount(idxParent);
    mModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    setDefaultValues(row, QModelIndex());
    // end NOTE

    tvCustomer->setCurrentIndex(mModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
    tvCustomer->scrollTo(mModel->index(row,  RB2::HIDDENCOLUMNS, QModelIndex()));
    mdiWin->getWidget()->slotResetWidgets();
}

/**
 * Set default column values for the relational fields
 * @param row row of the model
 * @param idxParent parent model index
 */
void ACC_CustMasterWidget::setDefaultValues(int row, const QModelIndex& idxParent) {
    QModelIndex index = mModel->index(row, mModel->fieldIndex("customerno"), idxParent);
    mModel->setData(index, tr("<NEW>"), Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("salestype_id"), idxParent);
    mModel->setData(index, "0", Qt::EditRole);
    QDateTime date = QDateTime::currentDateTime();
    index = mModel->index(row, mModel->fieldIndex("clientsince"), idxParent);
    mModel->setData(index, date, Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("type_id"), idxParent);
    mModel->setData(index, "0", Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("paymentterm_id"), idxParent);
    mModel->setData(index, "0", Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("holdreason_id"), idxParent);
    mModel->setData(index, "0", Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("currency_id"), idxParent);
    mModel->setData(index, "0", Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("invaddrbranch"), idxParent);
    mModel->setData(index, 0, Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("customerpoline"), idxParent);
    mModel->setData(index, 0, Qt::EditRole);

    index = mModel->index(row, mModel->fieldIndex("defaultlocation_id"), idxParent);
    mModel->setData(index, "0", Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("defaultshipvia_id"), idxParent);
    mModel->setData(index, "0", Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("salesman_id"), idxParent);
    mModel->setData(index, "0", Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("area_id"), idxParent);
    mModel->setData(index, "0", Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("taxgroup_id"), idxParent);
    mModel->setData(index, "0", Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("lastusedacct_id"), idxParent);
    mModel->setData(index, "0", Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("crmtype_id"), idxParent);
    mModel->setData(index, 0, Qt::EditRole);

//    mModel->submit();

}

/**
 * Button delete (customer) clicked
 */
void ACC_CustMasterWidget::on_pbDeleteCustomer_clicked() {
    if (!mModel) return;

    int res = ACC_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) return;

    QModelIndex index = tvCustomer->currentIndex();

    if (index.isValid()) {
        mModel->removeRows(index.row(), 1, index.parent());
        mModel->submitAllAndSelect();
        setWindowModified(false);
    }
}

/**
 * Button save clicked
 */
void ACC_CustMasterWidget::on_pbSave_clicked() {
    fileSave(true);
}

/**
 * Set source model filter
 */
void ACC_CustMasterWidget::on_pbFilterOn_clicked() {
    filterSourceModel(mModel, "acc_customer", "customerno", leFilter->text());
}

/**
 * Remove source modelfilter
 */
void ACC_CustMasterWidget::on_pbFilterOff_clicked() {
    leFilter->setText("");
    removeFilterSourceModel(mModel);
}

bool ACC_CustMasterWidget::closeWidget() {
    return RB_Widget::closeWidget();
    //    if (maybeSave()) { not here with navigation widgets
    //        return true;
    //    }
    //    return false;
}

/**
 * @returns current selected model index
 */
QModelIndex ACC_CustMasterWidget::getCurrentModelIndex(int modelType) const {
    if (modelType == ACC_ModelFactory::ModelCustomer) {
        return tvCustomer->currentIndex();
    }
    return QModelIndex();
}

/**
 * Change event, for example translation
 */
void ACC_CustMasterWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
