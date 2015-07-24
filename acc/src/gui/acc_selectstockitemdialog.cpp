/*****************************************************************
 * $Id: acc_selectstockitemdialog.cpp 2165 2014-09-24 19:22:36Z rutger $
 * Created: Jan 25, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_selectstockitemdialog.h"

#include "acc.h"
#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "acc_sqlcommonfunctions.h"

/**
 * Constructor
 */
ACC_SelectStockItemDialog::ACC_SelectStockItemDialog(QWidget *parent)
        : RB_Dialog(parent) {

    setupUi(this);

    mModel = NULL;
    mChildModel = NULL;
    mGrChildModel = NULL;
//    mSizeHint = QSize(500, 550);
    QList<int> list;
    list << 320 << 120;
    splitter->setSizes(list);
}

/**
 * Destructor
 */
ACC_SelectStockItemDialog::~ACC_SelectStockItemDialog() {
    // all non-shared models to be deleted by dialog, dock- or MDI window
    delete mModel;
    delete mChildModel;
    delete mGrChildModel;
    RB_DEBUG->print("ACC_SelectStockItemDialog::~ACC_SelectStockItemDialog() OK");
}

/**
 * Initialize dialog and models
 */
void ACC_SelectStockItemDialog::init() {

    setWindowTitle(tr("Select Stock Item[*]"));

    //
    // 1. Set model with ID, shared=false: new model will be created
    //
    mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelStockCategory, false);
    mModel->setRoot(ACC_MODELFACTORY->getRootId()); // connect parent child manually

    //
    // 2. No relations
    // 3. Select after relations have been set
    //
    mModel->select();

    //
    // 4. Connect model to main view
    //
    setFormatTreeView(trvStockCategory, mModel);
    trvStockCategory->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //
    // 1. Set item model and connect to table view
    //
    mChildModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelStockMaster, false);
    mChildModel->setParentManager(mModel); // connect parent child manually

    //
    // 2. No relations
    // 3. Select by parent
    // 4. Connect model to main view
    //
    setFormatTableView(tvStockMaster, mChildModel);
    tvStockMaster->setEditTriggers(QAbstractItemView::NoEditTriggers);
    readSettings();

    // Hidden columns only for tableviews
    int colCount = mModel->columnCount();
    for (int i = 0; i < colCount; ++i) {
        if (i >= 2) {
            trvStockCategory->hideColumn(i);
        } else {
            trvStockCategory->showColumn(i);

            if (trvStockCategory->columnWidth(i) < 5) {
                trvStockCategory->setColumnWidth(i, 100);
            }
        }
    }

    // Hidden columns only for tableviews
    colCount = mChildModel->columnCount();
    for (int i = 0; i < colCount; ++i) {
        if (i != RB2::HIDDENCOLUMNS && i != RB2::HIDDENCOLUMNS+1) {
            tvStockMaster->hideColumn(i);
        } else {
            tvStockMaster->showColumn(i);

            if (tvStockMaster->columnWidth(i) < 5) {
                tvStockMaster->setColumnWidth(i, 100);
            }
        }
    }

    //
    // Set price model, leave warning because no selectionmodel (and no tableView)
    //
    mGrChildModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelPrice, false);
    mGrChildModel->setParentManager(mChildModel); // connect parent child manually

}

/**
 * @returns Index of current selected stock item/master (product)
 */
QModelIndex ACC_SelectStockItemDialog::getCurrentChild1ModelIndex() const {
    // Not from the tableview (which gives the proxy index) but from the model,
    // tvStockMaster->currentIndex();
    return mChildModel->getProxyIndex();
}

/**
 * @returns ID of stock item
 */
RB_String ACC_SelectStockItemDialog::getStockId() const {
    RB_MmProxy* m = mChildModel;
    QModelIndex index = m->getProxyIndex();
    return m->hiddenData(m->index(index.row(), 0,
                            index.parent()), Qt::DisplayRole).toString();
}

/**
 * @returns Stock item code
 */
RB_String ACC_SelectStockItemDialog::getCode() const {
    RB_MmProxy* m = mChildModel;
    QModelIndex index = m->getProxyIndex();
    return m->data(m->index(index.row(), m->fieldIndex("stockcode"),
                            index.parent()), Qt::DisplayRole).toString();
}

/**
 * @returns Serial number
 */
RB_String ACC_SelectStockItemDialog::getSerialNumber() const {
    RB_MmProxy* m = mChildModel;
    QModelIndex index = m->getProxyIndex();

    // if serialized get next serial number from database
    int serialized = m->data(m->index(index.row(), m->fieldIndex("serialised"),
                                      index.parent()), Qt::DisplayRole).toInt();
    if (serialized == 1) {
        // Get next sequence number
        ACC_SqlCommonFunctions f;
        int serialNo = f.getNextTransNo(ACC2::TransStockSerialNumber,
                                       "none", "Stock Serial Number");
        return RB_String::number(serialNo);
    }

    return m->data(m->index(index.row(), m->fieldIndex("serialno"),
                            index.parent()), Qt::DisplayRole).toString();
}

/**
 * @returns Stock item description
 */
RB_String ACC_SelectStockItemDialog::getDescription() const {
    RB_MmProxy* m = mChildModel;
    QModelIndex index = m->getProxyIndex();
    return m->data(m->index(index.row(), m->fieldIndex("description"),
                            index.parent()), Qt::DisplayRole).toString();
}

