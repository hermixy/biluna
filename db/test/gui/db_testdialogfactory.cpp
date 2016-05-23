/*****************************************************************
 * $Id: db_testdialogfactory.cpp 1928 2013-04-25 20:46:58Z rutger $
 * Created: Nov 3, 2009 5634:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_testdialogfactory.h"

#include "db_dialogfactory.h"
#include "db_testmodelfactory.h"


DB_TestDialogFactory* DB_TestDialogFactory::mFactory = 0;

/**
 * Constructor, register with DB_DialogFactory
 */
DB_TestDialogFactory::DB_TestDialogFactory(RB_MainWindow* mw)
        : RB_DialogFactory(mw) {
    RB_DEBUG->print("DB_TestDialogFactory::DB_TestDialogFactory()");
    DB_DIALOGFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
DB_TestDialogFactory::~DB_TestDialogFactory() {
    DB_DIALOGFACTORY->unregisterFactory(this);
    mFactory = NULL;
    RB_DEBUG->print("DB_TestDialogFactory::~DB_TestDialogFactory() OK");
}

/**
 * Get unique (static) instance of dialog factory
 */
DB_TestDialogFactory* DB_TestDialogFactory::getInstance(RB_MainWindow* mw) {
    RB_DEBUG->print("DB_TestDialogFactory::getInstance()");

    if (!mFactory) {
        mFactory = new DB_TestDialogFactory(mw);
        // initiate factory
//        mActiveObjectFactory->createModelStructure("DB_TestProject");
//        mActiveObjectFactory->setName("DB_TestObjectFactory");
    }
    return mFactory;
}

/**
 * @return DB Test model factory
 */
RB_ModelFactory* DB_TestDialogFactory::getModelFactory() {
    return DB_TESTMODELFACTORY;
}


/**
 * Create widget for dialog, dock-, mdi- or mainwindow
 * @param type type of widget
 * @param parent parent for constructor of widget
 * @return widget
 */
RB_Widget* DB_TestDialogFactory::getWidget(int type, QWidget* parent) {
    RB_Widget* wgt = RB_DialogFactory::getWidget(type, parent);
    if (wgt) return wgt; // widget already found

    switch (type) {
        case WidgetTestDbObject:
        {
            wgt = new DB_TestDbObjectWidget(parent);
            break;

        }
        case WidgetTestTable:
        {
            wgt = new DB_TestTableWidget(parent);
            break;
        }
        case WidgetTestTableTree:
        {
            wgt = new DB_TestTableTreeWidget(parent);
            break;
        }
        case WidgetTestText:
        {
            wgt = new DB_TestTextWidget(parent);
            break;
        }
        case WidgetTestTree:
        {
            wgt = new DB_TestTreeWidget(parent);
            break;
        }
        default:
            RB_DEBUG->print(RB_Debug::D_ERROR,
                            "DB_TestDialogFactory::getWidget() "
                            "non-existing widget requested ERROR");
            break;
    }

    if (wgt) {
        wgt->setWidgetType(type);
        wgt->setDialogFactory(this);
        mWidgetList[type] = wgt;
    }
    return wgt;
}

/**
 * Remove and set unique widget as inactive (widget pointer set to NULL)
 * @param type widget type
 * @return true if widget type is part of this factory and widget was active
 */
bool DB_TestDialogFactory::removeWidget(int type) {
    bool success = RB_DialogFactory::removeWidget(type);
    if (success) return true;

    switch (type) {
        // nothing here
//        case (int)RB2::WidgetCommand:
//        {
//            if (mCommandWidget) {
//                mCommandWidget = NULL;
//                success = true;
//            }
//            break;
//        }
        default:
            break;
    }

    return success;
}

RB_Dialog* DB_TestDialogFactory::getDialog(int type) {
    RB_DEBUG->print("DB_TestDialogFactory::getDialog()");

    switch (type) {
    case DB_TestDialogFactory::DialogTestProject :
    {
//        DB_TestSelectProjectDialog* dlg = new DB_TestSelectProjectDialog(mMainWindow);
//        dlg->init();
//        return dlg;
    }
    case DialogTestRelation :
    {
        DB_TestRelationDialog* dlg = new DB_TestRelationDialog(mMainWindow);
        dlg->init();
        return dlg;
    }
    case DB_TestDialogFactory::WidgetTestDialog: // DlgTest:
    {
        DB_TestDlgDialog* dlg = new DB_TestDlgDialog(mMainWindow);
        dlg->init();
        return dlg;
        // break;
    }
    default:
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "DB_TestDialogFactory::getDialog(type, index, model)"
                        "non-existing dialog requested ERROR");
        break;
    }
    return 0;
}

/**
 * Update signal slot user interface connections
 */
void DB_TestDialogFactory::updateUiConnections() {
    RB_DEBUG->print("DB_TestDialogFactory::updateUiConnections()");
/*
    if (!getMainWindow()) return;

    // Dock windows
    if (mDrawingManagerWidget) {
        // disconnect(mDrawingManagerWidget->getTvManager(), 0,0,0);

        // open window when drawing is double clicked
        connect(mDrawingManagerWidget->getTvManager(),
                SIGNAL(doubleClicked(const QModelIndex&)),
                getMainWindow(),
                SLOT(slotDrawingManagerDoubleClicked(const QModelIndex&)));
        // set current object in model manager
        connect(mDrawingManagerWidget->getSelectionModel(),
                SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
                GV_MMDRAWING,
                SLOT(changeSelection(const QItemSelection&, const QItemSelection&)));

    }

    if (mPreviewWidget) {
// 			disconnect(GV_MMDRAWING, 0,0,0); carefull: this will disconnect all signals from model

        // open preview when drawing is single clicked
        // signal from selection model that current row or selection changed
        connect(GV_MMDRAWING,
                SIGNAL(currentRowChanged(const QModelIndex&)),
                mPreviewWidget,
                SLOT(slotPreviewDrawing(const QModelIndex&)));
    }

    if (mLegendWidget) {
//    	disconnect(mLegendWidget->getTvManager(), 0,0,0);

        // open window when drawing is double clicked
//        connect(mLegendWidget->getTvManager(),
//        		SIGNAL(doubleClicked(const QModelIndex&)),
//        		getMainWindow(),
//        		SLOT(drawingManagerDoubleClicked(const QModelIndex&)));
    }

    if (mPropertiesWidget) {
//    	disconnect(mLegendWidget->getTvManager(), 0,0,0);

        // open window when drawing is double clicked
//        connect(mLegendWidget->getTvManager(),
//        		SIGNAL(doubleClicked(const QModelIndex&)),
//        		getMainWindow(),
//        		SLOT(drawingManagerDoubleClicked(const QModelIndex&)));
    }

    GV_Drawing* dwg = GV_DIALOGFACTORY->getDrawing();

    if (dwg == NULL) {
        RB_DEBUG->print("DB_TestDialogFactory::updateUiConnections: "
            "scene is NULL");
        return;
    }
*/
}

