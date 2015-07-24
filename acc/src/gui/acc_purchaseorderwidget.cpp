/*****************************************************************
 * $Id: acc_purchaseorderwidget.cpp 2217 2015-02-13 18:18:14Z rutger $
 * Created: Jan 20, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_purchaseorderwidget.h"

#include "acc.h"
#include "acc_dialogfactory.h"
#include "acc_purchaseorderhtml.h"
#include "acc_modelfactory.h"
#include "acc_objectfactory.h"
#include "acc_orderpreviewdialog.h"
#include "acc_potransdoc.h"
#include "acc_processtransdoc.h"
#include "acc_selectstockitemdialog.h"
#include "acc_sqlcommonfunctions.h"
#include "acc_taxconfirmationdialog.h"
#include "rb_datawidgetmapper.h"
#include "rb_mdiwindow.h"
#include "rb_sqlrelationaldelegate.h"

/**
 * Constructor
 */
ACC_PurchaseOrderWidget::ACC_PurchaseOrderWidget(QWidget *parent)
                        : RB_Widget(parent) {

    setupUi(this);

    mModel = NULL;
    mChildModel = NULL;
    mParentModel = NULL;
    mMapper = NULL;
    mChildMapper = NULL;

    mCreditorTransList = NULL;
    mTransDoc = NULL;

    mCurrencyId = "";
    mExchangeRate = 1.0;
}

/**
 * Destructor
 */
ACC_PurchaseOrderWidget::~ACC_PurchaseOrderWidget() {
    // unique models are always deleted by the widgets,
    // can be MDI window or dialog, but also a dockWindow with table or tree
    delete mCreditorTransList;
    delete mTransDoc;
    // mappers are deleted by the models
    delete mChildModel;
    delete mModel;

    RB_DEBUG->print("ACC_PurchaseOrderWidget::~ACC_PurchaseOrderWidget() OK");
}

/**
 * Initialize widget and models
 */
