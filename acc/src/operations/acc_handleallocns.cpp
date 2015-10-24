/*****************************************************************
 * $Id: acc_handleallocns.cpp 2214 2015-01-28 17:07:32Z rutger $
 * Created: Oct 14, 2010 3:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_handleallocns.h"

#include <cmath>
#include "acc_modelfactory.h"
#include "acc_objectfactory.h"
#include "acc_qachartmaster.h"
#include "acc_sqlcommonfunctions.h"
#include "rb_mmproxy.h"
#include "rb_objectcontainer.h"

/**
 * Constructor
 */
ACC_HandleAllocns::ACC_HandleAllocns() {
    mTransDocList = new RB_ObjectContainer("", NULL, "ACC_TransDocList",
                                           ACC_OBJECTFACTORY);
    mBankTransList = new RB_ObjectContainer("", NULL, "ACC_BankTransList",
                                            ACC_OBJECTFACTORY);
    mMemoTransList = new RB_ObjectContainer("", NULL, "ACC_MemoTransList",
                                            ACC_OBJECTFACTORY);
    mTransAllocList = new RB_ObjectContainer("", NULL, "ACC_TransAllocnList",
                                             ACC_OBJECTFACTORY);
    mTransAllocList->setId(ACC_MODELFACTORY->getRootId()); // will be used as parent
    mIsAllocListCreated = false;
}

/**
 * Destructor
 */
ACC_HandleAllocns::~ACC_HandleAllocns() {
    delete mTransDocList;
    delete mBankTransList;
    delete mMemoTransList;
    delete mTransAllocList;
}

/**
 * Execute operation
 * @param obj result object, if applicable
 * @return true on success, default false
 */
bool ACC_HandleAllocns::execute(RB_ObjectBase* /* obj */) {
    RB_DEBUG->error("ACC_HandleAllocns::execute() not used ERROR");
    return false;
}

bool ACC_HandleAllocns::submitAllAndSelect() {
    // Start transaction
    ACC_MODELFACTORY->getDatabase().transaction();

    bool success = mTransDocList->dbUpdate(ACC_MODELFACTORY->getDatabase(),
                                           RB2::ResolveOne);
    success = success ? mBankTransList->dbUpdate(ACC_MODELFACTORY->getDatabase(),
                                                 RB2::ResolveOne) : false;
    success = success ? mMemoTransList->dbUpdate(ACC_MODELFACTORY->getDatabase(),
                                                 RB2::ResolveOne) : false;
    success = success ? mTransAllocList->dbUpdate(ACC_MODELFACTORY->getDatabase(),
                                                  RB2::ResolveOne) : false;

    RB_ObjectIterator* iter = mTransAllocList->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* transAlloc = iter->currentObject();

        if (transAlloc->getFlag(RB2::FlagIsDeleted)
                && transAlloc->getFlag(RB2::FlagFromDatabase)) {
            // undo allocation and set bank/cash or memo to default account
            // due to transDocTo amount is changed
            glTransToDefault(transAlloc->getValue("docallocto_id").toString());
        }
    }

    delete iter;

    // Commit transaction
    ACC_MODELFACTORY->getDatabase().commit();

    success = success ? mTransDocList->erase() : false;
    success = success ? mBankTransList->erase() : false;
    success = success ? mMemoTransList->erase() : false;
    success = success ? mTransAllocList->erase(): false;
    mIsAllocListCreated = false;
    return success;
}

bool ACC_HandleAllocns::revertAll() {
    bool success = mTransDocList->erase();
    success = success ? mBankTransList->erase() : false;
    success = success ? mMemoTransList->erase() : false;
    success = success ? mTransAllocList->erase(): false;
    mIsAllocListCreated = false;
    return success;
}

