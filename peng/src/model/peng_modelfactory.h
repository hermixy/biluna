/*****************************************************************
 * $Id: peng_modelfactory.h 2233 2015-04-29 19:10:59Z rutger $
 * Created: Apr 15, 2010 11:29 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_MODELFACTORY_H
#define PENG_MODELFACTORY_H

#include "rb_modelfactory.h"

#define PENG_MODELFACTORY PENG_ModelFactory::getInstance()

/**
 * Plant Engineering PENG model factory
 */
class PENG_ModelFactory : public RB_ModelFactory {

    Q_OBJECT

public:
    virtual ~PENG_ModelFactory();
    static PENG_ModelFactory* getInstance(RB_MainWindow* mw = 0);

    virtual RB_MmProxy* getModel(int type, bool shared = true);
    virtual RB_MmProxy* getParentModel(int type);
    virtual bool addDefaultRows();

    /**
     * Model manager types, PENG = 251XXX
     * example = 111XXX:
     * - 10 = DB,
     * - 1 = model,
     * - XXX = sequence number
     * 101100 is starting number leaving 101000 to 101099 for test/debug cases
     * Refer also WidgetType and DialogType
     */
    enum ModelType {
        // start numbering from 251100
        ModelNone = 251100,             /**< Invalid model */
        ModelEngCone,                   /**< Cone model for engineering */
        ModelEngWeldolet,               /**< Weldolet model for engineering */
        ModelEquipment,                 /**< Equipment model */
        ModelEquipmentType,             /**< Equipment type model */
        ModelFluid,                     /**< Fluid code model */
        ModelFluidStability,            /**< Fluid stability state model */
        ModelHazardGroup,               /**< Hazard group model */
        ModelInsulationCC,              /**< Insulation cold conservation model */
        ModelInsulationHCI,             /**< Insulation heat conservation indoor model */
        ModelInsulationHCO,             /**< Insulation heat conservation outdoor model */
        ModelInsulationPP,              /**< Insulation personnel protection model */
        ModelLine,                      /**< Pipe line model */
        ModelLineClassif,               /**< Pipe line classification submodel */
        ModelLineCustom,                /**< Pipe line custom data submodel */
        ModelLineEnginDes,              /**< Pipe line engineering and design submodel */
        ModelLineProcess,               /**< Pipe line process data submodel */
        ModelPaint,                     /**< Paint specification data model */
        ModelPedEqClassif,              /**< PED classification data model */
        ModelPedLineClassif,            /**< PED classification data model */
        ModelPipeClass,                 /**< Pipe class model */
        ModelPipeClassPT,               /**< Pipe class Pressure Temperature model */
        ModelPipeClassSize,             /**< Pipe class pipe size model */
        ModelProject,                   /**< Project model */
        ModelProjectEdit,               /**< Edit project model */
        ModelSetting,                   /**< Edit settings model */
        ModelDefault                    /**< Default- or all models */
    };

private:
    PENG_ModelFactory(RB_MainWindow* mw);

    //! Unique factory instance
    static PENG_ModelFactory* mActiveFactory;

};

#endif // PENG_MODELFACTORY_H
