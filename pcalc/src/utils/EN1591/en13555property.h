/*****************************************************************
 * $Id: pcalc_en13555property.h 0001 2016-02-20T16:56:54 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef EN13555PROPERTY_H
#define EN13555PROPERTY_H

#include <string>
#include <vector>
#include "gasket.h"
#include "rb_namespace.h"
#include "rb_tablemath.h"
#include "rb_utility.h"
NAMESPACE_BILUNA_CALC_EN1591

#define EN13555PROPERTY EN13555Property::getInstance()

/**
 * @brief EN13555 properties QminL QsminL Qsmax Pqr deltaEgc EG eG.
 * These values are from www.gasketdata.org.
 * Refer also www.europeansealing.com
 */
class EN13555Property : public RB_TableMath, RB_Utility {

public:
    virtual ~EN13555Property();
    static EN13555Property* getInstance();

    bool setCurrentGasket(const QString& gasketIdx);
    bool isValid();
    void refresh() {}

    double get_deltaeGc(double gasketPressure, double designTemp);
    double get_PQR(double gasketPressure, double designTemp);
    double get_eG(double gasketPressure, double designTemp);
    double get_EG(double gasketPressure, double designTemp);
    double get_Qsmax(double designTemp);
    double get_QsminL(double leakageRate, double QA, double testPressure);
    double get_QminL(double leakageRate, double testPressure);

private:
    EN13555Property();

    bool loadGasket(const QString& gasketId);
    double closestInnerPressureBar(double testPressure);
    double getMaxLinInterpValue(RB_ObjectContainer* fromObjC,
                                const QString& xField, const QString& yField,
                                double xValue, const QString& extraField = "",
                                double extraValue = 0.0);
    void updateMaxLeftRightObjects(RB_ObjectBase* obj);


    static EN13555Property* mActiveUtility;
    RB_ObjectContainer* mGasketList;
    RB_ObjectBase* mCurrentGasket;

};

END_NAMESPACE_BILUNA_CALC_EN1591
#endif // EN13555PROPERTY_H
