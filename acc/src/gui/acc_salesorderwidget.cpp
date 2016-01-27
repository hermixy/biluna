/*****************************************************************
 * $Id: acc_salesorderwidget.cpp 2217 2015-02-13 18:18:14Z rutger $
 * Created: Jan 20, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_salesorderwidget.h"

#include "acc.h"
#include "acc_dialogfactory.h"
#include "acc_salesinvoicehtml.h"
#include "acc_modelfactory.h"
#include "acc_objectfactory.h"
#include "acc_orderpreviewdialog.h"
#include "acc_processtransdoc.h"
#include "acc_salesquotationhtml.h"
#include "acc_selectstockitemdialog.h"
#include "acc_sotransdoc.h"
#include "acc_sqlcommonfunctions.h"
#include "acc_taxconfirmationdialog.h"
#include "rb_datawidgetmapper.h"
#include "rb_mdiwindow.h"
#include "rb_sqlrelationaldelegate.h"

/**
 * Constructor
 */
ACC_SalesOrderWidget::ACC_SalesOrderWidget(QWidget *parent)
                        : RB_Widget(parent) {

    setupUi(this);

    mModel = NULL;
    mChildModel = NULL;
    mParentModel = NULL;
    mMapper = NULL;
    mChildMapper = NULL;

    mDebtorTransList = NULL;
    mTransDoc = NULL;

    mCurrencyId = "";
    mExchangeRate = 1.0;
}

/**
 * Destructor
 */
ACC_SalesOrderWidget::~ACC_SalesOrderWidget() {
    // unique models are always deleted by the widgets,
    // can be MDI window or dialog, but also a dockWindow with table or tree
    delete mDebtorTransList;
    delete mTransDoc;
    // mappers are deleted by the models
    delete mChildModel;
    delete mModel;

    RB_DEBUG->print("ACC_SalesOrderWidget::~ACC_SalesOrderWidget() OK");
}

/**
 * Initialize widget and models
 */
