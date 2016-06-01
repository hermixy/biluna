/*****************************************************************
 * $Id: pcalc_dialogfactory.h 2165 2014-09-24 19:22:36Z rutger $
 * Created: Apr 29, 2015 11:34:42 AM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PCALC_DIALOGFACTORY_H
#define PCALC_DIALOGFACTORY_H

#include <QObject>
#include "rb_dialogfactory.h"
#include "rb_modelfactory.h"

class RB_ModelFactory;

#define PCALC_DIALOGFACTORY PCALC_DialogFactory::getInstance()

/**
 * Dialog and widget factory for PCALC project
 */
class PCALC_DialogFactory : public RB_DialogFactory {

    Q_OBJECT

public:
    virtual ~PCALC_DialogFactory();
    static PCALC_DialogFactory* getInstance(RB_MainWindow* mw = 0);

    virtual RB_ModelFactory* getModelFactory();

    virtual RB_Widget* getWidget(int type, QWidget* parent);
    virtual RB_Dialog* getDialog(int type);
    virtual void updateUiConnections();
    virtual void commandMessage(const RB_String& message);

    /**
     * DO NOT USE ANYMORE, USE RB_DialogWindow!!!
     *
     * PCALC Dialog types, example = 193XXX:
     * - 19 = PCALC,
     * - 3 = dialog,
     * - XXX = sequence number
     * 193100 is starting number leaving 193000 to 193099 for test/debug cases
     * Refer also WidgetType and DialogType
     */
    enum DialogType {
        // start number from 193000
//        DialogTest = 193000,                   /**< Test Dialog */
//        DialogBaseTest,                        /**< Test of base dialogs */
        // start numbering from 193100
        DialogNone = 193100,            /**< Invalid dialog */
        DialogProject,                  /**< Select project from database dialog */
        DialogDefault                   /**< Default- or all dialogs */
    };

    /**
     * DB Widget/dialog manager types, example = 192XXX:
     * - 19 = PCALC,
     * - 2 = widget,
     * - XXX = sequence number
     * 192100 is starting number leaving 192000 to 192099 for test/debug cases
     * Refer also ModelType and DialogType
     */
    enum WidgetType {
        // start numbering from 192000
//        WidgetTestDialog = 192000,      /**< Dialog widget */
//        WidgetTestTable,                /**< Table widget */
//        WidgetTestTableChild,           /**< Child table widget with table parent */
//        WidgetTestText,                 /**< Text widget */
//        WidgetTestTree,                 /**< Tree widget */
//        WidgetTestTreeChild,            /**< Child table widget with tree parent */
        // start numbering from 192100
        WidgetNone = 192100,            /**< Invalid model */
        WidgetEN13555Gasket,            /**< EN13555 gasket properties */
        WidgetEN13555Manuf,             /**< EN13555 gasket manufacturer edit and selection */
        WidgetEN13555Type,              /**< EN13555 gasket type edit and selection */
        WidgetEN1591Flange,             /**< EN1591 flange calculation */
        WidgetEN1591SelectGasket,       /**< EN1591 select gasket dialog */
        WidgetMatCreep,                 /**< Material creep values Dialog */
        WidgetMatElasModul,             /**< Material elasticity modulus Dialog */
        WidgetMaterial,                 /**< Material Dialog */
        WidgetMatThermExp,              /**< Material thermal expansion Dialog */
        WidgetProjectEdit,              /**< Edit project dialog */
        WidgetDefault                   /**< Default- or all widgets */
    };

private:
    //! Private constructor
    PCALC_DialogFactory(RB_MainWindow* mw);
    //! Unique (static) instance of factory
    static PCALC_DialogFactory* mActiveFactory;

};

#endif // PCALC_DIALOGFACTORY_H
