/*****************************************************************
 * $Id: acc_dbvisitor.h 1532 2012-01-23 22:44:15Z rutger $
 * Created: Apr 2, 2012 8:53:43 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_valueassethelper.h"

#include "acc.h"
#include "rb_objectcontainer.h"

/**
 * Constructor
 * @param outputList output list for asset value determination
 */
ACC_ValueAssetHelper::ACC_ValueAssetHelper(RB_ObjectContainer* outputList) {
    mOutputList = outputList;
    mPeriodAttr = NULL;
    mErrorMessage = "";
}

void ACC_ValueAssetHelper::visitObject(RB_ObjectBase *obj) {
    if (!mOutputList || !obj) {
        RB_DEBUG->error("ACC_ValueAssetHelper::visitObject() object(s) NULL ERROR");
        return;
    }

    if (obj->isList()) {
        return;
    }

    int interval = obj->getValue("interval").toInt();
    double weight = obj->getValue("weight").toDouble();
    double val = obj->getValue("value").toDouble();
    double tmpVal = 0.0; // intermediate value

    //
    // if no parent, set: intermediatevalue = value
    // else: intermediatevalue = parent.resultvalue * value
    //
    if (!obj->getGrandParent()) {
        tmpVal = val;
    } else {
        RB_ObjectBase* grParent = obj->getGrandParent();
        double resVal = grParent->getValue("resultvalue").toDouble();
        if (interval == ACC2::IntervalNone) {
            tmpVal = resVal * val;
        } else {
            tmpVal = resVal; // val is incremental value
        }
    }

    //
    // if interval = 0 (once) resultvalue = intermediatevalue * weight
    // else get end-start date create new objects accordingly based on interval
    //     and set first i = 0 and resultvalue = intermediatevalue * i * weight
    //
    if (interval == ACC2::IntervalNone) { // once
        obj->setValue("resultvalue", tmpVal * weight / 100.0);

        RB_ObjectContainer* objC = obj->getContainer("ACC_AttrTreeList");

        if (mPeriodAttr && (objC && objC->countObject() <= 0)) {
            // Period already and no children, create period output list
            createPeriodOutput(obj, tmpVal);
        } else if (!objC || (objC && objC->countObject() <= 0)) {
            // No children, add to output list
            obj->detachFromParent();
            mOutputList->addObject(obj);
        }
    } else if (!mPeriodAttr) {
        // Not yet passed a period attribute
        obj->setValue("resultvalue", tmpVal * weight/100);
        mPeriodAttr = obj;
        createPeriodOutput(obj, tmpVal);
    } else {
        mErrorMessage += "More than one period attribute error <br />";
    }
}

void ACC_ValueAssetHelper::leaveObject(RB_ObjectBase* obj) {
    if (mPeriodAttr == obj) {
        mPeriodAttr = NULL;
    }
}

void ACC_ValueAssetHelper::createPeriodOutput(RB_ObjectBase* obj,
                                              double tmpVal) {
    // calculate the number of periods
    QDate startDate = mPeriodAttr->getValue("startdate").toDate();
    QDate endDate = mPeriodAttr->getValue("enddate").toDate();
    int interval = mPeriodAttr->getValue("interval").toInt();
    double weight = obj->getValue("weight").toDouble();
    double periodVal = mPeriodAttr->getValue("value").toDouble();

    if (startDate < obj->getValue("startdate").toDate()) {
        startDate = obj->getValue("startdate").toDate();
    }

    if (endDate > obj->getValue("enddate").toDate()) {
        endDate = obj->getValue("enddate").toDate();
    }

    QDate periodDate = startDate;
    int periods = 0;

    if (interval == ACC2::IntervalMonth) {  // month
        int years = endDate.year() - startDate.year();
        int months = endDate.month() - startDate.month();
        periods = years * 12 + months;

        if (periods <= 0) {
            int seqNo = obj->getValue("seqno").toInt();
            mErrorMessage += "Warning: period is 0 in seqno = "
                    + RB_String::number(seqNo) + "<br />";
        }

        for (int i = 0; i < periods; ++i) {
            RB_ObjectBase* newObj = NULL;
            newObj = obj->copy(RB2::ResolveAll);
            periodDate = startDate.addMonths(i);
            newObj->setValue("startdate", periodDate);

            // set resultvalue
            if (i == 0) {
                newObj->setValue("resultvalue", tmpVal * weight/100);
            } else {
                newObj->setValue("resultvalue", tmpVal * weight/100 * pow(periodVal, i));
            }

            // In case of no children add to output list
            newObj->detachFromParent();
            mOutputList->addObject(newObj);
        }

    } else { // year
        periods = endDate.year() - startDate.year();

        if (periods <= 0) {
            int seqNo = obj->getValue("seqno").toInt();
            mErrorMessage += "Warning: period is 0 in seqno = "
                    + RB_String::number(seqNo) + "<br />";
        }

        for (int i = 0; i < periods; ++i) {
            RB_ObjectBase* newObj = NULL; // obj;

            // create new object
            newObj = obj->copy(RB2::ResolveAll);
            periodDate = startDate.addYears(i);
            newObj->setValue("startdate", periodDate);

            // set resultvalue
            if (i == 0) {
                newObj->setValue("resultvalue", tmpVal * weight/100);
            } else {
                newObj->setValue("resultvalue", tmpVal * weight/100 * pow(periodVal, i));
            }

            // In case of no children add to output list
            newObj->detachFromParent();
            mOutputList->addObject(newObj);
        }
    }
}

/**
 * @returns possible error messages
 */
RB_String ACC_ValueAssetHelper::getErrorMessage() {
    return mErrorMessage;
}