void ACC_SalesOrderWidget::init() {

    //
    // Set parent model, customer, for name only
    //
    mParentModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelCustomer);
    connect(mParentModel, SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)),
            this, SLOT(slotParentSelectionChanged(QModelIndex,QModelIndex)));

    //
    // 1. Set model (not ID, is set by parent model) and/or query
    //
    mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelSalesOrder);

    //
    // 2. Set relations and mapper for line edits etc.
    //
    mMapper = mModel->getMapper();

    // tab Delivery
    mMapper->addMapping(leOrderNo, mModel->fieldIndex("orderno"));
    mMapper->addMapping(leDeliverTo, mModel->fieldIndex("deliverto"));
    addComboBoxMapping(mModel,"fromstkloc_id", "ACC_Location", "id",
                       "locationname", cbWarehouse, mMapper);
    mMapper->addMapping(deOrderDate, mModel->fieldIndex("orddate"));
    mMapper->addMapping(deDispatchDate, mModel->fieldIndex("deliverydate"));
    mMapper->addMapping(ileContact, mModel->fieldIndex("contact_idx"));
    mMapper->addMapping(leContactPhoneNumber, mModel->fieldIndex("contactphone"));
    mMapper->addMapping(leContactEmail, mModel->fieldIndex("contactemail"));
    mMapper->addMapping(leCustomerReference, mModel->fieldIndex("customerref"));
    RB_StringList items;
    items << tr("Show company details") << tr("Hide company details");
    cbPacklistType->setModel(new QStringListModel(items, this));
    mMapper->addMapping(cbPacklistType, mModel->fieldIndex("deliverblind"),
                        "currentIndex");
    mMapper->addMapping(leChargeFreightcost, mModel->fieldIndex("freightcost"));
    addComboBoxMapping(mModel,"shipvia_id", "ACC_Shipper", "id",
                       "shippername", cbFreightMethod, mMapper);
    items.clear();
    items << tr("No") << tr("Yes");
    cbQuotationOnly->setModel(new QStringListModel(items, this));
    mMapper->addMapping(cbQuotationOnly, mModel->fieldIndex("quotation"),
                            "currentIndex");
    // tab Description
    mMapper->addMapping(teComment, mModel->fieldIndex("comments"));
    mMapper->addMapping(leOrderDescription, mModel->fieldIndex("ordertitle"));

    // tab Address
    mMapper->addMapping(leDeliveryAddress1, mModel->fieldIndex("deladd1"));
    mMapper->addMapping(leDeliveryAddress2, mModel->fieldIndex("deladd2"));
    mMapper->addMapping(leDeliveryAddress3, mModel->fieldIndex("deladd3"));
    mMapper->addMapping(leDeliveryAddress4, mModel->fieldIndex("deladd4"));
    mMapper->addMapping(leDeliveryAddress5, mModel->fieldIndex("deladd5"));
    mMapper->addMapping(leDeliveryAddress6, mModel->fieldIndex("deladd6"));
    // tab Invoice
    mMapper->addMapping(leInvoiceComment, mModel->fieldIndex("invoicecomment"));
    mMapper->addMapping(leConsignment, mModel->fieldIndex("consignment"));

    //
    // 3. Select after relations have been set, only for database models
    //
    // mModel->select(); not here done by parent model
    // mModel->setHeaderData(RB2::HIDDENCOLUMNS, Qt::Horizontal, tr("Rutger"));

    //
    // 4. Connect model to main view
    //
    setFormatTableView(tvOrders, mModel);

    //
    // 1. Set child model
    // 2. Mapping and relations
    // (3. Root will be set by selecting row in parent model)
    // 4. connect to child (table) view
    //
    mChildModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelSalesOrderDetail);

    mChildMapper = mChildModel->getMapper();
    mChildMapper->addMapping(ileStockCode, mChildModel->fieldIndex("stk_idx"));
    mChildMapper->addMapping(leItemDescription, mChildModel->fieldIndex("stkdescription"));
    mChildMapper->addMapping(leUnitOfMeasurement, mChildModel->fieldIndex("stkuom"));
    mChildMapper->addMapping(leSerialNumber, mChildModel->fieldIndex("serialno"));
    mChildMapper->addMapping(leUnitPrice, mChildModel->fieldIndex("unitprice"));
    mChildMapper->addMapping(leQuantity, mChildModel->fieldIndex("quantity"));
    mChildMapper->addMapping(leDiscountPercent, mChildModel->fieldIndex("discountpercent"));
    mChildMapper->addMapping(pteNarrative, mChildModel->fieldIndex("narrative"));

    // tab Dispatch
    // not mChildMapper->addMapping(leOrderedQuantity, mChildModel->fieldIndex("quantity"));
    mChildMapper->addMapping(leAlreadyDispatched, mChildModel->fieldIndex("qtyinvoiced"));
    mChildMapper->addMapping(leCurrentDispatch, mChildModel->fieldIndex("qtydispatched"));
    mChildMapper->addMapping(deItemDue, mChildModel->fieldIndex("itemdue"));

    setFormatTableView(tvDetails, mChildModel);

    connect(tvDetails->selectionModel(),
            SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)),
            this, SLOT(slotItemSelectionChanged(const QModelIndex&,const QModelIndex&)));
    // Calculate total item price
    connect(leUnitPrice, SIGNAL(editingFinished()),
            this, SLOT(slotCalculateItemTotal()));
    connect(leQuantity, SIGNAL(editingFinished()),
            this, SLOT(slotCalculateItemTotal()));
    connect(this->leDiscountPercent, SIGNAL(editingFinished()),
            this, SLOT(slotCalculateItemTotal()));

    readSettings();

    for (int i = 0; i < mModel->columnCount(QModelIndex()); ++i) {
        if (i != RB2::HIDDENCOLUMNS && i != RB2::HIDDENCOLUMNS + 1
                && i!= mModel->fieldIndex("ordertitle")) {
            tvOrders->hideColumn(i);
        } else {
            tvOrders->showColumn(i);

            if (tvOrders->columnWidth(i) < 5) {
                tvOrders->setColumnWidth(i, 100);
            }
        }
    }

    for (int i = 0; i < mChildModel->columnCount(QModelIndex()); ++i) {
        if (i != mChildModel->fieldIndex("stk_idx")
            && i!= mChildModel->fieldIndex("stkdescription")
            && i!= mChildModel->fieldIndex("stkuom")
            && i!= mChildModel->fieldIndex("unitprice")
            && i!= mChildModel->fieldIndex("quantity")
            && i!= mChildModel->fieldIndex("qtyinvoiced")
            && i!= mChildModel->fieldIndex("qtydispatched")
            && i!= mChildModel->fieldIndex("itemdue")){
            tvDetails->hideColumn(i);
        } else {
            tvDetails->showColumn(i);

            if (tvDetails->columnWidth(i) < 5) {
                tvDetails->setColumnWidth(i, 100);
            }
        }
    }

    // Navigation widget has been set to the correct tab
    // Request modelindex in case of already selected index
    RB_Widget* wgt = ACC_DIALOGFACTORY->getWidget(ACC_DialogFactory::WidgetNavigation, NULL);
    QModelIndex idx = wgt->getCurrentModelIndex(ACC_ModelFactory::ModelCustomer);
    mModel->slotParentCurrentRowChanged(idx, QModelIndex());
    slotParentSelectionChanged(idx, QModelIndex());

    // TODO: if MaterialManagement tables exist and stock management true
    // this->leCurrentDispatch->setEnabled(false);

    // testing only, old
    // RB_SignalSpyDialog* ssd = new RB_SignalSpyDialog(this, tvOrders /*mModel*/);
    // ssd->show();

    // testing only, new
    // ACC_DIALOGFACTORY->setSignalSpyTarget(tvOrders);
}

bool ACC_SalesOrderWidget::fileSave(bool withSelect) {
    beforeFileSave();
    if (withSelect) {
        mChildModel->submitAll();
        mModel->submitAllAndSelect();
    } else {
        mChildModel->submitAll();
        mModel->submitAll();
    }
    setWindowModified(false);
    return true;
}

void ACC_SalesOrderWidget::fileRevert() {
    mChildModel->revertAll();
    mModel->revertAll();
    setWindowModified(false);
}

/**
 * Button add order clicked
 */
void ACC_SalesOrderWidget::on_pbAddOrder_clicked() {
    if (!mModel) return;

    QModelIndex parIdx = mParentModel->getProxyIndex();

    if (!parIdx.isValid()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("Order cannot be added, \n"
                                                   "no customer selected."));
        return;
    }

    if (isWindowModified()) {
        fileSave(true);
    }

    mModel->undoFilterSort();

    // Get next sequence number
    ACC_SqlCommonFunctions f;
    int orderNo = f.getNextTransNo(ACC2::TransSalesOrder, "none", "Sales Order");

    if (orderNo < 0) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "ACC_SalesOrderWidget::on_pbAddOrder_clicked() orderNo ERROR");
        return;
    }

    // always insert at the beginning
    int row = 0; // mModel->rowCount();
    mModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex orderIdx = mModel->index(row, mModel->fieldIndex("shipvia_id"));
    mModel->setData(orderIdx, "0", Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("fromstkloc_id"));
    mModel->setData(orderIdx, "0", Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("deliverblind"));
    mModel->setData(orderIdx, 0, Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("quotation"));
    mModel->setData(orderIdx, 0, Qt::EditRole);
