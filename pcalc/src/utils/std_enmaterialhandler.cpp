/*****************************************************************
 * $Id: std_enmaterialhandler.cpp 0001 2016-06-24 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_enmaterialhandler.h"
#include "pcalc.h"
#include "pcalc_report.h"
#include "std_materialutility.h"

STD_EnMaterialHandler::STD_EnMaterialHandler() {
    // nothing
}

STD_EnMaterialHandler::~STD_EnMaterialHandler() {
    // nothing
}

double STD_EnMaterialHandler::allowableDesignStress(
                    double designTemp, STD2::CompType compType,
                    int loadCaseNo, const QString& variableName) {
    STD2::MatStruct matStruct = STD_MATERIALUTILITY->getMaterialStructure();
    double elongPercent = STD_MATERIALUTILITY->getElongationPercent();
    bool alternativeRoute = false; // clause 6.3
    double allowStress = 0.0;

    double Rp02 = 0.0;
    double Rp10 = 0.0;
    double RmMin = 0.0;

    if (matStruct != STD2::MatStructAustenitic
            && matStruct != STD2::MatStructCasting
            && elongPercent < 30.0
            && !alternativeRoute) {
        Rp02 = STD_MATERIALUTILITY->get_Rp02(designTemp);
        RmMin = STD_MATERIALUTILITY->get_RmMin(20); // 20 Celsius

        if (compType == STD2::CompBolt) {
            // EN 13445 clause 11.4.3
            allowStress = std::min(Rp02 / 3.0, RmMin / 4.0);
        } else {
            allowStress = std::min(Rp02 / 1.5, RmMin / 2.4);
        }
    } else if (matStruct != STD2::MatStructAustenitic
               && matStruct != STD2::MatStructCasting
               && elongPercent < 30.0
               && alternativeRoute) {
        Rp02 = STD_MATERIALUTILITY->get_Rp02(designTemp);
        RmMin = STD_MATERIALUTILITY->get_RmMin(20); // 20 Celsius

        if (compType == STD2::CompBolt) {
            // EN 13445 clause 11.4.3
            allowStress = std::min(Rp02 / 3.0, RmMin / 4.0);
        } else {
            allowStress = std::min(Rp02 / 1.5, RmMin / 1.875);
        }
    } else if (matStruct == STD2::MatStructAustenitic
               && 30.0 <= elongPercent
               && elongPercent < 35.0) {
        if (compType == STD2::CompBolt) {
            // EN 13445 clause 11.4.3
            RmMin = STD_MATERIALUTILITY->get_RmMin(designTemp);
            allowStress = RmMin / 4.0;
        } else {
            Rp10 = STD_MATERIALUTILITY->get_Rp10(designTemp);
            allowStress = Rp10 / 1.5;
        }
    } else if (matStruct == STD2::MatStructAustenitic
               && 35.0 <= elongPercent) {
        RmMin = STD_MATERIALUTILITY->get_RmMin(designTemp);

        if (compType == STD2::CompBolt) {
            // EN 13445 clause 11.4.3
            allowStress = RmMin / 4.0;
        } else if (compType == STD2::CompFlange) {
            // // EN 13445 clause 11.5.4.2
            Rp10 = STD_MATERIALUTILITY->get_Rp10(designTemp);
            allowStress = Rp10 / 1.5;
        } else {
            Rp10 = STD_MATERIALUTILITY->get_Rp10(designTemp);
            allowStress = std::max(Rp10 / 1.5,
                                   std::min(Rp10 / 1.2, RmMin / 3.0));
        }
    } else if (matStruct == STD2::MatStructCasting) {
        Rp02 = STD_MATERIALUTILITY->get_Rp02(designTemp);
        RmMin = STD_MATERIALUTILITY->get_RmMin(20); // 20 Celsius
        allowStress = std::min(Rp02 / 1.9, RmMin / 3.0);
    }

    return allowStress;
}

double STD_EnMaterialHandler::allowableTestStress(
                    double testTemp, STD2::CompType compType,
                    int loadCaseNo, const QString& variableName) {
    STD2::MatStruct matStruct = STD_MATERIALUTILITY->getMaterialStructure();
    double elongPercent = STD_MATERIALUTILITY->getElongationPercent();
    bool alternativeRoute = false; // clause 6.3
    double allowStress = 0.0;

    double Rp02 = 0.0;
    double Rp10 = 0.0;
    double RmMin = 0.0;

    if (matStruct != STD2::MatStructAustenitic
            && matStruct != STD2::MatStructCasting
            && elongPercent < 30.0
            && !alternativeRoute) {
        Rp02 = STD_MATERIALUTILITY->get_Rp02(testTemp);
        RmMin = STD_MATERIALUTILITY->get_RmMin(testTemp); // 20 Celsius

        if (compType == STD2::CompBolt) {
            // EN 13445 clause 11.4.3
            allowStress = std::min(Rp02 / 2.0, RmMin / 2.67);
        } else {
            allowStress = Rp02 / 1.05;
        }
    } else if (matStruct != STD2::MatStructAustenitic
               && matStruct != STD2::MatStructCasting
               && elongPercent < 30.0
               && alternativeRoute) {
        Rp02 = STD_MATERIALUTILITY->get_Rp02(testTemp);
        RmMin = STD_MATERIALUTILITY->get_RmMin(testTemp); // 20 Celsius

        if (compType == STD2::CompBolt) {
            // EN 13445 clause 11.4.3
            allowStress = std::min(Rp02 / 2.0, RmMin / 2.67);
        } else {
            allowStress = Rp02 / 1.05;
        }
    } else if (matStruct == STD2::MatStructAustenitic
               && 30.0 <= elongPercent
               && elongPercent < 35.0) {
        if (compType == STD2::CompBolt) {
            // EN 13445 clause 11.4.3
            RmMin = STD_MATERIALUTILITY->get_RmMin(testTemp);
            allowStress = RmMin / 2.67;
        } else {
            Rp10 = STD_MATERIALUTILITY->get_Rp10(testTemp);
            allowStress = Rp10 / 1.05;
        }
    } else if (matStruct == STD2::MatStructAustenitic
               && 35.0 <= elongPercent) {
        RmMin = STD_MATERIALUTILITY->get_RmMin(testTemp);

        if (compType == STD2::CompBolt) {
            // EN 13445 clause 11.4.3
            allowStress = RmMin / 2.67;
        } else {
            Rp10 = STD_MATERIALUTILITY->get_Rp10(testTemp);
            allowStress = std::max(Rp10 / 1.05, RmMin / 2.0);
        }
    } else if (matStruct = STD2::MatStructCasting) {
        Rp02 = STD_MATERIALUTILITY->get_Rp02(testTemp);
        allowStress = Rp02 / 1.33;
    }

    return allowStress;
}
