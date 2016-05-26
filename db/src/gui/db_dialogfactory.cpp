/*****************************************************************
 * $Id: db_dialogfactory.cpp 2248 2015-06-21 09:13:00Z rutger $
 * Created: Nov 22, 2009 11:34:42 AM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_dialogfactory.h"

#include "db_browserdialog.h"
#include "db_calculatorwidget.h"
#include "db_calendarwidget.h"
#include "db_commandwidget.h"
#include "db_connectiondialog.h"
#include "db_findreplacedialog.h"
#include "db_dashboardwidget.h"
#include "db_htmleditorwidget.h"
//#include "db_htmlviewwidget.h"
#include "db_inserthtmldialog.h"
#include "db_internetbrowserwidget.h"
#include "db_mappingdialog.h"
#include "db_modelfactory.h"
#include "db_modelobjectdialog.h"
#include "db_palettecolorpickerwidget.h"
#include "db_permissionperspectiveprojectdialog.h"
#include "db_permissionwidget.h"
#include "db_permissionreportwidget.h"
#include "db_plaintextwidget.h"
#include "db_plugintesterdialog.h"
#include "db_projecteditwidget.h"
#include "db_projectselectwidget.h"
#include "db_regexpdialog.h"
#include "db_screenshotdialog.h"
#include "db_selectmappingdialog.h"
#include "db_simplereportwidget.h"
#include "db_systemgroupdialog.h"

#ifdef DB_TEST
#include "db_testdbobjectwidget.h"
#include "db_testrelationwidget.h"
#include "db_testselectprojectwidget.h"
#include "db_testtabletreewidget.h"
#include "db_testtablewidget.h"
#include "db_testtextwidget.h"
#include "db_testtreewidget.h"
#endif

#include "db_textwidget.h"
#include "db_versiondialog.h"
#include "db_xmlviewvalidatorwidget.h"
#include "rb_debug.h"
#include "rb_mainwindow.h"
#include "rb_mdiwindow.h"
#include "rb_settings.h"
#include "thrd_chartwidget.h"

DB_DialogFactory* DB_DialogFactory::mActiveFactory = 0;

/**
 * Constructor
 */
DB_DialogFactory::DB_DialogFactory(RB_MainWindow* mw)
        : RB_DialogFactory(mw) {
    RB_DEBUG->print("DB_DialogFactory::DB_DialogFactory()");
    this->setPerspective(RB2::PerspectiveDB);
    mFindReplaceDialog = NULL;
    mIsMainWindowCloseEvent = false;
    mOriginalStyle = QApplication::style()->objectName(); // WindowsVista, Macintosh
    mOriginalPalette = QGuiApplication::palette();
}

/**
 * Destructor
 */
DB_DialogFactory::~DB_DialogFactory() {
    if (!mIsMainWindowCloseEvent) {
        closeAllMdiWindows();
    }
    deleteAllDockWidgets();
    closeAllFactories();

    // and DB_DialogFactory dockwindows and mdiwindows
    // which are not closed when database disconnects
    RB_DialogFactory::deleteAllMdiWindows();
    RB_DialogFactory::deleteAllDockWidgets();
    RB_DialogFactory::deleteAllToolBars();

    mActiveFactory = NULL;
    RB_DEBUG->print("DB_DialogFactory::~DB_DialogFactory() OK");
}

/**
 * Get unique (static) instance of dialog factory
 */
DB_DialogFactory* DB_DialogFactory::getInstance(RB_MainWindow* mw) {
    if (!mActiveFactory) {
        mActiveFactory = new DB_DialogFactory(mw);
    }
    return mActiveFactory;
}

/**
 * @return DB model factory
 */
RB_ModelFactory* DB_DialogFactory::getModelFactory() {
    return DB_MODELFACTORY;
}

/**
 * Create widget for dock- or MDI window, could also be for dialog or mainwindow
 * @param type type of widget
 * @param parent parent for constructor of widget
 * @return widget
 */
