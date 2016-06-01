/*****************************************************************
 * $Id: crm_dialogfactory.h 2076 2014-01-27 16:02:14Z rutger $
 * Created: Feb 12, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_DIALOGFACTORY_H
#define CRM_DIALOGFACTORY_H

#include <QObject>
#include "rb_dialogfactory.h"
#include "rb_modelfactory.h"

class RB_ModelFactory;

#define CRM_DIALOGFACTORY CRM_DialogFactory::getInstance()

/**
 * Dialog and widget factory for CRM plugin
 */
class CRM_DialogFactory : public RB_DialogFactory {

    Q_OBJECT

public:
    virtual ~CRM_DialogFactory();
    static CRM_DialogFactory* getInstance(RB_MainWindow* mw = 0);

    virtual RB_ModelFactory* getModelFactory();

    virtual RB_Widget* getWidget(int type, QWidget* parent);
    virtual RB_Dialog* getDialog(int type);
    virtual void updateUiConnections();
    virtual void commandMessage(const RB_String& message);

    /**
     * DB Dialog types, example = 273XXX:
     * - 27 = CRM,
     * - 3 = dialog,
     * - XXX = sequence number
     * 273100 is starting number leaving 273000 to 273099 for test/debug cases
     * Refer also WidgetType and DialogType
     */
    enum DialogType {
        // start number from 113000
//        DialogTest = 113000,                   /**< Test Dialog */
//        DialogBaseTest,                        /**< Test of base dialogs */

        // start numbering from 113100
        DialogNone = 273100,            /**< Invalid dialog */
        DialogLeadSourceType,           /**< View/edit lead source types dialog */
        DialogProject,                  /**< Select project (account) from database dialog */
        DialogSelectContact,            /**< Select contact dialog */
        DialogSelectCustomer,           /**< Select customer dialog */
        DialogSelectGroup,              /**< Select group of contacts dialog */
        DialogSelectSystemUser,         /**< Select DB system user */
        DialogSelectTemplate,           /**< Select template dialog */
        DialogSysSeqNo,                 /**< Edit system sequence number dialog */
        DialogDefault                   /**< Default- or all dialogs */
    };

    /**
     * DB Widget/dialog manager types, example = 272XXX:
     * - 27 = DB,
     * - 2 = widget,
     * - XXX = sequence number
     * 272100 is starting number leaving 272000 to 272099 for test/debug cases
     * Refer also ModelType and DialogType
     */
    enum WidgetType {
        // start numbering from 272000
//        WidgetTestDialog = 272000,      /**< Dialog widget */
//        WidgetTestTable,                /**< Table widget */
//        WidgetTestTableChild,           /**< Child table widget with table parent */
//        WidgetTestText,                 /**< Text widget */
//        WidgetTestTree,                 /**< Tree widget */
//        WidgetTestTreeChild,            /**< Child table widget with tree parent */
        // start numbering from 272100
        WidgetNone = 272100,            /**< Invalid widget */
        WidgetActivity,                 /**< Activity edit/view widget */
        WidgetActivityReport,           /**< Activity report widget */
        WidgetCampaignTarget,           /**< Campaign and target contacts widget */
        WidgetContact,                  /**< Contact edit/view widget */
        WidgetContactByCustReport,      /**< Contact by customer report widget */
        WidgetContactReport,            /**< Contact report widget */
        WidgetCustomer,                 /**< Customer edit/view widget */
        WidgetCustomerReport,           /**< Customer report widget */
        WidgetDocument,                 /**< Document edit/view widget */
        WidgetGroupContact,             /**< Group of contacts edit/view widget */
        WidgetProjectEdit,              /**< Edit project (account) dialog */
        WidgetTemplate,                 /**< Template edit/view widget */
        WidgetDefault                   /**< Default- or all widgets */
    };

private:
    //! Private constructor
    CRM_DialogFactory(RB_MainWindow* mw);
    //! Unique (static) instance of factory
    static CRM_DialogFactory* mActiveFactory;

};

#endif // CRM_DIALOGFACTORY_H
