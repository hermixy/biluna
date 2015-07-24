/*****************************************************************
 * $Id: sail_modelfactory.h 2166 2014-09-25 13:23:18Z rutger $
 * Created: Oct 24, 2014 21:17:25 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna MRP project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef MRP_MODELFACTORY_H
#define MRP_MODELFACTORY_H

#include "rb_modelfactory.h"

#define MRP_MODELFACTORY MRP_ModelFactory::getInstance()

/**
 * Model factory for the manufacturing resource planning MRP plugin
 */
class MRP_ModelFactory : public RB_ModelFactory {

    Q_OBJECT

public:
    virtual ~MRP_ModelFactory();
    static MRP_ModelFactory* getInstance(RB_MainWindow* mw = 0);

    virtual RB_MmProxy* getModel(int type, bool shared = true);
    virtual RB_MmProxy* getParentModel(int type);

    /**
     * Model manager types, example = 301XXX:
     * - 30 = MRP,
     * - 1 = model,
     * - XXX = sequence number
     * 301100 is starting number leaving 301000 to 301099 for test/debug cases
     * Refer also WidgetType and DialogType
     */
    enum ModelType {
        // start numbering from 301000
        //        ModelTest = 301000,             /**< Table model */
        //        ModelTestChild,                 /**< Child table model */
        //        ModelTestDialog,                /**< Dialog model */
        //        ModelTestTree,                  /**< Tree model */
        //        ModelTestTreeChild,             /**< Child Tree model */

        // start numbering from 301100
        ModelNone = 301100,             /**< Invalid model */
        ModelContact,                   /**< ACC contact model */
        ModelCustomer,                  /**< ACC customer model */
        ModelProject,                   /**< ACC project model */
        ModelSalesOrder,                /**< ACC sales order model */
        ModelSoftwareLicense,           /**< Software license model */
        ModelDefault                    /**< Default- or all models */
    };

private:
    MRP_ModelFactory(RB_MainWindow* mw);

    //! Unique factory instance
    static MRP_ModelFactory* mActiveFactory;

};

#endif // MRP_MODELFACTORY_H