//    setExchangeRate();
//    orderIdx = mModel->index(row, mModel->fieldIndex("rate"));
//    mModel->setData(orderIdx, mExchangeRate, Qt::EditRole);
    // end NOTE

    tvOrders->setCurrentIndex(mModel->index(row, mModel->fieldIndex("orderno"), QModelIndex()));

    QModelIndex custIdx = mParentModel->index(parIdx.row(), mParentModel->fieldIndex("mname"), parIdx.parent());
    orderIdx = mModel->index(row, mModel->fieldIndex("deliverto"));
    mModel->setData(orderIdx, custIdx.data(Qt::DisplayRole), Qt::EditRole);
    custIdx = mParentModel->index(parIdx.row(), mParentModel->fieldIndex("phoneno"), parIdx.parent());
    orderIdx = mModel->index(row, mModel->fieldIndex("contactphone"));
    mModel->setData(orderIdx, custIdx.data(Qt::DisplayRole), Qt::EditRole);
    custIdx = mParentModel->index(parIdx.row(), mParentModel->fieldIndex("email"), parIdx.parent());
    orderIdx = mModel->index(row, mModel->fieldIndex("contactemail"));
    mModel->setData(orderIdx, custIdx.data(Qt::DisplayRole), Qt::EditRole);
    custIdx = mParentModel->index(parIdx.row(), mParentModel->fieldIndex("brstreetaddress1"), parIdx.parent());
    orderIdx = mModel->index(row, mModel->fieldIndex("deladd1"));
    mModel->setData(orderIdx, custIdx.data(Qt::DisplayRole), Qt::EditRole);
    custIdx = mParentModel->index(parIdx.row(), mParentModel->fieldIndex("brstreetaddress2"), parIdx.parent());
    orderIdx = mModel->index(row, mModel->fieldIndex("deladd2"));
    mModel->setData(orderIdx, custIdx.data(Qt::DisplayRole), Qt::EditRole);
    custIdx = mParentModel->index(parIdx.row(), mParentModel->fieldIndex("brstreetaddress3"), parIdx.parent());
    orderIdx = mModel->index(row, mModel->fieldIndex("deladd3"));
    mModel->setData(orderIdx, custIdx.data(Qt::DisplayRole), Qt::EditRole);
    custIdx = mParentModel->index(parIdx.row(), mParentModel->fieldIndex("brstreetaddress4"), parIdx.parent());
    orderIdx = mModel->index(row, mModel->fieldIndex("deladd4"));
    mModel->setData(orderIdx, custIdx.data(Qt::DisplayRole), Qt::EditRole);
    custIdx = mParentModel->index(parIdx.row(), mParentModel->fieldIndex("brstreetaddress5"), parIdx.parent());
    orderIdx = mModel->index(row, mModel->fieldIndex("deladd5"));
    mModel->setData(orderIdx, custIdx.data(Qt::DisplayRole), Qt::EditRole);
    custIdx = mParentModel->index(parIdx.row(), mParentModel->fieldIndex("brstreetaddress6"), parIdx.parent());
    orderIdx = mModel->index(row, mModel->fieldIndex("deladd6"));
    mModel->setData(orderIdx, custIdx.data(Qt::DisplayRole), Qt::EditRole);
    custIdx = mParentModel->index(parIdx.row(), mParentModel->fieldIndex("defaultlocation_id"), parIdx.parent());
    orderIdx = mModel->index(row, mModel->fieldIndex("fromstkloc_id"));
    mModel->setData(orderIdx, custIdx.data(RB2::RoleOrigData), Qt::EditRole);
    custIdx = mParentModel->index(parIdx.row(), mParentModel->fieldIndex("defaultshipvia_id"), parIdx.parent());
    orderIdx = mModel->index(row, mModel->fieldIndex("shipvia_id"));
    mModel->setData(orderIdx, custIdx.data(RB2::RoleOrigData), Qt::EditRole);

    QDate dt = QDate::currentDate();
    orderIdx = mModel->index(row, mModel->fieldIndex("orddate"));
    mModel->setData(orderIdx, dt.toString(Qt::ISODate), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("deliverydate"));
    mModel->setData(orderIdx, dt.toString(Qt::ISODate), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("orderno"));
    mModel->setData(orderIdx, orderNo, Qt::EditRole);

    // tvOrders->setCurrentIndex(mModel->index(row, RB2::HIDDENCOLUMNS));
    tvOrders->scrollTo(tvOrders->currentIndex());
    twOrderDetails->setCurrentIndex(0);
    twItemDetails->setCurrentIndex(0);
    leDeliverTo->setFocus();
    leDeliverTo->selectAll();
}

/**
 * Button delete clicked
 */
void ACC_SalesOrderWidget::on_pbDeleteOrder_clicked() {
    int ret = ACC_DIALOGFACTORY->requestYesNoDialog(
            tr("Delete order ..."),
            tr("This action cannot be undone.\n"
               "Do you want to delete this order?"));
    if (ret == QMessageBox::No) {
        ACC_DIALOGFACTORY->statusBarMessage(tr("Deletion canceled"), 2000);
        return;
    }

    if (!mModel) return;

    QModelIndex index = tvOrders->currentIndex();
    if (index.isValid()) {
        mModel->removeRows(index.row(), 1, QModelIndex());
        fileSave(true);
        setWindowModified(false);
    }
}

/**
 * Button delete clicked
 */
void ACC_SalesOrderWidget::on_pbSaveOrder_clicked() {
    fileSave(true);
}

/**
 * Button revert/undo clicked
 */
void ACC_SalesOrderWidget::on_pbRevertOrder_clicked() {
    fileRevert();
}

