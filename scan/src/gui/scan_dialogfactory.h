/*****************************************************************
 * $Id: scan_dialogfactory.h 1832 2012-12-16 12:01:50Z rutger $
 * Created: Apr 26, 2011 11:34:42 AM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SCAN_DIALOGFACTORY_H
#define SCAN_DIALOGFACTORY_H

#include <QObject>
#include "rb_dialogfactory.h"
#include "rb_modelfactory.h"

class RB_ModelFactory;

#define SCAN_DIALOGFACTORY SCAN_DialogFactory::getInstance()

/**
 * Dialog and widget factory for PENG project
 */
class SCAN_DialogFactory : public RB_DialogFactory {

    Q_OBJECT

public:
    virtual ~SCAN_DialogFactory();
    static SCAN_DialogFactory* getInstance(RB_MainWindow* mw = 0);

    virtual RB_ModelFactory* getModelFactory();

    virtual RB_Widget* getWidget(int type, QWidget* parent);
    virtual RB_Dialog* getDialog(int type);
    virtual void updateUiConnections();

    /**
     * SCAN Dialog types, example = 103XXX:
     * - 10 = DB, 26 = SCAN,
     * - 3 = dialog,
     * - XXX = sequence number
     * 103100 is starting number leaving 103000 to 103099 for test/debug cases
     * Refer also WidgetType and DialogType
     */
    enum DialogType {
        // start number from 263000 for test
        // ...

        // start numbering from 263100
        DialogNone = 263100,            /**< Invalid dialog */
        DialogProject,                  /**< Select project (company) from database dialog */
        DialogProjectEdit,              /**< Project edit dialog */
        DialogScanEdit,                 /**< Scan edit dialog */
        DialogScanSelect,               /**< Scan selection dialog */
        DialogDefault                   /**< Default- or all dialogs */
    };

    /**
     * DB Widget/dialog manager types, example = 102XXX:
     * - 10 = DB, 26 = SCAN
     * - 2 = widget,
     * - XXX = sequence number
     * 102100 is starting number leaving 102000 to 102099 for test/debug cases
     * Refer also ModelType and DialogType
     */
    enum WidgetType {
        // start numbering from 262000 for test
        // ...

        // start numbering from 262100
        WidgetNone = 262100,            /**< Invalid widget */
        WidgetReviewChart,              /**< Review scan chart widget */
        WidgetScanResult,               /**< Scan result and report widget */
        // ...
        WidgetDefault                   /**< Default- or all widgets */
    };

private:
    //! Private constructor
    SCAN_DialogFactory(RB_MainWindow* mw);
    //! Unique (static) instance of factory
    static SCAN_DialogFactory* mActiveFactory;

};

#endif // SCAN_DIALOGFACTORY_H
