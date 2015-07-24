/*****************************************************************
 * $Id: acc_handleallocns_old.cpp 2212 2015-01-28 16:45:01Z rutger $
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
            // due to docTo amount is changed
            glTransToDefault(transAlloc->getValue("docallocto_id").toString());
        }
    }

    delete iter;

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
    // Undo existing allocation if exists
    // The relevant transAlloc, docTo and itemFrom are now part of list
    // Below is false because doc item is reversable and not deleted
    delItemAllocn(itemModel, false);

    // Check whether the docTo is already part of update list
    RB_ObjectBase* docTo = mTransDocList->getObject(docToId);

    if (!docTo) {
        // if not exists create new
        docTo = mTransDocList->newObject(docToId);

        // Read original data
        if (!docTo->dbRead(ACC_MODELFACTORY->getDatabase())) {
            RB_DEBUG->error("ACC_HandleAllocns::addAllocn() docTo read ERROR");
            return false;
        }
    }

    double docAmt = docTo->getValue("totalamountrec").toDouble();

    // if not received amount than apparently a payable amount
    if (std::abs(docAmt) < 0.005) { // not almost zero
        docAmt = -docTo->getValue("totalamountpay").toDouble();
    }

    // already allocated amount and calculate allocatable amount
    double docAllocAmt = docTo->getValue("alloc").toDouble();
    double allocAmt = docAmt - docAllocAmt; // allocatable/remaining amount

    if (allocAmt == 0.0) {
        RB_DEBUG->warning("ACC_HandleAllocns::addAllocn() allocatable amount=0 WARNING");
        return false;
    }

    double bankMemItemAmt = itemModel->getCurrentValue("amount", Qt::EditRole).toDouble();
    double newAllocAmt = 0.0;
    if (bankMemItemAmt <= allocAmt
            && itemModel->getModelType() == (int)ACC_MODELFACTORY->ModelBankTrans
            && docTo->getValue("doctype").toInt() == (int)ACC2::TransDebtor) {
        newAllocAmt = bankMemItemAmt;
    } else if (bankMemItemAmt >= allocAmt
                && itemModel->getModelType() == (int)ACC_MODELFACTORY->ModelBankTrans
                && docTo->getValue("doctype").toInt() == (int)ACC2::TransCreditor) {
        newAllocAmt = bankMemItemAmt;
    } else {
        newAllocAmt = allocAmt;
        // Update current item model bank/cash or gltrans
        itemModel->setCurrentValue("amount", allocAmt, Qt::EditRole);
    }

    itemModel->setCurrentValue("transallocn_idx", docToId + docToDspl, Qt::EditRole);

    // Use the document amount minus the already allocated amount
    // as starting point, therefore the bank amountcleared will be
    // the same as the document amount minus the allocated amount
    if (docTo->getValue("doctype").toInt() == (int)ACC2::TransDebtor
            || docTo->getValue("doctype").toInt() == (int)ACC2::TransSalesOrder) {
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

    if (itemModel->getModelType() == (int)ACC_MODELFACTORY->ModelBankTrans) {
        itemModel->setCurrentValue("amountcleared", newAllocAmt, Qt::EditRole);
    }

    // Update debtor or creditor document to 'settled' if full amount is set
    docTo->setValue("alloc", newAllocAmt + docAllocAmt);

    if (std::abs(newAllocAmt - allocAmt) < 0.005) {
        docTo->setValue("settled", 1);
    } else {
        docTo->setValue("settled", 0);
    }

    // Create allocation
    RB_ObjectBase* transAlloc = mTransAllocList->getObject("itemallocfrom_id",
                                                           itemModel->getCurrentId());
    if (!transAlloc) {
        transAlloc = mTransAllocList->newObject(RB_Uuid::createUuid().toString());
        transAlloc->setValue("parent", ACC_MODELFACTORY->getRootId());
        transAlloc->setValue("itemallocfrom_id", itemModel->getCurrentId());
    }

    QDateTime dt = QDateTime::currentDateTime();

    if (itemModel->getModelType() == (int)ACC_MODELFACTORY->ModelBankTrans) {
        dt = itemModel->getCurrentValue("transdate").toDateTime();
    }

    // Flag could have been set to deleted
    transAlloc->delFlag(RB2::FlagIsDeleted);
    transAlloc->setValue("amount", newAllocAmt);
    transAlloc->setValue("datealloc", dt);
    transAlloc->setValue("docfrom_id", docFromId);
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

    if (!transAlloc) {
        // Check if allocation exists in database or is created
        return;
    }

    undoItemAllocation(transAlloc, itemModel, isDocItemDeleted);

    if (isDocItemDeleted) {
        // Deletion is non reversable
        mTransAllocList->remove(transAlloc, true);
    }
}

/**
 * Delete the item list allocation of a bank/cash or journal
 * (General Ledger) document, cannot be undone.
 * @param bankGlTransModel GL transaction model with the document transactions
 */
