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

    if (p12X == p11X) {
        // now p11val and p12val normally should be the same,
        // same for p21val p22val
        val1 = (p11val + p12val) / 2;
        p1Y = p11Y;
    } else {
        val1 = (p12X - valX) / (p12X - p11X) * p11val
                + (valX - p11X) / (p12X - p11X) * p12val;
        p1Y = (p12X - valX) / (p12X - p11X) * p11Y
                + (valX - p11X) / (p12X - p11X) * p12Y;

    }

    if (p22X == p21X) {
        val2 = (p21val + p22val) / 2;
        p2Y = p21Y;
    } else {
        val2 = (p22X - valX) / (p22X - p21X) * p21val
                + (valX - p21X) / (p22X - p21X) * p22val;
        p2Y = (p22X - valX) / (p22X - p21X) * p21Y
                + (valX - p21X) / (p22X - p21X) * p22Y;
    }

//    if (p12X == p11X && p22X == p21X) {
//        // now p11val and p12val normally should be the same,
//        // same for p21val p22val
//        val1 = (p11val + p12val) / 2;
//        val2 = (p21val + p22val) / 2;
//        p1Y = p11Y;
//        p2Y = p21Y;
//    } else {
//        val1 = (p12X - valX) / (p12X - p11X) * p11val
//                + (valX - p11X) / (p12X - p11X) * p12val;
//        val2 = (p22X - valX) / (p22X - p21X) * p21val
//                + (valX - p21X) / (p22X - p21X) * p22val;
//        p1Y = (p12X - valX) / (p12X - p11X) * p11Y
//                + (valX - p11X) / (p12X - p11X) * p12Y;
//        p2Y = (p22X - valX) / (p22X - p21X) * p21Y
//                + (valX - p21X) / (p22X - p21X) * p22Y;
//    }

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

/**
 * @brief RB_TableMath::getLinearValue get linear interpolated value
 * @param valX x-coordinate for which the y value is required
 * @param p11X
 * @param p11val
 * @param p12X
 * @param p12val
 * @return
 */
double RB_TableMath::getLinearValue(double valX,
                                    double p1X, double p1val,
                                    double p2X, double p2val) {
    if (p1X > valX || valX > p2X) {
        RB_DEBUG->error("RB_TableMath::getLinearValue() p1X valX p2X ERROR");
        return 0.0;
    } else if (p1X == p2X) {
        return (p1val + p2val) / 2;
    }
    double ratio = (valX - p1X) / (p2X - p1X);
    double value = p1val * (1 - ratio) + p2val * ratio;
    return value;
}

/**
 * @brief RB_TableMath::getBilInterpValue get bilinear interpolated value
 * @param fromObjC
 * @param xField
 * @param yField
 * @param zField
 * @param xValue
 * @param yValue
 * @param extraField
 * @param extraValue
 * @return result value
 */