/**
 * Add selected allocation, called from individual BankTrans
 * or MemoTrans item only. The existing allocation is
 * first set to be deleted.
 * @param itemModel item model, bank/cash or journal model
 * @param docToId selected ID from transaction document dialog
 * @param docToDspl the displayed data from transaction document dialog
 * @param docFromId ID of item parent document model
 * @return true on success
 */
bool ACC_HandleAllocns::addAllocn(RB_MmProxy* itemModel,
                                  const RB_String& docToId,
                                  const RB_String& docToDspl,
                                  const RB_String& docFromId) {
    if (!itemModel ||
            (itemModel->getModelType() != (int)ACC_MODELFACTORY->ModelBankTrans
             && itemModel->getModelType() != (int)ACC_MODELFACTORY->ModelMemoTrans)) {
        RB_DEBUG->error("ACC_HandleAllocns::addAllocn() itemModel ERROR");
        return false;
    }
    // Undo existing allocation if exists
    // The relevant transAlloc, transDocTo and itemFrom are now part of list
    // Below is false because doc item is reversable and not deleted
    delItemAllocn(itemModel, false);

    // Get the transDocTo from the update list or add to the list
    RB_ObjectBase* transDocTo = getTransDocTo(docToId);

    if (!transDocTo) {
        return false;
    }

    // Receivable or payable amount for the transDocTo
    double transDocAmt = transDocTo->getValue("totalamountrec").toDouble();

    if (std::abs(transDocAmt) < 0.005) { // not almost zero
        transDocAmt = -transDocTo->getValue("totalamountpay").toDouble();
    }

    // already allocated amount and calculate max allocatable amount
    double docAllocAmt = getAllocatedAmount(docToId);
    double allocatableAmt = transDocAmt - docAllocAmt; // allocatable/remaining amount

    if (allocatableAmt == 0.0) {
        RB_DEBUG->warning("ACC_HandleAllocns::addAllocn() allocatable amount=0 WARNING");
        return false;
    }

    // Amount to be allocated from itemtrans
    double bankMemItemAmt = itemModel->getCurrentValue("amount", Qt::EditRole).toDouble();
    double newAllocAmt = 0.0;

    if (std::fabs(bankMemItemAmt) <= std::fabs(allocatableAmt)) {
        newAllocAmt = bankMemItemAmt;
    } else {
        // Bank/cash or memo item amount is too much for the transDocTo amount
        newAllocAmt = allocatableAmt;
        // Update current item model bank/cash or gltrans
        itemModel->setCurrentValue("amount", allocatableAmt, Qt::EditRole);
    }

    itemModel->setCurrentValue("transallocn_idx", docToId + docToDspl, Qt::EditRole);

    // Use the document amount minus the already allocated amount
    // as starting point, therefore the bank amountcleared will be
    // the same as the document amount minus the allocated amount
    if (transDocTo->getValue("doctype").toInt() == (int)ACC2::TransDebtor
            || transDocTo->getValue("doctype").toInt() == (int)ACC2::TransSalesOrder) {
        itemModel->setCurrentValue("chartmaster_idx",
                                   ACC_QACHARTMASTER->getAccRecId()
                                   + ACC_QACHARTMASTER->getAccRecName(),
                                   Qt::EditRole);
    } else {
        itemModel->setCurrentValue("chartmaster_idx",
                                   ACC_QACHARTMASTER->getAccPayId()
                                   + ACC_QACHARTMASTER->getAccPayName(),
                                   Qt::EditRole);
    }

    // Set amount cleared with the item transaction
    if (itemModel->getModelType() == (int)ACC_MODELFACTORY->ModelBankTrans) {
        itemModel->setCurrentValue("amountcleared", newAllocAmt, Qt::EditRole);
    }

    // Update debtor or creditor document to 'settled' if full amount is set
    transDocTo->setValue("alloc", newAllocAmt + docAllocAmt);

    if (std::abs(newAllocAmt - allocatableAmt) < 0.005) {
        transDocTo->setValue("settled", 1);
    } else {
        transDocTo->setValue("settled", 0);
    }

    // Create allocation
    RB_ObjectBase* transAlloc = getTransAllocn(itemModel);
    QDateTime dt = QDateTime::currentDateTime();

    if (itemModel->getModelType() == (int)ACC_MODELFACTORY->ModelBankTrans) {
        dt = itemModel->getCurrentValue("transdate").toDateTime();
    }

    transAlloc->deleteFlag(RB2::FlagIsDeleted);
    transAlloc->setValue("amount", newAllocAmt);
    transAlloc->setValue("datealloc", dt);
    transAlloc->setValue("docfrom_id", docFromId);
    transAlloc->setValue("itemallocfrom_id", itemModel->getCurrentId());
    transAlloc->setValue("docallocto_id", docToId);

    return true;
}

