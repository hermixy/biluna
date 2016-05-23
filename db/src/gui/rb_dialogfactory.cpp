/*****************************************************************
 * $Id: rb_dialogfactory.cpp 2222 2015-02-26 19:38:49Z rutger $
 * Created: Nov 16, 2009 3:07:28 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_dialogfactory.h"

#include "rb_debug.h"
#include "rb_dialogwindow.h"
#include "rb_dockwidget.h"
#include "rb_mainwindow.h"
#include "rb_mdiwindow.h"
#include "rb_settings.h"
#include "rb_toolbar.h"


/**
 * Constructor
 */
RB_DialogFactory::RB_DialogFactory(RB_MainWindow* mw)
        : QObject(mw) {
    RB_DEBUG->print("RB_DialogFactory::RB_DialogFactory()");
    mMainWindow = mw;
    mPerspective = RB2::PerspectiveNone;
}

/**
 * Destructor
 */
RB_DialogFactory::~RB_DialogFactory() {
    /*
    // Delete remaining widgets on dock windows and mdi windows
    // key: first, value: second
    std::map<int, RB_Widget*>::iterator iter;
    iter = mWidgetList.begin();
    while (iter != mWidgetList.end()) {
        if ((*iter).second != NULL) {
            delete (*iter).second;
            (*iter).second = NULL;
        }
        ++iter;
    }

    // empty list (vector), this is usually not overdone because otherwise
    // map contains invalid pointers
    mWidgetList.clear();
    */

    RB_DEBUG->print("RB_DialogFactory::~RB_DialogFactory() OK");
}

/**
 * Get active MDI window
 * @return active MDI window
 */
RB_MdiWindow* RB_DialogFactory::getActiveMdiWindow() {
    QMdiArea* mdiArea = mMainWindow->getMdiArea();
    if (!mdiArea) return NULL;
    return dynamic_cast<RB_MdiWindow*>(mdiArea->activeSubWindow());
}

/**
 * @brief RB_DialogFactory::getActiveMdiCentralWidget
 * @return Central widget of active MDI window
 */
RB_Widget *RB_DialogFactory::getActiveMdiCentralWidget() {
    RB_MdiWindow* mdiWin = getActiveMdiWindow();
    if (!mdiWin) return NULL;
    RB_Widget* mdiWgt = dynamic_cast<RB_Widget*>(mdiWin->widget());
    return mdiWgt;
}

/**
 * Get and/or create MDI window, type can be one of a kind or a type
 * with more windows, such as drawings
 * @param type widget type of MDI window
 * @param docName filename or database ID of document
 * @param isNewWidget set true if file has not been save before
 * and requires a fileSaveAs() first
 * @return MDI window or NULL if not exists
 */
RB_MdiWindow* RB_DialogFactory::getMdiWindow(int type, const RB_String& docName,
                                             bool isNewWidget) {
    if (!mMainWindow) {
        RB_DEBUG->print(RB_Debug::D_ERROR, "RB_DialogFactory::getMdiWindow() ERROR");
        return NULL;
    }

    QMdiArea* mdiArea = mMainWindow->getMdiArea();
    if (!mdiArea) return NULL;
    RB_MdiWindow* mdiWindow = NULL;

    // Find MDI window
    if (!isNewWidget) {
        mdiWindow = mMainWindow->findMdiWindow(docName, type);
    }

    // Create MDI window if does not exist
    if (!mdiWindow) {
        // parent can be NULL because RB_MdiWindow will be parent
        // when widget is set as centralWidget
        RB_Widget* wgt = getWidget(type, NULL);
        wgt->setName(docName);
        wgt->setIsNewWidget(isNewWidget);

        if (wgt) {
            mdiWindow = new RB_MdiWindow(mMainWindow, wgt);
            mMainWindow->getMdiArea()->addSubWindow(mdiWindow);
            wgt->setParent(mdiWindow);
            wgt->init(); // here because widget->init() will set size of MDI window
        }
    }

    return mdiWindow;
}

/**
 * @return mainwindow
 */
RB_MainWindow* RB_DialogFactory::getMainWindow() {
    return mMainWindow;
}

/**
 * To be overriden to return application or plugin specific modelfactory
 * @return model factory from mainwindow
 */
//RB_ModelFactoryInterface* RB_DialogFactory::getModelFactory() {
//    if (!mMainWindow) return NULL;
//    return mMainWindow->getModelFactory();
//}

/**
 * Create widget for dialog, dock-, mdi- or mainwindow
 * NOTE: to be overridden by implementation starting with:
 *   RB_Widget* wgt = RB_DialogFactory::findWidget(type);
 *   and
 *   if (wgt) return wgt;
 *   in case statement in case of a unique widget
 * to include also the standard widgets such as the commandwidget.
 * @param type type of widget
 * @param parent parent for constructor of widget
 * @return widget
 */
