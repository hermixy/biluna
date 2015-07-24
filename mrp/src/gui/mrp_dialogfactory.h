/*****************************************************************
 * $Id: sail_dialogfactory.h 1979 2013-08-22 19:56:38Z rutger $
 * Created: Oct 24, 2014 21:17:25 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna MRP project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef MRP_DIALOGFACTORY_H
#define MRP_DIALOGFACTORY_H

#include <QObject>
#include "rb_dialogfactory.h"
#include "rb_modelfactory.h"

class RB_ModelFactory;

#define MRP_DIALOGFACTORY MRP_DialogFactory::getInstance()

/**
 * Dialog and widget factory for MRP plugin
 */
class MRP_DialogFactory : public RB_DialogFactory {

    Q_OBJECT

public:
    virtual ~MRP_DialogFactory();
    static MRP_DialogFactory* getInstance(RB_MainWindow* mw = 0);

    virtual RB_ModelFactory* getModelFactory();

    virtual RB_Widget* getWidget(int type, QWidget* parent);
    virtual RB_Dialog* getDialog(int type);
    virtual void updateUiConnections();
    virtual void commandMessage(const RB_String& message);

    /**
     * DB Dialog types, example = 303XXX:
     * - 30 = MRP,
     * - 3 = dialog,
     * - XXX = sequence number
     * 303100 is starting number leaving 303000 to 303099 for test/debug cases
     * Refer also WidgetType and DialogType
     */
    enum DialogType {
        // start number from 303000
        // DialogTest = 303000,                   /**< Test Dialog */
        // DialogBaseTest,                        /**< Test of base dialogs */

        // start numbering from 303100
        DialogNone = 303100,            /**< Invalid dialog */
        DialogSelectContact,            /**< Select contact dialog */
        DialogSelectCustomer,           /**< Select customer dialog */
        DialogSelectOrder,              /**< Select (sales) order dialog */
        DialogSelectProject,            /**< Project selection dialog */
        DialogDefault                   /**< Default- or all dialogs */
    };

    /**
     * DB Widget/dialog manager types, example = 302XXX:
     * - 30 = SAIL,
     * - 2 = widget,
     * - XXX = sequence number
     * 302100 is starting number leaving 302000 to 302099 for test/debug cases
     * Refer also ModelType and DialogType
     */
    enum WidgetType {
        // start numbering from 302000
        // WidgetTestDialog = 302000,      /**< Dialog widget */
        // WidgetTestTable,                /**< Table widget */
        // WidgetTestTableChild,           /**< Child table widget with table parent */
        // WidgetTestText,                 /**< Text widget */
        // WidgetTestTree,                 /**< Tree widget */
        // WidgetTestTreeChild,            /**< Child table widget with tree parent */
        // start numbering from 272100
        WidgetNone = 302100,            /**< Invalid widget */
        WidgetSoftwareLicense,          /**< Software license widget */
        WidgetSoftwareLicenseReport,    /**< Software license report widget */
        WidgetDefault                   /**< Default- or all widgets */
    };

private:
    //! Private constructor
    MRP_DialogFactory(RB_MainWindow* mw);
    //! Unique (static) instance of factory
    static MRP_DialogFactory* mActiveFactory;

};

#endif // MRP_DIALOGFACTORY_H
