/*****************************************************************
 * $Id: db_testdialogfactory.h 1928 2013-04-25 20:46:58Z rutger $
 * Created: Nov 3, 2009 5634:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_TESTDIALOGFACTORY_H
#define DB_TESTDIALOGFACTORY_H

#include <QMessageBox>
#include <QObject>
#include "db_commandwidget.h" // not used?
#include "db_testdbobjectwidget.h"
#include "db_testdlgdialog.h"
#include "db_testrelationdialog.h"
#include "db_testtablewidget.h"
#include "db_testtabletreewidget.h"
#include "db_testtextwidget.h"
#include "db_testtreewidget.h"
#include "rb_debug.h"
#include "rb_dialogfactory.h"
#include "rb_mainwindow.h"
#include "rb_mdiwindow.h"
#include "rb_settings.h"


#define DB_TESTDIALOGFACTORY DB_TestDialogFactory::getInstance()

/**
 * Dialog and widget factory for DB test project
 */
class DB_TestDialogFactory : public RB_DialogFactory {

    Q_OBJECT

public:
    virtual ~DB_TestDialogFactory();
    static DB_TestDialogFactory* getInstance(RB_MainWindow* mw = 0);

    virtual RB_ModelFactory* getModelFactory();

    virtual RB_Widget* getWidget(int type, QWidget* parent);
    virtual bool removeWidget(int type);
    virtual RB_Dialog* getDialog(int type);
    virtual void updateUiConnections();

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
        DialogTest = 103000,                   /**< Test Dialog */
        DialogTestProject,                     /**< Test project selection dialog */
        DialogTestRelation,                    /**< Test relation table dialog */
        DialogBaseTest                         /**< Test of base dialogs */
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
        WidgetTestDbObject = 102000,    /**< DB versus object widget */
        WidgetTestDialog,               /**< Dialog widget */
        WidgetTestTable,                /**< Table widget */
        WidgetTestTableTree,            /**< Table with treechild widget */
        WidgetTestText,                 /**< Text widget */
        WidgetTestTree                  /**< Tree widget */
    };

private:
    //! Private constructor
    DB_TestDialogFactory(RB_MainWindow* mw);
    //! Unique (static) instance of factory
    static DB_TestDialogFactory* mFactory;

};

#endif // DB_TESTDIALOGFACTORY_H