RB_Widget* RB_DialogFactory::getWidget(int /* type */, QWidget* /* parent */) {
//    RB_Widget* wgt = findWidget(type);
//
//    switch (type) {
//        case (int)RB2::WidgetCommand:
//        {
//            if (wgt) return wgt; in case of unique widget
//            wgt = new DB_CommandWidget(parent);
//            break;
//        }
//        default:
//            break;
//    }
//
//    if (!wgt) {
//        wgt->setWidgetType(type);
//        wgt->setDialogFactory(this);
//        mWidgetList[type] = wgt;
//    }
//    return wgt;
    return NULL;
}

/**
 * Find (unique) widget in widgetList
 * @return widget or NULL
 */
RB_Widget* RB_DialogFactory::findWidget(int type) {
    RB_Widget* wgt = NULL;
    std::map<int, RB_Widget*>::iterator iter;
    iter = mWidgetList.find(type);

    if (iter != mWidgetList.end()) {
        // widget 1- exists or 2- has been removed/deleted and set to NULL
        wgt = (*iter).second;
    }

    return wgt;
}

/**
 * Remove and set unique widget as inactive (widget pointer set to NULL)
 * Unique widget is set by 'mWidgetList[type] = wgt' in getWidget()
 * to include also the standard widgets such as the commandwidget.
 * @param type widget type
 * @return true if widget type is part of this factory and widget was active
 */
bool RB_DialogFactory::removeWidget(int type) {
    RB_DEBUG->print("RB_DialogFactory::removeWidget()");

    bool success = false;
    RB_Widget* wgt = NULL;
    std::map<int, RB_Widget*>::iterator iter;
    iter = mWidgetList.find(type);

    if (iter != mWidgetList.end()) {
        // widget 1- exists or 2- has been removed/deleted and set to NULL
        wgt = (*iter).second;

        if (wgt) {
            // do not delete here, removeWidget is called by destructor of widget
            (*iter).second = NULL;
            success = true;
        }
    }

    return success;
}

/**
 * @return true if widget is active
 */
bool RB_DialogFactory::isWidgetActive(int type) {
    RB_Widget* wgt = findWidget(type);
    if (wgt) return true;
    return false;
}

/**
 * Get dialog
 * @param type type of widget
 * @return RB_DialogWindow
 */
RB_DialogWindow *RB_DialogFactory::getDialogWindow(int type,
                                                   bool isNewWidget) {
    if (!mMainWindow) {
        RB_DEBUG->error("RB_DialogFactory::getDialogWindow() ERROR");
        return NULL;
    }

    RB_DialogWindow* dlgWindow = NULL;
    RB_Widget* wgt = getWidget(type, NULL);

    if (wgt) {
        wgt->setIsNewWidget(isNewWidget);
        dlgWindow = new RB_DialogWindow(mMainWindow, wgt);
        dlgWindow->setWindowTitle(wgt->getName());
        dlgWindow->setObjectName("RB_DialogWindow");
        wgt->setParent(dlgWindow);
        wgt->init(); // here because widget->init() will set size of dialog
    }

    return dlgWindow;
}

/**
 * Get or create toolbar
 * @param widgetType widget type
 * @param tbName toolbar widget name
 * @param tbArea top, left, bottom or right dockarea in mainwindow
 */
RB_ToolBar* RB_DialogFactory::getToolBar(int widgetType,
                                         int tbArea) {
    RB_ToolBar* tb = NULL;
    RB_Widget* wgt = findWidget(widgetType);
    if (!wgt) {
        tb = new RB_ToolBar(getMainWindow(), NULL);
        wgt = getWidget(widgetType, tb);
        tb->setWidget(wgt);
        tb->setWindowTitle(wgt->getName());
        tb->setObjectName("RB_ToolBar");
        getMainWindow()->addToolBar(static_cast<Qt::ToolBarArea>(tbArea), tb);
        wgt->init(); // after creation is complete
    } else {
        tb = dynamic_cast<RB_ToolBar*>(wgt->parent());
    }
    return tb;
}

/**
 * Get or create dockwidget
 * @param widgetType widget type
 * @param dockName dockwidget name
 * @param dockArea top left bottom or right dockarea in mainwindow
 */
RB_DockWidget* RB_DialogFactory::getDockWidget(int widgetType,
                                               int dockArea) {
    RB_DockWidget* dw = NULL;
    RB_Widget* wgt = findWidget(widgetType);
    if (!wgt) {
        wgt = getWidget(widgetType, getMainWindow());
        dw = new RB_DockWidget(getMainWindow(), wgt);
        dw->setWindowTitle(wgt->getName());
        dw->setObjectName("RB_DockWidget");
        getMainWindow()->addDockWidget(static_cast<Qt::DockWidgetArea>(dockArea), dw);
        wgt->init(); // after creation is complete
    } else {
        dw = dynamic_cast<RB_DockWidget*>(wgt->parent());
    }
    return dw;
}