/**
 * Delete the allocation of a bank item or a journal (General Ledger) item.
 * An item of a document is a transaction.
 * A document is in this case a bank account statement or journal entries. This
 * function is called when an allocation is deleted or a bank (or journal)
 * transaction is deleted or a allocation is replaced by another allocation.
 * transaction. Itemmodel must have been set to the relevant item ID
 * @param itemModel GL transaction item model
 * @param isDocItemDeleted true if item will be deleted
 */
void ACC_HandleAllocns::delItemAllocn(RB_MmProxy* itemModel,
                                         bool isDocItemDeleted) {
    RB_String itemId = itemModel->getCurrentId();

    if (itemId.size() < 38) {
        RB_DEBUG->error("ACC_HandleAllocns::delItemAllocn() item id ERROR");
        return;
    }

    createAllocList(itemModel);

    // Get transAlloc, check if already edited
    RB_ObjectBase* transAlloc = mTransAllocList->getObject("itemallocfrom_id", itemId);

    if (transAlloc) {
        undoItemAllocationFromBM(transAlloc, itemModel, isDocItemDeleted);
    }
}

/**
 * Delete the item list allocation of a bank/cash or memo journal
 * (General Ledger) document, cannot be undone.
 * @param bankGlTransModel GL transaction model with the document transactions
 */
void ACC_HandleAllocns::delItemListAllocn(RB_MmProxy* itemModel) {
    if (itemModel->getModelType() != ACC_MODELFACTORY->ModelBankTrans
        && itemModel->getModelType() != ACC_MODELFACTORY->ModelMemoTrans) {
        RB_DEBUG->error("ACC_HandleAllocns::delTransAllocnList() modeltype ERROR");
        return;
    }

    int itemRowCount = itemModel->rowCount();
    QModelIndex currentIdx = itemModel->getCurrentIndex();
    QModelIndex idx = QModelIndex();
    bool isDeleted = true;

    for (int i = 0; i < itemRowCount; ++i) {
        idx = itemModel->index(i, 0);
        itemModel->getSelectionModel()
                ->setCurrentIndex(idx, QItemSelectionModel::ClearAndSelect);
        delItemAllocn(itemModel, isDeleted);
    }

    itemModel->getSelectionModel()
            ->setCurrentIndex(currentIdx, QItemSelectionModel::ClearAndSelect);
}

/**
 * Delete the allocations memo or bankcash item allocation(s)
 * to a debtor or creditor document:
 * \li called with a debtor or creditor document ID when this document
 * is deleted,
 * \li called when the amount in the debtor or creditor document is changed.
 * In that case the change is reversable.
 *
 * @param docToId document ID
 * @param isDeleted document or item is deleted and is not reversable
 */
void ACC_HandleAllocns::delDocAllocn(const RB_String& docToId, bool isDeleted) {
    createAllocList(docToId);

    // Get transAlloc, check if at least one exists
    RB_ObjectBase* transAlloc = mTransAllocList->getObject("docallocto_id", docToId);

    if (!transAlloc) {
        // Check if alloc exists in database or is already created
        return;
    }

    RB_ObjectIterator* iter = mTransAllocList->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        transAlloc = iter->currentObject();
        undoItemAllocationFromDC(transAlloc, isDeleted);
    }

    delete iter;
}

