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

/**
 * @brief EN13555 properties QminL QsminL Qsmax Pqr deltaEgc EG eG
 */
//class QminLQsminLProperty {

//public:
//    QminLQsminLProperty(double leakageRate,
//                        const RB_String& materialCode,
//                        double testPressure,
//                        double QA,
//                        double QminL,
//                        double QsminL);
//    double mLeakageRate;
//    RB_String mMaterialCode;
//    double mTestPressure;
//    double mQA;
//    double mQminL;
//    double mQsminL;
//};


#define TABLE02_15PROPERTY EN13555Property::getInstance()

/**
 * @brief EN13555 properties QminL QsminL Qsmax Pqr deltaEgc EG eG.
 * These values are from www.gasketdata.org.
 * Refer also www.europeansealing.com
 */
class EN13555Property : public RB_TableMath, RB_Utility {

public:
    virtual ~EN13555Property();
    static EN13555Property* getInstance();

    void refresh() {}


    double getTableQA(double leakageRate,
                         const RB_String& gasketId,
                         double testPressure = 40.0);
    double getTableQminL(double leakageRate,
                         const RB_String& gasketId,
                         double testPressure = 40.0);
    double getTableQsminL(double leakageRate,
                          const RB_String& materialCode,
                          double QA,
                          double testPressure = 40.0);




    // continue here ... how to allow for custom and tables 2-16 selection?
    RB_String gasketId(const RB_String& manufacturer,
                       const RB_String& gasketType);


private:
    EN13555Property();

//    void createList();
//    void cl(double leakageRate, const RB_String& materialCode,
//            double testPressure, double QA, double QminL, double QsminL);

//    void updateLeft(QminLQsminLProperty* obj);
//    void updateRight(QminLQsminLProperty* obj);

    static EN13555Property* mActiveUtility;

    RB_ObjectContainer* mList;

//    std::vector<QminLQsminLProperty*> mList;
//    double mTargetQA;
//    QminLQsminLProperty* mLeft;
//    QminLQsminLProperty* mRight;
};

END_NAMESPACE_BILUNA_CALC_EN1591
#endif // EN13555PROPERTY_H