/**
 * Request a dialog, implement in actual class
 * @param type type of dialog
 * @return dialog
 */
RB_Dialog* RB_DialogFactory::getDialog(int /*type*/) {
    RB_DEBUG->error("RB_DialogFactory::getDialog() ERROR");
    /*
    switch (type) {
        default:
            break;
    }
    */
    return NULL;
}

/**
 * Shows a information mesage dialog.
 */
void RB_DialogFactory::requestInformationDialog(const RB_String& info) {
    QMessageBox::information(mMainWindow, QMessageBox::tr("Information"),
                             info, QMessageBox::Ok);
}

/**
 * Show a basic text input dialog
 * @param dialogTitle title of the dialog
 * @param labelName label name or explanation of the text to be entered
 * @param defaultText default text
 * @return user text
 */
RB_String RB_DialogFactory::requestTextInputDialog(const RB_String& dialogTitle,
                                                   const RB_String& labelName,
                                                   const RB_String& defaultText) {
    bool ok;
    RB_String text = QInputDialog::getText(mMainWindow, dialogTitle,
                                           labelName, QLineEdit::Normal,
                                           defaultText, &ok);
    if (ok && !text.isEmpty()) {
      return text;
    }

    return RB_String();
}

/**
 * Shows a warning message dialog.
 */
void RB_DialogFactory::requestWarningDialog(const RB_String& warn) {
    QMessageBox::warning(mMainWindow, QMessageBox::tr("Warning"),
                         warn, QMessageBox::Ok);
}

/**
 * Shows a dialog with a question, for example: Do you want to save your data?
 * @return result yes or no
 */
int RB_DialogFactory::requestYesNoDialog(
        const RB_String& dialogTitle, const RB_String& question) {
    return QMessageBox::question(
                this->getMainWindow(), dialogTitle, question,
                QMessageBox::Yes | QMessageBox::Default,
                QMessageBox::No | QMessageBox::Escape);
}

/**
 * Shows a dialog with a question, for example: Do you want to save your data?
 * @return result yes, no or cancel
 */
int RB_DialogFactory::requestYesNoCancelDialog(
        const RB_String& dialogTitle, const RB_String& question) {
    return QMessageBox::question(
                this->getMainWindow(), dialogTitle, question,
                QMessageBox::Yes | QMessageBox::Default,
                QMessageBox::No,
                QMessageBox::Cancel | QMessageBox::Escape);
}

/**
 * Shows a dialog with the given text warning and a tick box the user can
 * tick to never show this message again, implement in actual class.
 * @param text text for the dialog
 * @param id dialog ID to identify the setting in RB_Settings
 */
bool RB_DialogFactory::requestOneTimeDialog(const RB_String& /*text*/,
                                            const RB_String& /*id*/) {
    RB_DEBUG->error("RB_DialogFactory::requestOneTimeDialog() ERROR");
    /*
    RB_SETTINGS->beginGroup("/ShownDialogs");
    int shown = RB_SETTINGS->readNumEntry(RB_String("/") + id, 0);
    RB_SETTINGS->endGroup();
    */
    bool ret = false;
    /*
    if (shown == 0) {
        GV_OneTimeDialog dlg(mParent);
        dlg.setText(text);
        ret = dlg.exec();

        int dontShowAgain = (int)dlg.getDontShowAgain();

        RB_SETTINGS->beginGroup("/ShownDialogs");
        RB_SETTINGS->writeEntry(RB_String("/") + id, dontShowAgain);
        RB_SETTINGS->endGroup();
    } else {
        ret = true;
    }
    */
    return ret;
}

/**
 * Called when an action needs to communicate 'message' to the user.
 * To be overriden (by DB_DialogFactory) TODO: make pure virtual
 */
void RB_DialogFactory::commandMessage(const RB_String& /* message */) {
    RB_DEBUG->print(RB_Debug::D_ERROR,
                    "RB_DialogFactory::commandMessage ERROR");
}

/**
 * Called when an action needs to communicate 'message' to the user.
 * To be overriden (by DB_DialogFactory) TODO: make pure virtual
 */
void RB_DialogFactory::commandMessage(const RB_StringList& /*messageList*/) {
    RB_DEBUG->print(RB_Debug::D_ERROR,
                    "RB_DialogFactory::commandMessage list ERROR");
}

/**
 * Show message on mainwindow dialog
 * @param text message test
 * @param timeOut in milliseconds
 */
