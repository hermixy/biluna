/*****************************************************************
 * $Id: rb_tablemath.h 2227 2015-03-09 20:08:03Z rutger $
 * Created: Mar 4, 2015 8:50:42 AM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_TABLEMATH_H
#define RB_TABLEMATH_H

#include "rb_objectcontainer.h"
#include "rb_namespace.h"
#include "rb_tablemathtest.h"

NAMESPACE_BILUNA_CALC

/**
 * Base class for table math such as interpolation. To be inherited
 * by calculation or utility.
 */
class RB_TableMath {

friend class RB_TableMathTest;
public:
    RB_TableMath();
    virtual ~RB_TableMath();

    double getBilinearValue(double valX, double valY,
                            double p11X, double p11Y, double p11val,
                            double p12X, double p12Y, double p12val,
                            double p21X, double p21Y, double p21val,
                            double p22X, double p22Y, double p22val);
    double getInterpolatedValue(RB_ObjectContainer* fromObjC,
                                const QString& xField, const QString& yField,
                                const QString& zField, double xValue,
                                double yValue, const QString& extraField = "",
                                double extraValue = 0.0);

private:
    void updateCornerObjects(RB_ObjectBase* obj);

    QString mXfield;
    QString mYfield;
    QString mZfield;
    double mXvalue;
    double mYvalue;
    QString mExtraField;
    double mExtraValue;
    RB_ObjectBase* mTopLeft;
    RB_ObjectBase* mTopRight;
    RB_ObjectBase* mBottomLeft;
    RB_ObjectBase* mBottomRight;

};

END_NAMESPACE_BILUNA_CALC

#endif //RB_TABLEMATH_H