/**
 * Update allocated amount after a journal- or bank item amount
 * has been changed. The allocation will be deleted if the amount is zero.
 * Otherwise in the allocation the amount will be updated,
 * in the document the allocated amount and settled will be updated and
 * in the item the amountcleared will be updated.
 * Scenarios:
 * - the to be allocated document amount is equal to the item amount
 * - the to be allocated document amount is lower than the item amount
 * - the to be allocated document amount is higher than the item amount,
 *   is reset to equal and item is set to this equal amount
 * TODO: how about credit sales invoices with possible negative amount?
 * @param docFromId document from ID (journal or bank/cash document)
 * @param itemModel item model (journal or bank/cash model)
 * @param amt current amount
 * @return total transDocTo (Debtor or Creditor) amount
 */
void ACC_HandleAllocns::updateItemAllocnAmt(RB_MmProxy* itemModel,
                                               double amt) {
    // Check if there is an allocation (this is the 'transDocTo' ID and Name)
    RB_String tmpTransDocToId = itemModel->getCurrentValue("transallocn_idx",
                                                   RB2::RoleOrigData).toString();
    if (tmpTransDocToId.size() < 38) {
        return; // no allocation
    }

    createAllocList(itemModel);

    // Get transAlloc, check if already edited and if amount is still the same
    RB_String itemId = itemModel->getCurrentId();
    RB_ObjectBase* transAlloc = mTransAllocList->getObject("itemallocfrom_id", itemId);
    double currentAllocAmt = 0.0;

    if (transAlloc) {
        currentAllocAmt = transAlloc->getValue("amount").toDouble();

        if (currentAllocAmt == amt) {
            return;
        }
    } else {
        return;
    }

    // Information from the transaction allocation is leading
    RB_String transDocToId = transAlloc->getValue("docallocto_id").toString();

    if (tmpTransDocToId != transDocToId) {
        RB_DEBUG->error("ACC_HandleAllocns::updateItemAllocnAmt() "
                        "transDocToId not the same ERROR");
        return;
    }

    // Get transDocTo, first check if already edited
    RB_ObjectBase* transDocTo = getTransDocTo(transDocToId);

    // Current allocation value and total document amount
    double recPayDocToAmt = 0.0; // always positive, also for debtor credited amount?

    if (transDocTo->getValue("doctype").toInt() == int(ACC2::TransDebtor)
            || transDocTo->getValue("doctype").toInt() == int(ACC2::TransSalesOrder)) {
        recPayDocToAmt = transDocTo->getValue("totalamountrec").toDouble();
    } else {
        // create negative value
        recPayDocToAmt = -transDocTo->getValue("totalamountpay").toDouble();
    }

    // Can be positive (ref. totalamountrec) or negative (ref. totalamountpay)
    double currentDocToAllocAmt = getAllocatedAmount(transDocToId);

    // Calculate what the newly total allocated amount for this document would be,
    // the allocated amount could also be too much, in that case reset amt
    double newDocToAllocAmt = currentDocToAllocAmt - currentAllocAmt + amt;

    // Select what to do
    if (std::fabs(recPayDocToAmt - newDocToAllocAmt) < 0.005) {
        // full allocation
        transDocTo->setValue("settled", 1);
        transDocTo->setValue("alloc", newDocToAllocAmt);

        if (itemModel->getModelType() == ACC_ModelFactory::ModelBankTrans) {
            itemModel->setCurrentValue("amountcleared", amt, Qt::EditRole);
        }
        transAlloc->setValue("amount", amt);
    } else if ((recPayDocToAmt < 0.0 && recPayDocToAmt < newDocToAllocAmt && amt < 0.0)
               || (recPayDocToAmt > 0.0 && recPayDocToAmt > newDocToAllocAmt && amt > 0.0)) {
        // partial allocation
        transDocTo->setValue("settled", 0);
        transDocTo->setValue("alloc", newDocToAllocAmt);

        if (itemModel->getModelType() == ACC_ModelFactory::ModelBankTrans) {
            itemModel->setCurrentValue("amountcleared", amt, Qt::EditRole);
        }
        transAlloc->setValue("amount", amt);
    } else if ((recPayDocToAmt < 0.0 && recPayDocToAmt > newDocToAllocAmt && amt < 0.0)
               || (recPayDocToAmt > 0.0 && recPayDocToAmt < newDocToAllocAmt && amt > 0.0)) {
        // over allocation
        double maxAllocAmt = 0.0;

        if (recPayDocToAmt > newDocToAllocAmt) {
            maxAllocAmt = recPayDocToAmt - (currentDocToAllocAmt - currentAllocAmt);
        }

        transDocTo->setValue("settled", 1);
        transDocTo->setValue("alloc", recPayDocToAmt);

        if (itemModel->getModelType() == ACC_ModelFactory::ModelBankTrans) {
            itemModel->setCurrentValue("amountcleared", maxAllocAmt, Qt::EditRole);
        }

        itemModel->setCurrentValue("amount", maxAllocAmt, Qt::EditRole);
        transAlloc->setValue("amount", maxAllocAmt);
    } else {
        // wrong sign of allocation
        itemModel->setCurrentValue("amount", 0.0, Qt::EditRole);
        transAlloc->setValue("amount", 0.0);
    }
}