void RB_DialogFactory::statusBarMessage(const RB_String& text, int timeout) {
    getMainWindow()->statusBar()->showMessage(text, timeout);
}

/**
 * Get perspective of current MDI window, called by actions
 * to decided whether the action shoulde be enabled
 */
int RB_DialogFactory::getCurrentPerspective() {
    RB_MdiWindow* mdiWin = getMainWindow()->getActiveMdiWindow();

    if (mdiWin) {
        return mdiWin->getPerspectiveType();
    }

    return RB2::PerspectiveNone;
}

/**
 * Set this dialogfactory perspective type
 * @param perspectiveType from RB2::PerspectiveType
 */
void RB_DialogFactory::setPerspective(int perspectiveType) {
    mPerspective = perspectiveType;
}

/**
 * Close all perspective MDI windows,
 * for example if project selected in database changed
 * also deletes the window later. Do not use when disconnect
 * from database.
 */
void RB_DialogFactory::closeAllMdiWindows() {
    RB_MainWindow* mw = getMainWindow();
    if (!mw) return;

    foreach (QMdiSubWindow* subWindow, mw->getMdiArea()->subWindowList()) {
        RB_MdiWindow* mdiWindow = qobject_cast<RB_MdiWindow*>(subWindow);
        if (mdiWindow->getPerspectiveType() == mPerspective) {
            mdiWindow->close();
        }
    }
}

/**
 * Delete all perspective MDI windows,
 * for example if project selected in database changed
 */
void RB_DialogFactory::deleteAllMdiWindows() {
    RB_MainWindow* mw = getMainWindow();
    if (!mw) return;

    foreach (QMdiSubWindow* subWindow, mw->getMdiArea()->subWindowList()) {
        RB_MdiWindow* mdiWindow = qobject_cast<RB_MdiWindow*>(subWindow);
        if (mdiWindow && mdiWindow->getPerspectiveType() == mPerspective) {
            delete mdiWindow;
        }
    }
}

/**
 * Close all dock windows for this perspective,
 * for example if project selected in database changed
 */
void RB_DialogFactory::closeAllDockWidgets() {
    RB_MainWindow* mw = getMainWindow();
    if (!mw) return;

    foreach (QObject* object, mw->children()) {
        // HACK: cast does not work
        RB_DEBUG->debug(object->objectName());

        if (object->objectName() == "RB_DockWidget") {
            RB_DockWidget* dw = dynamic_cast<RB_DockWidget*>(object);
            if (dw && dw->getPerspectiveType() == mPerspective) {
                dw->close();
            }
        }
    }
}

/**
 * Delete all dockwindows for this perspective,
 * for example if project selected in database changed
 */
void RB_DialogFactory::deleteAllDockWidgets() {
    RB_MainWindow* mw = getMainWindow();
    if (!mw) return;

    foreach (QObject* object, mw->children()) {
        // HACK: cast does not work
        RB_DEBUG->debug(object->objectName());

        if (object->objectName() == "RB_DockWidget") {
            RB_DockWidget* dw = dynamic_cast<RB_DockWidget*>(object);
            if (dw && dw->getPerspectiveType() == mPerspective) {
                delete dw;
            }
        }
    }
}

/**
 * Close all toolbars for this perspective,
 * for example if project selected in database changed
 */
void RB_DialogFactory::closeAllToolBars() {
    RB_MainWindow* mw = getMainWindow();
    if (!mw) return;

    foreach (QObject* object, mw->children()) {
        RB_ToolBar* tb = qobject_cast<RB_ToolBar*>(object);
        if (tb) {
            if (tb->getPerspectiveType() == mPerspective) {
                tb->close();
            }
        }
    }
}

/**
 * Delete all toolbars for this perspective,
 * for example if project selected in database changed
 */
void RB_DialogFactory::deleteAllToolBars() {
    RB_MainWindow* mw = getMainWindow();
    if (!mw) return;

    foreach (QObject* object, mw->children()) {
        RB_ToolBar* tb = dynamic_cast<RB_ToolBar*>(object);
        if (tb) {
            if (tb->getPerspectiveType() == mPerspective) {
                delete tb;
            }
        }
    }
}

/**
 * Set signal spy target, for debugging only
 */
void RB_DialogFactory::setSignalSpyTarget(QObject *target) {
    getMainWindow()->setSignalSpyTarget(target);
}

/**
 * Emit the signal settingChanged with the dialog or widget type as parameter.
 * MDI- and dock windows can connect to this signal to update their state
 * if required.
 * @brief RB_DialogFactory::slotSettingChanged
 * @param dialogWidgetType
 */
void RB_DialogFactory::slotSettingChanged(int dialogWidgetType) {
    emit settingChanged(dialogWidgetType);
}

