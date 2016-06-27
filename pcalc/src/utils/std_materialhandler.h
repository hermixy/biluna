/*****************************************************************
 * $Id: std_materialutility.h 0001 2016-06-25 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_MATERIALHANDLER_H
#define STD_MATERIALHANDLER_H

#include <QString>
#include "std.h"

/**
 * @brief Abstract class for material handler implementation. The derived
 * classes are the ASME and EN material handlers. The user of these classes
 * is the STD_MaterialUtility providing properties for reports,
 * calculations, etc.
 */
class STD_MaterialHandler {

public:
    STD_MaterialHandler() { }
    virtual ~STD_MaterialHandler() { }

    virtual bool setCurrentMaterial(const QString& materialIdx) = 0;
    virtual bool isValid() = 0;
    virtual void refresh() = 0;

    virtual double allowableDesignStress(double designTemp,
                                 STD2::CompType compType = STD2::CompDefault,
                                 int loadCaseNo = -1,
                                 const QString& variableName = "") = 0;
    virtual double allowableTestStress(double testTemp,
                               STD2::CompType compType = STD2::CompDefault,
                               int loadCaseNo = -1,
                               const QString& variableName = "") = 0;
    virtual double elasticityModulus(double designTemp,
                             int loadCaseNo = -1,
                             const QString& variableName = "") = 0;
    virtual double thermalExpansion(double designTemp,
                            int loadCaseNo = -1,
                            const QString& variableName = "") = 0;

    virtual STD2::MatStruct getMaterialStructure() = 0;
    virtual double getElongationPercent() = 0;

};

#endif // STD_MATERIALHANDLER_H
