/*****************************************************************
 * $Id: pcalc_modelfactory.h 2232 2015-04-23 16:04:08Z rutger $
 * Created: Apr 15, 2015 11:29 AM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PCALC_MODELFACTORY_H
#define PCALC_MODELFACTORY_H

#include "rb_modelfactory.h"

#define PCALC_MODELFACTORY PCALC_ModelFactory::getInstance()

/**
 * Plant Engineering PENG model factory
 */
class PCALC_ModelFactory : public RB_ModelFactory {

    Q_OBJECT

public:
    virtual ~PCALC_ModelFactory();
    static PCALC_ModelFactory* getInstance(RB_MainWindow* mw = 0);

    virtual RB_MmProxy* getModel(int type, bool shared = true);
    virtual RB_MmProxy* getParentModel(int type);
    virtual bool addDefaultRows();

    /**
     * Model manager types, PCALC = 191XXX
     * example = 111XXX:
     * - 19 = PCALC,
     * - 1 = model,
     * - XXX = sequence number
     * 101100 is starting number leaving 101000 to 101099 for test/debug cases
     * Refer also WidgetType and DialogType
     */
    enum ModelType {
        // start numbering from 191100
        ModelNone = 191100,             /**< Invalid model */
        ModelClassGenerator,            /**< Calculation class generator model */
        ModelDimension,                 /**< Dimension model */
        ModelEN13555EGeG,               /**< EN13555 EG and eG model */
        ModelEN13555Gasket,             /**< EN13555 Type of gasket */
        ModelEN13555Manuf,              /**< EN13555 Manufacturer of gasket model */
        ModelEN13555PQRdeltaeGC,        /**< EN13555 PQR and deltaeGC model */
        ModelEN13555QminL,              /**< EN13555 QminL model */
        ModelEN13555QsmaxAlphaG,        /**< EN13555 Qsmax alphaG model */
        ModelEN13555QsminL,             /**< EN13555 QsminL model */
        ModelEN1591Assembly,            /**< EN1591 Assembly model */
        ModelEN1591BoltNutWasher,       /**< EN1591 Bolt Nut Washer model */
        ModelEN1591Flange,              /**< EN1591 Flange model */
        ModelEN1591Gasket,              /**< EN1591 Gasket model */
        ModelEN1591LoadCase,            /**< EN1591 Load Case model */
        ModelEN1591Shell,               /**< EN1591 Shell model */
        ModelFlange,                    /**< EN flange model */
        ModelFlangeAsme,                /**< ASME flange model */
        ModelFlangeFacingDim,           /**< EN flange facing dimension model */
        ModelFlangeFacingDimAsme,       /**< ASME flange facing dimension model */
        ModelFlangeFacingType,          /**< Flange facing type model */
        ModelFlangeType,                /**< Flange type model */
        ModelFlangeTypeLimit,           /**< Flange type limit model */
        ModelGeneratorFormula,          /**< Calculation generator formula model */
        ModelMatCreepTable,             /**< Material creep table */
        ModelMatElasModulTable,         /**< Material elasticity modulus table */
        ModelMatElasModulValue,         /**< Material elasticity modulus values */
        ModelMaterial,                  /**< Material model */
        ModelMatThermExpTable,          /**< Material thermal expansion table */
        ModelMatThermExpValue,          /**< Material thermal expansion values */
        ModelProject,                   /**< Project model */
        ModelProjectEdit,               /**< Edit project model */
        ModelRating,                    /**< Base rating/schedule model */
        ModelRmMin,                     /**< Minimum tensile strength model */
        ModelRp02,                      /**< 0.2% yield or proof strength model */
        ModelRp10,                      /**< 1% yield or proof strength model */
        ModelRp10T100kh,                /**< 1% strain creep strength at 100 000 hrs model */
        ModelRp10T10kh,                 /**< 1% strain creep strength at 10 000 hrs model */
        ModelRpT100kh,                  /**< creep rupture strength at 100 000 hrs model */
        ModelRpT10kh,                   /**< creep rupture strength at 10 000 hrs model */
        ModelRpT200kh,                  /**< creep rupture strength at 200 000 hrs model */
        ModelDefault                    /**< Default- or all models */
    };

private:
    PCALC_ModelFactory(RB_MainWindow* mw);

    //! Unique factory instance
    static PCALC_ModelFactory* mActiveFactory;

};

#endif // PCALC_MODELFACTORY_H
