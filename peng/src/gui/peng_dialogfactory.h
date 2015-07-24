/*****************************************************************
 * $Id: peng_dialogfactory.h 2165 2014-09-24 19:22:36Z rutger $
 * Created: Dec 20, 2009 11:34:42 AM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_DIALOGFACTORY_H
#define PENG_DIALOGFACTORY_H

#include <QObject>
#include "rb_dialogfactory.h"
#include "rb_modelfactory.h"

class RB_ModelFactory;

#define PENG_DIALOGFACTORY PENG_DialogFactory::getInstance()

/**
 * Dialog and widget factory for PENG project
 */
class PENG_DialogFactory : public RB_DialogFactory {

    Q_OBJECT

public:
    virtual ~PENG_DialogFactory();
    static PENG_DialogFactory* getInstance(RB_MainWindow* mw = 0);

    virtual RB_ModelFactory* getModelFactory();

    virtual RB_Widget* getWidget(int type, QWidget* parent);
    virtual RB_Dialog* getDialog(int type);
    virtual void updateUiConnections();
    virtual void commandMessage(const RB_String& message);

    /**
     * PENG Dialog types, example = 253XXX:
     * - 25 = PENG,
     * - 3 = dialog,
     * - XXX = sequence number
     * 253100 is starting number leaving 253000 to 253099 for test/debug cases
     * Refer also WidgetType and DialogType
     */
    enum DialogType {
        // start number from 253000
//        DialogTest = 253000,                   /**< Test Dialog */
//        DialogBaseTest,                        /**< Test of base dialogs */
        // start numbering from 253100
        DialogNone = 253100,            /**< Invalid dialog */
        DialogEqTypeEdit,               /**< Edit equipment type dialog */
        DialogEqTypeSelect,             /**< Select equipment type dialog */
        DialogFluid,                    /**< Edit and/or select fluid dialog */
        DialogFluidStability,           /**< Edit and/or select fluid stability dialog */
        DialogHazardGroup,              /**< Edit hazard group dialog */
        DialogInsulation,               /**< Edit insulation table dialog */
        DialogPaint,                    /**< Edit and/or select paint systems dialog */
        DialogPipeClass,                /**< Edit and/or select pipe class dialog */
        DialogProject,                  /**< Select project (company) from database dialog */
        DialogProjectEdit,              /**< Edit project (company) dialog */
        DialogSettings,                 /**< Edit main settings dialog */
        DialogDefault                   /**< Default- or all dialogs */
    };

    /**
     * DB Widget/dialog manager types, example = 252XXX:
     * - 25 = PENG,
     * - 2 = widget,
     * - XXX = sequence number
     * 252100 is starting number leaving 252000 to 252099 for test/debug cases
     * Refer also ModelType and DialogType
     */
    enum WidgetType {
        // start numbering from 252000
//        WidgetTestDialog = 252000,      /**< Dialog widget */
//        WidgetTestTable,                /**< Table widget */
//        WidgetTestTableChild,           /**< Child table widget with table parent */
//        WidgetTestText,                 /**< Text widget */
//        WidgetTestTree,                 /**< Tree widget */
//        WidgetTestTreeChild,            /**< Child table widget with tree parent */
        // start numbering from 252100
        WidgetNone = 252100,            /**< Invalid model */
        WidgetConeStrikeOut,            /**< Design cone strikeout widget */
        WidgetEquipmentDetail,          /**< TODO: Equipment detail widget */
        WidgetEquipmentMaster,          /**< Tab equipment of navigation widget */
        WidgetLineDetail,               /**< Line detail widget */
        WidgetLineMaster,               /**< Tab line of navigation widget */
        WidgetNavigation,               /**< Navigation widget for P&ID, Equipment etc. */
        WidgetPedClassification,        /**< PED classification widget */
        WidgetPedEqSummary,             /**< PED equipment summary widget */
        WidgetWeldoletWeldVolume,       /**< Calculate weldolet weld volume widget */
        WidgetDefault                   /**< Default- or all models */
    };

private:
    //! Private constructor
    PENG_DialogFactory(RB_MainWindow* mw);
    //! Unique (static) instance of factory
    static PENG_DialogFactory* mActiveFactory;

};

#endif // PENG_DIALOGFACTORY_H
