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
NAMESPACE_REDBAG_CALC



RB_TableMath::RB_TableMath(RB_ObjectContainer* inputOutput)
            : RB_Report(inputOutput) {
    // nothing
}

RB_TableMath::~RB_TableMath() {
    // nothing
}

/**
 * @brief Biliniair interpolation, four points
 * with a value in a orthogonal square as per
 * http://en.wikipedia.org/wiki/Bilinear_interpolation
 * @param valX x-coordinate of requested value
 * @param valY y-coordinate of requested value
 * @param p11X top left
 * @param p11Y
 * @param p11val
 * @param p12X (p12Y not used, same as p11Y) top right
 * @param p12val
 * @param p21X bottom left
 * @param p21Y
 * @param p21val
 * @param p22X (p22Y not used, same as p21Y) bottom right
 * @param p22val
 * @return
 */
double RB_TableMath::getBilinearValue(
        double valX, double valY,
        double p11X, double p11Y, double p11val,
        double p12X, double /*p12Y*/, double p12val,
        double p21X, double p21Y, double p21val,
        double p22X, double /*p22Y*/, double p22val) {
    // data points to be in a square, orthogonal
    // first calculate value on both x-axis and then y-axis
    double val1 = (p12X - valX) / (p12X - p11X) * p11val
            + (valX - p11X) / (p12X - p11X) * p12val;
    double val2 = (p22X - valX) / (p22X - p21X) * p21val
            + (valX - p21X) / (p22X - p21X) * p22val;
    double value = (p21Y - valY) / (p21Y - p11Y) * val1
            + (valY - p11Y) / (p21Y - p11Y) * val2;
    addDetail("RB_TableMath", "getBilinearValue()", "", value, "-");
    return value;

}
END_NAMESPACE_REDBAG_CALC
