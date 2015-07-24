/*****************************************************************
 * $Id: sail_dialogfactory.h 1979 2013-08-22 19:56:38Z rutger $
 * Created: Jul 26, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SAIL_DIALOGFACTORY_H
#define SAIL_DIALOGFACTORY_H

#include <QObject>
#include "rb_dialogfactory.h"
#include "rb_modelfactory.h"

class RB_ModelFactory;

#define SAIL_DIALOGFACTORY SAIL_DialogFactory::getInstance()

/**
 * Dialog and widget factory for SAIL plugin
 */
class SAIL_DialogFactory : public RB_DialogFactory {

    Q_OBJECT

public:
    virtual ~SAIL_DialogFactory();
    static SAIL_DialogFactory* getInstance(RB_MainWindow* mw = 0);

    virtual RB_ModelFactory* getModelFactory();

    virtual RB_Widget* getWidget(int type, QWidget* parent);
    virtual RB_Dialog* getDialog(int type);
    virtual void updateUiConnections();
    virtual void commandMessage(const RB_String& message);

    /**
     * DB Dialog types, example = 283XXX:
     * - 28 = SAIL,
     * - 3 = dialog,
     * - XXX = sequence number
     * 283100 is starting number leaving 283000 to 283099 for test/debug cases
     * Refer also WidgetType and DialogType
     */
    enum DialogType {
        // start number from 283000
        // DialogTest = 283000,                   /**< Test Dialog */
        // DialogBaseTest,                        /**< Test of base dialogs */

        // start numbering from 283100
        DialogNone = 283100,            /**< Invalid dialog */
        DialogProject,                  /**< Project selection dialog */
        DialogProjectEdit,              /**< Project edit dialog */
        DialogSelectCoordinate,         /**< Select coordinate dialog */
        DialogSelectMap,                /**< Select map dialog */
        DialogSelectSymbol,             /**< Select symbol dialog */
        DialogDefault                   /**< Default- or all dialogs */
    };

    /**
     * DB Widget/dialog manager types, example = 282XXX:
     * - 28 = SAIL,
     * - 2 = widget,
     * - XXX = sequence number
     * 282100 is starting number leaving 282000 to 282099 for test/debug cases
     * Refer also ModelType and DialogType
     */
    enum WidgetType {
        // start numbering from 282000
        // WidgetTestDialog = 282000,      /**< Dialog widget */
        // WidgetTestTable,                /**< Table widget */
        // WidgetTestTableChild,           /**< Child table widget with table parent */
        // WidgetTestText,                 /**< Text widget */
        // WidgetTestTree,                 /**< Tree widget */
        // WidgetTestTreeChild,            /**< Child table widget with tree parent */
        // start numbering from 272100
        WidgetNone = 282100,            /**< Invalid widget */
        WidgetMapNavigation,            /**< Map navigation widget */
        WidgetPolarDiagram,             /**< Polar speed diagram widget */
        WidgetTrackDistanceReport,      /**< Track distance report widget */
        WidgetDefault                   /**< Default- or all widgets */
    };

private:
    //! Private constructor
    SAIL_DialogFactory(RB_MainWindow* mw);
    //! Unique (static) instance of factory
    static SAIL_DialogFactory* mActiveFactory;

};

#endif // SAIL_DIALOGFACTORY_H
