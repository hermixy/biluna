/*****************************************************************
 * $Id: gv_modelfactory.h 2199 2014-12-08 21:41:10Z rutger $
 * Created: Apr 17, 2010 11:29 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna GV project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_MODELFACTORY_H
#define GV_MODELFACTORY_H

#include "rb_database.h"
#include "rb_modelfactory.h"
#include "gv_drawing.h"

#define GV_MODELFACTORY GV_ModelFactory::getInstance()

/**
 * Test model factory
 */
class GV_ModelFactory : public RB_ModelFactory {

    Q_OBJECT

public:
    virtual ~GV_ModelFactory();
    static GV_ModelFactory* getInstance(RB_MainWindow* mw = 0);

    virtual RB_MmProxy* getModel(int type, bool shared = true);
    virtual RB_MmProxy* getParentModel(int type);

    /**
     * Model manager types, GV = 24
     * example = 101XXX:
     * - 10 = DB,
     * - 1 = model,
     * - XXX = sequence number
     * 101100 is starting number leaving 101000 to 101099 for test/debug cases
     * Refer also WidgetType and DialogType
     */
    enum ModelType {
        // start numbering from 241100
        ModelNone = 241100,             /**< Invalid model */
        ModelProject,                   /**< Project model */
        ModelDrawing,                   /**< Drawing model */
        ModelDefault                    /**< Default- or all models */
    };

private:
    GV_ModelFactory(RB_MainWindow* mw);

    //! Unique factory instance
    static GV_ModelFactory* mActiveFactory;
};

#endif // GV_MODELFACTORY_H
