/*****************************************************************
 * $Id: db_dialogfactory.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Nov 22, 2009 11:34:42 AM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_DIALOGFACTORY_H
#define DB_DIALOGFACTORY_H

#include <QMessageBox>
#include <QObject>
#include "rb_dialogfactory.h"
#include "rb_modelfactory.h"

class DB_FindReplaceDialog;

#define DB_DIALOGFACTORY DB_DialogFactory::getInstance()

/**
 * Dialog and widget factory for DB project
 */
class DB_EXPORT DB_DialogFactory : public RB_DialogFactory {

    Q_OBJECT

public:
    virtual ~DB_DialogFactory();
    static DB_DialogFactory* getInstance(RB_MainWindow* mw = 0);

    virtual RB_ModelFactory* getModelFactory();

    virtual RB_Widget* getWidget(int type, QWidget* parent);
    virtual RB_Dialog* getDialog(int type);
    virtual void updateUiConnections();
    virtual void commandMessage(const RB_String& message);
    virtual void commandMessage(const RB_StringList& messageList);

    virtual std::vector<RB_DialogFactory*> getFactoryList();
    virtual void registerFactory(RB_DialogFactory* factory);
    virtual void unregisterFactory(RB_DialogFactory* factory);
    virtual void closeAllFactories();
    virtual void closeAllMdiWindows();
    virtual void deleteAllMdiWindows();
    virtual void deleteAllDockWidgets();

    virtual void setMainWindowCloseEvent(bool isMainWindowCloseEvent = true);

    virtual RB_String getOriginalStyle();
    virtual QPalette getOriginalPalette();

    /**
     * DB Dialog types, example = 103XXX:
     * - 10 = DB,
     * - 3 = dialog,
     * - XXX = sequence number
     * 103100 is starting number leaving 103000 to 103099 for test/debug cases
     * Refer also WidgetType and DialogType
     */
    enum DialogType {
        // start number from 103000
//        DialogTest = 103000,                   /**< Test Dialog */
//        DialogBaseTest,                        /**< Test of base dialogs */
        // start numbering from 103100
        DialogNone = 103100,                   /**< Invalid dialog */
        DialogBaseEditSort,                    /**< Base table dialog with sorting */
        DialogBaseEditUpDown,                  /**< Base table dialog with up/down */
        DialogBaseSelectionOnly,               /**< Base table dialog for selection only */
        DialogDbBrowser,                       /**< Database browser dialog */
        DialogDbConnection,                    /**< Connect to database dialog */
        DialogDbMapping,                       /**< Execute (database) mapping dialog */
        DialogFindReplace,                     /**< Find and replace dialog */
        DialogInsertHTML,                      /**< Insert HTML dialog */
        DialogModelObject,                     /**< Create a model object dialog */
        DialogPermissionPerspectiveProject,    /**< Permission perspective project selection dialog */
        DialogPluginTester,                    /**< Available plugins dialog */
        DialogRegExp,                          /**< Regular expression test dialog */
        DialogScreenshot,                      /**< Screenshot dialog */
        DialogSelectMapping,                   /**< Select mapping dialog */
        DialogSystemGroup,                     /**< Edit and select system group dialog */
        DialogDefault                          /**< Default- or all dialogs */
    };

    /**
     * DB Widget manager types, example = 102XXX:
     * - 10 = DB,
     * - 2 = widget,
     * - XXX = sequence number
     * 102100 is starting number leaving 102000 to 102099 for test/debug cases
     * Refer also ModelType and DialogType
     */
    enum WidgetType {
        // start numbering from 102000
        WidgetTest = 102000,            /**< Test Dialog */
        WidgetTestDbObject,             /**< Test database object */
        WidgetTestRelation,             /**< Test relation table dialog */
        WidgetTestSelectProject,        /**< Test project selection dialog */
        WidgetTestTable,                /**< Table widget */
        WidgetTestTableTree,            /**< Table with treechild widget */
        WidgetTestText,                 /**< Text widget */
        WidgetTestTree,                 /**< Tree widget */
        DialogBaseTest,                 /**< Test of base dialogs */
        // start numbering from 102100
        WidgetNone = 102100,            /**< Invalid widget */
        WidgetCalculator,               /**< Calculator dockwidget */
        WidgetCalendar,                 /**< Calendar widget */
        WidgetCommand,                  /**< Command widget */
        WidgetDashboard,                /**< Biluna main dashboard widget */
        WidgetHtmlEditor,               /**< HTML editor widget */
        WidgetInternet,                 /**< Internet viewer widget */
        WidgetPaletteColorPicker,       /**< Color picker for custom GUI palette */
        WidgetPermission,               /**< Permission widget */
        WidgetPermissionReport,         /**< Permission report widget */
        WidgetPlainTextDocument,        /**< Plain text document widget */
        WidgetProjectEdit,              /**< Edit projects widget */
        WidgetProjectSelect,            /**< Select project widget */
        WidgetSimpleChart,              /**< Simple chart types review widget */
        WidgetSimpleReport,             /**< Simple report from database widget */
        WidgetSpreadSheet,              /**< Spreadsheet table widget */
        WidgetTextDocument,             /**< Rich text document widget */
        WidgetVersion,                  /**< View database version dialog */
        WidgetXmlViewValidator,         /**< XML view and validator widget */
        WidgetDefault                   /**< Default- or all widgets */
    };

signals:
    void alternativeApplicationStyle(bool isAlternativeStyle);

public slots:
    virtual void slotAlternativeApplicationStyle(bool isAlternativeStyle);
    virtual void slotFindReplaceDialogClosed() {
        mFindReplaceDialog = NULL;
    }

private:
    //! Private constructor
    DB_DialogFactory(RB_MainWindow* mw);
    //! Unique (static) instance of factory
    static DB_DialogFactory* mActiveFactory;
    //! List of all currently created object factories
    std::vector<RB_DialogFactory*> mFactoryList;
    //! Find and/or replace dialog
    DB_FindReplaceDialog* mFindReplaceDialog;
    //! HACK: Flag indicating the MainWindow is being closed
    bool mIsMainWindowCloseEvent;
    //! Original application style
    RB_String mOriginalStyle;
    //! Original application palette
    QPalette mOriginalPalette;
};

#endif // DB_DIALOGFACTORY_H
