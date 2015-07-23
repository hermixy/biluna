/*****************************************************************
 * $Id: db_actionfileopenmultiple.cpp 1585 2012-03-22 15:56:48Z rutger $
 * Created: Mar 21, 2012 4:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionfileopenmultiple.h"

#include <QFileDialog>
#include "db_actionfactory.h"
#include "db_dialogfactory.h"
#include "db_modelfactory.h"
#include "db_textwidget.h"
#include "rb_mdiwindow.h"
#include "rb_settings.h"


DB_ActionFileOpenMultiple::DB_ActionFileOpenMultiple() : RB_Action() {

}


RB_GuiAction* DB_ActionFileOpenMultiple::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(DB_ActionFileOpenMultiple::getName(),
                                  QIcon(":/images/icons/folder_page.png"),
                                  tr("&Open..."));
        ga->setShortcut("Ctrl+O");
        ga->setStatusTip(tr("Opens an existing files"));
//        ga->setCommand("db object, db");
//        ga->setKeycode("db");
//        ga->setStatusTip(tr("Test DB object"));
//        ga->setShortcut("Ctrl+O");
//        ga->setToolTip("Test DB object");

        // The setFactory() connects ga->triggered() to ga->slotTriggered()
        // which in turn executes factory(). In factory the choice is made
        // to execute this action on its own like below, or to add the action
        // to for example a graphicsView's eventHandler which sets the
        // handling of the command-, mouse- and keyEvents at the graphicsView
        // If ga->setFactory is not used the RB_GuiAction will emit only
        // triggered() which can be connected directly to a mainWindow
        ga->setFactory(DB_ActionFileOpenMultiple::factory);
    }                                  
    return ga;
}

RB_Action* DB_ActionFileOpenMultiple::factory() {
    RB_Action* a = new DB_ActionFileOpenMultiple();
    // no graphicsView with eventhandler which triggers and deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionFileOpenMultiple::trigger() {
    // No preliminary checks required

    // No models to prepare

    // Get file names to open
    RB_StringList fileNames;
    getOpenFileNames(fileNames);

    if (fileNames.count() == 0) {
        DB_DIALOGFACTORY->commandMessage(tr("File names empty, could not open files."));
        DB_DIALOGFACTORY->statusBarMessage(tr("File names empty, could not open files."), 4000);
        return;
    }

    // Create widget
    RB_String fileName;
    RB_MdiWindow* mdiWin = NULL;
    int ret = -1;

    for (int i = 0; i < fileNames.count(); ++i) {
        fileName = fileNames.at(i);

        // Activate existing window if file was already existing
        if (activateExistingWindow(fileName) || fileName.isEmpty()) continue;

        if (fileName.endsWith(".htm") || fileName.endsWith(".html")
                                        || fileName.endsWith(".xhtml")) {
            ret = DB_DIALOGFACTORY->requestYesNoCancelDialog(tr("Open type ..."),
                                                       tr("Do you want the document\n"
                                                          "- to open in the HTML editor (Yes)\n"
                                                          "- or in the RichText editor (No)"));
            if (ret == QMessageBox::Yes) {
                mdiWin = DB_DIALOGFACTORY->getMdiWindow(
                        DB_DialogFactory::WidgetHtmlEditor);
            } else if (ret == QMessageBox::No) {
                mdiWin = DB_DIALOGFACTORY->getMdiWindow(
                        DB_DialogFactory::WidgetTextDocument);
            } else {
                return;
            }
        } else {
            mdiWin = DB_DIALOGFACTORY->getMdiWindow(
                    DB_DialogFactory::WidgetTextDocument);
        }

        RB_Widget* wdgt = NULL;
        if (mdiWin) {
            wdgt = mdiWin->getWidget();
        } else {
            wdgt = mWidget;
        }

        // wdgt->fileOpen(); not required here
        wdgt->init();

        if (wdgt->loadFile(fileName)) {
            DB_DIALOGFACTORY->getMainWindow()->statusBar()->showMessage(
                    tr("File loaded"), 2000);
            mdiWin->show();
        } else {
            mdiWin->show();
        }

        DB_ACTIONFACTORY->setRecentFile(fileName);
    }


    // Show toolbar(s)
    QToolBar* toolBar = DB_DIALOGFACTORY->getMainWindow()->findChild<QToolBar*>("toolbar_edit");
    if (toolBar) {
        toolBar->show();
    }

    if (ret == QMessageBox::Yes || ret == QMessageBox::No) {
        toolBar = DB_DIALOGFACTORY->getMainWindow()->findChild<QToolBar*>("toolbar_edit_html");
        if (toolBar) {
            toolBar->show();
        }

        toolBar = DB_DIALOGFACTORY->getMainWindow()->findChild<QToolBar*>("toolbar_format");
        if (toolBar) {
            toolBar->show();
        }
    }
}

/**
 * Show openfile dialog and select file(s)
 * @param fn filename
 * @param filters filters for file dialog
 */
void DB_ActionFileOpenMultiple::getOpenFileNames(RB_StringList& fns,
                                                 const RB_String& filters) {
    // get file name
    RB_SETTINGS->beginGroup("paths");
    RB_String openPath = RB_SETTINGS->value("openPath", "").toString();
    RB_SETTINGS->endGroup();

    fns.clear();

    if (!filters.isEmpty()) {
        fns = QFileDialog::getOpenFileNames(
                    DB_DIALOGFACTORY->getMainWindow(),
                    tr("Open files"),
                    openPath,
                    filters);
    } else {
        fns = QFileDialog::getOpenFileNames(
                    DB_DIALOGFACTORY->getMainWindow(),
                    tr("Open files"),
                    openPath,
                    "All files (*.*);;"
                    "Text files (*.txt);;"
                    "Hyper Text Markup Language files (*.html *.htm);;"
                    "eXtensible Markup Language files (*.xml)");
                    // "SQLite (*.db3)",	// selected filter
                    // 0); 					// option
    }

    if (fns.count() < 1) return;

    openPath = QFileInfo(fns.at(0)).absolutePath();
    RB_SETTINGS->beginGroup("paths");
    RB_SETTINGS->setValue("openPath", openPath);
    RB_SETTINGS->endGroup();
}

/**
 * Activate MDI window if existing
 * @param fn filename
 * @return true if window with file was existing
 */
bool DB_ActionFileOpenMultiple::activateExistingWindow(const RB_String& fn) {
    if (!fn.isEmpty()) {
        RB_MdiWindow* mdiWin = DB_DIALOGFACTORY->getMainWindow()->findMdiWindow(fn);
        if (mdiWin) {
            DB_DIALOGFACTORY->getMainWindow()->setActiveMdiWindow(mdiWin);
            return true;
        }
    }
    return false;
}
