/*****************************************************************
 * $Id: sail_modelfactory.h 2166 2014-09-25 13:23:18Z rutger $
 * Created: Jul 26, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SAIL_MODELFACTORY_H
#define SAIL_MODELFACTORY_H

#include "rb_modelfactory.h"

#define SAIL_MODELFACTORY SAIL_ModelFactory::getInstance()

/**
 * Model factory for the sailing plugin
 */
class SAIL_ModelFactory : public RB_ModelFactory {

    Q_OBJECT

public:
    virtual ~SAIL_ModelFactory();
    static SAIL_ModelFactory* getInstance(RB_MainWindow* mw = 0);

    virtual RB_MmProxy* getModel(int type, bool shared = true);
    virtual RB_MmProxy* getParentModel(int type);

    /**
     * Model manager types, example = 281XXX:
     * - 28 = SAIL,
     * - 1 = model,
     * - XXX = sequence number
     * 281100 is starting number leaving 281000 to 281099 for test/debug cases
     * Refer also WidgetType and DialogType
     */
    enum ModelType {
        // start numbering from 281000
        //        ModelTest = 281000,             /**< Table model */
        //        ModelTestChild,                 /**< Child table model */
        //        ModelTestDialog,                /**< Dialog model */
        //        ModelTestTree,                  /**< Tree model */
        //        ModelTestTreeChild,             /**< Child Tree model */

        // start numbering from 281100
        ModelNone = 281100,             /**< Invalid model */
        ModelCoordinate,                /**< Coordinate model */
        ModelMap,                       /**< Land and sea maps */
        ModelProject,                   /**< Project model */
        ModelProjectEdit,               /**< Edit project model */
        ModelRoute,                     /**< Route, a series of coordinates model */
        ModelRouteCoordinate,           /**< Route coordinate model */
        ModelSymbol,                    /**< Symbols such buoy and beacons */
        ModelTrack,                     /**< Track, two coordinates model */
        ModelDefault                    /**< Default- or all models */
    };

private:
    SAIL_ModelFactory(RB_MainWindow* mw);

    //! Unique factory instance
    static SAIL_ModelFactory* mActiveFactory;

};

#endif // SAIL_MODELFACTORY_H