void ACC_PurchaseOrderWidget::init() {

    //
    // Set parent model, customer, for name only
    //
    mParentModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelSupplier);
    connect(mParentModel, SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)),
            this, SLOT(slotParentSelectionChanged(QModelIndex,QModelIndex)));

    //
    // 1. Set model (not ID, is set by parent model) and/or query
    //
    mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelPurchaseOrder);

    //
    // 2. Set relations and mapper for line edits etc.
    //
    mMapper = mModel->getMapper();

    // tab Receipt
    mMapper->addMapping(leOrderNo, mModel->fieldIndex("orderno"));
    mMapper->addMapping(this->leRequisitionNo, mModel->fieldIndex("requisitionno"));
    addComboBoxMapping(mModel,"receiptat_id", "ACC_Location", "id",
                       "locationname", cbWarehouse, mMapper);
    mMapper->addMapping(dteOrderDate, mModel->fieldIndex("orderdate"));
    mMapper->addMapping(ileContact, mModel->fieldIndex("contact_idx"));
    mMapper->addMapping(leContactPhoneNumber, mModel->fieldIndex("contactphone"));
    mMapper->addMapping(leContactEmail, mModel->fieldIndex("contactemail"));
    mMapper->addMapping(leSupplierReference, mModel->fieldIndex("supplierref"));
    mMapper->addMapping(dteReceiptDate, mModel->fieldIndex("receiptdate"));
    mMapper->addMapping(leRevision, mModel->fieldIndex("revision"));
    addComboBoxMapping(mModel,"shipvia_id", "ACC_Shipper", "id",
                       "shippername", cbFreightMethod, mMapper);
    RB_StringList items;
    items.clear();
    items << tr("Draft") << tr("Approved") << tr("Ongoing")
          << tr("Hold") << tr("Partly Received") << tr("All Received")
          << tr("All Invoiced") << tr("Closed");
    cbStatus->setModel(new QStringListModel(items, this));
    mMapper->addMapping(cbStatus, mModel->fieldIndex("orderstatus"),
                        "currentIndex");
    items.clear();
    items << tr("No") << tr("Yes");
    cbQuotationOnly->setModel(new QStringListModel(items, this));
    mMapper->addMapping(cbQuotationOnly, mModel->fieldIndex("quotation"),
                            "currentIndex");
    // tab Description
    mMapper->addMapping(teComment, mModel->fieldIndex("comments"));
    mMapper->addMapping(leOrderDescription, mModel->fieldIndex("ordertitle"));

    // tab Receipt/Delivery Address
    mMapper->addMapping(leDeliveryAddress1, mModel->fieldIndex("deladd1"));
    mMapper->addMapping(leDeliveryAddress2, mModel->fieldIndex("deladd2"));
    mMapper->addMapping(leDeliveryAddress3, mModel->fieldIndex("deladd3"));
    mMapper->addMapping(leDeliveryAddress4, mModel->fieldIndex("deladd4"));
    mMapper->addMapping(leDeliveryAddress5, mModel->fieldIndex("deladd5"));
    mMapper->addMapping(leDeliveryAddress6, mModel->fieldIndex("deladd6"));
    // tab Invoice
    mMapper->addMapping(leSupplierInvoiceNo, mModel->fieldIndex("suppinvoiceno"));
    mMapper->addMapping(deSupplierInvoiceDate, mModel->fieldIndex("suppinvoicedate"));

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
    mChildModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelPurchaseOrderDetail);

    mChildMapper = mChildModel->getMapper();
    mChildMapper->addMapping(ileStockCode, mChildModel->fieldIndex("stk_idx"));
    mChildMapper->addMapping(leItemDescription, mChildModel->fieldIndex("stkdescription"));
    mChildMapper->addMapping(leUnitOfMeasurement, mChildModel->fieldIndex("stkuom"));
    mChildMapper->addMapping(leSerialNumber, mChildModel->fieldIndex("serialno"));
    mChildMapper->addMapping(leUnitPrice, mChildModel->fieldIndex("unitprice"));
    mChildMapper->addMapping(leQuantity, mChildModel->fieldIndex("quantity"));
    mChildMapper->addMapping(leDiscountPercent, mChildModel->fieldIndex("discountpercent"));
    mChildMapper->addMapping(pteNarrative, mChildModel->fieldIndex("narrative"));

    // tab Receipt
    // not mChildMapper->addMapping(leOrderedQuantity, mChildModel->fieldIndex("quantity"));
    mChildMapper->addMapping(leAlreadyDispatched, mChildModel->fieldIndex("qtyinvoiced"));
    mChildMapper->addMapping(leCurrentDispatch, mChildModel->fieldIndex("qtyreceived"));
    mChildMapper->addMapping(dteItemDue, mChildModel->fieldIndex("itemdue"));

    setFormatTableView(tvDetails, mChildModel);

    // Calculate total item price
    connect(tvDetails->selectionModel(),
            SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)),
            this, SLOT(slotItemSelectionChanged(const QModelIndex&,const QModelIndex&)));
    connect(leUnitPrice, SIGNAL(editingFinished()),
            this, SLOT(slotCalculateItemTotal()));
    connect(leQuantity, SIGNAL(editingFinished()),
            this, SLOT(slotCalculateItemTotal()));
    connect(this->leDiscountPercent, SIGNAL(editingFinished()),
            this, SLOT(slotCalculateItemTotal()));

    readSettings();

    for (int i = 0; i < mModel->columnCount(QModelIndex()); ++i) {
        if (i != RB2::HIDDENCOLUMNS && i != RB2::HIDDENCOLUMNS + 1
                && i != RB2::HIDDENCOLUMNS + 6 && i != mModel->fieldIndex("ordertitle")) {
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
            && i!= mChildModel->fieldIndex("qtyreceived")
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
    QModelIndex idx = wgt->getCurrentModelIndex(ACC_ModelFactory::ModelSupplier);
    mModel->slotParentCurrentRowChanged(idx, QModelIndex());
    slotParentSelectionChanged(idx, QModelIndex());

    // TODO: if MaterialManagement tables exist and stock management true
    // this->leCurrentDispatch->setEnabled(false);
}

bool ACC_PurchaseOrderWidget::fileSave(bool withSelect) {
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

void ACC_PurchaseOrderWidget::fileRevert() {
    mChildModel->revertAll();
    mModel->revertAll();
    setWindowModified(false);
}

/**
 * Button add order clicked
 */
void ACC_PurchaseOrderWidget::on_pbAddOrder_clicked() {
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
    int orderNo = f.getNextTransNo(ACC2::TransPurchOrder, "none", "Purchase Order");

    if (orderNo < 0) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "ACC_PurchaseOrderWidget::on_pbAddOrder_clicked() orderNo ERROR");
        return;
    }

    // always insert at the beginning
    int row = 0; // mModel->rowCount();
    mModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex orderIdx = mModel->index(row, mModel->fieldIndex("shipvia_id"));
    mModel->setData(orderIdx, "0", Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("receiptat_id"));
    mModel->setData(orderIdx, "0", Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("status"));
    mModel->setData(orderIdx, 0, Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("quotation"));
    mModel->setData(orderIdx, 0, Qt::EditRole);
    // end NOTE

    tvOrders->setCurrentIndex(mModel->index(row, mModel->fieldIndex("orderno"), QModelIndex()));

    // supplier info
    QModelIndex suppIdx;
    suppIdx= mParentModel->index(parIdx.row(), mParentModel->fieldIndex("phoneno"));
    orderIdx = mModel->index(row, mModel->fieldIndex("contactphone"));
    mModel->setData(orderIdx, suppIdx.data(Qt::DisplayRole), Qt::EditRole);
    suppIdx = mParentModel->index(parIdx.row(), mParentModel->fieldIndex("email"));
    orderIdx = mModel->index(row, mModel->fieldIndex("contactemail"));
    mModel->setData(orderIdx, suppIdx.data(Qt::DisplayRole), Qt::EditRole);

    // Default to company main office address TODO: make possible to select location
    RB_ObjectBase* obj = ACC_MODELFACTORY->getRoot();
    orderIdx = mModel->index(row, mModel->fieldIndex("deladd1"));
    mModel->setData(orderIdx, obj->getValue("regoffice1").toString(), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("deladd2"));
    mModel->setData(orderIdx, obj->getValue("regoffice2").toString(), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("deladd3"));
    mModel->setData(orderIdx, obj->getValue("regoffice3").toString(), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("deladd4"));
    mModel->setData(orderIdx, obj->getValue("regoffice4").toString(), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("deladd5"));
    mModel->setData(orderIdx, obj->getValue("regoffice5").toString(), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("deladd6"));
    mModel->setData(orderIdx, obj->getValue("regoffice6").toString(), Qt::EditRole);

    orderIdx = mModel->index(row, mModel->fieldIndex("orderno"));
    mModel->setData(orderIdx, orderNo, Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("orderdate"));
    QDateTime dt = QDateTime::currentDateTime();
    mModel->setData(orderIdx, dt, Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("receiptdate"));
    mModel->setData(orderIdx, dt, Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("revision"));
    mModel->setData(orderIdx, 0, Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("suppinvoicedate"));
    mModel->setData(orderIdx, dt, Qt::EditRole);

    // tvOrders->setCurrentIndex(mModel->index(row, RB2::HIDDENCOLUMNS));
    tvOrders->scrollTo(tvOrders->currentIndex());
    twOrderDetails->setCurrentIndex(0);
    twItemDetails->setCurrentIndex(0);
    leRequisitionNo->setFocus();
    leRequisitionNo->selectAll();
}

