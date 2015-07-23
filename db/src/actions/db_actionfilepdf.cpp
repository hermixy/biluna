/*****************************************************************
 * $Id: db_actionfilepdf.cpp 2036 2013-11-21 22:18:13Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionfilepdf.h"

#include <QPrinter>
#include <QPrintDialog>
#include "db_actionfactory.h"
#include "db_actionfilesaveas.h"
#include "db_dialogfactory.h"
#include "db_htmleditorwidget.h"
#include "rb_mdiwindow.h"
#include "rb_settings.h"
#include "rb_widget.h"

DB_ActionFilePdf::DB_ActionFilePdf() : RB_Action() {}


RB_GuiAction* DB_ActionFilePdf::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (ga==NULL) {
        ga = new RB_GuiAction(DB_ActionFilePdf::getName(),
                                  QIcon(":/images/icons/page_white_acrobat.png"),
                                  tr("&PDF Export"));
        ga->setStatusTip(tr("Creates a PDF from the current document"));

        // The setFactory() connects ga->triggered() to ga->slotTriggered()
        ga->setFactory(DB_ActionFilePdf::factory);
    }
    return ga;
}

RB_Action* DB_ActionFilePdf::factory() {
    RB_Action* a = new DB_ActionFilePdf();
    // no graphicsView with eventhandler which triggers and deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionFilePdf::trigger() {
    // No preliminary checks required

    // No models to prepare

    // Get active widget
    RB_MdiWindow* mdiWin = DB_DIALOGFACTORY->getActiveMdiWindow();
    if (!mdiWin) return;
    RB_Widget* wdgt = mdiWin->getWidget();
    if (!wdgt) {
        wdgt = mWidget;
    }
    if (!wdgt) return;

#ifndef QT_NO_PRINTER
    // Get file name, can be path + fileName or only fileName
    RB_String fn = wdgt->getSaveAsFileName(); // empty if untitled

    RB_String fileName = QFileInfo(fn).fileName();
    RB_StringList strL = fileName.split(".");
    fileName = strL.at(0);
    fileName.replace(QRegExp("[^a-zA-Z\\d\\s]"), "_");
    fileName += ".pdf";
    RB_String filters = "PDF files (*.pdf);;All files (*.*)";

    if (fn.contains(QDir::separator())) {
        // fn was path plus file name
        fn = QFileInfo(fn).absolutePath() + "/" + fileName;
    } else {
        // fn was only file name
        fn = fileName;
    }

    getSaveFileName(fn, filters);

    if (fn.isEmpty()) return;

    QPrinter* pr = DB_ACTIONFACTORY->getPrinter();
    wdgt->setPrinterProperties(pr);

    pr->setOutputFormat(QPrinter::PdfFormat);
    pr->setOutputFileName(fn);
    wdgt->filePdf(pr);
 #endif

}

/**
 * @brief DB_ActionFilePdf::printDialog
 * @param dialog
 */
void DB_ActionFilePdf::printDialog(RB_Dialog* dialog) {
#ifndef QT_NO_PRINTER
    // Get file name, can be path + fileName or only fileName
    RB_String fn = dialog->getSaveAsFileName(); // empty if untitled

    RB_String fileName = QFileInfo(fn).fileName();
    RB_StringList strL = fileName.split(".");
    fileName = strL.at(0);
    fileName.replace(QRegExp("[^a-zA-Z\\d\\s]"), "_");
    fileName += ".pdf";
    RB_String filters = "PDF files (*.pdf);;All files (*.*)";

    if (fn.contains(QDir::separator())) {
        // fn was path plus file name
        fn = QFileInfo(fn).absolutePath() + "/" + fileName;
    } else {
        // fn was only file name
        fn = fileName;
    }

    getSaveFileName(fn, filters);

    if (fn.isEmpty()) return;

    QPrinter* pr = DB_ACTIONFACTORY->getPrinter();
    dialog->setPrinterProperties(pr);

    pr->setOutputFormat(QPrinter::PdfFormat);
    pr->setOutputFileName(fn);
    dialog->filePdf(pr);
 #endif
}

/**
 * Show savefile dialog and select file
 * @param fn filename (path + filename or filename only)
 * @param filters filters
 */
void DB_ActionFilePdf::getSaveFileName(RB_String& fn,
                                       const RB_String& filters) {
    RB_String savePath = "";
    RB_String path = "";

    if (fn.contains(QDir::separator())) {
        // fn was path plus file name
        path = QFileInfo(fn).absolutePath();
    }

    if (path.isEmpty()) {
        RB_SETTINGS->beginGroup("paths");
        savePath = RB_SETTINGS->value("savePdfPath", "").toString();
        RB_SETTINGS->endGroup();

        // separator can be used instead of QDir::separator()
        fn = savePath + "/" + QFileInfo(fn).fileName();
    }

    RB_String currentFilter;

    if (!filters.isEmpty()) {
        currentFilter = filters;
    } else {
        currentFilter = "PDF files (*.pdf);;All files (*.*)";
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
    RB_SETTINGS->setValue("savePdfPath", savePath);
    RB_SETTINGS->endGroup();
}

