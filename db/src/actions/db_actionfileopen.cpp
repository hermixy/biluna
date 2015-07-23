/*****************************************************************
 * $Id: db_actionfileopen.cpp 2217 2015-02-13 18:18:14Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionfileopen.h"

#include <QFileDialog>
#include "db_actionfactory.h"
#include "db_dialogfactory.h"
#include "db_modelfactory.h"
#include "db_textwidget.h"
#include "rb_mdiwindow.h"
#include "rb_settings.h"


DB_ActionFileOpen::DB_ActionFileOpen() : RB_Action() {
    mFileName = "";
}


RB_GuiAction* DB_ActionFileOpen::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(DB_ActionFileOpen::getName(),
                                  QIcon(":/images/icons/folder.png"),
                                  tr("&Open..."));
        ga->setShortcut("Ctrl+O");
        ga->setStatusTip(tr("Opens an existing file"));
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
        ga->setFactory(DB_ActionFileOpen::factory);
    }                                  
    return ga;
}

RB_Action* DB_ActionFileOpen::factory() {
    RB_Action* a = new DB_ActionFileOpen();
    // no graphicsView with eventhandler which triggers and deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionFileOpen::trigger() {
    // No preliminary checks required

    // No models to prepare

    // Get file name to open
    RB_String fileName = mFileName;
    if (fileName.isEmpty()) {
        getOpenFileName(fileName);
    }
    if (fileName.isEmpty()) {
        DB_DIALOGFACTORY->commandMessage(tr("File name empty, could not open file."));
        DB_DIALOGFACTORY->statusBarMessage(tr("File name empty, could not open file."), 4000);
        return;
    }

    // Activate existing window if file is already existing
    if (activateExistingWindow(fileName)) return;

    // Create widget
    RB_MdiWindow* mdiWin = NULL;
    int ret = -1;

    if (fileName.endsWith(".htm") || fileName.endsWith(".html")
                                    || fileName.endsWith(".xhtml")) {
        ret = DB_DIALOGFACTORY->requestYesNoCancelDialog(tr("Open type ..."),
                                                   tr("Do you want the document\n"
                                                      "- to open the RichText editor (Yes)\n"
                                                      "- or in in the HTML editor (No)"));
        if (ret == QMessageBox::Yes) {
            mdiWin = DB_DIALOGFACTORY->getMdiWindow(
                        DB_DialogFactory::WidgetTextDocument, fileName,  false);
        } else if (ret == QMessageBox::No) {
            mdiWin = DB_DIALOGFACTORY->getMdiWindow(
                        DB_DialogFactory::WidgetHtmlEditor, fileName, false);
        } else {
            return;
        }
    } else if (fileName.endsWith(".txt")) {
        mdiWin = DB_DIALOGFACTORY->getMdiWindow(
                DB_DialogFactory::WidgetPlainTextDocument, fileName, false);
    } else {
        // TODO: handle by relevant plugin based on file extension
        DB_DIALOGFACTORY->requestWarningDialog(tr("Could not open file,\n"
                                                  "file extension unknown."));
        return;
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
void DB_ActionFileOpen::getOpenFileName(RB_String& fn,
                                        const RB_String& filters) {
    // get file name
    RB_SETTINGS->beginGroup("paths");
    RB_String openPath = RB_SETTINGS->value("openPath", "").toString();
    RB_SETTINGS->endGroup();

    fn = "";

    // Not native dialog
    //    QFileDialog dlg(DB_DIALOGFACTORY->getMainWindow(), tr("Open file"), openPath,
    //                    "All files (*.*);;"
    //                    "Text files (*.txt);;"
    //                    "Hyper Text Markup Language files (*.html *.htm);;"
    //                    "eXtensible Markup Language files (*.xml)");
    //    if (!filters.isEmpty()) {
    //        dlg.setFilter(filters);
    //    }
    //    dlg.setFileMode(QFileDialog::ExistingFile);
    //    dlg.setAcceptMode(QFileDialog::AcceptOpen);
    //    dlg.setOption(QFileDialog::DontUseNativeDialog, false);
    //    dlg.exec();
    //    if (!dlg.result()) return;
    //    fn = dlg.selectedFiles().at(0);

    if (!filters.isEmpty()) {
        fn = QFileDialog::getOpenFileName(
                    DB_DIALOGFACTORY->getMainWindow(),
                    tr("Open file"),
                    openPath,
                    filters);
    } else {
        fn = QFileDialog::getOpenFileName(
                    DB_DIALOGFACTORY->getMainWindow(),
                    tr("Open file"),
                    openPath,
                    "All files (*.*);;"
                    "Text files (*.txt);;"
                    "Hyper Text Markup Language files (*.html *.htm);;"
                    "eXtensible Markup Language files (*.xml)");
                    // "SQLite (*.db3)",	// selected filter
                    // 0); 					// option
    }

    if (fn.isEmpty()) return;

    DB_DIALOGFACTORY->commandMessage("File open: " + fn);

    openPath = QFileInfo(fn).absolutePath();
    RB_SETTINGS->beginGroup("paths");
    RB_SETTINGS->setValue("openPath", openPath);
    RB_SETTINGS->endGroup();
}

/**
 * Set open file name, usually from the recent file list
 */
void DB_ActionFileOpen::setOpenFilename(const QString &fn) {
    mFileName = fn;
}

/**
 * Activate MDI window if existing
 * @param fn filename
 * @return true if window with file was existing
 */
bool DB_ActionFileOpen::activateExistingWindow(const RB_String& fn) {
    if (!fn.isEmpty()) {
        RB_MdiWindow* mdiWin = DB_DIALOGFACTORY->getMainWindow()->findMdiWindow(fn);
        if (mdiWin) {
            DB_DIALOGFACTORY->getMainWindow()->setActiveMdiWindow(mdiWin);
            return true;
        }
    }
    return false;
}
