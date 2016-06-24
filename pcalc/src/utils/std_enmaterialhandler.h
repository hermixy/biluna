/*****************************************************************
 * $Id: std_enmaterialhandler.h 0001 2016-06-24 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_ENMATERIALHANDLER_H
#define STD_ENMATERIALHANDLER_H

#include <string>
#include <vector>
#include "rb_namespace.h"
#include "rb_tablemath.h"
#include "rb_utility.h"
#include "std.h"

// For unit tests only
class STD_EnMaterialHandlerTest;

/**
 * @brief Material handler providing EN properties for reports,
 * calculations, etc.
 */
class STD_EnMaterialHandler {

public:
    STD_EnMaterialHandler();
    virtual ~STD_EnMaterialHandler();

    double allowableDesignStress(double designTemp,
                                 STD2::CompType compType = STD2::CompDefault,
                                 int loadCaseNo = -1,
                                 const QString& variableName = "");
    double allowableTestStress(double testTemp,
                               STD2::CompType compType = STD2::CompDefault,
                               int loadCaseNo = -1,
                               const QString& variableName = "");

    // For unit tests only
    friend class STD_EnMaterialHandlerTest;

};

#endif // STD_ENMATERIALHANDLER_H