void ACC_SalesOrderWidget::on_pbCopyOrder_clicked() {
    if (isWindowModified()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("Save data first"));
        return;
    }

    if (!tvOrders->currentIndex().isValid()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No order selected"));
        return;
    }

    int ret = ACC_DIALOGFACTORY->requestYesNoDialog(tr("Copy order/quotation"),
                                                    tr("Do you want to copy the selected\n"
                                                       "order or quotation?"));
    if (ret != QMessageBox::Yes) {
        return;
    }

    // Create objects from selected order, do not delete the object,
    // it will be refreshed with new data and children
    RB_ObjectBase* order = mModel->getObject(tvOrders->currentIndex(), RB2::ResolveAll);
    createNewOrder(order);

    leDeliverTo->setFocus();
    leDeliverTo->selectAll();
}

/**
 * Button select item clicked
 */
void ACC_SalesOrderWidget::on_ileContact_clicked() {
    if (!tvOrders->currentIndex().isValid()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No sales order selected.\n"
                                                   "Please select an order first."));
        return;
    }

    RB_Dialog* dlg = ACC_DIALOGFACTORY->getDialog(ACC_DialogFactory::DialogSelectContact);
    int res = dlg->exec();

    RB_ObjectBase* obj = dlg->getCurrentObject(); // was  index

    if (res == QDialog::Accepted && obj) {
        RB_MmProxy* m = mModel;
        int row = tvOrders->currentIndex().row();

        QModelIndex idx = m->index(row, m->fieldIndex("contact_idx"));
        m->setData(idx, obj->getId() + obj->getValue("firstname").toString()
                   + " " + obj->getValue("lastname").toString(), Qt::EditRole);
        idx = mModel->index(row, mModel->fieldIndex("contactphone"));
        mModel->setData(idx, obj->getValue("phonework"), Qt::EditRole);
        idx = mModel->index(row, mModel->fieldIndex("contactemail"));
        mModel->setData(idx, obj->getValue("email1"), Qt::EditRole);
    }

    dlg->deleteLater();
}

void ACC_SalesOrderWidget::on_ileContact_clear() {
    if (!tvOrders->currentIndex().isValid()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No sales order selected.\n"
                                                   "Please select an order first."));
        return;
    }

    RB_MmProxy* m = mModel;
    int row = tvOrders->currentIndex().row();

    QModelIndex idx = m->index(row, m->fieldIndex("contact_idx"));
    m->setData(idx, "0", Qt::EditRole);
    idx = mModel->index(row, mModel->fieldIndex("contactphone"));
    mModel->setData(idx, "", Qt::EditRole);
    idx = mModel->index(row, mModel->fieldIndex("contactemail"));
    mModel->setData(idx, "", Qt::EditRole);
}

/**
 * Button add order detail clicked
 */
void ACC_SalesOrderWidget::on_pbAddDetail_clicked() {
    QModelIndex orderIdx = tvOrders->currentIndex();
    if (!mChildModel || !orderIdx.isValid()) return; // ACC_SalesOrderDetail
    mChildModel->undoFilterSort();

    // always insert at the end
    int row = mChildModel->rowCount();
    mChildModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex idx = mChildModel->index(row, mChildModel->fieldIndex("stk_idx"));
    mChildModel->setData(idx, '0', Qt::EditRole);

//    idx = mChildModel->index(row, mChildModel->fieldIndex("unitprice"));
//    mChildModel->setData(idx, 0.0, Qt::EditRole);
//    idx = mChildModel->index(row, mChildModel->fieldIndex("quantity"));
//    mChildModel->setData(idx, 0.0, Qt::EditRole);
//    idx = mChildModel->index(row, mChildModel->fieldIndex("qtyinvoiced"));
//    mChildModel->setData(idx, 0.0, Qt::EditRole);
//    idx = mChildModel->index(row, mChildModel->fieldIndex("qtydispatched"));
//    mChildModel->setData(idx, 0.0, Qt::EditRole);
    // HACK:
    leUnitPrice->setText("0"); // "unitprice"
    leQuantity->setText("0"); // "quantity"
    leDiscountPercent->setText("0"); // "discountpercent"
    leAlreadyDispatched->setText("0"); // "qtyinvoiced"
    leCurrentDispatch->setText("0"); // "qtydispatched"

    idx = mChildModel->index(row, mChildModel->fieldIndex("estimate"));
    mChildModel->setData(idx, 0.0, Qt::EditRole);
    idx = mChildModel->index(row, mChildModel->fieldIndex("completed"));
    mChildModel->setData(idx, 0, Qt::EditRole);
    idx = mChildModel->index(row, mChildModel->fieldIndex("taxcat_id"));
    mChildModel->setData(idx, '0', Qt::EditRole);
    // end NOTE

    idx = mChildModel->index(row, mChildModel->fieldIndex("itemdue"));
    mChildModel->setData(idx, QDate::currentDate().toString(Qt::ISODate),
                         Qt::EditRole);

    tvDetails->setCurrentIndex(mChildModel->index(row,
                               mChildModel->fieldIndex("stk_idx"), QModelIndex()));
    tvDetails->scrollTo(tvDetails->currentIndex());
    twItemDetails->setCurrentIndex(0);
    ileStockCode->setFocus();
}

/**
 * Button delete order detail clicked
 */
void ACC_SalesOrderWidget::on_pbDeleteDetail_clicked() {
    int ret = ACC_DIALOGFACTORY->requestYesNoDialog(
            tr("Delete item ..."),
            tr("This action cannot be undone.\n"
               "Do you want to delete this item?"));
    if (ret == QMessageBox::No) {
        ACC_DIALOGFACTORY->statusBarMessage(tr("Deletion canceled"), 2000);
        return;
    }

    if (!mChildModel) return;

    QModelIndex index = tvDetails->currentIndex();
    if (index.isValid()) {
        mChildModel->removeRows(index.row(), 1, QModelIndex());
        mChildModel->submitAllAndSelect();
        // no setModified here because parent could have changed
    }
}

