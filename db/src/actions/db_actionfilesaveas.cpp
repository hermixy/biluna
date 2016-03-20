/*****************************************************************
 * $Id: db_actionfilesaveas.cpp 2217 2015-02-13 18:18:14Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionfilesaveas.h"

#include <QFileDialog>
#include "db_actionfactory.h"
#include "db_dialogfactory.h"
#include "rb_mdiwindow.h"
#include "rb_settings.h"
#include "rb_widget.h"

DB_ActionFileSaveAs::DB_ActionFileSaveAs() : RB_Action() {
    // nothing
}

RB_GuiAction* DB_ActionFileSaveAs::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (ga==NULL) {
        ga = new RB_GuiAction(DB_ActionFileSaveAs::getName(),
                                  QIcon(":/images/icons/disk_multiple.png"),
                                  tr("Save &as..."));
        ga->setShortcut("Ctrl+Shift+S");
        ga->setStatusTip(tr("Saves the current document with"
                                " a new filename"));
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
        ga->setFactory(DB_ActionFileSaveAs::factory);
    }
    return ga;
}

RB_Action* DB_ActionFileSaveAs::factory() {
    RB_Action* a = new DB_ActionFileSaveAs();
    // no graphicsView with eventhandler which triggers and deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionFileSaveAs::trigger() {
    // No preliminary checks required

    // No models to prepare

    // Get active widget
    RB_MdiWindow* mdiWin = DB_DIALOGFACTORY->getActiveMdiWindow();
    RB_Widget* wdgt = NULL;
    if (!mdiWin && !mWidget) {
        return;
    } else if (mdiWin) {
        wdgt = mdiWin->getWidget();
    } else {
        wdgt = mWidget; // set from outside
    }

    // Get file name or path and file name
    RB_String fn = wdgt->getSaveAsFileName();

    RB_String fileName = QFileInfo(fn).fileName();
    RB_StringList strL = fileName.split(".");
    fileName = strL.at(0);
    fileName.replace(QRegExp("[^a-zA-Z\\d\\s]"), "_");
    if (wdgt->getWidgetType() == DB_DialogFactory::WidgetHtmlEditor
            || wdgt->getWidgetType() == DB_DialogFactory::WidgetInternet) {
        fileName += ".html";
    } else if (wdgt->getWidgetType() == DB_DialogFactory::WidgetTextDocument) {
        fileName += ".odt";
    } else if (wdgt->getWidgetType() == DB_DialogFactory::WidgetPlainTextDocument) {
        fileName += ".txt";
    } else if (strL.count() > 1) {
        fileName += "." + strL.at(strL.count() - 1);
    }

    RB_String filters = "All files (*.*);;Text files (*.txt);;"
                        "Open Document Format files (*.odt);;"
                        "Hyper Text Markup Language files (*.html *.htm);;"
                        "eXtensible Markup Language files (*.xml)";

    if (fn.contains(QDir::separator())) {
        // fn was path plus file name
        fn = QFileInfo(fn).absolutePath() + "/" + fileName;
    } else {
        // fn was only file name
        fn = fileName;
    }

    getSaveFileName(fn, filters);

    if (fn.isEmpty()) {
        return;
    }

    // wdgt->fileSaveAs(); not required instead :
    wdgt->saveFile(fn);
    DB_DIALOGFACTORY->commandMessage("File save as: " + fn);
    DB_ACTIONFACTORY->setRecentFile(fn);
}

/**
 * Show savefile dialog and select file
 * @param fn filename
 */
void DB_ActionFileSaveAs::getSaveFileName(RB_String& fn,
                                          const RB_String& filters) {
    RB_String savePath = "";
    RB_String path = "";

    if (fn.contains(QDir::separator())) {
        // fn was path plus file name
        path = QFileInfo(fn).absolutePath();
    }

    if (path.isEmpty()) {
        RB_SETTINGS->beginGroup("paths");
        savePath = RB_SETTINGS->value("savePath", "").toString();
        RB_SETTINGS->endGroup();

        // separator can be used instead of QDir::separator()
        fn = savePath + "/" + QFileInfo(fn).fileName();
    }

    RB_String currentFilter;

    if (!filters.isEmpty()) {
        currentFilter = filters;
    } else {
        currentFilter = "All files (*.*);;Text files (*.txt);;"
                "Open Document Format files (*.odt);;"
                "Hyper Text Markup Language files (*.html *.htm);;"
                "eXtensible Markup Language files (*.xml)";
    }

    fn = QFileDialog::getSaveFileName(
                DB_DIALOGFACTORY->getMainWindow(),
                tr("Save file"),
                fn,
                currentFilter);

    if (fn.isEmpty()) return;

    if (QFileInfo(fn).suffix().isEmpty()) {
        int pos = currentFilter.indexOf("(");
        currentFilter = currentFilter.remove(0, pos + 1);

        if (currentFilter.count("*") == 1) {
            currentFilter = currentFilter.remove("*");
            currentFilter = currentFilter.remove(")");
            currentFilter = currentFilter.trimmed();
            fn.append(currentFilter);
        } else if (currentFilter.contains(".odt", Qt::CaseInsensitive)) {
            fn.append(".odt");
        } else if (currentFilter.contains(".htm", Qt::CaseInsensitive)
            && currentFilter.contains(".html", Qt::CaseInsensitive)) {
            fn.append(".html");
        } else if (currentFilter.contains(".jpg", Qt::CaseInsensitive)
            && currentFilter.contains(".jpeg", Qt::CaseInsensitive)) {
            fn.append(".jpg");
        }
    }

    savePath = QFileInfo(fn).absolutePath();
    RB_SETTINGS->beginGroup("paths");
    RB_SETTINGS->setValue("savePath", savePath);
    RB_SETTINGS->endGroup();
}