/**
 * Update allocation with transaction date if allocations exist
 * @param docFromId from transaction document ID
 * @param dt date time
 */
void ACC_HandleAllocns::updateTransDate(RB_MmProxy* itemModel,
                                           const QDateTime& dt) {
    createAllocList(itemModel);

    if (mTransAllocList->countObject() < 1) {
        return;
    }

    RB_ObjectIterator* iter = mTransAllocList->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* transAlloc = iter->currentObject();
        transAlloc->setValue("datealloc", dt);
    }

    delete iter;
}

/**
 * Undo the bank/memo transaction item allocation
 * Called from transaction deletion (bank/memo) or bank/memo item deletion
 * @param alloc allocation object to be undone, selected based on current item
 * @param itemModel item model, current index is selected
 * @param isPost is true if this action is to be posted immediately
 * from transDocTo to the database, does not delete the allocation.
 */
void ACC_HandleAllocns::undoItemAllocationFromBM(RB_ObjectBase* transAlloc,
                                                    RB_MmProxy* itemModel,
                                                    bool isPost) {
    RB_String docToId = transAlloc->getValue("docallocto_id").toString();

    if (docToId.size() < 38) {
        // not valid Uuid
        RB_DEBUG->error("ACC_HandleAllocns::undoItemAllocationFromBM() Uuid ERROR");
        return;
    }

    RB_ObjectBase* transDocTo = getTransDocTo(docToId);

    // Allocation values
    double allocAmt = transAlloc->getValue("amount").toDouble(); // pos or neg
    // Can be positive (ref. totalamountrec) or negative (ref. totalamountpay)
    double docToAllocAmt = getAllocatedAmount(docToId);

    // Unset transallocn_idx at bank/cash or memo transaction item
    // set chartmaster_idx to default GL account
    // and amountcleared for bank/cash only
    // Called from bank/cash or memo
    itemModel->setCurrentValue("transallocn_idx", "0", Qt::EditRole);
    itemModel->setCurrentValue("chartmaster_idx", ACC_QACHARTMASTER->getAccDefaultId()
                               + ACC_QACHARTMASTER->getAccDefaultName(),
                               Qt::EditRole);
    if (itemModel->getModelType() == ACC_ModelFactory::ModelBankTrans) {
        itemModel->setCurrentValue("amountcleared", 0.0, Qt::EditRole);
    }

    // Unset allocated value and settled
    // at debtor or creditor transaction document
    docToAllocAmt -= allocAmt;
    transDocTo->setValue("alloc", docToAllocAmt);
    transDocTo->setValue("settled", 0); // debtor or creditor document always 0

    // Finalize the preparation of the undo allocation
    transAlloc->setFlag(RB2::FlagIsDeleted);
    transAlloc->setValue("amount", 0.0); // in case re-used

    if (isPost) {
        // Post to database, usually in case of an item
        // or debtor creditor transdoc deletion
        transAlloc->dbUpdate(ACC_MODELFACTORY->getDatabase());
        transDocTo->dbUpdate(ACC_MODELFACTORY->getDatabase());

        // repost the corresponding ACC_GlTrans
        glTransToDefault(transAlloc->getValue("docallocto_id").toString());

        mTransAllocList->remove(transAlloc, true);
        mTransDocList->remove(transDocTo, true);
    }
}