/**
 * Button select item clicked
 */
void ACC_SalesOrderWidget::on_ileStockCode_clicked() {
    if (!tvDetails->currentIndex().isValid()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No sales order item selected.\n"
                                                   "Please select an order item first."));
        return;
    }

    RB_Dialog* dlga = ACC_DIALOGFACTORY->getDialog(ACC_DialogFactory::DialogSelectStockMaster);
    ACC_SelectStockItemDialog* dlg = dynamic_cast<ACC_SelectStockItemDialog*>(dlga);
    int res = dlg->exec();

    QModelIndex index = dlg->getCurrentChild1ModelIndex();
    if (res == QDialog::Accepted && index.isValid()) {
        RB_MmProxy* m = mChildModel;
        // int row = m->getProxyIndex().row();
        int row = tvDetails->currentIndex().row();

        QModelIndex idx = m->index(row, m->fieldIndex("stk_idx"));
        m->setData(idx, dlg->getStockId() + dlg->getCode(), Qt::EditRole);
        idx = m->index(row, m->fieldIndex("stkdescription"));
        m->setData(idx, dlg->getDescription(), Qt::EditRole);
        idx = m->index(row, m->fieldIndex("stkuom"));
        m->setData(idx, dlg->getUnitOfMeasurement(), Qt::EditRole);
        idx = m->index(row, m->fieldIndex("serialno"));
        m->setData(idx, dlg->getSerialNumber(), Qt::EditRole);
        idx = m->index(row, m->fieldIndex("unitprice"));
        m->setData(idx, dlg->getPrice(), Qt::EditRole);
        idx = m->index(row, m->fieldIndex("quantity"));
        m->setData(idx, 0.0, Qt::EditRole);
        idx = m->index(row, m->fieldIndex("narrative"));
        m->setData(idx, RB_String(""), Qt::EditRole);
        leDiscountPercent->setText("0");
        leTotalItemPrice->setText("0.00");
        idx = m->index(row, m->fieldIndex("taxcat_id"));
        m->setData(idx, dlg->getTaxCatId(), Qt::EditRole);
        idx = m->index(row, m->fieldIndex("costcenter_idx"));
        m->setData(idx, dlg->getCostCenterIdx(), Qt::EditRole);
    }

    dlg->deleteLater();
}

void ACC_SalesOrderWidget::on_ileStockCode_clear() {
    if (!tvDetails->currentIndex().isValid()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No sales order item selected.\n"
                                                   "Please select an order item first."));
        return;
    }

    RB_MmProxy* m = mChildModel;
    int row = tvDetails->currentIndex().row();

    // QModelIndex idx = m->index(row, m->fieldIndex("stk_idx")); already by ileStockCode
    // m->setData(idx, dlg->getStockId()+ dlg->getCode(), Qt::EditRole);
    QModelIndex idx = m->index(row, m->fieldIndex("stkdescription"));
    m->setData(idx, RB_String(""), Qt::EditRole);
    idx = m->index(row, m->fieldIndex("stkuom"));
    m->setData(idx, RB_String(""), Qt::EditRole);
    idx = m->index(row, m->fieldIndex("serialno"));
    m->setData(idx, RB_String(""), Qt::EditRole);
    idx = m->index(row, m->fieldIndex("unitprice"));
    m->setData(idx, 0.0, Qt::EditRole);
    idx = m->index(row, m->fieldIndex("quantity"));
    m->setData(idx, 0.0, Qt::EditRole);
    idx = m->index(row, m->fieldIndex("narrative"));
    m->setData(idx, RB_String(""), Qt::EditRole);
    leDiscountPercent->setText("0");
    leTotalItemPrice->setText("0.00");
    idx = m->index(row, m->fieldIndex("taxcat_id"));
    m->setData(idx, "0", Qt::EditRole);
    idx = m->index(row, m->fieldIndex("costcenter_idx"));
    m->setData(idx, "0", Qt::EditRole);
}

void ACC_SalesOrderWidget::on_pbPreviewPrintQuotation_clicked() {
    if (!tvOrders->currentIndex().isValid()) {
        ACC_DIALOGFACTORY->requestInformationDialog(tr("No order selected."));
        return;
    }

    if (isWindowModified()) {
        ACC_DIALOGFACTORY->requestInformationDialog(tr("Save your data first."));
        return;
    }

    if (cbQuotationOnly->currentIndex() != 1) {
        ACC_DIALOGFACTORY->requestInformationDialog(tr("Note: not a quotation,\n"
                                                       "this is a sales order."));
    }

    bool isOrder;

    if (mModel->getCurrentValue("quotation", Qt::EditRole) != 1) {
        isOrder = true;
    } else {
        isOrder = false;
    }

    ACC_SalesQuotationHtml oper;
    oper.setIsOrder(isOrder);

    if (!oper.execute(mModel->getObject(tvOrders->currentIndex(), RB2::ResolveAll))) {
        // Mistake, for example: the GL account of supplier is not set
        ACC_DIALOGFACTORY->requestWarningDialog(tr("Could not process the order. \n"
                                                   "For example: order is completed, \n"
                                                   "order has no items or \n"
                                                   "value of items is not set."));
        return;
    }

    RB_String fileName;

    if (isOrder) {
        fileName = "so_" + leOrderNo->text();
    } else {
        fileName = "sq_" + leOrderNo->text();
    }

    // Show preview invoice dialog
    ACC_OrderPreviewDialog* dlg = new ACC_OrderPreviewDialog(this);
    dlg->setWindowTitle("ACC - Quotation");
    dlg->setCurrentFileName(fileName);
    dlg->setHtml(oper.getHtml());
    dlg->exec();
    dlg->deleteLater();

}


