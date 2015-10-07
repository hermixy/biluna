/*****************************************************************
 * $Id: acc_bankimportcheckdialog.h 2214 2015-01-28 17:07:32Z rutger $
 * Created: Aug 29, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_BANKIMPORTCHECKDIALOG_H
#define ACC_BANKIMPORTCHECKDIALOG_H

#include <map>
#include "acc.h"
#include "acc_handleallocns.h"
#include "acc_transdoc.h"
#include "rb_dialog.h"
#include "ui_acc_bankimportcheckdialog.h"

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
class ACC_BankImportCheckDialog : public RB_Dialog, private Ui::ACC_BankImportCheckDialog {

    Q_OBJECT

public:
    ACC_BankImportCheckDialog(QWidget *parent = 0);
    virtual ~ACC_BankImportCheckDialog();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "ACC Bank Import"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveACC; }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual void fileRevert();
    virtual RB_String getHelpSubject() const;

public slots:
    virtual void on_pbOpenFile_clicked();

    virtual void on_ileItemAccount_clicked();
    virtual void on_ileAllocation_clicked();
    virtual void on_ileAllocation_clear();
    virtual void on_pbHelp_clicked();
    virtual void on_pbPrevious_clicked();
    virtual void on_pbNext_clicked();
    virtual void on_pbImport_clicked();
    virtual void on_pbCancel_clicked();


protected:
    virtual bool maybeSave();
    virtual void changeEvent(QEvent* e);

protected slots:
    virtual void slotUpdateStatementNumbers(QTableWidgetItem* currentItem);
    virtual void slotTabChanged(int);


private:
    void fillTable(RB_ObjectBase* importBankList);
    void setTableHeader(RB_ObjectBase* obj);
    void setTableRow(int row, RB_ObjectBase* obj);
    bool prepareImportList(const QDate& lastTransDate);
    bool prepareTransDoc();

    void updateGlTransWidget();
    void createNewGlTrans(bool isSelectedDocOnly = true);
    void createNewHelper(const RB_String& descr, const RB_String& acctName,
                         const RB_String& acctId, double amount);
    bool isValidTransDoc();
    void updateBeforeSave();

    bool isDebtorCreditorAccount(RB_ObjectBase* actObj);
    bool ensureTransDocExisted(RB_ObjectContainer* transDocList);


    static const int IMPORTMESSAGE = 7;
    static const int STATBANKNAME = 8;
    static const int STATACTNUMBER = 9;
    static const int STATSTARTAMOUNT = 10;
    static const int STATENDAMOUNT = 11;
    static const int STATTIMESTAMP = 12;
    static const int STATNUMBER = 13;
    static const int TRANSACCOUNT = 14;
    static const int TRANSACTNUMBER = 15;
    static const int TRANSAMOUNT = 16;
    static const int TRANSDEBITCREDIT = 17;
    static const int TRANSDESCRIPTION = 18;
    static const int TRANSSECONDDESCRIPTION = 19;
    static const int TRANSENTRYTIMESTAMP = 20;
    static const int TRANSVALUETIMESTAMP = 21;
    static const int TRANSCODE = 22;


    RB_ObjectContainer* mBankImportList;
    int mColCount;
    QColor mColorLightYellow;
    bool mIsImportInProgress;
    bool mIsNumbersUpdated;
    RB_String mBankAccount;

    //! Chart of Accounts (GL) model for combobox
//    RB_MmProxy* mChartMasterModel;
    //! Document model of transaction
    RB_MmProxy* mDocModel;
    //! Item model, child of document model
    RB_MmProxy* mItemModel;
//    //! GL transaction model, for deletion only
//    RB_MmProxy* mGlTransModel;
    //! (Debtor, creditor) document transaction model for selection of allocation
//    RB_MmProxy* mAllocDocModel;
//    //! ID list of documents (to be) deleted, for the deletion of GL transactions
//    RB_StringList mDeleteDocIdList;

    //! Data widget mapper for document model
    RB_DataWidgetMapper* mDocMapper;
    //! Data widget mapper for item model
    RB_DataWidgetMapper* mItemMapper;

    //! Flag indicating maybeSave() is in progress, preventing endless loop
    bool mSaveInProgress;
    //! Flag indicating init() is in progress, preventing endless loop
    bool mInitInProgress;
    //! Current transaction types
    ACC2::TransType mTransType;

//    //! Current transaction document
//    ACC_TransDoc* mTransDoc;
    //! GL transaction list
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

//    //! True if validate amounts is already scheduled
//    bool mIsValidateAmounts;
//    //! True if slotAmountItemChanged is passed
//    bool mIsItemChangedPassed;
//    //! Time for single shot for validation
//    QTimer mTimer;
    //! Handle allocation operation
    ACC_HandleAllocns mHandleAllocn;

//    //! Combobox style without dropdown arrow
//    RB_String mCbStyle;

    //! Bank GL account ID
    RB_String mBankGlAccId;
    //! Bank GL account name
    RB_String mBankGlAccName;
    //! Last used bank statement sequence number
    int mLastTransNo;
    //! Last sequence number of imported statements
    int mLastImportedNo;
    //! Bank statement sequence number
    int mBankStatSeqNo;
    //! Transaction document list based on import of bank statement list
    RB_ObjectContainer* mTransDocListRoot;
};

#endif // ACC_BANKIMPORTCHECKDIALOG_H