/**
 * Undo the bank/memo transaction item allocation, set chartmaster_idx
 * to default GL account and amountcleared for bank/cash only to zero. The
 * transaction document alloc and settled is set in ACC_GlTransWidget.
 * Called from Debtor/Creditor document deletion or totalamountrec/pay changed
 * @param alloc allocation object to be undone
 * @param isPost is true if this action is to be posted immediately
 * from transDocTo to the database, does not delete the allocation.
 */
void ACC_HandleAllocns::undoItemAllocationFromDC(RB_ObjectBase* transAlloc,
                                                    bool isPost) {
    RB_ObjectBase* itemFrom = getItemFrom(transAlloc);
    RB_String docFromId = transAlloc->getValue("docfrom_id").toString();

    itemFrom->setValue("transallocn_idx", "0");
    itemFrom->setValue("chartmaster_idx", ACC_QACHARTMASTER->getAccDefaultId()
                       + ACC_QACHARTMASTER->getAccDefaultName());

    if (getTransDocType(docFromId) == ACC2::TransBankCash) {
        itemFrom->setValue("amountcleared", 0.0);
    }

    // Finalize the preparation of the undo allocation
    transAlloc->setFlag(RB2::FlagIsDeleted);
    transAlloc->setValue("amount", 0.0); // in case re-used

    if (isPost) {
        // Post to database, usually in case of an item
        // or debtor creditor transdoc deletion
        // transAlloc deleted by calling function
        transAlloc->dbUpdate(ACC_MODELFACTORY->getDatabase());

        if (itemFrom) {
            itemFrom->dbUpdate(ACC_MODELFACTORY->getDatabase());
            itemFrom->getParent()->remove(itemFrom, true);
        }

        // repost the corresponding ACC_GlTrans
        glTransToDefault(transAlloc->getValue("docallocto_id").toString());

        mTransAllocList->remove(transAlloc, true);
    }
}

RB_ObjectBase* ACC_HandleAllocns::getItemFrom(RB_ObjectBase* transAlloc) {
    RB_String docToId = transAlloc->getValue("docallocto_id").toString();
    RB_String docFromId = transAlloc->getValue("docfrom_id").toString();
    RB_String itemFromId = transAlloc->getValue("itemallocfrom_id").toString();

    if (docToId.size() < 38 || docFromId.size() < 38 || itemFromId.size() < 38) {
        // not valid Uuid
        RB_DEBUG->error("ACC_HandleAllocns::getItemFrom() Uuid ERROR");
        return NULL;
    }

    ACC2::TransType transType = getTransDocType(docFromId);

    if (transType != ACC2::TransBankCash && transType != ACC2::TransMemo) {
        return NULL;
    }

    // Get itemFrom in case of Debtor or Creditor transaction,
    // first check if already edited
    RB_ObjectBase* itemFrom = mBankTransList->getObject(itemFromId);

    if (!itemFrom) {
        itemFrom = mMemoTransList->getObject(itemFromId);
    }

    if (!itemFrom) {
        if (transType == ACC2::TransBankCash) {
            itemFrom = mBankTransList->newObject(itemFromId);
        } else {
            itemFrom = mMemoTransList->newObject(itemFromId);
        }
    }

    if (!itemFrom->dbRead(ACC_MODELFACTORY->getDatabase())) {
        // transaction document not found
        RB_DEBUG->error("ACC_HandleAllocns::getItemFrom() get itemFrom ERROR");

        if (transType == ACC2::TransBankCash) {
            mBankTransList->remove(itemFrom, true);
        } else {
            mMemoTransList->remove(itemFrom, true);
        }

        return NULL;
    }

    return itemFrom;
}

