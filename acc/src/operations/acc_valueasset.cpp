/*****************************************************************
 * $Id: acc_valueasset.cpp 1616 2012-04-18 20:14:54Z rutger $
 * Created: Mar 31, 2012 3:53:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_valueasset.h"

#include "acc_valueassethelper.h"

/**
 * Constructor
 */
ACC_ValueAsset::ACC_ValueAsset() {
    mInputList = NULL;
    mOutputList = NULL;
    mAttrTree = NULL;

    mErrorMessage = "";
}

/**
 * Destructor
 */
ACC_ValueAsset::~ACC_ValueAsset() {
    if (mAttrTree) {
        delete mAttrTree;
    }

    RB_DEBUG->print("ACC_ValueAsset::~ACC_ValueAsset() OK");
}

/**
 * Execute operation
 * @param inputList list with input data
 * @param outputList list with output data
 * @returns true if operation was successfull
 */
bool ACC_ValueAsset::execute(RB_ObjectBase* inputList,
                               RB_ObjectBase* outputList) {
    mInputList = dynamic_cast<RB_ObjectContainer*>(inputList);
    mOutputList = dynamic_cast<RB_ObjectContainer*>(outputList);

    if (!mInputList || !mOutputList) {
        RB_DEBUG->error("ACC_ValueAsset::execute() valuation date,"
                        " input- or outputList not valid ERROR");
        return false;
    }

    if (!mAttrTree) {
        mAttrTree = new RB_ObjectContainer("", NULL, "ACC_AttrTreeList");
    }

    // to make sure
    mOutputList->erase();
    mInputList->sort("seqno", RB2::SortOrderAscending, RB2::MemberInteger);

    bool success = checkInputList();

    if (success) {
        success = createAttributeTree();
    }

    if (success) {
        success = createOutputList();
    } else {
        mErrorMessage += "Create attribute tree ERROR <br />";
    }

    if (!success) {
        mErrorMessage += "Create output list ERROR <br />";
    }

    return success;
}

/**
 * Set valuation (start date)
 * @param date valuation date, usually today
 */
void ACC_ValueAsset::setValuationDate(const QDate& date) {
    mDate = date;
}

RB_String ACC_ValueAsset::getErrorMessage() {
    return mErrorMessage;
}

/**
 * Check input list that all seqno are larger thane multiply with
 * @returns true if check successfull
 */
bool ACC_ValueAsset::checkInputList() {
    int seqNo;
    int multiplyWith;
    RB_ObjectIterator* iter = mInputList->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();
        seqNo = obj->getValue("seqno").toInt();
        multiplyWith = obj->getValue("multiplywith").toInt();

        if (seqNo <= multiplyWith) {
            mErrorMessage += "seqno ";
            mErrorMessage += RB_String::number(seqNo);
            mErrorMessage += " should be larger than the multiplywith value ";
            mErrorMessage += RB_String::number(multiplyWith) + "<br />";
        }
    }

    delete iter;

    if (!mErrorMessage.isEmpty()) {
        return false;
    }

    return true;
}

/**
 * Create tree of input attribute objects
 * Note: this function is not optimized for efficiency or speed
 */
bool ACC_ValueAsset::createAttributeTree() {
    int seqNo;
    int multiplyWith;
    RB_ObjectIterator* iter = mInputList->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();
        multiplyWith = obj->getValue("multiplywith").toInt();
        seqNo = obj->getValue("seqno").toInt();

        RB_ObjectBase* parentObj = mAttrTree->getObject("seqno", multiplyWith);
        RB_ObjectBase* childObj = mAttrTree->getObject("multiplywith", seqNo);
        RB_ObjectBase* treeObj = obj->clone(RB2::ResolveNone);
        // Set name otherwise getObject() does not search tree
        treeObj->setName("ACC_AttrTree");
        // Remove object factory, otherwise copy and cloning will crash
        treeObj->setFactory(NULL);

        RB_ObjectContainer* objC = treeObj->getContainer("ACC_AttrTreeList");

        // Create container if not exists
        if (!objC) {
            objC = new RB_ObjectContainer("", treeObj, "ACC_AttrTreeList");
            treeObj->addObject(objC);
        }

        if (parentObj) {
            objC = parentObj->getContainer("ACC_AttrTreeList");
            treeObj->detachFromParent();
            objC->addObject(treeObj);
        } else {
            treeObj->detachFromParent();
            mAttrTree->addObject(treeObj);
        }

        if (childObj) {
            childObj->detachFromParent();
            objC->addObject(childObj);
        }

    }

    delete iter;
    return true;
}


/**
 * @returns true in case output list was created
 */
bool ACC_ValueAsset::createOutputList() {
    // in visitor:

    // loop: start with first object in mAttrTree

    // if no parent set: intermediatevalue = value
    // else: intermediatevalue = parent.resultvalue * value

    // if interval = 0 (once) resultvalue = intermediatevalue * weight
    // else get end-start date create new objects accordingly based on interval
    //     and set first i = 0 and resultvalue = intermediatevalue * i * weight

    // end loop

    ACC_ValueAssetHelper* visitor = new ACC_ValueAssetHelper(mOutputList);
    mAttrTree->acceptVisitor(*visitor);

    if (!visitor->getErrorMessage().isEmpty()) {
        mErrorMessage += visitor->getErrorMessage();
        delete visitor;
        return false;
    }

    delete visitor;
    return true;
}