/**
 * Button preview invoice clicked
 */
void ACC_SalesOrderWidget::on_pbPreviewInvoice_clicked() {
    if (!tvOrders->currentIndex().isValid()) {
        ACC_DIALOGFACTORY->requestInformationDialog(tr("No order selected."));
        return;
    }

    if (isWindowModified()) {
        ACC_DIALOGFACTORY->requestInformationDialog(tr("Save your data first."));
        return;
    }

    if (cbQuotationOnly->currentIndex() != 0) {
        ACC_DIALOGFACTORY->requestInformationDialog(tr("Quotation only."));
        return;
    }

    // Translate sales order in transaction document and debtor details
    if (translateSalesOrder(false) == QDialog::Rejected) {
        // Canceled or nothing to invoice
        return;
    }

    ACC_SalesInvoiceHtml oper;
    if (!oper.execute(mTransDoc, mDebtorTransList)) {
        QApplication::restoreOverrideCursor();

        // Mistake, for example: the GL account of customer is not set
        ACC_DIALOGFACTORY->requestWarningDialog(tr("Could not preview the invoice. \n"
                                                   "For example:\n"
                                                   "- sales GL posting of product category,\n"
                                                   "- sales area, \n"
                                                   "- sales type, \n"
                                                   "- company tax province, \n"
                                                   "- customer tax group or \n"
                                                   "- delivery location is not set."));
        return;
    }

    // Show preview invoice dialog
    ACC_OrderPreviewDialog* dlg = new ACC_OrderPreviewDialog(this);
    dlg->setWindowTitle("ACC - Preview Invoice");
    dlg->setCurrentFileName("so_invoice_" + mTransDoc->getValue("transno").toString());
    dlg->setHtml(oper.getHtml());
    dlg->exec();
    dlg->deleteLater();

}

/**
 * Button confirm dispatch and process invoice clicked
 */
void ACC_SalesOrderWidget::on_pbProcessInvoice_clicked() {
    if (!tvOrders->currentIndex().isValid()) {
        ACC_DIALOGFACTORY->requestInformationDialog(tr("No order selected."));
        return;
    }

    if (isWindowModified()) {
        ACC_DIALOGFACTORY->requestInformationDialog(tr("Save your data first."));
        return;
    }

    if (cbQuotationOnly->currentIndex() != 0) { // not No is Yes
        ACC_DIALOGFACTORY->requestInformationDialog(tr("Quotation only."));
        return;
    }

    // Translate sales order in transaction document and debtor details
    if (translateSalesOrder(true) == QDialog::Rejected) {
        // Canceled or nothing to invoice
        return;
    }

    setExchangeRate();

    // Rate could have been changed in the mean time
//    QModelIndex orderIdx = tvOrders->currentIndex();
//    if (orderIdx.isValid()) {
//        orderIdx = mModel->index(orderIdx.row(), mModel->fieldIndex("rate"));
//        mModel->setData(orderIdx, mExchangeRate, Qt::EditRole);
//    }

    // Process invoice and post debtor and general ledger transactions
    // and handle exchange rate of customer currency
    ACC_ProcessTransDoc operTransDoc;
    if (!operTransDoc.execute(mTransDoc, mDebtorTransList, mExchangeRate)) {
        // Mistake, for example: the GL account of customer is not set
        ACC_DIALOGFACTORY->requestWarningDialog(tr("Could not process the invoice. \n"
                                                   "For example:\n"
                                                   "- sales GL posting of product category,\n"
                                                   "- sales area, \n"
                                                   "- sales type, \n"
                                                   "- company tax province, \n"
                                                   "- customer tax group or \n"
                                                   "- delivery location is not set."));
        return;
    }

    // Process stock moves the TODO to be part of material management
    /* From ACC_ProcessInvoice based on stockmoves from webERP, not used anymore
     *
     * Execute debtor, stock and General Ledger transactions
     *
     * -> Salesorder, DONE
     * 1-Update sales order comment
     *
     * -> TODO: Stockmoves and sales
     * 21-Update salesorderdetail quantities
     * 22-Insert orderdeliverydifferenceslog
     * 23-Update salesorderdetail for quantity invoiced and actual dispatch dates
     * 24-Update location stock records if not a dummy stock item M, B (MBflag Make, Buy)
     *    else A (Assembly)
     * 25-Insert stockmoves, different for M, B flag and A flag
     * 26-Insert stockmovestaxes
     * 27-Insert stockserialmovements
     * 28-Insert and update salesanalysis records
     *
     * -> General Ledger, DONE
     * 31-Cost of sales
     * 32-Stock
     * 33-Net sales
     * 34-Discount
     * 35-Gross sales
     * 36-Freight
     * 37-Tax
     *
     * @returns true on success
     */


    // Show final invoice dialog
    ACC_OrderPreviewDialog* dlg = new ACC_OrderPreviewDialog(this);
    dlg->setWindowTitle("ACC - Invoice");

    ACC_SalesInvoiceHtml oper;
    oper.execute(mTransDoc, mDebtorTransList);
    dlg->setCurrentFileName("so_invoice_" + mTransDoc->getValue("transno").toString());
    dlg->setHtml(oper.getHtml());
    dlg->exec();
    dlg->deleteLater();
}

/**
 * Translate sales order in transaction document,
 * create price salestaxes per line item for confirmation
 * @param isPost false if this is only to preview the invoice
 */
