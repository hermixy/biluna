/*****************************************************************
 * $Id: scan_modelfactory.h 2166 2014-09-25 13:23:18Z rutger $
 * Created: 2011-04-17 rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SCAN_MODELFACTORY_H
#define SCAN_MODELFACTORY_H

#include "rb_modelfactory.h"

#define SCAN_MODELFACTORY SCAN_ModelFactory::getInstance()

/**
 * Plant Engineering PENG model factory
 */
class SCAN_ModelFactory : public RB_ModelFactory {

    Q_OBJECT

public:
    virtual ~SCAN_ModelFactory();
    static SCAN_ModelFactory* getInstance(RB_MainWindow* mw = 0);

    virtual RB_MmProxy* getModel(int type, bool shared = true);
    virtual RB_MmProxy* getParentModel(int type);

    /**
     * Model manager types, DB = 101XXX
     * example = 101XXX:
     * - 10 = DB, (26 = SCAN)
     * - 1 = model,
     * - XXX = sequence number
     * 101100 is starting number leaving 101000 to 101099 for test/debug cases
     * Refer also WidgetType and DialogType
     */
    enum ModelType {
        // start numbering from 261100
        ModelNone = 261100,             /**< Invalid model */
        ModelAnswer,                    /**< Possible or suggested answers of questions model */
        ModelAnswerResult,              /**< Given or selected answers by user model */
        ModelBlock,                     /**< Block texts of report model */
        ModelProject,                   /**< Project model */
        ModelProjectEdit,               /**< Edit project model */
        ModelQuestion,                  /**< Questions in a scan model */
        ModelQuestionResult,            /**< Question results model */
//        ModelReport,                    /**< Report of result format model */
        ModelScan,                      /**< Scan, questionair or interview setup model */
        ModelScanResult,                /**< Scan result model */
        ModelDefault                    /**< Default- or all models */
    };

private:
    SCAN_ModelFactory(RB_MainWindow* mw);

    //! Unique factory instance
    static SCAN_ModelFactory* mActiveFactory;

};

#endif // SCAN_MODELFACTORY_H