/**
 * @returns Stock item unit of measurement
 */
RB_String ACC_SelectStockItemDialog::getUnitOfMeasurement() const {
    // Gets the ID of unit of measurement
    RB_MmProxy* m = mChildModel;
    QModelIndex index = m->getProxyIndex();
    if (!index.isValid()) return RB_String();
    RB_String id = m->data(m->index(index.row(), m->fieldIndex("unitofmeasurement"),
                           index.parent()), Qt::DisplayRole).toString();
    RB_MmProxy* mUom = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelUnitOfMeasure);
    mUom->setRoot(""); // select all
    mUom->select();

    return mUom->getValue(id, "unitname").toString();
}

/**
 * TODO: in case of database connection use SQL statement for better performance?
 * @returns Stock item price
 */
double ACC_SelectStockItemDialog::getPrice() const {
    //Get ID of selected stockmaster (item), stockmaster model exists
    RB_MmProxy* msm = mChildModel; // ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelSelectStockMaster);
    QModelIndex index = msm->getProxyIndex();
    if (!index.isValid()) return 0.0;
    RB_String smId = msm->hiddenData(msm->index(index.row(), 0)).toString();

    // Get current customer sales_type as required second match, customer model exists
    RB_MmProxy* mcm = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelCustomer);
    QModelIndex cmIdx = mcm->getProxyIndex();
    if (!cmIdx.isValid()) return 0.0;

    // Hack to get the correct field in the treemodel
//    int col = mcm->fieldIndex("salestype_id") + RB_MmSource::HiddenColumns;

    RB_String stId = mcm->data(mcm->index(cmIdx.row(), mcm->fieldIndex("salestype_id"),
                                          cmIdx.parent()), RB2::RoleOrigData).toString();

    // stId = 'Discount'? id is not returned because of relation? SQL statement instead?
    // smId is the id of the selected stock
    // dmId is the id of the current debtor master (customer)
    // get the id of the salestype from the debtor master in stId
    // ->search pricelist

    // TODO: handle with SQL statement
    // Get current debtor sales_type as required second match

    RB_Variant var = mGrChildModel->getValue(smId, "price", "salestype_id", stId);
    if (!var.isValid()) {
        ACC_DIALOGFACTORY->requestWarningDialog("No price available for the sales type\n"
                                                "of this customer, check the price list.");
        return 0.0;
    }

    return var.toDouble();
}

/**
 * Get standard material cost. Also used as first input
 * for the purchase order
 */
double ACC_SelectStockItemDialog::getMaterialCost() const {
    RB_MmProxy* m = mChildModel;
    QModelIndex index = m->getProxyIndex();
    if (!index.isValid()) return 0.0;
    double cost = m->data(m->index(index.row(), m->fieldIndex("materialcost"),
                                   index.parent()), Qt::DisplayRole).toDouble();
    return cost;
}

/**
 * Get standard labour cost. Also used as first input
 * for the purchase order
 */
double ACC_SelectStockItemDialog::getLabourCost() const {
    RB_MmProxy* m = mChildModel;
    QModelIndex index = m->getProxyIndex();
    if (!index.isValid()) return 0.0;
    double cost = m->data(m->index(index.row(), m->fieldIndex("labourcost"),
                                   index.parent()), Qt::DisplayRole).toDouble();
    return cost;
}

/**
 * Get standard overhead cost. Also used as first input
 * for the purchase order
 */
double ACC_SelectStockItemDialog::getOverheadCost() const {
    RB_MmProxy* m = mChildModel;
    QModelIndex index = m->getProxyIndex();
    if (!index.isValid()) return 0.0;
    double cost = m->data(m->index(index.row(), m->fieldIndex("overheadcost"),
                                   index.parent()), Qt::DisplayRole).toDouble();
    return cost;
}

RB_String ACC_SelectStockItemDialog::getTaxCatId() const {
    RB_MmProxy* m = mModel;
    QModelIndex index = m->getProxyIndex();
    return m->data(m->index(index.row(), m->fieldIndex("taxcat_id"),
                            index.parent()), (Qt::UserRole + 1)).toString();
}

RB_String ACC_SelectStockItemDialog::getCostCenterIdx() const {
    RB_MmProxy* m = mChildModel;
    QModelIndex index = m->getProxyIndex();
    RB_String str = m->data(m->index(index.row(), m->fieldIndex("costcenter_idx"),
                                     index.parent()), RB2::RoleOrigData).toString();
    str += m->data(m->index(index.row(), m->fieldIndex("costcenter_idx"),
                            index.parent()), Qt::DisplayRole).toString();
    return str;
}

/**
 * Button Help clicked
 */
void ACC_SelectStockItemDialog::on_pbHelp_clicked() {
    // this->setHelpSubject("ACC_SelectStockItemDialog");
}

/**
 * Button Select item clicked
 */
void ACC_SelectStockItemDialog::on_pbSelect_clicked() {
    accept();
}

/**
 * Button Cancel clicked
 */
void ACC_SelectStockItemDialog::on_pbCancel_clicked() {
    reject();
}

/**
 * Change event, for example translation
 */
void ACC_SelectStockItemDialog::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