int ACC_SalesOrderWidget::translateSalesOrder(bool isPost) {
    QApplication::setOverrideCursor(Qt::WaitCursor);

    // Reset debtor transactions
    if (mDebtorTransList) {
        delete mDebtorTransList;
    }
    mDebtorTransList = new RB_ObjectContainer(RB_Uuid::createUuid().toString(), NULL,
                                 "ACC_DebtorTransList", ACC_OBJECTFACTORY);

    // Reset document
    if (mTransDoc) {
        delete mTransDoc;
    }

    mTransDoc = ACC_OBJECTFACTORY->newSingleObject("ACC_TransDoc");

    // Set transaction document data and relevant taxes of this order
    ACC_SoTransDoc oper;
    if (!oper.preparePreview(mTransDoc, mDebtorTransList)) {
        QApplication::restoreOverrideCursor();
        ACC_DIALOGFACTORY->requestWarningDialog(tr("Could not preview the invoice. \n"
                                                   "For example:\n"
                                                   "- sales GL posting of product category,\n"
                                                   "- sales area, \n"
                                                   "- sales type, \n"
                                                   "- company tax province, \n"
                                                   "- customer tax group or \n"
                                                   "- delivery location\n"
                                                   "is not set."));
        return QDialog::Rejected;
    }

    // Check whether there are amounts to be invoiced
    double invoiceAmount = 0.0;
    RB_ObjectIterator* iterDt = mDebtorTransList->createIterator();

    for (iterDt->first(); !iterDt->isDone(); iterDt->next()) {
        RB_ObjectBase* debtorTrans = iterDt->currentObject();
        invoiceAmount += debtorTrans->getValue("amount").toDouble();
    }

    delete iterDt;

    if (invoiceAmount <= 0.0) {
        QApplication::restoreOverrideCursor();
        ACC_DIALOGFACTORY->requestWarningDialog(tr("There are no quantities to invoice\n"
                                                   "or the salesorder is completed."));
        return QDialog::Rejected;
    }

    QApplication::restoreOverrideCursor();

    // Show tax confirmation dialog if not for posting invoice
    bool isSalesOrder = true;
    ACC_TaxConfirmationDialog* dlg = new ACC_TaxConfirmationDialog(this);
    dlg->setTransModel(mTransDoc, mDebtorTransList, isSalesOrder);
    dlg->init();
    int result = dlg->exec();
    dlg->deleteLater();

    if (isPost && result == QDialog::Accepted) {
        // Get invoice number and update quantities in order detail items
        oper.preparePost(mTransDoc);
    }

    return result;
}

/**
 * Create new order. Use by button Copy Order
 * @param order original order to be copied
 */
void ACC_SalesOrderWidget::createNewOrder(RB_ObjectBase* order) {
    on_pbAddOrder_clicked();

    // always insert at the beginning
    int row = 0; // mModel->rowCount();
    QModelIndex orderIdx;

    // orderIdx = mModel->index(row, mModel->fieldIndex("orderno"));
    // mModel->setData(orderIdx, order->getValue("orderno"), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("customerref"));
    mModel->setData(orderIdx, order->getValue("customerref"), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("buyername"));
    mModel->setData(orderIdx, order->getValue("buyername"), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("ordertitle"));
    mModel->setData(orderIdx, order->getValue("ordertitle"), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("comments"));
    mModel->setData(orderIdx, order->getValue("comments"), Qt::EditRole);
    // orderIdx = mModel->index(row, mModel->fieldIndex("orddate"));
    // mModel->setData(orderIdx, order->getValue("orddate"), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("ordertype"));
    mModel->setData(orderIdx, order->getValue("ordertype"), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("shipvia_id"));
    mModel->setData(orderIdx, order->getValue("shipvia_id").toString(), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("deladd1"));
    mModel->setData(orderIdx, order->getValue("deladd1"), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("deladd2"));
    mModel->setData(orderIdx, order->getValue("deladd2"), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("deladd3"));
    mModel->setData(orderIdx, order->getValue("deladd3"), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("deladd4"));
    mModel->setData(orderIdx, order->getValue("deladd4"), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("deladd5"));
    mModel->setData(orderIdx, order->getValue("deladd5"), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("deladd6"));
    mModel->setData(orderIdx, order->getValue("deladd6"), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("contact_idx"));
    mModel->setData(orderIdx, order->getValue("contact_idx").toString(), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("contactphone"));
    mModel->setData(orderIdx, order->getValue("contactphone"), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("contactemail"));
    mModel->setData(orderIdx, order->getValue("contactemail"), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("deliverto"));
    mModel->setData(orderIdx, order->getValue("deliverto"), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("deliverblind"));
    mModel->setData(orderIdx, order->getValue("deliverblind"), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("freightcost"));
    mModel->setData(orderIdx, order->getValue("freightcost"), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("fromstkloc_id"));
    mModel->setData(orderIdx, order->getValue("fromstkloc_id"), Qt::EditRole);
    // orderIdx = mModel->index(row, mModel->fieldIndex("deliverydate"));
    // mModel->setData(orderIdx, order->getValue("deliverydate"), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("quotedate"));
    mModel->setData(orderIdx, order->getValue("quotedate"), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("confirmeddate"));
    mModel->setData(orderIdx, order->getValue("confirmeddate"), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("printedpackingslip"));
    mModel->setData(orderIdx, order->getValue("printedpackingslip"), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("datepackingslipprinted"));
    mModel->setData(orderIdx, order->getValue("datepackingslipprinted"), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("quotation"));
    mModel->setData(orderIdx, order->getValue("quotation"), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("invoicecomment"));
    mModel->setData(orderIdx, order->getValue("invoicecomment"), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("consignment"));
    mModel->setData(orderIdx, order->getValue("consignment"), Qt::EditRole);

    RB_ObjectContainer* detailList = order->getContainer("ACC_SalesOrderDetailList");

    if (!detailList) {
        RB_DEBUG->error("ACC_SalesOrderWidget::createNewOrder() detail list ERROR");
        return;
    }

    QModelIndex detailIdx;
    RB_ObjectIterator* iter = detailList->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* orderDetail = iter->currentObject();

        on_pbAddDetail_clicked();
        // always insert at the end
        row = mChildModel->rowCount() - 1;

        detailIdx = mChildModel->index(row, mChildModel->fieldIndex("stk_idx"));
        mChildModel->setData(detailIdx, orderDetail->getValue("stk_idx").toString(), Qt::EditRole);
        detailIdx = mChildModel->index(row, mChildModel->fieldIndex("stkdescription"));
        mChildModel->setData(detailIdx, orderDetail->getValue("stkdescription"), Qt::EditRole);
        detailIdx = mChildModel->index(row, mChildModel->fieldIndex("stkuom"));
        mChildModel->setData(detailIdx, orderDetail->getValue("stkuom"), Qt::EditRole);
        detailIdx = mChildModel->index(row, mChildModel->fieldIndex("unitprice"));
        mChildModel->setData(detailIdx, orderDetail->getValue("unitprice"), Qt::EditRole);
        detailIdx = mChildModel->index(row, mChildModel->fieldIndex("quantity"));
        mChildModel->setData(detailIdx, orderDetail->getValue("quantity"), Qt::EditRole);
        // detailIdx = mChildModel->index(row, mChildModel->fieldIndex("qtyinvoiced"));
        // mChildModel->setData(detailIdx, orderDetail->getValue("qtyinvoiced"), Qt::EditRole);
        // detailIdx = mChildModel->index(row, mChildModel->fieldIndex("qtydispatched"));
        // mChildModel->setData(detailIdx, orderDetail->getValue("qtydispatched"), Qt::EditRole);
        detailIdx = mChildModel->index(row, mChildModel->fieldIndex("estimate"));
        mChildModel->setData(detailIdx, orderDetail->getValue("estimate"), Qt::EditRole);
        detailIdx = mChildModel->index(row, mChildModel->fieldIndex("discountpercent"));
        mChildModel->setData(detailIdx, orderDetail->getValue("discountpercent"), Qt::EditRole);
        // detailIdx = mChildModel->index(row, mChildModel->fieldIndex("actualdispatchdate"));
        // mChildModel->setData(detailIdx, orderDetail->getValue("actualdispatchdate"), Qt::EditRole);
        // detailIdx = mChildModel->index(row, mChildModel->fieldIndex("completed"));
        // mChildModel->setData(detailIdx, orderDetail->getValue("completed"), Qt::EditRole);
        detailIdx = mChildModel->index(row, mChildModel->fieldIndex("narrative"));
        mChildModel->setData(detailIdx, orderDetail->getValue("narrative"), Qt::EditRole);
        // detailIdx = mChildModel->index(row, mChildModel->fieldIndex("itemdue"));
        // mChildModel->setData(detailIdx, orderDetail->getValue("itemdue"), Qt::EditRole);
        detailIdx = mChildModel->index(row, mChildModel->fieldIndex("poline"));
        mChildModel->setData(detailIdx, orderDetail->getValue("poline"), Qt::EditRole);
        detailIdx = mChildModel->index(row, mChildModel->fieldIndex("taxcat_id"));
        mChildModel->setData(detailIdx, orderDetail->getValue("taxcat_id"), Qt::EditRole);
        detailIdx = mChildModel->index(row, mChildModel->fieldIndex("costcenter_idx"));
        mChildModel->setData(detailIdx, orderDetail->getValue("costcenter_idx").toString(), Qt::EditRole);
    }

    delete iter;
}