RB_Widget* DB_DialogFactory::getWidget(int type, QWidget* parent) {
    RB_Widget* wgt = NULL;

    switch (type) {
    case WidgetCalculator: {
        // Unique widget
        wgt = RB_DialogFactory::findWidget(type);
        if (wgt) return wgt;      // called once by mainwindow
        wgt = new DB_CalculatorWidget(parent);
        mWidgetList[type] = wgt;  // for retrieval
        break;
    }
    case WidgetCalendar: {
        // Unique widget
        wgt = RB_DialogFactory::findWidget(type);
        if (wgt) return wgt;
        wgt = new DB_CalendarWidget(parent);
        mWidgetList[type] = wgt;
        break;
    }
    case WidgetCommand: {
        // Unique widget
        wgt = RB_DialogFactory::findWidget(type);
        if (wgt) return wgt;      // called once by mainwindow
        wgt = new DB_CommandWidget(parent);
        mWidgetList[type] = wgt;  // for retrieval
        break;
    }
    case WidgetDashboard: {
        // Unique widget
        wgt = RB_DialogFactory::findWidget(type);
        if (wgt) return wgt;
        wgt = new DB_DashboardWidget(parent);
        mWidgetList[type] = wgt;
        break;
    }
    case WidgetHtmlEditor: {
        // Non-unique widget
        wgt = new DB_HtmlEditorWidget(parent);
        break;
    }
    case WidgetInternet: {
        // Non-unique widget
        wgt = new DB_InternetBrowserWidget(parent);
        break;
    }
    case WidgetPaletteColorPicker: {
        wgt = RB_DialogFactory::findWidget(type);
        if (wgt) return wgt;
        wgt = new DB_PaletteColorPickerWidget(parent);
        mWidgetList[type] = wgt;
        break;
    }
    case WidgetPermission:
    {
        wgt = RB_DialogFactory::findWidget(type);
        if (wgt) return wgt;
        wgt = new DB_PermissionWidget(parent);
        mWidgetList[type] = wgt;
        break;
    }
    case WidgetPermissionReport:
    {
        wgt = RB_DialogFactory::findWidget(type);
        if (wgt) return wgt;
        wgt = new DB_PermissionReportWidget(parent);
        mWidgetList[type] = wgt;
        break;
    }
    case WidgetPlainTextDocument: {
        // Non-unique widget
        wgt = new DB_PlainTextWidget(parent);
        break;
    }
    case WidgetProjectEdit: {
        wgt = new DB_ProjectEditWidget(parent);
        break;
    }
    case WidgetProjectSelect: {
        wgt = new DB_ProjectSelectWidget(parent);
        break;
    }
    case WidgetSimpleChart: {
        // Unique widget
        wgt = RB_DialogFactory::findWidget(type);
        if (wgt) return wgt;
        wgt = new THRD_ChartWidget(parent);
        mWidgetList[type] = wgt;
        break;
    }
    case WidgetSimpleReport: {
        // Unique widget
        wgt = RB_DialogFactory::findWidget(type);
        if (wgt) return wgt;
        wgt = new DB_SimpleReportWidget(parent);
        mWidgetList[type] = wgt;
        break;
    }
    case WidgetTextDocument: {
        // Non-unique widget
        wgt = new DB_TextWidget(parent);
        break;
    }
    case WidgetXmlViewValidator: {
        // Unique widget
        wgt = RB_DialogFactory::findWidget(type);
        if (wgt) return wgt;
        wgt = new DB_XmlViewValidatorWidget(parent);
        break;
    }
#ifdef DB_TEST
    case WidgetTestDbObject:
    {
        wgt = new DB_TestDbObjectWidget(parent);
        break;

    }
    case WidgetTestRelation :
    {
        wgt = new DB_TestRelationWidget(parent);
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
    case WidgetTestSelectProject :
    {
        wgt = new DB_TestSelectProjectWidget(parent);
        break;
    }
    case WidgetTestDialog: // DlgTest:
    {
//        DB_TestDlgDialog* dlg = new DB_TestDlgDialog(parent);
//        dlg->init();
//        return dlg;
    }
#endif
    default:
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "DB_DialogFactory::getWidget() "
                        "non-existing widget requested ERROR");
        break;
    }

    if (wgt) {
        wgt->setWidgetType(type);
        wgt->setDialogFactory(this);
        // Set widget as unique widget, set 'inactive' via removeWidget()
        // mWidgetList[type] = wgt; Except a few widgets no unique widgets here.
        // wgt->init(); not here but is done in RB_DialogFactory::getMdiWindow()
    }

    return wgt;
}

/**
 * Request a dialog
 * @param type type of dialog
 * @return Dialog
 */