/**
 * Undo the allocation to document by updating the corresponding
 * GL transaction (ACC_GlTrans), and set the GL Chart Master account to default
 * @param docToId ID of 'document to' of allocation to be removed
 */
void ACC_HandleAllocns::glTransToDefault(const RB_String& docToId) {
    // Temporary GL transaction list to be set to default
    RB_ObjectContainer* glTransList;
    glTransList = new RB_ObjectContainer("", NULL, "ACC_GlTransList", ACC_OBJECTFACTORY);
    glTransList->setValue("parent", docToId); // NOTE: actual parent is ACC_Project
    glTransList->dbReadList(ACC_MODELFACTORY->getDatabase(), RB2::ResolveOne,
                            "SUBSTR(transallocn_idx, 1, 38)");

    RB_ObjectIterator* iter = glTransList->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* glTrans = iter->currentObject();
        glTrans->setFlag(RB2::FlagIsDeleted);
        mPostGlTrans.execute(glTrans);

        glTrans->setValue("chartmaster_idx", ACC_QACHARTMASTER->getAccDefaultId()
                          + ACC_QACHARTMASTER->getAccDefaultName());
        glTrans->setValue("transallocn_idx", "0");
        glTrans->deleteFlag(RB2::FlagIsDeleted);
        mPostGlTrans.execute(glTrans);

        glTrans->dbUpdate(ACC_MODELFACTORY->getDatabase());
    }

    delete iter;
    delete glTransList;
}

/**
 * Create the allocation list of the itemModel.
 * This function is called when working with Bank/Cash or Memo transactions
 * @param itemModel Bank/Cash or Memo model
 */
void ACC_HandleAllocns::createAllocList(RB_MmProxy* itemModel) {
    int itemRowCount = itemModel->rowCount();

    if (itemRowCount < 1) {
        // no items in bank/cash or memo item model
        mIsAllocListCreated = true;
        return;
    }

    if (mIsAllocListCreated) {
        // already created from database
        // update not required from database
        return;
    }

    // get document from id
    QModelIndex idx = itemModel->index(0, itemModel->fieldIndex("transdoc_id"));
    RB_String docFromId = itemModel->data(idx, Qt::EditRole).toString();

    mTransAllocList->setValue("parent", docFromId);
    mTransAllocList->dbReadList(ACC_MODELFACTORY->getDatabase(),
                                RB2::ResolveOne, "docfrom_id");
    mIsAllocListCreated = true;
}

/**
 * Create the allocation list of the transaction document ID, has dual use.
 * This function is called when working with Debtor or Creditor transactions,
 * or a new allocation is created or an existing is removed,
 * from the Bank/Cash and Memo transactions
 * @param docToId debtor or creditor document ID
 */
void ACC_HandleAllocns::createAllocList(const RB_String& docToId) {
    if (mIsAllocListCreated) {
        // already created from database
        return;
    }

    if (docToId.size() < 38) {
        RB_DEBUG->error("ACC_HandleAllocns::createAllocList() docToId ERROR");
        return;
    }

    mTransAllocList->setValue("parent", docToId);
    mTransAllocList->dbReadList(ACC_MODELFACTORY->getDatabase(),
                                RB2::ResolveOne, "docallocto_id");
    mIsAllocListCreated = true;
}