void ACC_HandleAllocns::delItemListAllocn(RB_MmProxy* itemModel, bool isDeleted) {
    if (itemModel->getModelType() != ACC_MODELFACTORY->ModelBankTrans
        && itemModel->getModelType() != ACC_MODELFACTORY->ModelMemoTrans) {
        RB_DEBUG->error("ACC_HandleAllocns::delTransAllocnList() modeltype ERROR");
        return;
    }

    int itemRowCount = itemModel->rowCount();
    QModelIndex currentIdx = itemModel->getCurrentIndex();
    QModelIndex idx = QModelIndex();

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
 * Delete the allocation to a debtor or creditor document,
 * only called with a debtor or creditor document ID when the document
 * or the item is deleted. Also when the amount in the debtor or creditor
 * document is changed. In that case the change is reversable.
 * @param docToId document ID
 * @param isDeleted document or item is deleted and is not reversable
 */
void ACC_HandleAllocns::delDocAllocn(const RB_String& docToId, bool isDeleted) {
    createAllocList(docToId);

    // Get transAlloc, check if already edited
    RB_ObjectBase* transAlloc = mTransAllocList->getObject("docallocto_id", docToId);

    if (!transAlloc) {
        // Check if alloc exists in database or is already created
        return;
    }

    RB_ObjectIterator* iter = mTransAllocList->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* transAlloc = iter->currentObject();
        undoItemAllocation(transAlloc, NULL, isDeleted);
    }

    delete iter;

    if (isDeleted) {
        mTransAllocList->remove(transAlloc, true);
    }
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
 *   reset to equal and return this equal amount
 * TODO: how about credit sales invoices with possible negative amount?
 * @param docFromId document from ID (journal or bank/cash document)
 * @param itemModel item model (journal or bank/cash model)
 * @param amt current amount
 * @return total docTo (Debtor or Creditor) amount
 */
void ACC_HandleAllocns::updateItemAllocnAmt(RB_MmProxy* itemModel,
                                            double amt) {
    // Check if there is an allocation (this is the 'docTo' ID and Name)
    RB_String allocId = itemModel->getCurrentValue("transallocn_idx",
                                                   RB2::RoleOrigData).toString();
    if (allocId.size() < 38) {
        return; // no allocation
    }

    createAllocList(itemModel);

    // Get transAlloc, check if already edited
    RB_String itemId = itemModel->getCurrentId();
    RB_ObjectBase* transAlloc = mTransAllocList->getObject("itemallocfrom_id", itemId);

    if (transAlloc) {
        double previousAllocAmt = transAlloc->getValue("amount").toDouble();

        if (previousAllocAmt == amt) {
            return;
        }
    } else {
        return;
    }

    RB_String docToId = transAlloc->getValue("docallocto_id").toString();

    // Get transDocTo, first check if already edited
    RB_ObjectBase* transDocTo = mTransDocList->getObject(docToId);

    if (!transDocTo) {
        transDocTo = mTransDocList->newObject(docToId);

        // Get transDocTo
        if (!transDocTo->dbRead(ACC_MODELFACTORY->getDatabase())) {
            // transaction document not found
            RB_DEBUG->error("ACC_HandleAllocns::updateItemAllocnAmt() get transDocTo ERROR");
            mTransAllocList->remove(transAlloc, true);
            mTransDocList->remove(transDocTo, true);
            return;
        }
    }

    // Allocation values
    double prevAllocAmt = transAlloc->getValue("amount").toDouble(); // previous allocated, pos or neg
    double docToAmt = 0.0; // always positive

    if (transDocTo->getValue("doctype").toInt() == int(ACC2::TransDebtor)
            || transDocTo->getValue("doctype").toInt() == int(ACC2::TransSalesOrder)) {
        docToAmt = transDocTo->getValue("totalamountrec").toDouble();
    } else {
        // create negative value
        docToAmt = -transDocTo->getValue("totalamountpay").toDouble();
    }

    // Can be positive (ref. totalamountrec) or negative (ref. totalamountpay)
    double currentDocToAllocAmt = transDocTo->getValue("alloc").toDouble();

    // Calculate what the newly total allocated amount for this document would be,
    // the allocated amount can also be too much, in that case settled=0
    double newDocAllocAmt = currentDocToAllocAmt - prevAllocAmt + amt;

    // Select what to do
    if (std::fabs(docToAmt - newDocAllocAmt) < 0.005) {
        // full allocation
        transDocTo->setValue("settled", 1);
        transDocTo->setValue("alloc", newDocAllocAmt);
        itemModel->setCurrentValue("amountcleared", amt, Qt::EditRole);
        transAlloc->setValue("amount", amt);
    } else if ((docToAmt < 0.0 && docToAmt < newDocAllocAmt && amt < 0.0)
               || (docToAmt > 0.0 && docToAmt > newDocAllocAmt && amt > 0.0)) {
        // partial allocation
        transDocTo->setValue("settled", 0);
        transDocTo->setValue("alloc", newDocAllocAmt);
        if (itemModel->getModelType() != ACC_ModelFactory::ModelMemoTrans) {
            itemModel->setCurrentValue("amountcleared", amt, Qt::EditRole);
        }
        transAlloc->setValue("amount", amt);
    } else if ((docToAmt < 0.0 && docToAmt > newDocAllocAmt && amt < 0.0)
               || (docToAmt > 0.0 && docToAmt < newDocAllocAmt && amt > 0.0)) {
        // over allocation
        transDocTo->setValue("settled", 1);
        transDocTo->setValue("alloc", docToAmt);
        itemModel->setCurrentValue("amountcleared", docToAmt, Qt::EditRole);
        itemModel->setCurrentValue("amount", docToAmt, Qt::EditRole);
        transAlloc->setValue("amount", docToAmt);
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
void ACC_HandleAllocns::updateTransDate(RB_MmProxy* itemModel, const QDateTime& dt) {
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
 * @param alloc allocation object to be undone
 * @param isPost is true if this action is to be posted immediately
 * to the database, does not delete the allocation.
 */
void ACC_HandleAllocns::undoItemAllocation(RB_ObjectBase* transAlloc,
                                           RB_MmProxy* itemModel, bool isPost) {
    RB_String docToId = transAlloc->getValue("docallocto_id").toString();
    RB_String docFromId = transAlloc->getValue("docfrom_id").toString();
    RB_String itemFromId = transAlloc->getValue("itemallocfrom_id").toString();

    if (docToId.size() < 38 || docFromId.size() < 38 || itemFromId.size() < 38) {
        // not valid Uuid
        RB_DEBUG->error("ACC_HandleAllocns::undoItemAllocation() Uuid ERROR");
        return;
    }

    // Get the transdoc (from) only to determine the item allocation from type
    RB_ObjectBase* transDocFrom = ACC_OBJECTFACTORY->newSingleObject("ACC_TransDoc");
    transDocFrom->setId(docFromId);

    if (!transDocFrom->dbRead(ACC_MODELFACTORY->getDatabase())) {
        // transaction document not found
        RB_DEBUG->error("ACC_HandleAllocns::undoItemAllocation() get transDocFrom ERROR");
        delete transDocFrom;
        return;
    }

    ACC2::TransType transType = ACC2::TransBankCash;

    if (transDocFrom->getValue("doctype").toInt() == int(ACC2::TransMemo)) {
        transType = ACC2::TransMemo;
    }

    delete transDocFrom;
    transDocFrom = NULL;

    // itemFrom in case of no itemModel
    RB_ObjectBase* itemFrom = NULL;

    if (!itemModel) {
        // Get itemFrom in case of Debtor or Creditor transaction,
        // first check if already edited
        itemFrom = mBankTransList->getObject(itemFromId);

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
            RB_DEBUG->error("ACC_HandleAllocns::undoItemAllocation() get itemFrom ERROR");

            if (transType == ACC2::TransBankCash) {
                mBankTransList->remove(itemFrom, true);
            } else {
                mMemoTransList->remove(itemFrom, true);
            }

            return;
        }
    }

    // Get transDocTo, first check if already edited
    RB_ObjectBase* transDocTo = mTransDocList->getObject(docToId);

    if (!transDocTo) {
        transDocTo = mTransDocList->newObject(docToId);

        if (!transDocTo->dbRead(ACC_MODELFACTORY->getDatabase())) {
            // transaction document not found
            RB_DEBUG->error("ACC_HandleAllocns::undoItemAllocation() get transDocTo ERROR");

            if (transType == ACC2::TransBankCash) {
                mBankTransList->remove(itemFrom, true);
            } else {
                mMemoTransList->remove(itemFrom, true);
            }

            mTransDocList->remove(transDocTo, true);
            return;
        }
    }


    // Allocation values
    double allocAmt = transAlloc->getValue("amount").toDouble(); // pos or neg
    // Can be positive (ref. totalamountrec) or negative (ref. totalamountpay)
    double docToAllocAmt = transDocTo->getValue("alloc").toDouble();

    // Unset transallocn_idx at bank/cash or memo transaction item
    // set chartmaster_idx to default GL account
    // and amountcleared for bank/cash only
    if (!itemModel) {
        // Called from debtor or creditor
        itemFrom->setValue("transallocn_idx", "0");
        itemFrom->setValue("chartmaster_idx", ACC_QACHARTMASTER->getAccDefaultId());
        itemFrom->setDValue("chartmaster_idx", ACC_QACHARTMASTER->getAccDefaultName());

        if (transType == ACC2::TransBankCash) {
            itemFrom->setValue("amountcleared", 0.0);
        }
    } else {
        // Called from bank/cash or memo
        itemModel->setCurrentValue("transallocn_idx", "0", Qt::EditRole);
        itemModel->setCurrentValue("chartmaster_idx", ACC_QACHARTMASTER->getAccDefaultId()
                                   + ACC_QACHARTMASTER->getAccDefaultName(),
                                   Qt::EditRole);
        if (transType == ACC2::TransBankCash) {
            itemModel->setCurrentValue("amountcleared", 0.0, Qt::EditRole);
        }
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
        // transAlloc deleted by calling function
        transAlloc->dbUpdate(ACC_MODELFACTORY->getDatabase());
        transDocTo->dbUpdate(ACC_MODELFACTORY->getDatabase());
        mTransDocList->remove(transDocTo, true);

        if (itemFrom) {
            itemFrom->dbUpdate(ACC_MODELFACTORY->getDatabase());

            if (transType == ACC2::TransBankCash) {
                mBankTransList->remove(itemFrom, true);
            } else {
                mMemoTransList->remove(itemFrom, true);
            }
        }

        // repost the corresponding ACC_GlTrans
        glTransToDefault(transAlloc->getValue("docallocto_id").toString());
    } else {
        if (!itemModel) {
            // transDocTo is not required, since there is no itemModel
            // and transDocTo is handled by ACC_GlTransactionWidget
            mTransDocList->remove(transDocTo, true);
        } else if (itemFrom) {
            // itemFrom is not required, since there is a itemModel
            if (transType == ACC2::TransBankCash) {
                mBankTransList->remove(itemFrom, true);
            } else {
                mMemoTransList->remove(itemFrom, true);
            }
        }
    }
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

        glTrans->setValue("chartmaster_idx", ACC_QACHARTMASTER->getAccDefaultId());
        glTrans->setDValue("chartmaster_idx", ACC_QACHARTMASTER->getAccDefaultName());
        glTrans->setValue("transallocn_idx", "0");
        glTrans->delFlag(RB2::FlagIsDeleted);
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
    if (mIsAllocListCreated) {
        // already created from database
        return;
    }

    int itemRowCount = itemModel->rowCount();

    if (itemRowCount < 1) {
        // no items in bank/cash or memo item model
        mIsAllocListCreated = true;
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
 * Update allocation list with all allocation to transaction document
 * (a debtor or creditor document) with ID docToId
 * @brief ACC_HandleAllocns::updateAllocList
 * @param docToId ID of document to which allocations are made
 */
void ACC_HandleAllocns::updateAllocList(const RB_String& docToId) {
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
 * @returns transaction document list to update
 * the select alloction document dialog
 */
RB_ObjectContainer *ACC_HandleAllocns::getTransDocList() {
    return mTransDocList;
}

