/*****************************************************************
 * $Id: srm_dialogfactory.h 2074 2014-01-26 12:21:13Z rutger $
 * Created: Feb 12, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_DIALOGFACTORY_H
#define SRM_DIALOGFACTORY_H

#include <QObject>
#include "rb_dialogfactory.h"
#include "rb_modelfactory.h"

class RB_ModelFactory;

#define SRM_DIALOGFACTORY SRM_DialogFactory::getInstance()

/**
 * Dialog and widget factory for CRM plugin
 */
class SRM_DialogFactory : public RB_DialogFactory {

    Q_OBJECT

public:
    virtual ~SRM_DialogFactory();
    static SRM_DialogFactory* getInstance(RB_MainWindow* mw = 0);

    virtual RB_ModelFactory* getModelFactory();

    virtual RB_Widget* getWidget(int type, QWidget* parent);
    virtual RB_Dialog* getDialog(int type);
    virtual void updateUiConnections();
    virtual void commandMessage(const RB_String& message);

    /**
     * DB Dialog types, example = 293XXX:
     * - 29 = SRM,
     * - 3 = dialog,
     * - XXX = sequence number
     * 293100 is starting number leaving 293000 to 293099 for test/debug cases
     * Refer also WidgetType and DialogType
     */
    enum DialogType {
        // start number from 113000
//        DialogTest = 113000,                   /**< Test Dialog */
//        DialogBaseTest,                        /**< Test of base dialogs */

        // start numbering from 113100
        DialogNone = 293100,            /**< Invalid dialog */
        DialogLeadSourceType,           /**< View/edit lead source types dialog */
        DialogProject,                  /**< Select project (account) from database dialog */
        DialogProjectEdit,              /**< Edit project (account) dialog */
        DialogSelectContact,            /**< Select contact dialog */
        DialogSelectGroup,              /**< Select group of contacts dialog */
        DialogSelectStockMaster,        /**< Select a stock item dialog */
        DialogSelectSupplier,           /**< Select supplier dialog */
        DialogSelectTemplate,           /**< Select template dialog */
        DialogSysSeqNo,                 /**< Edit system sequence number dialog */
        DialogDefault                   /**< Default- or all dialogs */
    };

    /**
     * DB Widget/dialog manager types, example = 292XXX:
     * - 29 = DB,
     * - 2 = widget,
     * - XXX = sequence number
     * 292100 is starting number leaving 292000 to 292099 for test/debug cases
     * Refer also ModelType and DialogType
     */
    enum WidgetType {
        // start numbering from 292000
//        WidgetTestDialog = 292000,      /**< Dialog widget */
//        WidgetTestTable,                /**< Table widget */
//        WidgetTestTableChild,           /**< Child table widget with table parent */
//        WidgetTestText,                 /**< Text widget */
//        WidgetTestTree,                 /**< Tree widget */
//        WidgetTestTreeChild,            /**< Child table widget with tree parent */
        // start numbering from 292100
        WidgetNone = 292100,            /**< Invalid widget */
        WidgetActivity,                 /**< Activity edit/view widget */
        WidgetActivityReport,           /**< Activity report widget */
        WidgetCampaignTarget,           /**< Campaign and target contacts widget */
        WidgetContact,                  /**< Contact edit/view widget */
        WidgetContactBySuppReport,      /**< Contact by supplier report widget */
        WidgetContactReport,            /**< Contact report widget */
        WidgetDocument,                 /**< Document edit/view widget */
        WidgetGroupContact,             /**< Group of contacts edit/view widget */
        WidgetSupplier,                 /**< Supplier edit/view widget */
        WidgetSupplierReport,           /**< Supplier report widget */
        WidgetTemplate,                 /**< Template edit/view widget */
        WidgetDefault                   /**< Default- or all widgets */
    };

private:
    //! Private constructor
    SRM_DialogFactory(RB_MainWindow* mw);
    //! Unique (static) instance of factory
    static SRM_DialogFactory* mActiveFactory;

};

#endif // SRM_DIALOGFACTORY_H