/**
 * Button delete clicked
 */
void ACC_PurchaseOrderWidget::on_pbDeleteOrder_clicked() {
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
 * Button save order clicked
 */
void ACC_PurchaseOrderWidget::on_pbSaveOrder_clicked() {
    fileSave(true);
}

/**
 * Button revert/undo clicked
 */
void ACC_PurchaseOrderWidget::on_pbRevertOrder_clicked() {
    fileRevert();
}

void ACC_PurchaseOrderWidget::on_pbCopyOrder_clicked() {
    if (isWindowModified()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("Save data first"));
        return;
    }

    if (!tvOrders->currentIndex().isValid()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No order selected"));
        return;
    }

    int ret = ACC_DIALOGFACTORY->requestYesNoDialog(tr("Copy order"),
                                                    tr("Do you want to copy\n"
                                                       "the selected order?"));
    if (ret != QMessageBox::Yes) {
        return;
    }

    // Create objects from selected order, do not delete the object,
    // it will be refreshed with new data and children
    RB_ObjectBase* order = mModel->getObject(tvOrders->currentIndex(), RB2::ResolveAll);
    createNewOrder(order);

    leRequisitionNo->setFocus();
    leRequisitionNo->selectAll();
}

/**
 * Button select item clicked
 */
void ACC_PurchaseOrderWidget::on_ileContact_clicked() {
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

void ACC_PurchaseOrderWidget::on_ileContact_clear() {
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
void ACC_PurchaseOrderWidget::on_pbAddDetail_clicked() {
    QModelIndex orderIdx = tvOrders->currentIndex();
    if (!mChildModel || !orderIdx.isValid()) return; // ACC_PurchaseOrderDetail
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
//    idx = mChildModel->index(row, mChildModel->fieldIndex("qtyreceived"));
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
    mChildModel->setData(idx, QDateTime::currentDateTime(), Qt::EditRole);

    tvDetails->setCurrentIndex(mChildModel->index(row,
                               mChildModel->fieldIndex("stk_idx"), QModelIndex()));
    tvDetails->scrollTo(tvDetails->currentIndex());
    twItemDetails->setCurrentIndex(0);
    ileStockCode->setFocus();
}

/**
 * Button delete order detail clicked
 */
void ACC_PurchaseOrderWidget::on_pbDeleteDetail_clicked() {
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
        // no setModified because parent could have changed
    }
}

/**
 * Button select item clicked
 */
void ACC_PurchaseOrderWidget::on_ileStockCode_clicked() {
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
        m->setData(idx, dlg->getMaterialCost(), Qt::EditRole);
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

void ACC_PurchaseOrderWidget::on_ileStockCode_clear() {
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

/**
 * Button preview/print order clicked
 */
void ACC_PurchaseOrderWidget::on_pbCheckOrder_clicked() {
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

    if (translatePurchaseOrder(false) == QDialog::Rejected) {
        // Canceled or nothing to invoice
        return;
    }

    ACC_PurchaseOrderHtml oper;
    if (!oper.execute(mTransDoc, mCreditorTransList)) {
        // Mistake, for example: the GL account of supplier is not set
        ACC_DIALOGFACTORY->requestWarningDialog(tr("Could not process the order. \n"
                                                   "For example:\n"
                                                   "- purchase GL posting of product category,\n"
                                                   "- supplier tax group,\n"
                                                   "- company tax location.\n"
                                                   "- receipt location tax province or \n"
                                                   "is not set."));
        return;
    }

    // Show preview invoice dialog
    //ACC_ReportPreviewDialog* dlg = new ACC_ReportPreviewDialog(this); QWebView
    ACC_OrderPreviewDialog* dlg = new ACC_OrderPreviewDialog(this);
    dlg->setWindowTitle("ACC - Preview Order");
    dlg->setCurrentFileName("po_" + leOrderNo->text());
    dlg->setHtml(oper.getHtml());
    dlg->exec();
    dlg->deleteLater();
}

/**
 * Button confirm receipt and process order clicked
 */
void ACC_PurchaseOrderWidget::on_pbProcessOrder_clicked() {
    if (!tvOrders->currentIndex().isValid()) {
        ACC_DIALOGFACTORY->requestInformationDialog(tr("No order selected."));
        return;
    }

    if (isWindowModified()) {
        ACC_DIALOGFACTORY->requestInformationDialog(tr("Save your data first."));
        return;
    }

    // only posting or processing if invoice has been received from supplier
    RB_String invoiceNo = mModel->getCurrentValue("suppinvoiceno").toString();
    if (invoiceNo.isEmpty()) {
        ACC_DIALOGFACTORY->requestInformationDialog(tr("Not a valid supplier invoice number."));
        return;
    }

    QDateTime dt = mModel->getCurrentValue("suppinvoicedate").toDateTime();
    QDateTime cdt = QDateTime::currentDateTime();
    if (!dt.isValid() || dt.daysTo(cdt) > 365) {
        ACC_DIALOGFACTORY->requestInformationDialog(tr("Not a valid supplier invoice date."));
        return;
    }

    if (translatePurchaseOrder(true) == QDialog::Rejected) {
        // Canceled or nothing to invoice
        return;
    }

    // Process invoice and post debtor and general ledger transactions
    ACC_ProcessTransDoc operTransDoc;
    if (!operTransDoc.execute(mTransDoc, mCreditorTransList)) {
        // Mistake, for example: the GL account of customer is not set
        ACC_DIALOGFACTORY->requestWarningDialog(tr("Could not process the order. \n"
                                                   "For example: GL account for \n"
                                                   "the product category, tax province, \n"
                                                   "supplier tax location, \n"
                                                   "or receipt tax location is not set."));
        return;
    }

    // Show final invoice dialog
    // ACC_ReportPreviewDialog* dlg = new ACC_ReportPreviewDialog(this); QWebView
    ACC_OrderPreviewDialog* dlg = new ACC_OrderPreviewDialog(this);
    dlg->setWindowTitle("ACC - Order");

    ACC_PurchaseOrderHtml oper;
    oper.setIsCheckPrint(false);
    oper.execute(mTransDoc, mCreditorTransList);

    dlg->setCurrentFileName("po_" + leOrderNo->text());
    dlg->setHtml(oper.getHtml());
    dlg->exec();
    dlg->deleteLater();
}

/**
 * Translate sales order in transaction document,
 * create price salestaxes per line item for confirmation
 * @param isPost false if this is only to preview the invoice
 */
int ACC_PurchaseOrderWidget::translatePurchaseOrder(bool isPost) {
    QApplication::restoreOverrideCursor();

    // Reset debtor transactions
    if (mCreditorTransList) {
        delete mCreditorTransList;
    }
    mCreditorTransList = new RB_ObjectContainer(RB_Uuid::createUuid().toString(), NULL,
                                 "ACC_CreditorTransList", ACC_OBJECTFACTORY);

    // Reset document
    if (mTransDoc) {
        delete mTransDoc;
    }

    mTransDoc = ACC_OBJECTFACTORY->newSingleObject("ACC_TransDoc");

    // Set transaction document data and relevant taxes of this order
    ACC_PoTransDoc oper;
    if (!oper.preparePreview(mTransDoc, mCreditorTransList)) {
        QApplication::restoreOverrideCursor();
        ACC_DIALOGFACTORY->requestWarningDialog(tr("Could not process the invoice. \n"
                                                   "For example:\n"
                                                   "- purchase GL posting for the product category,\n"
                                                   "- company tax province,\n"
                                                   "- supplier tax group or\n"
                                                   "- receipt location is not set."));
        return QDialog::Rejected;
    }

    // Check whether there are amounts to be invoiced
    double invoiceAmount = 0.0;
    RB_ObjectIterator* iterDt = mCreditorTransList->createIterator();

    for (iterDt->first(); !iterDt->isDone(); iterDt->next()) {
        RB_ObjectBase* creditorTrans = iterDt->currentObject();
        invoiceAmount += creditorTrans->getValue("amount").toDouble();
    }

    delete iterDt;

    if (invoiceAmount <= 0.0) {
        QApplication::restoreOverrideCursor();
        ACC_DIALOGFACTORY->requestWarningDialog(tr("There are no quantities to invoice\n"
                                                   "or the purchaseorder is completed."));
        return QDialog::Rejected;
    }

    QApplication::restoreOverrideCursor();

    // Show tax confirmation dialog if not for posting invoice
    bool isSalesOrder = false;
    ACC_TaxConfirmationDialog* dlg = new ACC_TaxConfirmationDialog(this);
    dlg->setTransModel(mTransDoc, mCreditorTransList, isSalesOrder);
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
void ACC_PurchaseOrderWidget::createNewOrder(RB_ObjectBase* order) {
    on_pbAddOrder_clicked();

    // always insert at the beginning
    int row = 0; // mModel->rowCount();
    QModelIndex orderIdx;

    // orderIdx = mModel->index(row, mModel->fieldIndex("orderno"));
    // mModel->setData(orderIdx, order->getValue("orderno"), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("requisitionno"));
    mModel->setData(orderIdx, order->getValue("requisitionno"), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("receiptat_id"));
    mModel->setData(orderIdx, order->getValue("receiptat_id"), Qt::EditRole);
    //    orderIdx = mModel->index(row, mModel->fieldIndex("orderdate"));
    //    mModel->setData(orderIdx, order->getValue("orderdate"), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("contact_idx"));
    mModel->setData(orderIdx, order->getValue("contact_idx").toString()
                    + order->getDValue("contact_idx").toString(), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("contactphone"));
    mModel->setData(orderIdx, order->getValue("contactphone"), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("contactemail"));
    mModel->setData(orderIdx, order->getValue("contactemail").toString(), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("supplierref"));
    mModel->setData(orderIdx, order->getValue("supplierref"), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("receiptdate"));
    mModel->setData(orderIdx, order->getValue("receiptdate"), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("revision"));
    mModel->setData(orderIdx, order->getValue("revision"), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("shipvia_id"));
    mModel->setData(orderIdx, order->getValue("shipvia_id"), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("orderstatus"));
    mModel->setData(orderIdx, order->getValue("orderstatus"), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("quotation"));
    mModel->setData(orderIdx, order->getValue("quotation"), Qt::EditRole);
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
    orderIdx = mModel->index(row, mModel->fieldIndex("ordertitle"));
    mModel->setData(orderIdx, order->getValue("ordertitle"), Qt::EditRole);
    orderIdx = mModel->index(row, mModel->fieldIndex("comments"));
    mModel->setData(orderIdx, order->getValue("comments"), Qt::EditRole);
    //    orderIdx = mModel->index(row, mModel->fieldIndex("suppinvoiceno"));
    //    mModel->setData(orderIdx, order->getValue("suppinvoiceno"), Qt::EditRole);
    //    orderIdx = mModel->index(row, mModel->fieldIndex("suppinvoicedate"));
    //    mModel->setData(orderIdx, order->getValue("suppinvoicedate"), Qt::EditRole);


    RB_ObjectContainer* detailList = order->getContainer("ACC_PurchaseOrderDetailList");

    if (!detailList) {
        RB_DEBUG->error("ACC_PurchaseOrderWidget::createNewOrder() detail list ERROR");
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
        mChildModel->setData(detailIdx, orderDetail->getValue("stk_idx").toString()
                             + orderDetail->getDValue("stk_idx").toString(), Qt::EditRole);
        detailIdx = mChildModel->index(row, mChildModel->fieldIndex("serialno"));
        mChildModel->setData(detailIdx, orderDetail->getValue("serialno"), Qt::EditRole);
        detailIdx = mChildModel->index(row, mChildModel->fieldIndex("stkdescription"));
        mChildModel->setData(detailIdx, orderDetail->getValue("stkdescription"), Qt::EditRole);
        detailIdx = mChildModel->index(row, mChildModel->fieldIndex("stkuom"));
        mChildModel->setData(detailIdx, orderDetail->getValue("stkuom"), Qt::EditRole);
        detailIdx = mChildModel->index(row, mChildModel->fieldIndex("unitprice"));
        mChildModel->setData(detailIdx, orderDetail->getValue("unitprice"), Qt::EditRole);
        detailIdx = mChildModel->index(row, mChildModel->fieldIndex("quantity"));
        mChildModel->setData(detailIdx, orderDetail->getValue("quantity"), Qt::EditRole);
        detailIdx = mChildModel->index(row, mChildModel->fieldIndex("estimate"));
        mChildModel->setData(detailIdx, orderDetail->getValue("estimate"), Qt::EditRole);
        // detailIdx = mChildModel->index(row, mChildModel->fieldIndex("qtyinvoiced"));
        // mChildModel->setData(detailIdx, orderDetail->getValue("qtyinvoiced"), Qt::EditRole);
        // detailIdx = mChildModel->index(row, mChildModel->fieldIndex("qtyreceived"));
        // mChildModel->setData(detailIdx, orderDetail->getValue("qtyreceived"), Qt::EditRole);
        detailIdx = mChildModel->index(row, mChildModel->fieldIndex("discountpercent"));
        mChildModel->setData(detailIdx, orderDetail->getValue("discountpercent"), Qt::EditRole);
        // detailIdx = mChildModel->index(row, mChildModel->fieldIndex("actualreceiptdate"));
        // mChildModel->setData(detailIdx, orderDetail->getValue("actualreceiptdate"), Qt::EditRole);
        // detailIdx = mChildModel->index(row, mChildModel->fieldIndex("completed"));
        // mChildModel->setData(detailIdx, orderDetail->getValue("completed"), Qt::EditRole);
        detailIdx = mChildModel->index(row, mChildModel->fieldIndex("narrative"));
        mChildModel->setData(detailIdx, orderDetail->getValue("narrative"), Qt::EditRole);
        detailIdx = mChildModel->index(row, mChildModel->fieldIndex("poline"));
        mChildModel->setData(detailIdx, orderDetail->getValue("poline"), Qt::EditRole);
        // detailIdx = mChildModel->index(row, mChildModel->fieldIndex("itemdue"));
        // mChildModel->setData(detailIdx, orderDetail->getValue("itemdue"), Qt::EditRole);
        detailIdx = mChildModel->index(row, mChildModel->fieldIndex("taxcat_id"));
        mChildModel->setData(detailIdx, orderDetail->getValue("taxcat_id"), Qt::EditRole);
        detailIdx = mChildModel->index(row, mChildModel->fieldIndex("costcenter_idx"));
        mChildModel->setData(detailIdx, orderDetail->getValue("costcenter_idx").toString()
                             + orderDetail->getDValue("costcenter_idx").toString(), Qt::EditRole);
    }

    delete iter;

}

void ACC_PurchaseOrderWidget::setExchangeRate() {
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
void ACC_PurchaseOrderWidget::slotParentSelectionChanged(const QModelIndex &current,
                                                          const QModelIndex& /*previous*/) {
    if (current.isValid()) {
        leSupplier->setText(mParentModel->getCurrentValue("suppname").toString());
        mCurrencyId = mParentModel->getCurrentValue("currency_id", RB2::RoleOrigData).toString();
    } else {
        leSupplier->setText("");
        mCurrencyId = "";
    }
}

/**
 * Slot calculate item total based on selected item changed
 */
void ACC_PurchaseOrderWidget::slotItemSelectionChanged(const QModelIndex& /*current*/,
                                                     const QModelIndex& /* previous */) {
    slotCalculateItemTotal();
}

/**
 * Slot calculate item total price based on quantity, unit price and discount
 */
void ACC_PurchaseOrderWidget::slotCalculateItemTotal() {
    double price = leUnitPrice->text().toDouble();
    double qty = leQuantity->text().toDouble();
    double discount = leDiscountPercent->text().toDouble() / 100.0;

    if (0.0 <= discount && discount <= 1.0 && qty >= 0.0 && price >= 0.0) {
        double totalPrice = price * qty * (1.0 - discount);
        leTotalItemPrice->setText(RB_String::number(totalPrice));
    } else {
        leTotalItemPrice->setText("error");
    }

    // Repeat value
    leOrderedQuantity->setText(leQuantity->text());
}

/**
 * @returns the help subject, in this case the widget objectname
 */
RB_String ACC_PurchaseOrderWidget::getHelpSubject() const {
    return objectName();
}

bool ACC_PurchaseOrderWidget::closeWidget() {
    RB_Widget::closeWidget();
    if (maybeSave()) {
        return true;
    }
    return false;
}

/**
 * Change event, for example translation
 */
void ACC_PurchaseOrderWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