RB_Dialog* DB_DialogFactory::getDialog(int type) {
    RB_DEBUG->print("DB_DialogFactory::getDialog()");

    RB_Dialog* dlg = NULL;

    switch (type) {
        case DialogDbBrowser:
        {
            dlg = new DB_BrowserDialog(mMainWindow);
            dlg->init();
            break;
        }
        case DialogDbConnection:
        {
            dlg = new DB_ConnectionDialog(mMainWindow);
            dlg->init();
            break;
        }
        case DialogDbMapping:
        {
            dlg = new DB_MappingDialog(mMainWindow);
            dlg->init();
            break;
        }
        case DialogFindReplace: // is also Find dialog
        {
            if (!mFindReplaceDialog) {
                mFindReplaceDialog = new DB_FindReplaceDialog(mMainWindow);
            }
            dlg = mFindReplaceDialog;
            dlg->init();
            connect(mFindReplaceDialog, SIGNAL(closed()),
                    this, SLOT(slotFindReplaceDialogClosed()));
            break;
        }
        case DialogInsertHTML:
        {
            dlg = new DB_InsertHtmlDialog(mMainWindow);
            dlg->init();
            break;
        }
        case DialogModelObject:
        {
            dlg = new DB_ModelObjectDialog(mMainWindow);
            dlg->init();
            break;
        }
        case DialogPermissionPerspectiveProject:
        {
            dlg = new DB_PermissionPerspectiveProjectDialog(mMainWindow);
            dlg->init();
            break;
        }
        case DialogPluginTester:
        {
            dlg = new DB_PluginTesterDialog(mMainWindow);
            dlg->init();
            break;
        }
//        case DialogProject:
//        {
//            dlg = new DB_ProjectDialog(mMainWindow);
//            dlg->init();
//            break;
//        }
        case DialogRegExp:
        {
            dlg = new DB_RegExpDialog(mMainWindow);
            dlg->init();
            break;
        }
        case DialogScreenshot:
        {
            dlg = new DB_ScreenshotDialog(mMainWindow);
            dlg->init();
            break;
        }
        case DialogSelectMapping:
        {
            dlg = new DB_SelectMappingDialog(mMainWindow);
            dlg->init();
            break;
        }
        case DialogSystemGroup:
        {
            dlg = new DB_SystemGroupDialog(mMainWindow);
            dlg->init();
            break;
        }
        case DialogVersion:
        {
            dlg = new DB_VersionDialog(mMainWindow);
            dlg->init();
            break;
        }
        default:
            RB_DEBUG->print(RB_Debug::D_ERROR,
                            "DB_DialogFactory::getDialog(type)"
                            "non-existing dialog requested ERROR");
            break;
    }
    return dlg;
}

/**
 * Update signal slot user interface connections
 */
void DB_DialogFactory::updateUiConnections() {
    RB_DEBUG->print("DB_DialogFactory::updateUiConnections()");

    // For dock windows

}

/**
 * Called when an action needs to communicate 'message' to the user.
 */
void DB_DialogFactory::commandMessage(const RB_String& message) {
    if (message.isEmpty()) {
        RB_DEBUG->warning("DB_DialogFactory::commandMessage() message "
                          "is empty WARNING");
        return;
    }

    DB_CommandWidget* wgt = dynamic_cast<DB_CommandWidget*>(
                            findWidget(DB_DialogFactory::WidgetCommand));
    if (wgt != NULL) {
        wgt->appendHistory(message);
    }
}

/**
 * Called when an action needs to communicate a 'message' list to the user.
 */
void DB_DialogFactory::commandMessage(const RB_StringList& messageList) {
    if (messageList.isEmpty()) {
        RB_DEBUG->warning("DB_DialogFactory::commandMessage() message list "
                          "is empty WARNING");
        return;
    }

    DB_CommandWidget* wgt = dynamic_cast<DB_CommandWidget*>(
                            findWidget(DB_DialogFactory::WidgetCommand));
    if (wgt != NULL) {
        int count = messageList.size();

        for (int i = 0; i < count; ++i) {
            wgt->appendHistory(messageList.at(i));
        }
    }
}

/**
 * @return factory list
 */
std::vector<RB_DialogFactory*> DB_DialogFactory::getFactoryList() {
    return mFactoryList;
}

/**
 * Register factory
 * @param f Objectfactory
 */
