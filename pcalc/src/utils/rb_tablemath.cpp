/*****************************************************************
 * $Id: rb_tablemath.cpp 2227 2015-03-09 20:08:03Z rutger $
 * Created: Mar 4, 2015 8:50:42 AM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_tablemath.h"
#include "rb_objectiterator.h"
NAMESPACE_BILUNA_CALC

RB_TableMath::RB_TableMath() {
    // nothing
}

RB_TableMath::~RB_TableMath() {
    // nothing
}

/**
 * @brief Biliniair interpolation, four points
 * with a value in a non-orthogonal square based on
 * http://en.wikipedia.org/wiki/Bilinear_interpolation
 *  p11 --- p1 --- p12
 *   |      |       |
 *   |    value     |
 *   |      |       |
 *  p21 --- p2 --- p22
 * @param valX x-coordinate of requested value
 * @param valY y-coordinate of requested value
 * @param p11X top left
 * @param p11Y
 * @param p11val
 * @param p12X top right
 * @param p12Y
 * @param p12val
 * @param p21X bottom left
 * @param p21Y
 * @param p21val
 * @param p22X bottom right
 * @param p22Y
 * @param p22val
 * @return result
 */
double RB_TableMath::getBilinearValue(
        double valX, double valY,
        double p11X, double p11Y, double p11val,
        double p12X, double p12Y, double p12val,
        double p21X, double p21Y, double p21val,
        double p22X, double p22Y, double p22val) {
    // data points do not need to be in a square, orthogonal
    // first calculate value on both in horizontal direction
    // and then the vertical direction
    double val1 = 0.0;
    double p1Y = 0.0;
    double val2 = 0.0;
    double p2Y = 0.0;

    if (p12X == p11X && p22X == p21X) {
        // now p11val and p12val normally should be the same,
        // same for p21val p22val
        val1 = (p11val + p12val) / 2;
        val2 = (p21val + p22val) / 2;
        p1Y = p11Y;
        p2Y = p21Y;
    } else {
        val1 = (p12X - valX) / (p12X - p11X) * p11val
                + (valX - p11X) / (p12X - p11X) * p12val;
        val2 = (p22X - valX) / (p22X - p21X) * p21val
                + (valX - p21X) / (p22X - p21X) * p22val;
        p1Y = (p12X - valX) / (p12X - p11X) * p11Y
                + (valX - p11X) / (p12X - p11X) * p12Y;
        p2Y = (p22X - valX) / (p22X - p21X) * p21Y
                + (valX - p21X) / (p22X - p21X) * p22Y;
    }

    double value = 0.0;

    if (p1Y == p2Y) {
        // now val1 and val2 normally should be the same
        value = (val1 + val2) / 2;
    } else {
        value = (p1Y - valY) / (p1Y - p2Y) * val2
                + (valY - p2Y) / (p1Y - p2Y) * val1;
    }

    return value;
}

double RB_TableMath::getInterpolatedValue(
        RB_ObjectContainer* fromObjC, const QString &xField,
        const QString &yField, const QString &zField, double xValue,
        double yValue, const QString& extraField, double extraValue) {
    mXfield = xField;
    mYfield = yField;
    mZfield = zField;
    mXvalue = xValue;
    mYvalue = yValue;
    mExtraField = extraField;
    mExtraValue = extraValue;
    mTopLeft = nullptr;
    mTopRight = nullptr;
    mBottomLeft = nullptr;
    mBottomRight = nullptr;

    RB_ObjectIterator* iter = fromObjC->createIterator();

    for(iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();
        updateCornerObjects(obj);
    }

    delete iter;

    double value = getBilinearValue(
                xValue, yValue,
                mTopLeft->getValue(mXfield).toDouble(),
                mTopLeft->getValue(mYfield).toDouble(),
                mTopLeft->getValue(mZfield).toDouble(),
                mTopRight->getValue(mXfield).toDouble(),
                mTopRight->getValue(mYfield).toDouble(),
                mTopRight->getValue(mZfield).toDouble(),
                mBottomLeft->getValue(mXfield).toDouble(),
                mBottomLeft->getValue(mYfield).toDouble(),
                mBottomLeft->getValue(mZfield).toDouble(),
                mBottomRight->getValue(mXfield).toDouble(),
                mBottomRight->getValue(mYfield).toDouble(),
                mBottomRight->getValue(mZfield).toDouble());
    return value;
}

void RB_TableMath::updateCornerObjects(RB_ObjectBase* obj) {
    if (!mExtraField.isEmpty()
            && obj->getValue(mExtraField).toString() != mExtraValue) {
        return;
    }

    double xValue = obj->getValue(mXfield).toDouble();
    double yValue = obj->getValue(mYfield).toDouble();

    // Top left
    if (!mTopLeft && xValue < mXvalue && yValue > mYvalue) {
        mTopLeft = obj;
    } else if (xValue > mTopLeft->getValue(mXfield).toDouble()
               && yValue < mTopLeft->getValue(mYfield).toDouble()
               && xValue < mXvalue
               && yValue > mYvalue) {
        mTopLeft = obj;
    }

    // Top right
    if (!mTopRight && xValue > mXvalue && yValue > mYvalue) {
        mTopRight = obj;
    } else if (xValue < mTopRight->getValue(mXfield).toDouble()
               && yValue < mTopRight->getValue(mYfield).toDouble()
               && xValue > mXvalue
               && yValue > mYvalue) {
        mTopRight = obj;
    }

    // Bottom left
    if (!mBottomLeft && xValue < mXvalue && yValue < mYvalue) {
        mBottomLeft = obj;
    } else if (xValue > mBottomLeft->getValue(mXfield).toDouble()
               && yValue > mBottomLeft->getValue(mYfield).toDouble()
               && xValue < mXvalue
               && yValue < mYvalue) {
        mBottomLeft = obj;
    }

    // Bottom right
    if (!mBottomRight && xValue > mXvalue && yValue < mYvalue) {
        mBottomRight = obj;
    } else if (xValue < mBottomRight->getValue(mXfield).toDouble()
               && yValue > mBottomRight->getValue(mYfield).toDouble()
               && xValue > mXvalue
               && yValue < mYvalue) {
        mBottomRight = obj;
    }
}

END_NAMESPACE_BILUNA_CALC
