/*****************************************************************
 * $Id: acc_handleallocns.h 2214 2015-01-28 17:07:32Z rutger $
 * Created: Mar 8, 2010 8:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_HANDLEALLOCNS_H
#define ACC_HANDLEALLOCNS_H

#include "acc.h"
#include "acc_operation.h"
#include "acc_postgltransaction.h"

class RB_MmProxy;
class RB_ObjectContainer;

/**
 * Operation class to handle transaction alloctions from bank/cash and journal
 * entries to debtor and creditor transaction documents, such as invoices
 */
class ACC_HandleAllocns : public ACC_Operation  {

public:
    ACC_HandleAllocns();
    virtual ~ACC_HandleAllocns();

    virtual bool execute(RB_ObjectBase* obj = NULL);

    bool submitAllAndSelect();
    bool revertAll();

    virtual bool addAllocn(RB_MmProxy* itemModel,
                           const RB_String& docToId,
                           const RB_String& docToDspl,
                           const RB_String& docFromId);

    virtual void delItemAllocn(RB_MmProxy* itemModel,
                               bool isDocItemDeleted);
    virtual void delItemListAllocn(RB_MmProxy* itemModel);

    virtual void delDocAllocn(const RB_String& docId, bool isDeleted);

    virtual void updateItemAllocnAmt(RB_MmProxy *itemModel, double amt);
    virtual void updateTransDate(RB_MmProxy* itemModel, const QDate &date);
    virtual bool updateGlTransList(RB_ObjectContainer* glTransList);

    virtual RB_ObjectContainer* getTransDocList();

protected:
    void undoItemAllocationFromBM(RB_ObjectBase* transAlloc,
                                  RB_MmProxy* itemModel,
                                  bool isPost);
    void undoItemAllocationToDC(RB_ObjectBase* transAlloc,
                                bool isPost);
    RB_ObjectBase* getItemFrom(RB_ObjectBase* transAlloc);
    void glTransToDefault(const RB_String& docToId);
    void createAllocList(RB_MmProxy* itemModel);
    void createAllocList(const RB_String& docToId);
    void updateAllocList(const RB_String& docToId);
    RB_ObjectBase* getTransAllocn(RB_MmProxy* itemModel);

    RB_ObjectBase* getTransDocTo(const RB_String& docToId);
    double getAllocatedAmount(const RB_String& docToId);
    ACC2::TransType getTransDocType(const RB_String& docId);

    //! Transaction document list for debtor and creditor documents (docallocto_id)
    RB_ObjectContainer* mTransDocList;
    //! Transaction item list for bank/cash transaction items (itemallocfrom_id)
    RB_ObjectContainer* mBankTransList;
    //! Transaction item list for memorial transaction items (itemallocfrom_id)
    RB_ObjectContainer* mMemoTransList;
    //! Transaction allocation list
    RB_ObjectContainer* mTransAllocList;
    //! GL transaction list changed by allocations
    RB_ObjectContainer* mGlTransList;
    //! Flag true if existing allocation list is created
    bool mIsAllocListCreated;

    //! Post GL transaction operation
    ACC_PostGlTransaction mPostGlTrans;
};

#endif // ACC_HANDLEALLOCNS_H