/**
 * NOT YET USED
 * Update allocation list with all allocation to the transaction document
 * (a debtor or creditor invoice document) with ID docToId
 * @brief ACC_HandleAllocns::updateAllocList
 * @param docToId ID of document to which allocations are made
 */
void ACC_HandleAllocns::updateAllocList(const RB_String& docToId) {
    if (docToId.size() < 38) {
        RB_DEBUG->error("ACC_HandleAllocns::updateAllocList() docToId ERROR");
        return;
    }

    if (mTransAllocList->getObject("docallocto_id", docToId)) {
        // apparently already done or newly allocation created
        return;
    }

    mTransAllocList->setValue("parent", docToId);
    mTransAllocList->dbReadList(ACC_MODELFACTORY->getDatabase(),
                                RB2::ResolveOne, "docallocto_id");
}

/**
 * Get or create the allocation related to the current item transaction
 * @brief ACC_HandleAllocns::getTransAllocn
 * @param itemModel
 * @return ACC_TransAllocn
 */
RB_ObjectBase *ACC_HandleAllocns::getTransAllocn(RB_MmProxy* itemModel) {
    RB_ObjectBase* transAlloc = mTransAllocList->getObject("itemallocfrom_id",
                                                           itemModel->getCurrentId());
    if (!transAlloc) {
        transAlloc = mTransAllocList->newObject(RB_Uuid::createUuid().toString());
        transAlloc->setValue("parent", ACC_MODELFACTORY->getRootId());
    }

    return transAlloc;
}

/**
 * Get transaction document (debtor or creditor invoice) from list or
 * read from database
 * @brief ACC_HandleAllocns::getTransDocTo
 * @param docToId
 * @return ACC_TransDoc
 */
RB_ObjectBase *ACC_HandleAllocns::getTransDocTo(const RB_String& docToId) {
    // Check whether the transDocTo is already part of update list
    RB_ObjectBase* transDocTo = mTransDocList->getObject(docToId);

    if (!transDocTo) {
        // if not exists create new
        transDocTo = mTransDocList->newObject(docToId);

        // Read original data
        if (!transDocTo->dbRead(ACC_MODELFACTORY->getDatabase())) {
            RB_DEBUG->error("ACC_HandleAllocns::getTransDocTo() read ERROR");
            return NULL;
        }
    }

    return transDocTo;
}

double ACC_HandleAllocns::getAllocatedAmount(const RB_String& docToId) {
    updateAllocList(docToId);

    double allocatedAmount = 0;
    RB_ObjectIterator* iter = mTransAllocList->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* transAlloc = iter->currentObject();

        if (transAlloc->getValue("docallocto_id").toString() == docToId) {
            allocatedAmount += transAlloc->getValue("amount").toDouble();
        }
    }

    delete iter;
    return allocatedAmount;
}

/**
 * @brief ACC_HandleAllocns::getTransDocType
 * @param docId
 * @return
 */
ACC2::TransType ACC_HandleAllocns::getTransDocType(const RB_String& docId) {
    RB_ObjectBase* transDoc = ACC_OBJECTFACTORY->newSingleObject("ACC_TransDoc");
    transDoc->setId(docId);

    if (!transDoc->dbRead(ACC_MODELFACTORY->getDatabase())) {
        RB_DEBUG->error("ACC_HandleAllocns::getTransDocType() get transDoc ERROR");
        delete transDoc;
        return ACC2::TransNone;
    }

    ACC2::TransType transType = (ACC2::TransType)transDoc->getValue("doctype").toInt();
    delete transDoc;
    return transType;
}

/**
 * @returns transaction document list to update
 * the select alloction document dialog
 */
RB_ObjectContainer *ACC_HandleAllocns::getTransDocList() {
    return mTransDocList;
}