/**
 * Set exchange rate based on the currency of the customer
 */
void ACC_SalesOrderWidget::setExchangeRate() {
    if (mCurrencyId.size() < 38) {
        mExchangeRate = 1.0;
    } else {
        ACC_SqlCommonFunctions f;
        mExchangeRate = f.selectFromWhereId("rate", "ACC_Currency", mCurrencyId).toDouble();
    }
}

/**
 * Slot to set name of supplier
 */
void ACC_SalesOrderWidget::slotParentSelectionChanged(const QModelIndex& current,
                                                       const QModelIndex& /*previous*/) {
    if (current.isValid()) {
        leCustomer->setText(mParentModel->getCurrentValue("mname").toString());
        mCurrencyId = mParentModel->getCurrentValue("currency_id", RB2::RoleOrigData).toString();
    } else {
        leCustomer->setText("");
        mCurrencyId = "";
    }
}

/**
 * Slot calculate item total based on selected item changed
 */
void ACC_SalesOrderWidget::slotItemSelectionChanged(const QModelIndex& /*current*/,
                                                     const QModelIndex& /* previous */) {
    slotCalculateItemTotal();
}

/**
 * Slot calculate item total price based on quantity, unit price and discount
 */
void ACC_SalesOrderWidget::slotCalculateItemTotal() {
    double price = leUnitPrice->text().toDouble();
    double qty = leQuantity->text().toDouble();
    double discount = leDiscountPercent->text().toDouble() / 100.0;

    if (0.0 <= discount && discount <= 1.0 && qty >= 0.0 && price >= 0.0) {
        double totalPrice = price * qty * (1.0 - discount);
        leTotalItemPrice->setText(RB_String::number(totalPrice));
    } else if (discount == 0.0 && qty >= 0.0 && price < 0.0) {
        leTotalItemPrice->setText("discount");
    } else {
        leTotalItemPrice->setText("error");
    }

    // Repeat value
    leOrderedQuantity->setText(leQuantity->text());
}

/**
 * @returns the help subject, in this case the widget objectname
 */
RB_String ACC_SalesOrderWidget::getHelpSubject() const {
    return objectName();
}

bool ACC_SalesOrderWidget::closeWidget() {
    RB_Widget::closeWidget();
    if (maybeSave()) {
        return true;
    }
    return false;
}

/**
 * Change event, for example translation
 */
void ACC_SalesOrderWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
