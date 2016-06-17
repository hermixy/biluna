/*****************************************************************
 * $Id: std_materialutility.h 0001 2016-06-17 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_MATERIALUTILITY_H
#define STD_MATERIALUTILITY_H

#include <string>
#include <vector>
#include "rb_namespace.h"
#include "rb_tablemath.h"
#include "rb_utility.h"

#define STD_MATERIALUTILITY STD_MaterialUtility::getInstance()

/**
 * @brief Standard material utility providing properties for reports,
 * calculations, etc.
 */
class STD_MaterialUtility : public Biluna::Calc::RB_TableMath, RB_Utility {

public:
    virtual ~STD_MaterialUtility();
    static STD_MaterialUtility* getInstance();

    bool setCurrentMaterial(const QString& materialIdx);
    bool isValid();
    void refresh() {}

    double get_Rp02(double designTemp);


private:
    STD_MaterialUtility();

    bool loadMaterial(const QString& materialId);

    static STD_MaterialUtility* mActiveUtility;
    RB_ObjectContainer* mMaterialList;
    RB_ObjectBase* mCurrentMaterial;

};

#endif // STD_MATERIALUTILITY_H