void DB_DialogFactory::registerFactory(RB_DialogFactory* f) {
    bool exists = false;

    std::vector<RB_DialogFactory*>::iterator iter;
    iter = mFactoryList.begin();
    while (iter != mFactoryList.end() && !exists) {
        if(f == *iter) {
            exists = true;
            RB_DEBUG->print(RB_Debug::D_ERROR,
                            "DB_DialogFactory::registerFactory() "
                            "objectfactory already registered ERROR");
        }
        ++iter;
    }

    if (!exists) {
        mFactoryList.push_back(f);
    }
}

/**
 * Unregister factory
 * @param f Objectfactory
 */
void DB_DialogFactory::unregisterFactory(RB_DialogFactory* f) {
    bool exists = false;

    std::vector<RB_DialogFactory*>::iterator iter;
    iter = mFactoryList.begin();
    while (!exists && iter != mFactoryList.end()) {
        if (f == *iter) {
            mFactoryList.erase(iter);
            exists = true;
        } else {
            ++iter;
        }
    }

    if (!exists) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "DB_DialogFactory::unregisterFactory() "
                        "objectfactory was not registered ERROR");
    }
}

/**
 * Close and delete all factories except this (DB) factory
 */
void DB_DialogFactory::closeAllFactories() {
    while (!mFactoryList.empty()) {
        RB_DialogFactory* f
                = dynamic_cast<RB_DialogFactory*>(mFactoryList.back());
        f->closeAllMdiWindows();
        f->deleteAllDockWidgets();
        f->deleteAllToolBars();
        delete f; // unregisters factory and removes, therefor no pop_back()
    }
}

/**
 * Close all MDI windows, also deletes the window later
 */
void DB_DialogFactory::closeAllMdiWindows() {
    RB_DialogFactory::closeAllMdiWindows();

    std::vector<RB_DialogFactory*>::iterator iter;
    iter = mFactoryList.begin();
    while (iter != mFactoryList.end()) {
        RB_DialogFactory* f
                = dynamic_cast<RB_DialogFactory*>(*iter);
        f->closeAllMdiWindows();
        ++iter;
    }
}

/**
 * Delete all MDI windows of other factories.
 * DB only deletes here:
 * - DB_PermissionWidget
 * - DB_PermissionReportWidget
 * - DB_SimpleReportWidget
 */
void DB_DialogFactory::deleteAllMdiWindows() {
    std::vector<RB_DialogFactory*>::iterator iter;
    iter = mFactoryList.begin();
    while (iter != mFactoryList.end()) {
        RB_DialogFactory* f
                = dynamic_cast<RB_DialogFactory*>(*iter);
        f->deleteAllMdiWindows();
        ++iter;
    }

    // The below deletion is an exception here, handled differently in plugins
    if (isWidgetActive(DB_DialogFactory::WidgetPermission)) {
        RB_MdiWindow* mdiWindow = getMdiWindow(DB_DialogFactory::WidgetPermission);
        delete mdiWindow;
    }
    if (isWidgetActive(DB_DialogFactory::WidgetPermissionReport)) {
        RB_MdiWindow* mdiWindow = getMdiWindow(
                    DB_DialogFactory::WidgetPermissionReport);
        delete mdiWindow;
    }
    if (isWidgetActive(DB_DialogFactory::WidgetSimpleReport)) {
        RB_MdiWindow* mdiWindow = getMdiWindow(
                    DB_DialogFactory::WidgetSimpleReport);
        delete mdiWindow;
    }
}

/**
 * Delete all dock windows of other factories, no DB (yet)
 */
void DB_DialogFactory::deleteAllDockWidgets() {
    std::vector<RB_DialogFactory*>::iterator iter;
    iter = mFactoryList.begin();
    while (iter != mFactoryList.end()) {
        RB_DialogFactory* f
                = dynamic_cast<RB_DialogFactory*>(*iter);
        f->deleteAllDockWidgets();
        ++iter;
    }
}

void DB_DialogFactory::setMainWindowCloseEvent(bool isMainWindowCloseEvent) {
    mIsMainWindowCloseEvent = isMainWindowCloseEvent;
}

RB_String DB_DialogFactory::getOriginalStyle() {
    return mOriginalStyle;
}

QPalette DB_DialogFactory::getOriginalPalette() {
    return mOriginalPalette;
}

/**
 * Transfer the signal that the style is changed
 * @brief DB_DialogFactory::slotAlternativeApplicationStyle
 * @param isAlternativeStyle
 */
void DB_DialogFactory::slotAlternativeApplicationStyle(bool isAlternativeStyle) {
    emit alternativeApplicationStyle(isAlternativeStyle);
}

