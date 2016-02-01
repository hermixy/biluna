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
        ModelEN13555Manuf,              /**< EN13555 Manufacturer of gasket model */
        ModelEN13555Gasket,             /**< EN13555 Type of gasket */
        ModelEN1591Assembly,            /**< EN1591 Assembly model */
        ModelEN1591BoltNutWasher,       /**< EN1591 Bolt Nut Washer model */
        ModelEN1591Flange,              /**< EN1591 Flange model */
        ModelEN1591Gasket,              /**< EN1591 Gasket model */
        ModelEN1591LoadCase,            /**< EN1591 Load Case model */
        ModelEN1591Shell,               /**< EN1591 Shell model */
        ModelProject,                   /**< Project model */
        ModelProjectEdit,               /**< Edit project model */
        ModelDefault                    /**< Default- or all models */
    };

private:
    PCALC_ModelFactory(RB_MainWindow* mw);

    //! Unique factory instance
    static PCALC_ModelFactory* mActiveFactory;

};

#endif // PCALC_MODELFACTORY_H
