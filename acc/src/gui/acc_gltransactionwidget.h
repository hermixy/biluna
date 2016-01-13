/*****************************************************************
 * $Id: acc_gltransactionwidget.h 2217 2015-02-13 18:18:14Z rutger $
 * Created: Aug 29, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_GLTRANSACTIONWIDGET_H
#define ACC_GLTRANSACTIONWIDGET_H

#include <map>
#include "acc.h"
#include "acc_handleallocns.h"
#include "acc_transdoc.h"
#include "rb_widget.h"
#include "ui_acc_gltransactionwidget.h"


/**
 * Widget (on MDI window) to edit General Ledger transactions. The transactions
 * are document controlled. The documents are
 *  1. debtor documents, usually the sales invoices,
 *  2. creditor documents, usually the purchase invoices,
 *  3. bank or cash documents, usually bank account reports,
 *     cash sale or purchase notes
 *  4. journal documents, for example depreciation calculation or
 *     undocumented correction bookings.
 * The software creates a document with number for the journal internally
 * also even if a (paper) document is missing for the transaction. The journal
 * transactions cannot be directly against the debtor or creditor accounts
 * since this application allows only document controlled bookings.
 *
 * The flow in this widget is
 *  a. Select a type of basic account (debtor, creditor, bank/cash or journal)
 *  b. Add, delete or edit a document with values for amount excluding tax and tax
 *  c. Add, delete or edit item(s) of the document with the amount excluding tax
 *  d. Edit tax(es) per item
 * During b, c or d the some widgets are disabled if the value of the amounts
 * do not match and when clicked will ask the user to update the values before
 * selection of another basic account, other document or addition of a document
 * is possible.
 */
class ACC_GlTransactionWidget : public RB_Widget, private Ui::ACC_GlTransactionWidget {

    Q_OBJECT

public:
    ACC_GlTransactionWidget(QWidget *parent = 0);
    virtual ~ACC_GlTransactionWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "ACC GL Transactions"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveACC; }

    virtual void init();
//    virtual void filePdf(QPrinter* printer);
//    virtual void filePrint(QPrinter* printer);
//    virtual void filePrintPreview(QPrinter* printer);
    virtual bool fileSave(bool withSelect);
    virtual void fileRevert();
//    virtual RB_String getHelpSubject() const;
//    virtual bool closeWidget();

public slots:
    virtual void on_pbCheckBalance_clicked();
    virtual void on_pbAddDoc_clicked();
    virtual void on_pbDeleteDoc_clicked();
    virtual void on_ileCustomer_clicked();
    virtual void on_ileSupplier_clicked();
    virtual void on_pbReprintInvoice_clicked();
    virtual void on_pbReprintPurchaseOrder_clicked();
    virtual void on_pbAddItem_clicked();
    virtual void on_pbDeleteItem_clicked();
    virtual void on_pbSave_clicked();
    virtual void on_pbRevert_clicked();
    virtual void on_pbFind_clicked();
    virtual void on_pbTotalReceivable_clicked();
    virtual void on_pbTotalPayable_clicked();
    virtual void on_pbTotalTax_clicked();
    virtual void on_pbEditInvoiceItemDetail_clicked();
    virtual void on_ileItemAccount_clicked();
    virtual void on_ileItemAccount_clear();
    virtual void on_ileAllocation_clicked();
    virtual void on_ileAllocation_clear();
    virtual void on_ileCostCenter_clicked();
    virtual void on_pbHighTaxAmount_clicked();
    virtual void on_pbLowTaxAmount_clicked();
    virtual void on_pbOtherTaxAmount_clicked();
    virtual void on_ileHighTaxAccount_clicked();
    virtual void on_ileLowTaxAccount_clicked();
    virtual void on_ileOtherTaxAccount_clicked();
public slots:
    virtual void slotDataIsChanged(const QModelIndex& current,
                                   const QModelIndex& previous);

protected:
    virtual bool maybeSave();
    virtual void mousePressEvent(QMouseEvent* e);
    virtual void changeEvent(QEvent* e);

protected slots:
    virtual void slotLedgerAccountChanged(int number);
    virtual void slotTabChanged(int);
    virtual void slotRefreshBankModel();

private:
    void setGlTransAsDelete();
    bool processGlTrans();

    void setDocWidgets(ACC2::TransType type);
    void setItemModel(ACC2::TransType type);
    void setTaxModel(ACC2::TransType type);
    void enableWidgets(bool enable);
    void updateGlTransWidget();
    void updateGlTransactions();
    void createNewGlTrans();
    void createNewHelper(const RB_String& descr, const RB_String& acctName,
                         const RB_String& acctId, double amount);
    void createExistingGlTrans();
    bool isValidTransDoc();
    void updateBeforeSave();

    void taxAccountClicked(const RB_String& taxAccountFieldName);
    bool isDebtorCreditorAccount(RB_ObjectBase* actObj);

    //! Chart of Accounts (GL) model for combobox
    RB_MmProxy* mChartMasterModel;

    //! Document model of transaction
    RB_MmProxy* mTransDocModel;
    //! Data widget mapper for document model
    RB_DataWidgetMapper* mTransDocMapper;
    //! Item model, child of document model
    RB_MmProxy* mItemTransModel;
    //! Data widget mapper for item model
    RB_DataWidgetMapper* mItemTransMapper;

    //! GL transaction model, for deletion only
    RB_MmProxy* mGlTransModel;
    //! ID list of documents (to be) deleted, for the deletion of GL transactions
    RB_StringList mDeleteDocIdList;


    //! Flag indicating maybeSave() is in progress, preventing endless loop
    bool mSaveInProgress;
    //! Flag indicating init() is in progress, preventing endless loop
    bool mInitInProgress;
    /**
     * Flag indicating adding or deletion of doc or item is in progress,
     * preventing execution of mHandleAlloc in slotDataChanged()
     */
    bool mAddDeleteInProgress;
    //! Current transaction types
    ACC2::TransType mTransType;

    //! GL Transaction list
    RB_ObjectContainer* mGlTransList;

    // Used only in create new GL transaction
    RB_String mRootId;
    RB_String mDate;
    int mType;
    int mTypeNo;
    int mPeriod;
    RB_String mDocId;

    //! Transaction validation message
    RB_String mValidationMessage;

    //! True if slotAmountItemEdited is passed
    bool mIsItemChangedPassed;
    //! Time for single shot for validation
    QTimer mTimer;
    //! Handle allocation operation
    ACC_HandleAllocns mHandleAllocn;

};

#endif // ACC_GLTRANSACTIONWIDGET_H