double RB_TableMath::getBilInterpValue(
        RB_ObjectContainer* fromObjC, const QString& xField,
        const QString& yField, const QString& zField, double xValue,
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
    double value = 0.0;

    if (!mTopLeft || !mTopRight || !mBottomLeft || !mBottomRight) {
        value = getOutOfBoundValue();
    } else {
        value = getBilinearValue(
                mXvalue, mYvalue,
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
    }

    return value;
}

/**
 * @brief RB_TableMath::getLinInterpValue get linear interpolated value
 * @param fromObjC
 * @param xField
 * @param yField
 * @param xValue
 * @param extraField
 * @param extraValue
 * @return interpolated value
 */
double RB_TableMath::getLinInterpValue(RB_ObjectContainer *fromObjC,
                                       const QString &xField,
                                       const QString &yField,
                                       double xValue,
                                       const QString &extraField,
                                       double extraValue) {
    mXfield = xField;
    mYfield = yField;
    mXvalue = xValue;
    mExtraField = extraField;
    mExtraValue = extraValue;
    mTopLeft = nullptr;
    mTopRight = nullptr;
    mBottomLeft = nullptr;
    mBottomRight = nullptr;

    RB_ObjectIterator* iter = fromObjC->createIterator();

    for(iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();
        updateLeftRightObjects(obj); // only xField/xValue
    }

    delete iter;
    double value = 0.0;

    if (!mTopLeft || !mTopRight) {
        // out of bound value uses zField for result out of XYZ area
        mZfield = yField;
        value = getOutOfBoundValue();
    } else {
        value = getLinearValue(
                mXvalue,
                mTopLeft->getValue(mXfield).toDouble(),
                mTopLeft->getValue(mYfield).toDouble(),
                mTopRight->getValue(mXfield).toDouble(),
                mTopRight->getValue(mYfield).toDouble());
    }

    return value;
}

void RB_TableMath::updateCornerObjects(RB_ObjectBase* obj) {
    if (!mExtraField.isEmpty()
            && obj->getValue(mExtraField).toDouble() != mExtraValue) {
        return;
    }

    double xValue = obj->getValue(mXfield).toDouble();
    double yValue = obj->getValue(mYfield).toDouble();

    // Top left
    if (!mTopLeft && xValue <= mXvalue && yValue >= mYvalue) {
        mTopLeft = obj;
    } else if (mTopLeft
               && xValue >= mTopLeft->getValue(mXfield).toDouble()
               && yValue <= mTopLeft->getValue(mYfield).toDouble()
               && xValue <= mXvalue
               && yValue >= mYvalue) {
        mTopLeft = obj;
    }

    // Top right
    if (!mTopRight && xValue >= mXvalue && yValue >= mYvalue) {
        mTopRight = obj;
    } else if (mTopRight
               && xValue <= mTopRight->getValue(mXfield).toDouble()
               && yValue <= mTopRight->getValue(mYfield).toDouble()
               && xValue >= mXvalue
               && yValue >= mYvalue) {
        mTopRight = obj;
    }

    // Bottom left
    if (!mBottomLeft && xValue <= mXvalue && yValue <= mYvalue) {
        mBottomLeft = obj;
    } else if (mBottomLeft
               && xValue >= mBottomLeft->getValue(mXfield).toDouble()
               && yValue >= mBottomLeft->getValue(mYfield).toDouble()
               && xValue <= mXvalue
               && yValue <= mYvalue) {
        mBottomLeft = obj;
    }

    // Bottom right
    if (!mBottomRight && xValue >= mXvalue && yValue <= mYvalue) {
        mBottomRight = obj;
    } else if (mBottomRight
               && xValue <= mBottomRight->getValue(mXfield).toDouble()
               && yValue >= mBottomRight->getValue(mYfield).toDouble()
               && xValue >= mXvalue
               && yValue <= mYvalue) {
        mBottomRight = obj;
    }
}

/**
 * @brief RB_TableMath::setOutOfBoundCornerObjects set missing corner objects
 * if position of value requested is outside the table bounderies.
 * For example: asAs a minimum only one corner object is found,
 * then all other corner objects will be set to this corner object.
 * Vertical neighbor is first choice, horizontal second and diagonal last.
 */
double RB_TableMath::getOutOfBoundValue() {
    double value = 0.0;
    int missingCornerCount = 0;

    if (!mTopLeft) {
        ++missingCornerCount;
    }
    if (!mTopRight) {
        ++missingCornerCount;
    }
    if (!mBottomLeft) {
        ++missingCornerCount;
    }
    if (!mBottomRight) {
        ++missingCornerCount;
    }

    switch (missingCornerCount) {
    case 3:
        value = getOneCornerValue();
        break;
    case 2:
        value = getTwoCornerValue();
        break;
    case 1:
        value = getThreeCornerValue();
        break;
    default:
        break;
    }

    return value;
}

double RB_TableMath::getThreeCornerValue() {
    RB_ObjectBase* tempObj = nullptr;

    if (!mTopLeft) {
        if (mBottomLeft && mTopRight) {
            tempObj = mTopLeft = mBottomLeft->clone();
            mTopLeft->setValue(mXfield, mBottomLeft->getValue(mXfield));
            mTopLeft->setValue(mYfield, mTopRight->getValue(mYfield));
            mTopLeft->setValue(mZfield,
                               (mBottomLeft->getValue(mZfield).toDouble()
                                + mTopRight->getValue(mZfield).toDouble()) / 2);
        } else {
             RB_DEBUG->error("RB_TableMath::getThreeCornerValue() 1 ERROR");
             return 0.0;
        }
    } else if (!mTopRight) {
        if (mTopLeft && mBottomRight) {
            tempObj = mTopRight = mBottomRight->clone();
            mTopRight->setValue(mXfield, mBottomRight->getValue(mXfield));
            mTopRight->setValue(mYfield, mTopLeft->getValue(mYfield));
            mTopRight->setValue(mZfield,
                               (mBottomRight->getValue(mZfield).toDouble()
                                + mTopLeft->getValue(mZfield).toDouble()) / 2);
        } else {
             RB_DEBUG->error("RB_TableMath::getThreeCornerValue() 2 ERROR");
             return 0.0;
        }
    } else if (!mBottomLeft) {
        if (mTopLeft && mBottomRight) {
            tempObj = mBottomLeft = mTopLeft->clone();
            mBottomLeft->setValue(mXfield, mTopLeft->getValue(mXfield));
            mBottomLeft->setValue(mYfield, mBottomRight->getValue(mYfield));
            mBottomLeft->setValue(mZfield,
                               (mTopLeft->getValue(mZfield).toDouble()
                                + mBottomRight->getValue(mZfield).toDouble()) / 2);
        } else {
             RB_DEBUG->error("RB_TableMath::getThreeCornerValue() 3 ERROR");
             return 0.0;
        }
    } else if (!mBottomRight) {
        if (mBottomLeft && mTopRight) {
            tempObj = mBottomRight = mTopRight->clone();
            mBottomRight->setValue(mXfield, mTopRight->getValue(mXfield));
            mBottomRight->setValue(mYfield, mBottomLeft->getValue(mYfield));
            mBottomRight->setValue(mZfield,
                               (mTopRight->getValue(mZfield).toDouble()
                                + mBottomLeft->getValue(mZfield).toDouble()) / 2);
        } else {
             RB_DEBUG->error("RB_TableMath::getThreeCornerValue() 2 ERROR");
             return 0.0;
        }
    }

    double value = getBilinearValue(
                mXvalue, mYvalue,
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


    delete tempObj;
    return value;
}

double RB_TableMath::getTwoCornerValue() {
    RB_ObjectBase* obj1 = nullptr;
    RB_ObjectBase* obj2 = nullptr;

    if (mTopLeft) {
        obj1 = mTopLeft;
    }

    if (mTopRight) {
        if (!obj1) {
            obj1 = mTopRight;
        } else {
            obj2 = mTopRight;
        }
    }

    if (mBottomLeft) {
        if (!obj1) {
            obj1 = mBottomLeft;
        } else if (!obj2) {
            obj2 = mBottomLeft;
        } else {
            RB_DEBUG->error("RB_TableMath::getTwoCornerValue() 1 ERROR");
        }
    }

    if (mBottomRight) {
        if (!obj1) {
            RB_DEBUG->error("RB_TableMath::getTwoCornerValue() 2 ERROR");
        } else if (!obj2) {
            obj2 = mBottomRight;
        } else {
            RB_DEBUG->error("RB_TableMath::getTwoCornerValue() 3 ERROR");
        }
    }

    if (!obj1 || !obj2) {
        RB_DEBUG->error("RB_TableMath::getTwoCornerValue() 4 ERROR");
        return 0.0;
    }

    double x1 = obj1->getValue(mXfield).toDouble();
    double y1 = obj1->getValue(mYfield).toDouble();
    double x2 = obj2->getValue(mXfield).toDouble();
    double y2 = obj2->getValue(mYfield).toDouble();
    double x = mXvalue;
    double y = mYvalue;
    double lengthTo1 = pow(pow(x - x1, 2) + pow(y - y1, 2), 0.5);
    double lengthTo2 = pow(pow(x - x2, 2) + pow(y - y2, 2), 0.5);
    double value = (lengthTo1 * obj2->getValue(mZfield).toDouble()
            + lengthTo2 * obj1->getValue(mZfield).toDouble())
            / (lengthTo1 + lengthTo2);
    return value;
}

double RB_TableMath::getOneCornerValue() {
    if (mTopLeft) {
        return mTopLeft->getValue(mZfield).toDouble();
    }
    if (mTopRight) {
        return mTopRight->getValue(mZfield).toDouble();
    }
    if (mBottomLeft) {
        return mBottomLeft->getValue(mZfield).toDouble();
    }
    if (mBottomRight) {
        return mBottomRight->getValue(mZfield).toDouble();
    }
    RB_DEBUG->error("RB_TableMath::getOneCornerValue() ERROR");
    return 0.0;
}

void RB_TableMath::updateLeftRightObjects(RB_ObjectBase* obj) {
    if (!mExtraField.isEmpty()
            && obj->getValue(mExtraField).toDouble() != mExtraValue) {
        return;
    }

    double xValue = obj->getValue(mXfield).toDouble();

    // Top left
    if (!mTopLeft && xValue <= mXvalue) {
        mTopLeft = obj;
    } else if (mTopLeft
               && xValue >= mTopLeft->getValue(mXfield).toDouble()
               && xValue <= mXvalue) {
        mTopLeft = obj;
    }

    // Top right
    if (!mTopRight && xValue >= mXvalue) {
        mTopRight = obj;
    } else if (mTopRight
               && xValue <= mTopRight->getValue(mXfield).toDouble()
               && xValue >= mXvalue) {
        mTopRight = obj;
    }
}

END_NAMESPACE_BILUNA_CALC
