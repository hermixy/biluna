/*****************************************************************
 * $Id: acc_processinvoice.h 1438 2011-07-20 18:31:56Z rutger $
 * Created: Mar 8, 2010 8:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_PROCESSINVOICE_H
#define ACC_PROCESSINVOICE_H

#include "acc_operation.h"

/**
 * Class responsible for the processing of the invoice, including
 * updating debtortransaction, debtortaxtransaction after (part) delivery is
 * completed and confirmed
 * NOTE: class is not used anymore for reference only
 */
class ACC_ProcessInvoice : public ACC_Operation  {

public:
    ACC_ProcessInvoice();
    virtual ~ACC_ProcessInvoice();

    virtual bool execute(RB_ObjectBase* td, RB_ObjectContainer* dtList);

private:
    bool isQuantityDispatchedPositive();
    bool debtorTransaction();
    bool debtorTransTaxes(RB_ObjectBase* debtorTrans, const RB_String& taxAuthId,
                          double taxAmount, double currencyRate);
    bool transaction();
    void addGlTrans(const RB_String& accountId, const RB_String& narrative,
                    double amount);


    //! Stockmoves list based on the salesorder dispatched quantities
    RB_ObjectBase* mStockMovesList;
    //! Debtortransaction and debtortaxes created here based on the stockmoves list
    RB_ObjectBase* mDebtorTrans;
    //! General Ledger transactionlist created here
    RB_ObjectBase* mGlTransList;

};

#endif // ACC_PROCESSINVOICE_H
