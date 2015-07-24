/*****************************************************************
 * $Id: acc_bankmappingdialog.h 1961 2013-08-05 12:15:53Z rutger $
 * Created: Jul 01, 2013 10:04:25 AM - rutger
 *
 * Copyright (C) 2013 Biluna. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_BANKMAPPINGDIALOG_H
#define ACC_BANKMAPPINGDIALOG_H

#include <map>
#include "acc.h"
#include "acc_handleallocns.h"
#include "acc_transdoc.h"
#include "rb_dialog.h"
#include "ui_acc_bankmappingdialog.h"

class ACC_CsvBankParser;

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
class ACC_BankMappingDialog : public RB_Dialog, private Ui::ACC_BankMappingDialog {

    Q_OBJECT

public:
    ACC_BankMappingDialog(QWidget *parent = 0);
    virtual ~ACC_BankMappingDialog();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "ACC Bank Import Mapping"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveACC; }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual void fileRevert();

public slots:
    virtual void on_pbAdd_clicked();
    virtual void on_pbDelete_clicked();
    virtual void on_pbOpenFile_clicked();
    virtual void on_pbHelp_clicked();
    virtual void on_pbOk_clicked();
    virtual void on_pbCancel_clicked();

protected:
    virtual void changeEvent(QEvent* e);

private:
    void setParserSettings(ACC_CsvBankParser* parser);
    void fillTable(RB_ObjectBase* importBankList);
    void setTableHeader(RB_ObjectBase* obj);
    void setTableRow(int row, RB_ObjectBase* obj);

    // TODO: copied coding from acc_bankimportcheckdialog
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

    //! Mapping model
    RB_MmProxy* mModel;
    //! Data widget mapper for mapping model
    RB_DataWidgetMapper* mMapper;
    //! Object list with bank data parsed from CSV file
    RB_ObjectContainer* mBankImportList;

    QColor mColorLightYellow;
    int mColCount;
//    RB_String mBankAccount;
};

#endif // ACC_BANKMAPPINGDIALOG_H
